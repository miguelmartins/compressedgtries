#include "GraphMatrix.h"
#include "GraphUtils.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"
#include "Graph.h"
#include "GraphStats.h"
#include <iostream>
#include <list>

int main(int argc, char* argv[])
{
	GraphMatrix graph;
	int dimensions;
	bool settings;

	if(argc < 2)
	{
		std::cerr << "Please specify file location. ";
	}

	if(argc < 3)
	{
		std::cerr << " Please state if it is weighted(true) or not.";
	}


	if(std::string(argv[2]).compare("true") == 0) 
	{
		std::cout << "Weighted." << endl;
		settings = true; 
	}
	else
	{
		settings = false;

	} 
		
	GraphUtils::readFileTxt(&graph, argv[1], false, settings);

	int i, j;

	dimensions = graph.numNodes();


	GraphCompressor compressor;
	CompressedGraph *C = compressor.compressGraph(&graph);


	GraphStats *stats = new GraphStats(C);
	
	stats->compressionRate();
	int max = C->numNodes();

	


	std::cout << endl;

	return 0;
}