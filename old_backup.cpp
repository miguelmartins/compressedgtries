bool finger_test(Graph *g, int dimensions, int cand, bool *visited)
{
	int count = 0;
	int i;

	for(i = 0; i < dimensions; i++)
	{
		if(g->adjacencyMatrix()[cand][i] != 0)
		{
			++count;
			if(count > 1) return false;
		}
	}

	/* If it has degree 1, we need not visit it again, since it is a peripheral node.*/
	visited[cand] = true;
	return true;
}

void claw_test(Graph *g, int dimensions, int cand, bool *visited)
{
	int num_fingers = 0;
	list<int> fingers_list;
	/* NOTE: Having the info about the degree of each node can accelerate the
	        finger test to O(1). 
	   What should we do in the case (1) - - - (2)                         */
	for(int i = 0; i < dimensions; i++)
	{
		if(i != cand)
		{
			if(g->adjacencyMatrix()[cand][i] != 0)
			{
				if(finger_test(g, dimensions, i, visited))
				{
					++num_fingers;
					fingers_list.push_back(i+1);
				}
			}
		}
	}

	/*OUTPUT: SEED_NODE SIZE_OF_CLAW FINGER1 ... FINGERN*/
	if(num_fingers > 0)
	{
		std::cout << cand + 1 << " " << num_fingers << " ";
		while(!fingers_list.empty())
		{
			std::cout << fingers_list.front() << " ";
			fingers_list.pop_front();
		}

		std::cout << endl;
	}
}

void bfsClawDetector(Graph *g, int dimensions, int curr, bool *visited)
{
	/*BF traversal of a graph. Each visited node is a candidate for being a claw. */
	
	list<int> candidateq;

	visited[curr] = true;
	candidateq.push_back(curr);

	list<int>::iterator it;

	while(!candidateq.empty())
	{
		curr = candidateq.front();

		/* Checks and outputs if current node is a claw. */
		claw_test(g, dimensions, curr, visited);

		candidateq.pop_front();

		for(int i = 0; i < dimensions; ++i)
		{
			if(i != curr && !visited[i] && g->adjacencyMatrix()[curr][i] != 0)
			{
				visited[i] = true;
				candidateq.push_back(i);
			}
		}
	}


}

 /* ./clawfinder < "./facebook.txt" */

/*
int main(int argc, char* argv[])
{
	GraphMatrix graph;
	int dimensions;

	if(argc < 2)
	{
		std::cerr << "Please specify file location. ";
	}

	GraphUtils::readFileTxt(&graph, argv[1], false, true);

	int i;

	dimensions = graph.numNodes();

	bool *visited = new bool[dimensions];

	/* Initializes visited array*/
	/*for (i = 0; i < dimensions; ++i)
		visited[i] = false;

	/* Does the BFS search for all connected components of the Network. */
	/*
	for (i = 0; i < dimensions; i++)
	{
		if(!visited[i])
			bfsClawDetector(&graph, dimensions, i, visited);
	}

	return 0;
}
*/