if [ -d "./query/" ]
then
    rm ./query/*
fi
make clean
make
./run.exe star dataset/yeast_new.igraph query_requirement/star_query_requirement.txt yeast_star_query
