#include "GraphMatrix.h"
#include "GraphUtils.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"
#include "Graph.h"
#include "GraphStats.h"
#include "GraphTree.h"
#include "Esu.h"
#include "Timer.h"
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

	if(argc < 4)
	{
		std::cerr << " Please state motif size (>=3).";
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

	std::cout << endl; 
		
	GraphUtils::readFileTxt(&graph, argv[1], false, settings);
	GraphUtils::readFileTxt(&graph2, argv[1], false, settings);
	int i, j;

	dimensions = graph.numNodes();

	int motif_size = std::stoi(argv[3]);

	GraphTree sg;
	GraphTree sg2;
  	
	GraphCompressor compressor;
	Timer::start(0);
	CompressedGraph *C = compressor.compressGraph(&graph);

	C->sortNeighbours();
	C->makeArrayNeighbours();
	

	Esu::compressedFaSE(C, motif_size);

	Esu::compressedCountSubgraphs(C, motif_size, &sg2);
	Timer::stop(0);

	std::cout << endl << "*****COMPRESSED*****" << endl;
	printf("Time: %.2f\n", Timer::elapsed(0));
	printf("%d subgraphs, ",   sg2.countGraphs());
  	printf("%.0f occurrences\n", sg2.countOccurrences());

	Timer::start(0);
	graph2.sortNeighbours();
	graph2.makeArrayNeighbours();
	
	Esu::countSubgraphs(&graph2, motif_size, &sg);
	Timer::stop(0);
	std::cout << "Original" << endl;
	printf("Time: %.2f\n", Timer::elapsed(0));
	printf("%d subgraphs, ",   sg.countGraphs());
  	printf("%.0f occurrences\n", sg.countOccurrences());
	

	GraphStats *stats = new GraphStats(C);
	CompressedGraph *X = compressor.decompressGraph(C);


	stats->compressionRate();
	int max = C->numNodes();


	std::cout << endl;

	return 0;
}