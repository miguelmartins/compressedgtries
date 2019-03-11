#!/bin/bash

declare -a arr=("facebook.txt" "s420_st.txt" "yeast.txt" "hiv.txt" "netsci.txt" "gene_fusion.txt" "hiv.txt" "netsci.txt" "similarities.txt" "euroroad.txt" "icefiresocial.txt" "metabolite.txt" "hamster.txt" "sistercities.txt")

for i in "${arr[@]}"
do
	./clawfinder ./$i false  > ./outfinal/$i
done