#ifndef _LINE_MATCH_H
#define _LINE_MATCH_H

#include "../util/Util.h"
#include "../graph/Graph.h"
#include "../io/IO.h"

class Line_match
{
public:
    static int query_count;
	Line_match(int queryNodeNum, int queryEdgeNum, int queryNum, Graph* _data);
	void match(IO& io);
	void match(std::string _query_dir);

	~Line_match();

private:
	int qsize, dsize;
	Graph* query;
	Graph* data;
	int edgeNum;
    int queryNum;    //generate how many queries in this file

    bool isDuplicate(std::vector<int*>&, std::vector<int>&, std::vector<std::pair<int,int>*>&, std::vector<int>&);
	// int get_RandStartId(int _tmp_query_len);
	// int get_LineStartId(int _tmp_query_len);
};

#endif