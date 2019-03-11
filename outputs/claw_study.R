library(dplyr)

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/facebook.txt", header = FALSE, sep = " ")
barplot(table(claws$V2), main = "facebook")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/s420_st.txt", header = FALSE, sep = " ")
barplot(table(claws$V2), main = "s420_txt")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/yeastInter.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "yeastInter")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/hiv.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "hiv")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/netsci.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "netsci")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/gene_fusion.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "gene_fusion")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/similarities.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "similarities")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/euroroad.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "euroroad")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/icefiresocial.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "icefiresocial")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/metabolite.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "metabolite")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/hamster.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "hamster")

claws = read.table("/Users/Miguel/Documents/MCC/thesis/gtrieScanner_src_01/claw/outputs/sistercities.txt", header = FALSE, sep = " ")
claws = claws[claws$V2 > 1,]
barplot(table(claws$V2), main = "sistercities")

# Redes sociais e biol??gicas parecem apresentar mais claws (tamanho > 1)!
# Gerar scale-frees com v??rios par??metros para ver o que acontece?
# Distribui????o do grau dos melhores casos.