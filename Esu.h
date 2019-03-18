/* -------------------------------------------------
      _       _     ___                            
 __ _| |_ _ _(_)___/ __| __ __ _ _ _  _ _  ___ _ _ 
/ _` |  _| '_| / -_)__ \/ _/ _` | ' \| ' \/ -_) '_|
\__, |\__|_| |_\___|___/\__\__,_|_||_|_||_\___|_|  
|___/                                          
    
gtrieScanner: quick discovery of network motifs
Released under Artistic License 2.0
(see README and LICENSE)

Pedro Ribeiro - CRACS & INESC-TEC, DCC/FCUP

----------------------------------------------------
Esu implementation

Last Update: 11/02/2012
---------------------------------------------------- */

#ifndef _ESU_
#define _ESU_

#include "Common.h"
#include "GraphTree.h"
#include "CompressedGraph.h"
#include "GraphCompressor.h"

typedef struct candidate{
  int claw;
  int k;
  int label;
}candidate;

/*! This class implements the ESU subgraph enumeration algorithm */
class Esu {
 private:
  static int *_current;
  static int * _ext;
  static int _next;
  static int _graph_size;
  static int _motif_size;
  static int claw_memory;
  static Graph * _g;
  static GraphTree *_sg;
  static double *_prob;

  static CompressedGraph *c;
  static CompressedGraph *x;
  static int count;
  static candidate *ext;
  static candidate *current;
  static int *claw_seq;
  static int *claw_start_index;

  static void _go(int n, int size, int next, int *ext);
  static void _goSample(int n, int size, int next, int *ext);

  static unsigned nChoosek(unsigned n, unsigned k);
  static void compressedGo(candidate n, int size, int actual_size, int next, candidate *ext);
  static int min(int a, int b);

 public:
  static void countSubgraphs(Graph *g, int k, GraphTree *sg);
  static void countSubgraphsSample(Graph *g, int k, GraphTree *sg, double *p);
  static void compressedCountSubgraphs(CompressedGraph *g, int k, GraphTree *sg);
  
};

#endif
