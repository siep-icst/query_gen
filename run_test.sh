if [ -d "./query/" ]
then
    rm query/*
fi
make clean
make
./run.exe line data/delaunay_prev.txt query_requirement/line_query_request.txt
