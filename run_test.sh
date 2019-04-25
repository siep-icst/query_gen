if [ -d "./query/" ]
then
    rm ./query/*
fi
make clean
make
./run.exe clique data/delaunay_prev.txt query_requirement/clique_query_request.txt
