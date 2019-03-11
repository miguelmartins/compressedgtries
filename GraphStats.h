#include "GraphUtils.h"
#include "Graph.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"
#include <stdio.h>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <bits/stdc++.h>

#ifndef _GRAPHSTATSH_
#define _GRAPHSTATSH_

class GraphStats{
	private:
		std::unordered_map<int, int> *claw_map;
		std::unordered_map<int, int> *degree_map;
		int _num_nodes;
		CompressedGraph *C;
	public:
		GraphStats(CompressedGraph* G);
		void outputStats();
		void outputClaws();
		void outputDegree();
		void outputMatrix();
		void outputAdjList();
		void compressionRate();

		
};

#endif