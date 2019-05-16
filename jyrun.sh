if [ -d "./query/" ]
then
    rm ./query/*
fi
make clean
make
./run.exe clique dataset/human_new.igraph query_requirement/clique_query_requirement.txt
