#include "line_query.h"
using namespace std;


line_query::line_query()
{
	this->query_req_ptr = NULL;
	this->data_ptr = NULL;
	this->query_ptr = NULL;
    this->data_id=-1;
}

line_query::~line_query()
{
    fclose(this->query_req_ptr);
	this->dfp = NULL;
    fclose(this->data_ptr)
    this->data_ptr=NULL;
    if(this->query_ptr != NULL)
    {
        //NOTICE: fclose(NULL) will cause error, while fflush(NULL) is ok
        fclose(this->query_ptr);
        this->query_ptr = NULL;
    }
}

bool 
line_query::req_gen(std::vector<int>& node_list, std::vector<int>& edge_list, std::vector<int>& query_list)
{

}
