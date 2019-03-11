#include "Graph.h"
#include "GraphUtils.h"
#include <stdio.h>
#include <algorithm>

#ifndef _COMPRESSEDGRAPH_
#define _COMPRESSEDGRAPH_


class CompressedGraph : public Graph {
 private:
  GraphType _type;

  int _num_nodes;
  int _num_edges;

  int *_in;
  int *_out;
  int *_num_neighbours;
  int *claws;


  bool **_adjM;
  int  **_array_neighbours;
  vector<int> *_adjOut;
  vector<int> *_adjIn;
  vector<int> *_neighbours;


  void _init();
  void _delete();
  void _removeVector(vector<int> &v, int b);

 public:
  CompressedGraph();
  ~CompressedGraph();

  bool **adjacencyMatrix() {return _adjM;}

  void createGraph(int n, GraphType t);

  GraphType type() {return _type;}

  void zero();


  int numNodes() {return _num_nodes;}
  int numEdges() {return _num_edges;}
  int numClaws(int i) {return claws[i];}

  void addEdge(int a, int b); // add edge from a to b
  void rmEdge(int a, int b);  // remove edge from a to b
  void setClaw(int a, int claw){claws[a] = claw;}


  bool hasEdge(int a, int b) {return _adjM[a][b];} 
  bool isConnected(int a, int b)  {return _adjM[a][b] || _adjM[b][a];}

  int nodeOutEdges(int a) {return _out[a];}
  int nodeInEdges(int a)  {return _in[a];}
  int numNeighbours(int a) {return _num_neighbours[a];}
  void sortNeighbours();
  void sortNeighboursArray();
  void makeArrayNeighbours();
  void makeVectorNeighbours();

  vector<int> *neighbours(int a) {return &_neighbours[a];}
  int **matrixNeighbours()       {return _array_neighbours;}
  int *arrayNeighbours(int a)    {return _array_neighbours[a];}
  int *arrayNumNeighbours()      {return _num_neighbours;}
  vector<int> *outEdges(int a)   {return &_adjOut[a];}
  vector<int> *inEdges(int a)    {return &_adjIn[a];}

  bool isPeripheral(int a) {return _num_neighbours[a] == 1 ? true : false;}
};

#endif