/*=============================================================================
io data file, create data graph
=============================================================================*/



#include "../util/Util.h"
#include "../graph/Graph.h"

class IO_data
{
public:

	IO_data();
	IO_data(std::string data_path);
	~IO_data();

	bool get_data_graph(Graph*& data_graph);
	Graph* input(FILE* fp);
	bool output(int qid);
	bool output();
	bool output(int* m, int size);
	void flush();
    FILE* getOFP() const
    {
        return this->ofp;
    }


private:
	std::string line;
	//graph id
	int data_id;
	//data file pointer
	FILE* data_ptr;
};



