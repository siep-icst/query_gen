
#include "Ring_match.h"
#include <utility>

using namespace std;

#define MAXSEARCHTIME 100000
#define MAXSEARCHTIME2 100000

int Ring_match::query_count = 0;

Ring_match::Ring_match(int queryNodeNum, int queryEgdeNum, int queryNum, Graph* _data)
{
	this->data = _data;
	this->qsize = queryNodeNum;
	this->dsize = _data->vSize();
	this->edgeNum = queryEgdeNum;
    this->queryNum = queryNum;
}

Ring_match::~Ring_match()
{

}

int Ring_match::myFind_in_Ring(std::vector<Neighbor>& list,  int value) 
{
    //find the neighbor with id = value, the label of this edge
	vector<Neighbor>::iterator it;
	int elb = -1;
	if (list.size() == 0)
		return -1;
	for (it = list.begin(); it != list.end(); it ++) 
    {
		if (it->vid == value) 
        {
			elb = it->elb;
			break;
		}
	}
	return elb;
}

int Ring_match::find_pos_in_nei_list(std::vector<Neighbor>& list,int id)
{
    vector<Neighbor>::iterator it;
	int pos = -1;
	if (list.size() == 0)
		return -1;
	for (it = list.begin(); it != list.end(); it ++) 
    {
		if (it->vid == id) 
        {
			pos=it - list.begin();
			break;
		}
	}
	return pos;
}

struct sortEdges
{
	int src;
	int dst;
	int label;
	bool operator < (const sortEdges& e)const
    {
		return src < e.src || (src == e.src && dst < e.dst);
	}
};

bool 
Ring_match::isDuplicate(std::vector<int*>& query_set, vector<int>& vlabel, std::vector<std::pair<int,int>*>& edges, std::vector<int>& elabel)
{
    //qsize: vertex count of this query
    int* record = new int[qsize+3*edgeNum];
    for(int i = 0; i < qsize; ++i)
    {
        record[i] = vlabel[i];
    }
    for (int i = 0, pos = qsize; i < edges.size(); i ++, pos+=3) 
    {
        record[pos] = edges[i]->first;
        record[pos+1] = edges[i]->second;
        record[pos+2] = elabel[i];
    }
	//cout  << "sizeof(struct) is " << sizeof(sortEdges) << endl;
    //QUERY: no alignment in sortEdges? 12 bytes instead of 16 bytes?
    //sort edges by src and dst value
	sort((sortEdges*)(record+qsize),(sortEdges*)(record+qsize+3*edgeNum));
	bool dupl = true;
	if (query_set.size() == 0) 
    {
		query_set.push_back(record);
//		cout<<"a result found"<<endl;
		return false;
	}
	for (int r = 0; r < query_set.size(); r ++) 
    {
		dupl = true;
		for (int i = 0; i < qsize+3*edgeNum; i++) 
        {
			if(record[i] != query_set[r][i]) 
            {
				dupl = false;
				break;
			}
		}
		if(dupl == true)
			break;
	}
    // whether find a query which meets the requirement of the query parameter
	if (dupl) 
    {
		delete [] record;
		return true;
	}
//    cout<<"a result found"<<endl;
    query_set.push_back(record);
    return false;
}



int Ring_match::get_start_pos(int _head_pos,int _tail_pos)
{
    srand((unsigned)time(NULL)); 
    int flag=rand()%2;
    if(flag==0)
    {
        return _head_pos;
    }
    else
    {
        return _tail_pos;
    }
}


void 
Ring_match::match(std::string _query_dir)
{
//    cout<<"check sortEdges: "<<sizeof(sortEdges)<<endl;
	if(qsize > dsize)
	{
        printf(" number of vertices of query is too large\n");
        //vertex count of query > vertex count of graph
		return;
	}
	//cout << "node num is " << qsize << ", edge num is " << edgeNum << endl;
    //NOTICE: duplicate queries generated can only occur in a specific (node num, edge num) pair
    vector<int*> query_set;

    //BETTER: use goto here instead of nested break
    for(int qq = 0; qq < this->queryNum; ++qq)
    {
        //NOTICE: srand is needed to produce random number, otherwise every time the number sequence generated will be the same
        printf("aim to get %d-vertices query\n",qsize);
        srand((unsigned)time(NULL)); 
        //NOTICE: the searching is based on random rather than dfs, which is not strictly in random
        int t;
        // find one query
        bool queryFound=false;
        for (t = 0; t < MAXSEARCHTIME; t ++) 
        {
            

            std::vector<int> vid;
            std::vector<int> vlabel;
            std::vector<pair<int,int>*> edge;
            std::vector<int> elabel;
            int head_pos=-1;
            int tail_pos=-1;
            int i;
            for (i = 0; i < qsize; i ++)
            {
                // one vertex at a time

                //random select a mapping of the first query node
                if (i == 0) 
                {
                    int qid = rand()%dsize;
                    vid.push_back(qid);
                    vlabel.push_back(data->vertices[qid].label);
                    head_pos=0;
                    tail_pos=0;
                    continue;
                }

				bool nodeAdded = false;
                    
                for (int t2 = 0; t2 <MAXSEARCHTIME2; t2 ++) 
                {
                    // search from head or tail
                    // bool queryFound2 = true;
                    //random select a vertex in the possible vertex list

                    
                    int rand_start_pos=this->get_start_pos(head_pos,tail_pos);

                    

                    //random select a known node to expand
				 	//printf("vid size is %d, randForStartId is %d\n",vid.size(),randForStartId);
                    int startId = vid[rand_start_pos];
                    //in degree
                    int vertexInSize = data->vertices[startId].in.size();
                    int vertexOutSize = data->vertices[startId].out.size();
                    //degree
                    int startIdNeighbor = vertexInSize + vertexOutSize;
					if (startIdNeighbor == 0)
						continue;		
                    //select a neighbour
                    int randNum = rand()%startIdNeighbor;
                    int nextId;
                    if (randNum < vertexInSize) 
                    {
						if (vertexInSize == 0)
							continue;
                        int randPosInNeibList = rand()%vertexInSize;
			
                        nextId = data->vertices[startId].in[randPosInNeibList].vid;
                        bool inner_ring=false;
                        for (int c = 0; c < i; c ++) 
                        {
                            if (nextId == vid[c]) 
                            {
                                //query contains a partial ring
                                inner_ring=true;
                                break;
                            }
                        }
                        if(inner_ring==true)
                        {
                            continue;
                        }

						
                        vid.push_back(nextId);
                        vlabel.push_back(data->vertices[nextId].label);
                        // the two ids are id in the vector"vid"
                        pair<int,int> * tmpPairPtr = new pair<int,int>(i,rand_start_pos);
                        edge.push_back(tmpPairPtr);
                        elabel.push_back(data->vertices[startId].in[randPosInNeibList].elb);
                        if(rand_start_pos==head_pos)
                        {
                            head_pos=i;
                        }
                        else if(rand_start_pos==tail_pos)
                        {
                            tail_pos=i;
                        }
                        nodeAdded=true;
                        break;
                    }
                    else 
                    {
                        //select part of neighbours in the out part
    //					randNum -= vertexInSize;
						if (vertexOutSize == 0)
							continue;
                        int randPosOutNeibList = rand()%vertexOutSize;
                        nextId = data->vertices[startId].out[randPosOutNeibList].vid;
                        bool inner_ring=false;
                        for (int c = 0; c < i; c ++) 
                        {
                            if (nextId == vid[c]) 
                            {
                                //query contains a partial ring
                                inner_ring=true;
                                break;
                            }
                        }
                        if(inner_ring==true)
                        {
                            continue;
                        }

						
                        vid.push_back(nextId);
                        vlabel.push_back(data->vertices[nextId].label);
                        pair<int,int> * tmpPairPtr = new pair<int,int>(rand_start_pos,i);
                        edge.push_back(tmpPairPtr);
                        elabel.push_back(data->vertices[startId].out[randPosOutNeibList].elb);
                        if(rand_start_pos==head_pos)
                        {
                            head_pos=i;
                        }
                        else if(rand_start_pos==tail_pos)
                        {
                            tail_pos=i;
                        }
                        nodeAdded=true;
                        break;
                    }

                }
                // if (!queryFound) 
                //     break;
				if (!nodeAdded)
                {
                    queryFound=false;
					break;
                }
            }
            //cout << "minimum graph find!" << endl;

            if(i==qsize)
            {
                queryFound=true;
            }

            // if(queryFound==false)
            // {
            //     continue;
            // }

            if (queryFound) 
            {
                int remainEdgeNum = edgeNum + 1 - qsize;
                if(remainEdgeNum!=1)
                {
                    //not a ring query
                    cerr<<"not a ring query"<<endl;
                }
                //cout << "the remainEdge number is " << remainEdgeNum  << endl;
                int head_id=vid[head_pos];
                int tail_id=vid[tail_pos];
                Vertex* head_vertex_ptr=&(this->data->vertices[head_id]);
                int tail_pos_in_nei_list=this->find_pos_in_nei_list(head_vertex_ptr->in,tail_id);
                if(tail_pos_in_nei_list>=0)
                {
                    // int tail_label=data->vertices[tail_id].label;
                    pair<int,int> * tmpPairPtr = new pair<int,int>(tail_pos,head_pos);
                    edge.push_back(tmpPairPtr);

                    
                    elabel.push_back(head_vertex_ptr->in[tail_pos_in_nei_list].elb);
                }
                else if(tail_pos_in_nei_list<0)
                {
                    // try out edge
                    tail_pos_in_nei_list=this->find_pos_in_nei_list(head_vertex_ptr->out,tail_id);
                    if(tail_pos_in_nei_list<0)
                    {
                        queryFound=false;
                        continue;
                    }
                    pair<int,int> * tmpPairPtr = new pair<int,int>(head_pos,tail_pos);
                    edge.push_back(tmpPairPtr);

                    
                    elabel.push_back(head_vertex_ptr->out[tail_pos_in_nei_list].elb);
                }
            }

            if (queryFound) 
            {
                
                //found a random query which satisfies the query vertex count and edge count

                //check if duplicates
                if(isDuplicate(query_set, vlabel, edge, elabel))
                {

                    continue;
                }
                printf("query found, no duplicate\n");
                //output the query graph
                string file = _query_dir + "/q" + Util::int2string(Ring_match::query_count) + ".g";
                FILE* ofp = fopen(file.c_str(), "w+");

                fprintf(ofp, "t # %d\n", Ring_match::query_count);
                query_count++;
				int maxVLabel = 0, maxELabel = 0;
				for (int i = 0; i < qsize; i ++)
					if (vlabel[i] > maxVLabel)
						maxVLabel = vlabel[i];
				for (int i = 0; i < edge.size(); i ++)
					if (elabel[i] > maxELabel)
						maxELabel = elabel[i];
                fprintf(ofp, "%d %d %d\n", qsize, edgeNum, maxVLabel);
                for (int i = 0; i < qsize; i ++)
                {
                    fprintf(ofp, "v %d %d\n", i, vlabel[i]);
                }
                printf(" vid list size in this query: %d\n",vid.size());
                printf(" edge list size in this query: %d\n",edge.size());

                for (int i = 0; i < edge.size(); i ++) 
                {
                    fprintf(ofp, "e %d %d\n", edge[i]->first, edge[i]->second);
                }

                fprintf(ofp, "t # -1\n");
                fclose(ofp);

                for (int i = 0; i < edge.size(); i ++)
                    delete  edge[i];
				edge.clear();
                break;
            }
			if (!queryFound) 
            {
				for (int i = 0; i < edge.size(); i ++)
					delete  edge[i];
				edge.clear();
			}
            if (!queryFound && t == MAXSEARCHTIME-1)
                cout << "After " << MAXSEARCHTIME << " times search, no query matched! "<< endl;
        }
    }

    for(int i = 0; i < query_set.size(); ++i)
    {
        delete[] query_set[i];
    }


}

