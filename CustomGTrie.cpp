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
G-Trie implementation

---------------------------------------------------- */

#include "CustomGTrie.h"

FILE* CustomGTrie::f;
long long int CustomGTrie::count;
long long int CustomGTrie::nodes;
CustomGTrie::labelTrie** CustomGTrie::labelList;
long long int* CustomGTrie::canCount;
int CustomGTrie::numLabel;
int CustomGTrie::maxLabel;
char CustomGTrie::globS[MAXS];

struct CustomGTrie::labelTrie
{
  labelTrie* childs[2];
  labelTrie* parent;
  int num;
  char let;
};

struct CustomGTrie::childTrie
{
  //  childTrie* list[1 << (8 * sizeof(char))];
  childTrie* list[MAXMOTIF + 1];
  trie* ref;
};

struct CustomGTrie::trie
{
  trie* parent;
  childTrie* childs;
  int leaf;
  long long int count;
};

CustomGTrie::trie* current;
CustomGTrie::labelTrie* CustomGTrie::labelRoot;

CustomGTrie::childTrie* CustomGTrie::initChild()
{
  childTrie* nn = new childTrie();
  return nn;
}

void CustomGTrie::init()
{
  nodes = 0;
  count = 0;
  numLabel = 1;
  maxLabel = 1000;
  labelList = (labelTrie **)malloc(maxLabel * sizeof(labelTrie *));
  canCount = (long long int *)malloc(maxLabel * sizeof(long long int));
  labelRoot = new labelTrie();
  labelRoot->parent = NULL;
  current = new CustomGTrie::trie();
  current->parent = NULL;
  current->childs = initChild();
  current->count = 0;
}

void CustomGTrie::destroy()
{
  free(labelList);
  free(canCount);
}

void CustomGTrie::augment()
{
  maxLabel *= 2;
  labelList = (labelTrie **)realloc(labelList, maxLabel * sizeof(labelTrie *));
  canCount = (long long int *)realloc(canCount, maxLabel * sizeof(long long int));
}

long long int CustomGTrie::getCanonicalNumber()
{
  return numLabel - 1;
}

long long int CustomGTrie::getNodeNumber()
{
  return nodes;
}

long long int CustomGTrie::getClassNumber()
{
  return count;
}

CustomGTrie::childTrie* CustomGTrie::searchChild(childTrie* node, char* s)
{
  if (*s == '\0')
  {
    return node;
  }
  if (node->list[*s] == NULL)
  {
    node->list[*s] = initChild();
  }
  return searchChild(node->list[*s], s + 1);
}

void CustomGTrie::insert(char* s)
{

  childTrie* temp = searchChild(current->childs, s);

  if (temp->ref != NULL)
  {
    temp->ref->count++;
    canCount[temp->ref->leaf]++;
    current = temp->ref;
  }
  else
  {

    nodes++;
    temp->ref = new CustomGTrie::trie();
    temp->ref->parent = current;
    temp->ref->childs = initChild();
    temp->ref->count = 1;
    temp->ref->leaf = 0;
    current = temp->ref;
  }

}
/* hold this thought, adicionar ocurencia so no fim
void CustomGtrie::clawInsert(char *s, long long int occur)
{
  childTrie* temp = searchChild(current->childs, s);
  if (temp->ref != NULL)
  {
    std::cout << temp->ref->count << " added " << occur << endl;
    temp->ref->count = temp->ref->count + occur;
    canCount[temp->ref->leaf]++;
    current = temp->ref;
  }
  else
  {

    nodes++;
    temp->ref = new CustomGTrie::trie();
    temp->ref->parent = current;
    temp->ref->childs = initChild();
    temp->ref->count = occur;
    temp->ref->leaf = 0;
    current = temp->ref;
  }
} */

void CustomGTrie::deepenClaw(char *s, long long int occur)
{
  childTrie *temp = searchChild(current->childs, s);

  if (temp->ref != NULL)
  {
    /*Não sei se claws intermédias são leaves*/
   canCount[temp->ref->leaf]++;
   current = temp->ref;


   /*Acho que nao devemos incrementar as claws intermedias*/
   //temp->ref->count = temp->ref->count + occur;
  }
  //else std::cout << "Whoops!\n";
}

void CustomGTrie::compressedInsert(char *s, long long int occur)
{
    
    
  //if(current == NULL) printf("oh dear\n");
  childTrie* temp = searchChild(current->childs, s);
 
  if (temp->ref != NULL)
  {
    temp->ref->count = temp->ref->count + occur;
    canCount[temp->ref->leaf]++;
    current = temp->ref;
  }
  else
  {

    nodes++;
    temp->ref = new CustomGTrie::trie();
    temp->ref->parent = current;
    temp->ref->childs = initChild();
    temp->ref->count = occur;
    temp->ref->leaf = 0;
    current = temp->ref;
  }
}

int CustomGTrie::searchLabel(labelTrie* node, char* s)
{
  if (*s == 0)
  {
    if (node->num == 0)
    {
      node->num = numLabel;
      canCount[numLabel] = 1;
      labelList[numLabel++] = node;
      if (numLabel == maxLabel - 2)
        augment();
    }
    else
      canCount[node->num]++;

    return node->num;
  }
  else
  {
    if (node->childs[*s - '0'] == NULL)
    {
      node->childs[*s - '0'] = new labelTrie();
      node->childs[*s - '0']->num = 0;
      node->childs[*s - '0']->let = *s;
      node->childs[*s - '0']->parent = node;
    }
    return searchLabel(node->childs[*s - '0'], s + 1);
  }
}

void CustomGTrie::setCanonicalLabel(char* s)
{
  CustomGTrie::count++;
  int vl = searchLabel(labelRoot, s);
  current->leaf = vl;
}

int CustomGTrie::getCurrentLeaf()
{
  return current->leaf;
}

void CustomGTrie::jump()
{
  //std::cout << "Jump!" << endl;
  current = current->parent;
}

char* CustomGTrie::getLabel(int key)
{
  int size = 0;
  labelTrie* dp = labelList[key];
  while (dp->parent != NULL)
  {
    globS[size++] = dp->let;
    dp = dp->parent;
  }
  globS[size] = 0;
  return globS;
}

void CustomGTrie::printCustomGTrie(CustomGTrie::childTrie *node, char* label, int sz)
{
  if (node->ref != NULL)
  {
    if (sz != 0)
      label[sz++] = '+';
    label[sz] = '\0';
//       printf("Class: %s, Found: %lld, Sample Model: %s\n", label, node->ref->count, getLabel(node->ref->leaf));
    if (!node->ref->leaf)
      printCustomGTrie(node->ref->childs, label, sz);
    else
    {
      label[sz - 1] = '\0';
      fprintf(f, "%s %d\n", label, node->ref->count);
    }
    if (sz != 0)
      label[--sz] = '\0';
  }
  int i;
  for (i = 1; i < MAXMOTIF + 1; i++)
  {
    label[sz] = i + '0' - 1;
    label[sz + 1] = '\0';
    if (node->list[i] != NULL)
      printCustomGTrie(node->list[i], label, sz + 1);
  }
}

void CustomGTrie::listCustomGTrie(FILE* out)
{
  f = out;
  fprintf(f, "=\n");
  while (current->parent != NULL)
    CustomGTrie::jump();
  char tmpStr[MAXS];
  tmpStr[0] = '\0';
  CustomGTrie::printCustomGTrie(current->childs, tmpStr, 0);
  fprintf(f, "done\n");
}


void CustomGTrie::printClass(CustomGTrie::labelTrie *node, char* label, int sz, double prob)
{
  if (node->num != 0)
  {
    label[sz] = '\0';
    fprintf(f, "Canonical Class: %s - Occurrences: %lld\n", label, (long long int)(canCount[node->num] / prob));
    return;
  }
  if(node->childs[0] != NULL)
  {
    label[sz] = '0';
    printClass(node->childs[0], label, sz + 1, prob);
  }
  if(node->childs[1] != NULL)
  {
    label[sz] = '1';
    printClass(node->childs[1], label, sz + 1, prob);
  }
}
 
void CustomGTrie::listClasses(FILE* out, double prob)
{
  f = out;
  char tmpStr[MAXS];
  tmpStr[0] = '\0';
  CustomGTrie::printClass(labelRoot, tmpStr, 0, prob);
}
