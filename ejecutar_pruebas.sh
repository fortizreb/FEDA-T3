#!/bin/bash
N=${1:-5} # si no se especifica un número, se usa 5 por defecto

make clean && make
for ((i=1; i<=N; i++)); do
    echo "------ Iteración $i ------"    
    ./test
    cd graficos/
    python3 graficos_generar.py
    cd ..
    echo "------ Iteración $i completa ------"
done
