#include "GraphUtils.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"
#include <stdio.h>
#include <algorithm>

CompressedGraph* GraphCompressor::performCompression(Graph *g, int max, int *claws, int *visited)
{
	/* Creates graph of size |V \ P| where P is the periphery node set. */
	CompressedGraph *C = new CompressedGraph();
	C->createGraph(max, UNDIRECTED);

	int _num_nodes = g->numNodes();
	int *_num_neighbours = g->arrayNumNeighbours();

	int i, j;

	for(i = 0; i < _num_nodes; i++)
	{
		/* Visited also serves as the compressed labels lookup.*/
		if(visited[i] >= 0)
		{
			for(j = 0; j < _num_neighbours[i]; j++)
			{
				C->addEdge(visited[i], visited[(*g->neighbours(i))[j]]);
				C->addEdge(visited[(*g->neighbours(i))[j]], visited[i]);
			}

			C->setClaw(visited[i], claws[i]);
		}
	}	

	return C;

}

CompressedGraph* GraphCompressor::compressGraph(Graph *g)
{
	int i;
	int node_counter = 0;
	int _num_nodes = g->numNodes();

	int *_num_neighbours = g->arrayNumNeighbours();


	int *visited = new int[_num_nodes];
	int *claws = new int[_num_nodes];

	for(i = 0; i < _num_nodes; i++)
	{
		claws[i] = 0;
		visited[i] = -1;
	}

	for(i = 0; i < _num_nodes; i++)
	{
		/* Checks claws for special case of (a)---(b) */
		if(_num_neighbours[i] == 1  && claws[(*g->neighbours(i))[0]] != -1)
		{
			/* Mark as peripheral node. */
			claws[i] = -1;
			visited[i] = -1;

			/* Updates border node claw counter. */
			claws[(*g->neighbours(i))[0]]++;
			/* It only works for undirected graphs. */
		}
		else
		{
			/*Visite*/
			visited[i] = node_counter;
			/* Only accounts for nodes that are not peripheral. */ 
			node_counter++;
		}
	}

	for(i = 0; i < _num_nodes; i++)
	{
		if(claws[i] == -1)
		{
			g->rmEdge(i, (*g->neighbours(i))[0]);
			g->rmEdge((*g->neighbours(i))[0], i);
		}
	}

	return performCompression(g, node_counter, claws, visited);
}

CompressedGraph* GraphCompressor::decompressGraph(CompressedGraph *c)
{
	int original_size, size;
	int i, j, label_counter;
	int *_num_neighbours = c->arrayNumNeighbours();
	original_size = size = c->numNodes();

	for(i = 0; i < size; i++)
	{
		original_size += c->numClaws(i);
	}

    /* Compressed nodes will have labels >= |C| - 1*/
	label_counter = size;

    CompressedGraph *g = new CompressedGraph();

    g->createGraph(original_size, UNDIRECTED);

    for(i = 0; i < size; i++)
    {
    	/* Add peripheral nodes. */
		for(j = 0; j < c->numClaws(i); j++)
		{
			g->addEdge(i, label_counter);
			g->addEdge(label_counter, i);

			label_counter++;
		}	

    	/* Add connections to regular neighbours. */
    	for(j = 0; j < _num_neighbours[i]; j++)
    	{
    		g->addEdge(i, (*c->neighbours(i))[j]);
			g->addEdge((*c->neighbours(i))[j], i);
    	}
    	
    }

    std::cout << "Successfully decompressed " << original_size - size << " nodes!" << endl;
    return g;
}