

#include "Star_match.h"
#include <utility>

using namespace std;

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

void Star_match::match(std::string _query_dir)
{
    int data_size=this->data_ptr.vSize();
    int query_size=this->sate_num+1;
    if(query_size>data_size)
    {
        printf("error: query size exceeds data size\n");
        return;
    }

    std::vector<int*> query_set;

    //candidate id for core vertex
    std::vector<int> core_candidate;
    //minimum degree of core vertex
    int min_degree=this->sate_num;
    
    for(int v_id=0;v_id<data_size;++v_id)
    {
        int tmp_v_degree=this->data_ptr->vertices[v_id].degree;
        if(tmp_v_degree>=min_degree)
            core_candidate.push_back(v_id);
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
        std::vector<int> elabel;


    }










    string file = _query_dir + "/q" + Util::int2string(Match::query_count) + ".g";
    FILE* ofp = fopen(file.c_str(), "w+");
}

