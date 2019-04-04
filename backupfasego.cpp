void Esu::compressedFaSEGo(candidate n, int size, int actual_size, int next, candidate *ext, int total_occurences) {

  current[size].label = n.label;
  current[size].claw = n.claw;
  current[size].k = n.k;

  if(current[size].claw == 0)
  { 
    _current[actual_size] = n.label;
    total_occurences *= 1;
    actual_size += 1;
    if(actual_size > 1)
    {
      char* addLabel = LSLabel(_current[actual_size], actual_size); 
      CustomGTrie::compressedInsert(addLabel, total_occurences);
      
      for(int p = 0; p < actual_size - 1; p++) printf("\t");
          std::cout << "Inserted " << _current[actual_size] << " Size: " << actual_size << endl;
    }
  }
  else
  {
    int start = actual_size;
    actual_size += current[size].k;

    unsigned occ = nChoosek(current[size].claw, current[size].k);
    for(int j = 0; j < current[size].k ; j++)
    {
       _current[start + j] = claw_start_index[current[size].label] + j; 
    }

    total_occurences *= occ;

    for(int p = 0; p < actual_size - 1; p++) 
      printf("\t");
    
    std::cout << "Inserted " << _current[actual_size] << " Size: " << actual_size <<  endl << "Sol: ";

    for(int p = 0; p < actual_size; p++)
    {
      std::cout << _current[actual_size] << " ";
    }
    printf("\n");
    char* addLabel = LSLabel(_current[actual_size], actual_size - 1); 
    CustomGTrie::compressedInsert(addLabel, total_occurences);
    
  }

  size++;


  if (actual_size == _motif_size) {
    /*Constrói occcurencia*/
    if(!CustomGTrie::getCurrentLeaf())
    {
        char s[_motif_size*_motif_size+1];
        Isomorphism::compressedCanonicalStrNauty(x, _current, s);
        CustomGTrie::setCanonicalLabel(s);
    }
    /*_sg->addString(s, total_occurences);*/
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
           compressedFaSEGo(ext2[next2], size, actual_size, next2, ext2, total_occurences);
         }

         CustomGTrie::jump();
         /*Nao esta dentro do for para evitar saltos para claws maiores*/
         
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
        CustomGTrie::jump();
      }
    }
  }
}