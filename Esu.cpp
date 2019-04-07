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

#include "Esu.h"
#include "Isomorphism.h"
#include "Random.h"

// Class static variables
int     Esu::_motif_size = 0;
int     Esu::_graph_size = 0;
int     Esu::_next = 0;
int    *Esu::_current = NULL;
int    *Esu::_ext = NULL;
Graph  *Esu::_g = NULL;
double *Esu::_prob;
GraphTree *Esu::_sg;

CompressedGraph *Esu::c = NULL;
CompressedGraph *Esu::x = NULL;

long long int    Esu::count = 0;
candidate *Esu::ext = NULL;
candidate *Esu::current = NULL;

int     Esu::claw_memory = 0;

int *Esu::claw_seq = NULL;
int *Esu::claw_start_index = NULL;
char Esu::globStr[MAXS];
int  Esu::typeLabel = LSLabeling::TYPE_PICK;


char* Esu::LSLabel(int w, int subSize)
{
  return LSLabeling::Label(_current, subSize, w, typeLabel, globStr, false);
}

/*! Recursively extend a partial subgraph
    \param n the current position in the constructed subgraph 
    \param size the subgraph size
    \param next number of elements in the list "ext"
    \param ext of the nodes that can be used to extend the subgraph */
void Esu::_go(int n, int size, int next, int *ext) {
  _current[size++] = n;

  /*
  std::cout << "Curr array ";
  for(int p = 0; p < size-1; p++) std::cout << _current[p] << " "; std::cout << endl;
  */

  if (size==_motif_size) {

    /*Constrói occcurencia*/
    char s[_motif_size*_motif_size+1];
    Isomorphism::canonicalStrNauty(_g, _current, s);

    /* Incrementa. */
    _sg->incrementString(s);

  } else {
    int i,j;
    int next2 = next;
    int ext2[_graph_size];

    for (i=0;i<next;i++) ext2[i] = ext[i];

    int *v  = _g->arrayNeighbours(_current[size-1]);
    int num = _g->numNeighbours(_current[size-1]);


    for (i=0;i<num;i++) 
    {
      /*Ver se w >= v*/
      if (v[i]<=_current[0]) 
        continue;
      /*Neighbourhood exclusiva*/
      for (j=0;j+1<size;j++)
        if (_g->isConnected(v[i],_current[j])) 
          break;

      /*Se nao falhou no for adiciona-o à vizinhança*/
      if (j+1==size) 
        ext2[next2++]=v[i];      
    }
    /*Faz a recursão para todos os candidatos válidos*/
    while (next2>0) {      
      next2--;
      _go(ext2[next2], size, next2, ext2);
    }
  }
}


/*! Recursively extend a partial compressed subgraph
    \param n the current position in the constructed subgraph 
    \param size the subgraph size
    \param next number of elements in the list "ext"
    \param ext of the nodes that can be used to extend the subgraph */
void Esu::compressedGo(candidate n, int size, int actual_size, int next, candidate *ext, int total_occurences) {

  current[size].label = n.label;
  current[size].claw = n.claw;
  current[size].k = n.k;

  if(current[size].claw == 0)
  {
    _current[actual_size] = n.label;
    total_occurences *= 1;
    actual_size += 1;
  }
  else
  {
    int start = actual_size;
    actual_size += current[size].k;
    for(int j = 0; j < current[size].k ; j++)
    {
       _current[start + j] = claw_start_index[current[size].label] + j;
    }

    total_occurences *= nChoosek(current[size].claw, current[size].k);
  }

  size++;


  if (actual_size == _motif_size) {
    /*Constrói occcurencia*/

    char s[_motif_size*_motif_size+1];

    Isomorphism::compressedCanonicalStrNauty(x, _current, s);
    _sg->addString(s, total_occurences);
    /*count += total_occurences;*/

  } else {
    int i,j;
    int next2 = next;
    candidate ext2[_graph_size + claw_memory];

    for (i=0;i<next;i++)
    {
     ext2[i].claw = ext[i].claw;
     ext2[i].k = ext[i].k;
     ext2[i].label = ext[i].label;
    }

    if(current[size - 1].claw == 0)
    {
      int *v  = c->arrayNeighbours(current[size-1].label);
      int num = c->numNeighbours(current[size-1].label);

      for (i=0; i<num; i++) 
      {
        /*Ver se w >= v*/
        if (v[i] <= current[0].label) 
        {
          continue;
        }
        /*Neighbourhood exclusiva*/

        for (j=0; j+1<size; j++)
          if (c->isConnected(v[i],current[j].label)) 
            break;

        /*Se nao falhou no for adiciona-o à vizinhança*/
        if (j+1 == size)
        {
          ext2[next2].label = v[i];
          ext2[next2].claw = 0;
          ext2[next2].k = 0;
          next2++; 
        }   
      }

      if(c->numClaws(current[size - 1].label) > 0)
      {
         int remain_size;

         remain_size = _motif_size - actual_size;
         /*Podemos gerar as combinacoes ate no maximo o tamanho da claw*/
         int bound = min(remain_size, c->numClaws(current[size - 1].label));


         for(int comb = 1; comb <= bound; comb++)
         {
           ext2[next2].label = current[size - 1].label;
           ext2[next2].claw = c->numClaws(current[size - 1].label);
           ext2[next2].k = comb;
           compressedGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
         }
      }

      /*Faz a recursão para todos os candidatos válidos*/
      while (next2 > 0) {     
        next2--;
        compressedGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
      }
    }

    /*É um nó do tipo claw, logo não tem vizinhos novos*/
    else
    {
       while (next2 > 0) {      
        next2--;
        compressedGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
      }
    }
  }
}

void Esu::compressedCountSubgraphs(CompressedGraph *g, int k, GraphTree *sg) {
  int i;
  int next = 0;
  GraphCompressor compressor;
  _motif_size = k;
  _graph_size = g->numNodes();
  current = new candidate[k];
  _current = new int[k];
  
  _next = 0;
  c = g;
  _sg = sg;

  /*In the worst case we have to generate every combination for every claw.*/
  for(i = 0; i <_graph_size; i++)
  {
    if(c->numClaws(i) > 0)
    {
      claw_memory += c->numClaws(i);
    }
  }

  ext = new candidate[_graph_size + claw_memory];

  x = compressor.decompressGraph(c);

  claw_seq = new int[c->numNodes()];
  claw_start_index = new int[c->numNodes()];

  int accum = 0;
  int total = c->numNodes();
  
  for(i = 0; i < total; i++)
  {
    claw_seq[i] = c->numClaws(i);
    if(claw_seq[i] > 0)
    {
      claw_start_index[i] = total + accum;
      accum += claw_seq[i];
    }
    else
    {
      claw_start_index[i] = 0;
    }
  }

  Isomorphism::initNauty(k, false);

  sg->zeroFrequency();

 for (i = 0; i < _graph_size; i++)
 {
    candidate q;
    q.label = i;
    q.claw = 0;
    q.k = 0;
    candidate v[1]; 
    compressedGo(q, 0, 0, 0, v, 1);
 }

  count = 0;
  delete[] current;
  delete[] ext;
  delete[] claw_seq;
  delete[] claw_start_index;
}


/*! Recursively extend a partial compressed subgraph
    \param n the current position in the constructed subgraph 
    \param size the subgraph size
    \param next number of elements in the list "ext"
    \param ext of the nodes that can be used to extend the subgraph */
void Esu::compressedFaSEGo(candidate n, int size, int actual_size, int next, candidate *ext, int total_occurences) {

  current[size].label = n.label;
  current[size].claw = n.claw;
  current[size].k = n.k;

  if(current[size].claw == 0)
  {
    _current[actual_size] = n.label;
    total_occurences *= 1;
    /*Só inserimos para subgrafos de k >=2 .*/
    if(actual_size > 0)
    {
      char* addLabel = LSLabel(_current[actual_size], actual_size); 
      CustomGTrie::compressedInsert(addLabel, total_occurences);
    }
    actual_size += 1;
  }
  else
  {
    int start = actual_size;
    actual_size += current[size].k;
    total_occurences *= nChoosek(current[size].claw, current[size].k);
    for(int j = 0; j < current[size].k; j++)
    {
       _current[start + j] = claw_start_index[current[size].label] + j;
    }

    char* addLabel = LSLabel(_current[actual_size - 1], actual_size - 1);
    CustomGTrie::compressedInsert(addLabel, total_occurences);
  }

  size++;


  if (actual_size == _motif_size) {
    /*Constrói occcurencia*/
    count += total_occurences;
    if(!CustomGTrie::getCurrentLeaf())
    {
      char s[_motif_size*_motif_size+1];
      s[0] = '\0';
      Isomorphism::compressedCanonicalStrNauty(x, _current, s);
      CustomGTrie::compressedSetCanonicalLabel(s, total_occurences);
    }

  } else {
    int i,j;
    int next2 = next;
    candidate ext2[_graph_size + claw_memory];

    for (i=0;i<next;i++)
    {
     ext2[i].claw = ext[i].claw;
     ext2[i].k = ext[i].k;
     ext2[i].label = ext[i].label;
    }

    if(current[size - 1].claw == 0)
    {
      int *v  = c->arrayNeighbours(current[size-1].label);
      int num = c->numNeighbours(current[size-1].label);

      for (i=0; i<num; i++) 
      {
        /*Ver se w >= v*/
        if (v[i] <= current[0].label) 
        {
          continue;
        }
        /*Neighbourhood exclusiva*/

        for (j=0; j+1<size; j++)
          if (c->isConnected(v[i],current[j].label)) 
            break;

        /*Se nao falhou no for adiciona-o à vizinhança*/
        if (j+1 == size)
        {
          ext2[next2].label = v[i];
          ext2[next2].claw = 0;
          ext2[next2].k = 0;
          next2++; 
        }   
      }

      if(c->numClaws(current[size - 1].label) > 0)
      {
         int remain_size;

         remain_size = _motif_size - actual_size;
         /*O tamanho restante do motif é o majorante para o número de combinações possíveis.*/
         int bound = min(remain_size, c->numClaws(current[size - 1].label));


         for(int comb = 1; comb <= bound; comb++)
         {
           ext2[next2].label = current[size - 1].label;
           ext2[next2].claw = c->numClaws(current[size - 1].label);
           ext2[next2].k = comb;
           compressedFaSEGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
           /* Coloca o ponteiro da GTrie no vértice periférico. */
         }

         for(int jmp = 0; jmp < bound; jmp++)
            CustomGTrie::jump();
         
      }

      /*Faz a recursão para todos os candidatos válidos*/
      while (next2 > 0) {     
        next2--;
        compressedFaSEGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
        CustomGTrie::jump();
      }
    }

    /*É um nó do tipo claw, logo não tem vizinhos novos*/
    else
    {
       while (next2 > 0) {      
        next2--;
        compressedFaSEGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
        /* Os jumps das claws são tratados quando são gerados, já os vértices normais não. */
        if(ext2[next2].k == 0) 
        {
          CustomGTrie::jump();
        }
        /*else for(int jmp = 0; jmp < ext2[next2].k; jmp++)*/
      }
    }
  }
}


void Esu::compressedFaSE(CompressedGraph *g, int k) {
  int i;
  int next = 0;
  GraphCompressor compressor;
  _motif_size = k;
  _graph_size = g->numNodes();
  current = new candidate[k];
  _current = new int[k];
  
  _next = 0;
  c = g;

  /*In the worst case we have to generate every combination for every claw.*/
  for(i = 0; i <_graph_size; i++)
  {
    if(c->numClaws(i) > 0)
    {
      claw_memory += c->numClaws(i);
    }
  }

  ext = new candidate[_graph_size + claw_memory];

  x = compressor.decompressGraph(c);

  claw_seq = new int[c->numNodes()];
  claw_start_index = new int[c->numNodes()];


  CustomGTrie::init();
  LSLabeling::init(x);
  int accum = 0;
  int total = c->numNodes();
  
  for(i = 0; i < total; i++)
  {
    claw_seq[i] = c->numClaws(i);
    if(claw_seq[i] > 0)
    {
      claw_start_index[i] = total + accum;
      accum += claw_seq[i];
    }
    else
    {
      claw_start_index[i] = 0;
    }
  }

  Isomorphism::initNauty(k, false);

 for (i = 0; i < _graph_size; i++)
 {
    candidate q;
    q.label = i;
    q.claw = 0;
    q.k = 0;
    candidate v[1]; 
    compressedFaSEGo(q, 0, 0, 0, v, 1);
 }

  
  printf("LS-Classes: %lld\n", CustomGTrie::getClassNumber());
  CustomGTrie::listCustomGTrie(stdout);
  CustomGTrie::listClasses(stdout, 1);
  printf("Total occurences: %lld\n",count);

  count = 0;
  delete[] current;
  delete[] ext;
  delete[] claw_seq;
  delete[] claw_start_index;
}



int Esu::min(int a, int b)
{
  return a <= b ? a : b;
}

/*! Make a complete k-census of a Graph
    \param g the graph to be explored
    \param k the size of the subgraphs
    \param sg The GraphTree where the results should be stored */
void Esu::countSubgraphs(Graph *g, int k, GraphTree *sg) {
  int i, v[1];

  _motif_size = k;
  _graph_size = g->numNodes();
  _current = new int[k];
  _ext = new int[_graph_size];
  _next = 0;
  _g = g;
  _sg = sg;

  sg->zeroFrequency();

   for (i=0; i<_graph_size; i++)
    _go(i, 0, 0, v);
  
  delete[] _current;
  delete[] _ext;
}



/*! Recursively extend a partial subgraph (sampling version)
    \param n the current position in the constructed subgraph 
    \param size the subgraph size
    \param next number of elements in the list "ext"
    \param ext of the nodes that can be used to extend the subgraph */
void Esu::_goSample(int n, int size, int next, int *ext) {

  _current[size++] = n;

  if (size==_motif_size) {
    char s[_motif_size*_motif_size+1];
    Isomorphism::canonicalStrNauty(_g, _current, s);
    _sg->incrementString(s);
  } else {
    int i,j;
    int next2 = next;
    int ext2[_graph_size];

    for (i=0;i<next;i++) ext2[i] = ext[i];

    int *v  = _g->arrayNeighbours(_current[size-1]);
    int num = _g->numNeighbours(_current[size-1]);
    for (i=0;i<num;i++) {
      if (v[i]<=_current[0]) continue;
      for (j=0;j+1<size;j++)
  if (_g->isConnected(v[i],_current[j])) break;
      if (j+1==size) ext2[next2++]=v[i];      
    }
    while (next2>0) {
      next2--;
      if (Random::getDouble()<=_prob[size])
  _goSample(ext2[next2], size, next2, ext2);
    }
  }
  
}



/*! Make a complete k-census of a Graph (sampling version)
    \param g the graph to be explored
    \param k the size of the subgraphs
    \param sg The GraphTree where the results should be stored
    \param p array of probabilities to use in sampling*/
void Esu::countSubgraphsSample(Graph *g, int k, GraphTree *sg, double *p) {
  int i, v[1];

  _motif_size = k;
  _graph_size = g->numNodes();
  _current = new int[k];
  _ext = new int[_graph_size];
  _next = 0;
  _g = g;
  _sg = sg;
  _prob = p;

  sg->zeroFrequency();

  for (i=0; i<_graph_size; i++)
    if (Random::getDouble()<=_prob[0])
      _goSample(i, 0, 0, v);
  
  delete[] _current;
  delete[] _ext;
}

unsigned Esu::nChoosek(unsigned n, unsigned k)
{
  /* Source: https://stackoverflow.com/questions/9330915/number-of-combinations-n-choose-r-in-c */
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}
