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
Main File For Testing

---------------------------------------------------- */

#include "Common.h"
#include "GraphMatrix.h"
#include "GraphList.h"
#include "GraphUtils.h"
#include "Fase.h"
#include "LSLabeling.h"
#include "Timer.h"
#include "Graph.h"
#include "Isomorphism.h"
#include "Random.h"

using namespace std;

Graph *G;
int K = 0;
double samp_prob[MAXMOTIF], prob;
bool dir = false, detailed = false, draw = false;
char ifilename [100];
char ofilename [100];
FILE *outFile;
time_t t_start, t_end;

void initNauty()
{
  Isomorphism::initNauty(K, dir);
}

void init()
{
  printf("  _____                 _ \n"
         " |  __ \\               | |\n"
         " | |__) |__ _ _ __   __| |\n"
         " |  _  // _` | '_ \\ / _` |\n"
         " | | \\ \\ (_| | | | | (_| |\n"
         " |_|  \\_\\__,_|_| |_|\\__,_|\n"
         "                          \n");
  printf("  88888888888           ad88888ba   88888888888  \n"
         "  88                   d8\"     \"8b  88           \n"
         "  88                   Y8,          88           \n"
         "  88aaaaa  ,adPPYYba,  `Y8aaaaa,    88aaaaa      \n"
         "  88\"\"\"\"\"  \"\"     `Y8    `\"\"\"\"\"8b,  88\"\"\"\"\"      \n"
         "  88       ,adPPPPP88          `8b  88           \n"
         "  88       88,    ,88  Y8a     a8P  88           \n"
         "  88       `\"8bbdP\"Y8   \"Y88888P\"   88888888888  \n\n"
         "Rand-FaSE - Fast Subgraph Enumeration with Sampling\n"
         "\n\n\tPedro {Paredes, Ribeiro} - DCC/FCUP\n\n\n\n\n");
  t_start = time(0);
}

void displayHelp()
{
  printf("------------ FaSE Usage --------------\nMain Settings: ./FASE -s <Subgraph Size> -i <input file> [arguments...]\n\n\tAll commands:\n-h : Displays this help information\n-s <Integer> : Subgraph Size\n-i <Filename> : Name of input file (Format in Readme)\n-d : Directed Subgraph (Default undirected)\n-o : Name of output file (Default is stdout)\n-dt : Detailed Result (Displays all subgraph types and occurrences)\n-z : Use 0-based input (Suitable for input files starting at node 0)\n-tm : Use Adjacency Matrix LS-Labeling (Default is Adjacency List Labeling)\n-l : Use Adjacency List Only (Suitable for Large Scale or large networks [>10^5 nodes])\n-p <P1> <P2> ... <Ps> : Sets the sampling probabilities by depth (note that -s must have been selected first and this flag is optional)\n-pl <Psamp> : Sets the probabilities like in -p but with the low sampling setup\n-pm <Psamp> : Sets the probabilities like in -p but with the medium sampling setup\n-ph <Psamp> : Sets the probabilities like in -p but with the high sampling setup\n-q : Ignore arguments and prompt input\n--------------------------------------\n\n---- More Info about running FaSE in the Readme file");
}

void read(int argc, char **argv)
{
  int E, V, i, check = 0, itera = 0, samp = 0;
  int zeroBased = 1;
  ofilename[0] = '0';
  ofilename[1] = '\0';
  Fase::typeLabel = LSLabeling::TYPE_PICK;
  G = new GraphMatrix();
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
      continue;
    if (argv[i][1] == 'h')
    {
      displayHelp();
      K = 0;
      return;
    }
    if (argv[i][1] == 'd' && argv[i][2] == 't')
      detailed = true;
    else if (argv[i][1] == 'd' && argv[i][2] == 'r')
      draw = true;
    else if (argv[i][1] == 'd')
      dir = true;
    if (argv[i][1] == 't' && argv[i][2] == 'm')
      Fase::typeLabel = LSLabeling::TYPE_MATRIX;
    if (argv[i][1] == 'l')
      G = new GraphList();
    if (argv[i][1] == 'z')
      zeroBased = 0;

    if (argv[i][1] == 'i')
    {
      i++;
      strcpy(ifilename, argv[i]);
      check |= (1 << 0);
      continue;
    }

    if (argv[i][1] == 's')
    {
      i++;
      K = argv[i][0] - '0';
      int j = 1;
      while (argv[i][j] != '\0')
      {
	K *= 10;
	K += argv[i][j] - '0';
	j++;
      }
      check |= (1 << 1);
      continue;
    }

    if (argv[i][1] == 'o')
    {
      i++;
      strcpy(ofilename, argv[i]);
      continue;
    }

    if (argv[i][1] == 'p')
    {
      int j;
      for (j = 0; j < K; j++)
        samp_prob[j] = 1;
        
      if (argv[i][2] == 'l')
      {
        double prb = pow(atof(argv[i + 1]), 1.0 / (K - 1));
        for (j = 1; j < K; j++)
          samp_prob[j] = prb;
      }
      else if (argv[i][2] == 'm')
      {
        samp_prob[K - 1] = sqrt(atof(argv[i + 1]));
        samp_prob[K - 2] = sqrt(atof(argv[i + 1]));
      }
      else if (argv[i][2] == 'h')
        samp_prob[K - 1] = atof(argv[i + 1]);
      else
      {
        for (j = 0, i++; j < K; j++, i++)
          samp_prob[j] = atof(argv[i]);
        i--;
        i--;
      }

      i++;
      samp = 1;
      continue;
    }

    if (argv[i][1] == 'q')
    {
      itera = 1;
      break;
    }
  }

  if (!itera)
  {
    if (check != (1 << 2) - 1)
    {
      K = 0;
      if (check != 0)
	printf("Warning: Incorrect number of necessary arguments provided\n");
      displayHelp();
      return;
    }
    GraphUtils::readFileTxt(G, ifilename, dir, false, zeroBased);
    G->sortNeighbours();
    G->makeArrayNeighbours();
    if (ofilename[0] == '0' && ofilename[1] == '\0')
      outFile = stdout;
    else
      outFile = fopen(ofilename, "w");

    // Default Sampling probabilities
    if (!samp)
      for (i = 0; i < K; i++)
        samp_prob[i] = 1.0;
    return;
  }

  // Direction
  printf("Directed? (Y/n) ");
  char chdir;
  scanf(" %c", &chdir);
  if (chdir == 'n' || chdir == 'N')
    dir = false;

  // Initial
  printf("Input 0 or 1 based: ");
  scanf("%d", &zeroBased);

  // Input filename
  printf("Insert input file name: ");
  scanf(" %s", ifilename);
  GraphUtils::readFileTxt(G, ifilename, dir, false, zeroBased);
  G->sortNeighbours();
  G->makeArrayNeighbours();

  // Subgraph Size
  printf("Input the value K of the subgraph search: ");
  scanf("%d", &K);

  // Input filename
  printf("Insert output file name or 0 to stdout: ");
  scanf(" %s", ofilename);
  if (ofilename[0] == '0' && ofilename[1] == '\0')
    outFile = stdout;
  else
    outFile = fopen(ofilename, "w");
}

void initSamplingProbabilities()
{
  int i;
  prob = 1.0;
  for (i = 0; i < K; i++)
  {
    prob *= samp_prob[i];
    Fase::samp_prob[i] = samp_prob[i];
  }
}

void finishNauty()
{
  Isomorphism::finishNauty();
}

void output()
{
  printf("Finished Calculating\n");
  FILE *f = outFile;
  fprintf(f, "\tOutput:\n");
  fprintf(f, "Network: %s\n", ifilename);
  fprintf(f, "Directed: %s\n", dir ? "Yes" : "No");
  fprintf(f, "Type: %s\n", Fase::typeLabel == LSLabeling::TYPE_PICK ? "List" : "Matrix");
  fprintf(f, "Nodes: %d\n", G->numNodes());
  fprintf(f, "Edges: %d\n", G->numEdges() / (dir ? 1 : 2));
  fprintf(f, "Subgraph Size: %d\n", K);

  t_end = time(0);
  struct tm *tm_start = localtime(&t_start);
  fprintf(f, "Start of Computation: %02dh%02dm%02ds %02d/%02d/%02d\n\
", tm_start->tm_hour, tm_start->tm_min, tm_start->tm_sec, tm_start->tm_mday, tm_start->tm_mon+1, 1900+tm_start->tm_year);
  struct tm *tm_end   = localtime(&t_end);
  fprintf(f, "End of Computation: %02dh%02dm%02ds %02d/%02d/%02d\n", tm_end->tm_hour, tm_end->tm_min, tm_end->tm_sec, tm_end->tm_mday, tm_end->tm_mon+1, 1900+tm_end->tm_year);
  
  fprintf(f, "\n\n\tResults:\n");
  fprintf(f, "Subgraph Occurrences: %lld\n", (long long int)(Fase::MotifCount / prob));
  fprintf(f, "Subgraph Types: %lld\n", Fase::getTypes());
  fprintf(f, "G-Trie Leafs: %lld\n", Fase::getLeafs());
  fprintf(f, "G-Trie Nodes: %lld\n", Fase::getNodes() + 1);
  fprintf(f, "Computation Time (ms): %0.4lf\n", Timer::elapsed());
  
  fprintf(f, "\n\n\tSampling Information:\n");
  if (fabs(prob - 1.0) <= 10e-7)
    fprintf(f, "Full Enumeration, no Sampling done\n");
  else
  {
    fprintf(f, "Percentage of Sampled Subgraphs: %0.2lf\%\n", 100 * prob);
    fprintf(f, "Percentage by depth:\n");
    int i;
    for (i = 0; i < K; i++)
      fprintf(f, "P[%d]: %0.3lf\%\n", i, 100 * samp_prob[i]);
  }
    
  if (detailed)
  {
    fprintf(f, "\n\n\tDetailed Output:\n");
    Fase::listClasses(f, prob);
  }
  if (draw)
  {
    fprintf(f, "\n\n\tVisual G-Trie Output:\n");
    Fase::listTree(f);
  }
}

void finish()
{
  Fase::destroy();
  finishNauty();
  delete G;
  fclose(outFile);
}

int main(int argc, char **argv)
{
  init();
  read(argc, argv);
  if (K <= 2)
    return 0;
  initNauty();
  initSamplingProbabilities();
  timeval t1;
  gettimeofday(&t1, NULL);
  Random::init(t1.tv_usec * t1.tv_sec);
  Timer::start();
  Fase::directed = dir;
  Fase::EnumerateSubgraphs(G, K);
  Timer::stop();
  output();
  finish();

  return 0;
}
