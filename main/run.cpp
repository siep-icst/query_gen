/*=============================================================================
# Description: for line query pattern
=============================================================================*/
#include "../util/Util.h"
#include "../io/IO.h"
#include "../io/IO_data.h"
#include "../graph/Graph.h"
#include "../match/Match.h"
#include "../patterns/Query_patterns.h"
#include <utility>


using namespace std;

//NOTICE:a pattern occurs in a graph, then support++(not the matching num in a graph), support/N >= minsup
vector<int> node_list;
vector<int> edge_list;
vector<int> query_list;




int
main(int argc, const char * argv[])
{

	string query_dir = "query";
	if(argc > 4 || argc < 3)
	{
		cerr<<"invalid arguments!"<<endl;
		return -1;
	}
    //data file
	string data_path = argv[1];
    //query requirement file
	string query_req_path = argv[2];
	if(argc == 4)
	{
		query_dir = argv[3];
	}

	cerr<<"args all got!"<<endl;
	long t1 = Util::get_cur_time();
    
	//initialize line_query with query requirement
	Line_query line_query(query_req_path,query_dir);
	// fill this three list
	line_query.get_req_list(node_list,edge_list,query_list);


	int qnum = query_list.size();
	


	//read data graph
	Graph* data_graph = NULL;
	IO_data io_data=IO_data(data_path);
	
	// can read more than one data graph, and random walk on each graph to generate query
	while(true)
	{
		if(!io_data.get_data_graph(data_graph))
		{
			break;
		}
		cout << "one dataset read done!" << endl;
		for(int i = 0; i < qnum; ++i)
		{
			print("in query requirement txt, there are %d different requirement\n", qnum);
			//initialize
			Match m(node_list[i], edge_list[i], query_list[i], data_graph);
			//random walk to find match
			// generate query and put into this dir
            m.match(query_dir);
		}
		
		print("in this data graph, all querys are done\n");
		delete data_graph;
	}

//	cerr<<"match ended!"<<endl;
//	long t3 = Util::get_cur_time();

	//output the time for contrast
//	cerr<<"part 1 used: "<<(t2-t1)<<"ms"<<endl;
//	cerr<<"part 2 used: "<<(t3-t2)<<"ms"<<endl;
//	cerr<<"total time used: "<<(t3-t1)<<"ms"<<endl;

	//release all and flush cached writes
	//io.flush();

	return 0;
}

