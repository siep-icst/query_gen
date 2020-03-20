

#ifndef _STAR_MATCH_MATCH_H
#define _STAR_MATCH_MATCH_H

#include "../util/Util.h"
#include "../graph/Graph.h"
#include "../io/IO.h"

class Rand_match
{
public:
    static int query_count;
    Rand_match();
    Rand_match(int _node_num,char _sparse_type, int _sparse_ratio, int _query_req_num, Graph* _data_graph);
    ~Rand_match();
    void match(std::string _query_dir);
    void one_match(std::string _query_dir);
    bool is_duplicate(std::vector<int*>& query_set, std::vector<int>& vlabel, std::vector<std::pair<int,int>*>& edges, std::vector<int>& elabel);
    void get_edges(std::vector<int>)

private:
    int qnode_num;
    int sparse_ratio;
    int query_req_num;
    char sparse_type;
    Graph* data_ptr;
    std::vector<int*> query_set;
};

#endif

