

#include "Rand_match.h"
#include <utility>

using namespace std;

int Rand_match::query_count=0;

int get_random_int(int _range)
{
    srand((unsigned)time(NULL)); 
    return rand()%_range;
}


Rand_match::Rand_match()
{

}

Rand_match::Rand_match(int _node_num,char _sparse_type,int _sparse_ratio, int _query_req_num, Graph* _data_graph)
{
    this->qnode_num=_node_num;
    this->sparse_type=_sparse_type;
    this->sparse_ratio=_sparse_ratio;
    this->query_req_num=_query_req_num;
    this->data_ptr=_data_graph;
}

Rand_match::~Rand_match()
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
Rand_match::is_duplicate(std::vector<int*>& _query_set, std::vector<int>& _vlabel_list, std::vector<std::pair<int,int>*>& _edge_list, std::vector<int>& _elabel_list)
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

void Rand_match::match(std::string _query_dir)
{
    int data_v_num=this->data_ptr->vSize();
    if(this->qnode_num>data_v_num)
    {
        printf("error: query size exceeds data size\n");
        return;
    }
    for(i=0;i<this->query_req_num;++i)
        this->one_match(_query_dir);
}

void Rand_match::one_match(std::string _query_dir)
{

    int data_v_num=this->data_ptr->vSize();
    int max_try_cnt=10000;

    for(int try_cnt=0;try_cnt<max_try_cnt;++try_cnt)
    {
        std::vector<int> vid_list;
        std::vector<int> vlabel_list;
        std::vector<pair<int,int>*> edge_list;
        std::vector<int> elabel_list;

        bool* dv_visited_list=new bool [data_v_num];
        for(int i=0;i<data_v_num;++i)
            dv_visited_list[i]=false;

        int first_vid=get_random_int(data_v_num);
        vid_list.push_back(first_vid);
        vlabel_list.push_back(this->data_ptr->vertices[first_vid].label);
        dv_visited_list[first_vid]=true;

        bool pattern_found=true;
        for(int qv_cnt=1;qv_cnt<this->qnode_num;++qv_cnt)
        {
            int rand_start_pos=get_random_int(qv_cnt);
            int rand_start_vid=vid_list[rand_start_pos];
            int nei_num=this->data_ptr->vertices[rand_start_vid].degree;
            int rand_nei_pos=get_random_int(nei_num);
            int nei_vid=this->data_ptr->vertices[rand_start_vid].nei_list[rand_nei_pos].vid;
            int remain_loop=nei_num-1;
            while(dv_visited_list[nei_vid]&&remain_loop!=0)
            {
                rand_nei_pos=(rand_nei_pos+1)%nei_num;
                nei_vid=this->data_ptr->vertices[rand_start_vid].nei_list[rand_nei_pos].vid;
                --remain_loop;
            }
            if(dv_visited_list[nei_vid])
            {
                pattern_found=false;
                break;
            }
            dv_visited_list[nei_vid]=true;
            vid_list.push_back(nei_vid);
            vlabel_list.push_back(this->data_ptr->vertices[nei_vid].label);
        }
        delete []dv_visited_list;

        this->get_edges()

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
        fprintf(ofp, "%d %d %d\n", query_v_num, query_e_num, max_v_label);
        for (int i = 0; i < query_v_num; i ++)
        {
            fprintf(ofp, "v %d %d\n", i, vlabel_list[i]);
        }
        for (int i = 0; i < query_e_num; i ++) 
        {
            fprintf(ofp, "e %d %d\n", edge_list[i]->first, edge_list[i]->second);
        }

        fprintf(ofp, "t # -1\n");
        fclose(ofp);

        for (int i = 0; i < query_e_num; i ++)
            delete  edge_list[i];
        edge_list.clear();






    }











}

