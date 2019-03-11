#include "Graph.h"
#include "GraphUtils.h"
#include "CompressedGraph.h"
#include <stdio.h>
#include <algorithm>

#ifndef _GRAPHCOMPRESSOR_
#define _GRAPHCOMPRESSOR_

class GraphCompressor{
	public:
		CompressedGraph* compressGraph(Graph *g);
		CompressedGraph* decompressGraph(CompressedGraph *c);
	private:
		CompressedGraph* performCompression(Graph *g, int max, int *claws, int *visited);

};

#endif	