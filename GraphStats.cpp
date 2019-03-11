#include "GraphUtils.h"
#include "Graph.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"
#include "GraphStats.h"
#include <stdio.h>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <bits/stdc++.h>

GraphStats::GraphStats(CompressedGraph* G)
{
	C = G;
	_num_nodes = C->numNodes();
	claw_map = new std::unordered_map<int, int>;
	degree_map = new std::unordered_map<int, int>;

	int i;

	for(i = 0; i < _num_nodes; i++)
	{
		int claw_key = C->numClaws(i);
		int degree_key = C->numNeighbours(i);

		std::unordered_map<int, int>::iterator it = claw_map->find(claw_key);
		if(it == claw_map->end())
		{
			claw_map->insert(std::make_pair(claw_key, 1));
		}
		else
		{
			it->second++;
		}

		it = degree_map->find(degree_key);
		if(it == degree_map->end())
		{
			degree_map->insert(std::make_pair(degree_key, 1));
		}
		else
		{
			it->second++;
		}	
	}
}

void GraphStats::outputClaws()
{
	std::unordered_map<int, int>::iterator it;

	for(it = claw_map->begin(); it != claw_map->end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}
}

void GraphStats::outputDegree()
{
	std::unordered_map<int, int>::iterator it;

	for(it = degree_map->begin(); it != degree_map->end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}
}

void GraphStats::outputStats()
{
	std::cout << "CLAWS" << endl;
	GraphStats::outputClaws();
	std::cout << "DEGREE" << endl;
	GraphStats::outputDegree();
}

void GraphStats::outputMatrix()
{
	int i, j;

	std::cout << endl;
	for(i = 0; i < _num_nodes; i++)
	{
		for(j = 0; j < _num_nodes; j++)
		{
			std::cout << C->adjacencyMatrix()[i][j] << " ";
		}

		std::cout << endl;
	}
}

void GraphStats::outputAdjList()
{
	int i, j;

	std::cout << endl;
	int *_num_neighbours = C->arrayNumNeighbours();

	for(i = 0; i < _num_nodes;  i++)
	{
		std::cout << i << ": ";
		for(j = 0; j < _num_neighbours[i]; j++)
		{
			std::cout << (*C->neighbours(i))[j] << " ";
		}

		std::cout << endl;
	}
}

void GraphStats::compressionRate()
{
	int i;
	int compressed_nodes = 0, orig;
	float ratio, rate;
	for(i = 0; i < _num_nodes;i++)
	{
		compressed_nodes += C->numClaws(i);
	}

	orig = _num_nodes + compressed_nodes;

	if(orig == 0) 
		return;

	ratio = float(compressed_nodes)/float(orig);
	rate = 1 - ratio;
	std::cout << orig << " " << compressed_nodes << " " << ratio << " " << rate; 
}



