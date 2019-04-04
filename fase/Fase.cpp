/* -------------------------------------------------

//                                                 
//  88888888888           ad88888ba   88888888888  
//  88                   d8"     "8b  88           
//  88                   Y8,          88           
//  88aaaaa  ,adPPYYba,  `Y8aaaaa,    88aaaaa      
//  88"""""  ""     `Y8    `"""""8b,  88"""""      
//  88       ,adPPPPP88          `8b  88           
//  88       88,    ,88  Y8a     a8P  88           
//  88       `"8bbdP"Y8   "Y88888P"   88888888888  
//                                                 
//

Pedro {Paredes, Ribeiro} - DCC/FCUP

----------------------------------------------------
Base FaSE implementation

---------------------------------------------------- */

#include "Fase.h"

int Fase::K;
long long int Fase::MotifCount = 0;
int Fase::typeLabel;
bool Fase::directed;
Graph* Fase::G;
int *Fase::sub;
int Fase::subNum;
int Fase::graphSize;
int **Fase::extCpy;
char Fase::globStr[MAXS];
double Fase::samp_prob[MAXMOTIF];

long long int Fase::getTypes()
{
  return GTrie::getCanonicalNumber();
}

long long int Fase::getLeafs()
{
  return GTrie::getClassNumber();
}

long long int Fase::getNodes()
{
  return GTrie::getNodeNumber();
}

void Fase::listTree(FILE* f)
{
  return GTrie::listGtrie(f);
}

void Fase::listClasses(FILE* f, double prob)
{
  return GTrie::listClasses(f, prob);
}

/*! Generate the next piece of the LSLabel
    \param w the newly added vertex*/
char* Fase::LSLabel(int w, int subSize)
{
  return LSLabeling::Label(sub, subSize, w, typeLabel, globStr, directed);
}

void Fase::destroy()
{
  int i;
  GTrie::destroy();
  delete[] sub;
  for (i = 0; i < K; i++)
    delete[] extCpy[i];
  delete [] extCpy;
}

/*! List all k-subgraphs in a larger graph
    \param G the graph to be explored
    \param K the size of the subgraphs*/
void Fase::EnumerateSubgraphs(Graph *_G, int _K)
{
  K = _K;
  G = _G;
  sub = new int[K];
  graphSize = G->numNodes();
  GTrie::init();
  LSLabeling::init(G);
  int i, j, ext[graphSize], extNum = 0;
  extCpy = new int*[K];

  for (i = 0; i < K; i++)
    extCpy[i] = new int[graphSize];

  for (i = 0; i < graphSize; i++)
    if (Random::testProb(samp_prob[0]))
    {
      sub[0] = i;
      int *nei = G->arrayNeighbours(i);
      int neiNum = G->numNeighbours(i);
      extNum = 0;
      for (j = 0; j < neiNum; j++)
	       if (nei[j] > i)
	         ext[extNum++] = nei[j];
      ExtendSubgraph(ext, extNum, 1, i);
    }
  printf("LS-Classes: %lld\n", GTrie::getClassNumber());
  GTrie::listGtrie(stdout);
  GTrie::listClasses(stdout, 1);
  //  printf("\t\tWith G-Tries:\n");
  //   printf("Found %d Motifs\n", MotifCount);
  //  printf("LS-Classes: %lld\n", GTrie::getClassNumber());
  //  GTrie::listClasses();
  //  GTrie::listGtrie();
}

/*! Generates the next node on the Fase-tree
    \param ext the list of possible next chosen vertices
    \param extNum the number of elements in ext
    \param subSize the size of the enumerating subgraph
    \param v the label of the base vertex*/
void Fase::ExtendSubgraph(int *ext, int extNum, int subSize, int v)
{
  int i;

  if (subSize == K - 1)
  {
    for (i = 0; i < extNum; i++)
      if (Random::testProb(samp_prob[subSize]))
      {
      	char* addLabel = LSLabel(ext[i], subSize);
      	GTrie::insert(addLabel);
      	sub[subSize] = ext[i];

      	MotifCount++;

      	if (!GTrie::getCurrentLeaf())
        {
      	  char s[K * K + 1];
      	  s[0] = '\0';
      	  Isomorphism::canonicalStrNauty(G, sub, s);
      	  GTrie::setCanonicalLabel(s);
      	}

      	GTrie::jump();    
      }
    return;
  }


  int j, o;
  int extCpyNum;

  for (j = 1; j < extNum; j++)
    extCpy[subSize][extNum - j - 1] = ext[j];

  for (i = 0; i < extNum; i++)
    if (Random::testProb(samp_prob[subSize]))
    {
      extCpyNum = extNum - i - 1;
      int *eExcl = G->arrayNeighbours(ext[i]);
      int eExclNum = G->numNeighbours(ext[i]);
	    
      for (j = 0; j < eExclNum; j++)
      {
      	if (eExcl[j] <= v)
      	  continue;
      	for (o = 0; o < subSize; o++)
      	  if (eExcl[j] == sub[o] || G->isConnected(eExcl[j], sub[o]))
      	    break;
      	if (o == subSize)
      	  extCpy[subSize][extCpyNum++] = eExcl[j];
      }
      
      char* addLabel = LSLabel(ext[i], subSize);
      GTrie::insert(addLabel);

      sub[subSize] = ext[i];

      ExtendSubgraph(extCpy[subSize], extCpyNum, subSize + 1, v);

      GTrie::jump();
    }

}
