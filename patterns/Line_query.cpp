#include "Line_query.h"
using namespace std;


Line_query::Line_query()
{
	this->query_req_ptr = NULL;
	this->query_ptr = NULL;
    this->segreg_line = "============================================================";
}

Line_query::Line_query(std::string _query_req_path, std::string _query_dir)
{
	this->segreg_line = "============================================================";
	this->query_req_ptr=fopen(_query_req_path.c_str(),"r");

    this->query_dir = _query_dir;
    Util::create_dir(_query_dir);
}

Line_query::~Line_query()
{
    if(this->query_req_ptr!=NULL)
    {
        fclose(this->query_req_ptr);
	    this->query_req_ptr = NULL;
    }
    if(this->query_ptr != NULL)
    {
        //NOTICE: fclose(NULL) will cause error, while fflush(NULL) is ok
        fclose(this->query_ptr);
        this->query_ptr = NULL;
    }
}

bool 
Line_query::get_req_list(std::vector<int>& node_list, std::vector<int>& edge_list, std::vector<int>& query_list)
{
    //to generate what kind of query

	while (true)
	{
        FILE* query_req_f=this->query_req_ptr;
        int queryNodeNum;
		query_req_f >> queryNodeNum;
		if (queryNodeNum <=0) 
        {
			break;
		}

	    int queryEdgeNum = queryNodeNum-1;
        int queryNum;
        query_req_f >> queryNum;
		node_list.push_back(queryNodeNum);
		edge_list.push_back(queryEdgeNum);
        query_list.push_back(queryNum);
	}
	return true;
}
