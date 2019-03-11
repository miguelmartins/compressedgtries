#include "GraphUtils.h"
#include "CompressedGraph.h"
#include <stdio.h>
#include <algorithm>

CompressedGraph::CompressedGraph() {
  _init();
}

CompressedGraph::~CompressedGraph() {
  _delete();
}

// ------------------------------
// Graph Creation
// ------------------------------

void CompressedGraph::_init() {
  _num_nodes = _num_edges = 0;

  _adjM             = NULL;
  _adjOut           = NULL;
  _adjIn            = NULL;
  _neighbours       = NULL;  
  _in               = NULL;
  _out              = NULL;
  _num_neighbours   = NULL;
  _array_neighbours = NULL;
  claws 			= NULL; 
}

void CompressedGraph::_delete() {
  int i;

  if (_adjM!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_adjM[i]!=NULL) delete[] _adjM[i];
    delete[] _adjM;
  }
  if (_adjIn!=NULL) delete[] _adjIn;
  if (_adjOut!=NULL) delete[] _adjOut;
  if (_neighbours!=NULL) delete[] _neighbours;
  if (claws != NULL) delete[] claws;

  if (_in!=NULL) delete[] _in;
  if (_out!=NULL) delete[] _out;
  if (_out!=NULL) delete[] _num_neighbours;

  if (_array_neighbours!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_array_neighbours[i]!=NULL) delete[] _array_neighbours[i];
    delete[] _array_neighbours;
  }
}

void CompressedGraph::zero() {
  int i,j;
  _num_edges = 0;

  for (i=0; i<_num_nodes;i++) {
    _in[i] = 0;
    _out[i] = 0;
    _num_neighbours[i] = 0;
    claws[i] = 0;
    _adjIn[i].clear();
    _adjOut[i].clear();
    _neighbours[i].clear();
    
    for (j=0; j<_num_nodes;j++)
      _adjM[i][j]=false;
  }
}

void CompressedGraph::createGraph(int n, GraphType t) {
  int i;

  _num_nodes = n;
  _type = t;

  _delete();

  _adjM = new bool*[n];  
  for (i=0; i<n; i++) _adjM[i] = new bool[n];
  _adjIn      = new vector<int>[n];
  _adjOut     = new vector<int>[n];
  _neighbours = new vector<int>[n];
  
  claws 	  	  = new int[n];
  _in             = new int[n]; 
  _out            = new int[n];
  _num_neighbours = new int[n];

  zero();
}

void CompressedGraph::addEdge(int a, int b) {

  if (_adjM[a][b]) return;

  _adjM[a][b] = true;

  _adjOut[a].push_back(b);
  _out[a]++;

  _adjIn[b].push_back(a);
  _in[b]++;

  _num_edges++;

  if (!_adjM[b][a]) {
    _neighbours[a].push_back(b);
    _num_neighbours[a]++;
    _neighbours[b].push_back(a);
    _num_neighbours[b]++;
  }

}


void CompressedGraph::rmEdge(int a, int b) {
  
  if (!_adjM[a][b]) return;

  _adjM[a][b] = false;

  _removeVector(_adjOut[a], b);
  _out[a]--;

  _removeVector(_adjIn[b], a);
  _in[b]--;

  _num_edges--;

  if (!_adjM[b][a]) {
    _removeVector(_neighbours[a],b);
    _num_neighbours[a]--;
    _removeVector(_neighbours[b],a);
    _num_neighbours[b]--;
  }

  /*TODO: CLAWS*/
}

void CompressedGraph::_removeVector(vector<int> &v, int b) {
  int i, s = v.size();
  for (i=0; i<s; i++)
    if (v[i] == b) break;
  if (i<s) v.erase(v.begin()+i);
}

void CompressedGraph::sortNeighbours() {
  int i;
  for (i=0; i<_num_nodes; i++)
    sort(_neighbours[i].begin(), _neighbours[i].begin()+_neighbours[i].size());
}

void CompressedGraph::sortNeighboursArray() {
  int i;
  for (i=0; i<_num_nodes; i++)
    qsort(_array_neighbours[i], _num_neighbours[i], sizeof(int), GraphUtils::int_compare);
}

void CompressedGraph::makeArrayNeighbours() {
  int i,j;
  vector<int>:: iterator ii;
  _array_neighbours = new int*[_num_nodes];  
  for (i=0; i<_num_nodes; i++) {
    _array_neighbours[i] = new int[_neighbours[i].size()];
    for (ii=_neighbours[i].begin(), j=0; ii!=_neighbours[i].end(); ++ii, j++)
      _array_neighbours[i][j] = *ii;
    _neighbours[i].clear();
  }
}

void CompressedGraph::makeVectorNeighbours() {
  int i,j;
  vector<int>:: iterator ii;

  for (i=0; i<_num_nodes; i++)
    for (j=0; j<_num_neighbours[i]; j++)
      _neighbours[i].push_back(_array_neighbours[i][j]);

  if (_array_neighbours!=NULL) {
    for (i=0; i<_num_nodes; i++)
      if (_array_neighbours[i]!=NULL) delete[] _array_neighbours[i];
    delete[] _array_neighbours;
  }
}
