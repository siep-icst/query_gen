/*=============================================================================
# Filename: Graph.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 23:01
# Description: 
=============================================================================*/

#include "Graph.h"

using namespace std;


Graph::Vertex()
{
	label = -1;
	indegree=0;
	outdegree=0;
	degree=0;
}
Graph::Vertex(LABEL lb):label(lb)
{
	indegree=0;
	outdegree=0;
	degree=0;
}


void 
Graph::addVertex(LABEL _vlb)
{
	this->vertices.push_back(Vertex(_vlb));
}

void 
Graph::addEdge(VID _from, VID _to, LABEL _elb)
{
	this->vertices[_from].out.push_back(Neighbor(_to, _elb));
	this->vertices[_to].in.push_back(Neighbor(_from, _elb));
	++this->vertices[_from].outdegree;
	++this->vertices[_from].degree;
	++this->vertices[_to].indegree;
	++this->vertices[_to].degree;
}

