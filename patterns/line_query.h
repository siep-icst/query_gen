
#ifndef _IO_IO_H
#define _IO_IO_H

#include "../util/Util.h"
#include "../graph/Graph.h"

class line_query
{
    line_query();
	line_query(std::string query_req_path, std::string data_path, std::string query_dir);

    //to use previous IO class, need to generate corresponding data
    bool req_gen(std::vector<int>& node_list, std::vector<int>& edge_list, std::vector<int>& query_list);


    private:
    std::string segreg_line;
	// std::string line;
	int data_id;
	//query file pointer
	FILE* query_req_ptr;
	//data file pointer
	FILE* data_ptr;
	//output file pointer
	FILE* query_ptr;
    std::string output_directory;
}