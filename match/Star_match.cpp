

#include "Star_match.h"
#include <utility>

using namespace std;

int Star_match::query_count=0;

int get_random_int(int _range)
{
    srand((unsigned)time(NULL)); 
    return rand()%_range;
}


Star_match::Star_match()
{

}

Star_match::Star_match(int _node_num, int _edge_num, int _query_req_num, Graph* _data_graph)
{
    this->query_req_num=_query_req_num;
    this->sate_num=_edge_num;
    this->data_ptr=_data_graph;
}

Star_match::~Star_match()
{

}

int my_find(std::vector<Neighbor>& list,  int value) 
{
    //find the neighbor with id = value, the label of this edge
	vector<Neighbor>::iterator it;
	int elb = -1;
	if (list.size() == 0)
		return -1;
	for (it = list.begin(); it != list.end(); it ++) {
		if (it->vid == value) 
        {
			elb = it->elb;
			break;
		}
	}
	return elb;
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
Star_match::is_duplicate(std::vector<int*>& _query_set, std::vector<int>& _vlabel_list, std::vector<std::pair<int,int>*>& _edge_list, std::vector<int>& _elabel_list)
{
    //qsize: vertex count of this query
    int query_v_num=_vlabel_list.size();
    int query_e_num=_edge_list.size();
    int* record = new int[query_v_num+3*query_e_num];
    for(int i = 0; i < query_v_num; ++i)
    {
        record[i] = _vlabel_list[i];
    }
    for (int i = 0, pos = query_v_num; i < query_e_num; i ++, pos+=3) 
    {
        record[pos] = _edge_list[i]->first;
        record[pos+1] = _edge_list[i]->second;
        record[pos+2] = _elabel_list[i];
    }
	//cout  << "sizeof(struct) is " << sizeof(sortEdges) << endl;
    //QUERY: no alignment in sortEdges? 12 bytes instead of 16 bytes?
    //sort edges by src and dst value
	sort((sortEdges*)(record+query_v_num),(sortEdges*)(record+query_v_num+3*query_e_num));
	bool dupl = true;
	if (_query_set.size() == 0) 
    {
		_query_set.push_back(record);
//		cout<<"a result found"<<endl;
		return false;
	}
	for (int r = 0; r < _query_set.size(); r ++) 
    {
		dupl = true;
		for (int i = 0; i < query_v_num+3*query_e_num; i++) 
        {
			if(record[i] != _query_set[r][i]) 
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
    _query_set.push_back(record);
    return false;
}

void Star_match::match(std::string _query_dir)
{
    int try_cnt=0;
    int max_try_cnt=10000;
    int data_size=this->data_ptr->vSize();
    int query_size=this->sate_num+1;
    if(query_size>data_size)
    {
        printf("error: query size exceeds data size\n");
        return;
    }

    std::vector<int*> query_set;

    //candidate id for core vertex
    std::vector<int> core_candidates;
    //minimum degree of core vertex
    int min_degree=this->sate_num;
    
    for(int v_id=0;v_id<data_size;++v_id)
    {
        int tmp_v_degree=this->data_ptr->vertices[v_id].degree;
        if(tmp_v_degree>=min_degree)
            core_candidates.push_back(v_id);
    }
    int core_candidate_num=core_candidates.size();
    if(core_candidate_num==0)
    {
        printf("cannot match core vertex\n");
        return;
    }


    for(int tmp_query_cnt=0;tmp_query_cnt<this->query_req_num;++tmp_query_cnt)
    {
        std::vector<int> vid_list;
        std::vector<int> vlabel_list;
        std::vector<pair<int,int>*> edge_list;
        std::vector<int> elabel_list;
        int core_vpos=get_random_int(core_candidate_num);
        int core_vid=core_candidates[core_vpos];
        //core selected
        vid_list.push_back(core_vid);
        int core_label=this->data_ptr->vertices[core_vid].label;
        vlabel_list.push_back(core_label);
        int core_degree=this->data_ptr->vertices[core_vid].degree;
        int core_indegree=this->data_ptr->vertices[core_vid].indegree;
        int core_outdegree=this->data_ptr->vertices[core_vid].outdegree;
        bool* rand_neighbor_used=new bool[core_degree];
        for(int i=0;i<core_degree;++i)
        {
            rand_neighbor_used[i]=false;
        }
        for(int sate_cnt=0;sate_cnt<this->sate_num;++sate_cnt)
        {
            int rand_neighbor=get_random_int(core_degree);
            if(rand_neighbor_used[rand_neighbor])
            {
                rand_neighbor=(rand_neighbor+1)%core_degree;
            }

            if(rand_neighbor<core_indegree)
            {
                Neighbor tmp_neighbor=this->data_ptr->vertices[core_vid].in[rand_neighbor];
                int neighbor_vid=tmp_neighbor.vid;
                int edge_label=tmp_neighbor.elb;
                vid_list.push_back(neighbor_vid);
                vlabel_list.push_back(this->data_ptr->vertices[neighbor_vid].label);
                elabel_list.push_back(edge_label);
                int tmp_v_num=vid_list.size();
                pair<int,int>* edge_pair = new pair<int,int>(tmp_v_num-1,0);
                edge_list.push_back(edge_pair);
            }
            else
            {
                // out edge
                rand_neighbor=rand_neighbor-core_indegree;
                Neighbor tmp_neighbor=this->data_ptr->vertices[core_vid].out[rand_neighbor];
                int neighbor_vid=tmp_neighbor.vid;
                int edge_label=tmp_neighbor.elb;
                vid_list.push_back(neighbor_vid);
                vlabel_list.push_back(this->data_ptr->vertices[neighbor_vid].label);
                elabel_list.push_back(edge_label);
                int tmp_v_num=vid_list.size();
                pair<int,int>* edge_pair = new pair<int,int>(0,tmp_v_num-1);
                edge_list.push_back(edge_pair);
            }
        }
        delete rand_neighbor_used;
        if(is_duplicate(query_set,vlabel_list,edge_list,elabel_list))
        {
            ++try_cnt;
            if(try_cnt>=max_try_cnt)
            {
                printf("%d times duplicate, no more trying for this query requirement\n",max_try_cnt);
                return;
            }
            --tmp_query_cnt;
            continue;
        }
        // get a query
        // write a query file
        
        string output_path = _query_dir + "/q" + Util::int2string(Star_match::query_count) + ".g";
        FILE* ofp = fopen(output_path.c_str(), "w+");
        fprintf(ofp, "t # %d\n", Star_match::query_count);
        Star_match::query_count++;
        int max_v_label = 0, max_e_label = 0;
        int query_v_num=vid_list.size();
        int query_e_num=edge_list.size();
        for (int i = 0; i < query_v_num; i ++)
            if (vlabel_list[i] > max_v_label)
                max_v_label = vlabel_list[i];
        for (int i = 0; i < query_e_num; i ++)
            if (elabel_list[i] > max_e_label)
                max_e_label = elabel_list[i];
        fprintf(ofp, "%d %d %d %d\n", query_v_num, query_e_num, max_v_label, max_e_label);
        for (int i = 0; i < query_v_num; i ++)
        {
            fprintf(ofp, "v %d %d\n", i, vlabel_list[i]);
        }
        for (int i = 0; i < query_e_num; i ++) 
        {
            fprintf(ofp, "e %d %d %d\n", edge_list[i]->first, edge_list[i]->second, elabel_list[i]);
        }

        fprintf(ofp, "t # -1\n");
        fclose(ofp);

        for (int i = 0; i < query_e_num; i ++)
            delete  edge_list[i];
        edge_list.clear();






    }











}

