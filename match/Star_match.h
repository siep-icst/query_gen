

#ifndef _STAR_MATCH_MATCH_H
#define _STAR_MATCH_MATCH_H

#include "../util/Util.h"
#include "../graph/Graph.h"
#include "../io/IO.h"

class Star_match
{
public:
    static int query_count;
    Star_match();
    Star_match(int _node_num, int _edge_num, int _query_req_num, Graph* _data_graph);
    ~Star_match();
    void match(std::string _query_dir);
    bool is_duplicate(std::vector<int*>& query_set, vector<int>& vlabel, std::vector<std::pair<int,int>*>& edges, std::vector<int>& elabel);

private:
    int query_req_num;
    //satellite_num
    int sate_num;
    Graph* data_ptr;
};

#endif

