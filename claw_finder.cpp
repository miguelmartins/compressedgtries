#include "GraphMatrix.h"
#include "GraphUtils.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"
#include "Graph.h"
#include "GraphStats.h"
#include "GraphTree.h"
#include "Esu.h"
#include <iostream>
#include <list>

bool  Global::show_occ;
FILE *Global::occ_file;

int main(int argc, char* argv[])
{
	GraphMatrix graph, graph2;
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
	GraphUtils::readFileTxt(&graph2, argv[1], false, settings);
	int i, j;

	dimensions = graph.numNodes();

	int motif_size = 3;

	GraphTree sg;
	GraphTree sg2;
	
  	
	GraphCompressor compressor;
	CompressedGraph *C = compressor.compressGraph(&graph);

	C->sortNeighbours();
	C->makeArrayNeighbours();
	
	std::cout << endl;

	std::cout << endl << "*****COMPRESSED*****" << endl;
	Esu::compressedCountSubgraphs(C, motif_size, &sg2);
	

	graph2.sortNeighbours();
	graph2.makeArrayNeighbours();
	std::cout << "Original" << endl;
	Esu::countSubgraphs(&graph2, motif_size, &sg);
	printf("%d subgraphs, ",   sg.countGraphs());
  	printf("%.0f occurrences\n", sg.countOccurrences());
	

	GraphStats *stats = new GraphStats(C);
	CompressedGraph *X = compressor.decompressGraph(C);

	std::cout << graph2.numNodes()- X->numNodes() << endl;

	stats->compressionRate();
	int max = C->numNodes();


	std::cout << endl;

	return 0;
}