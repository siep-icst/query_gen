if [ -d "./query/" ]
then
    rm query/*
fi
make clean
make
./run.exe line delaunay_n13_prev.txt query_requirement/line_query_request.txt
