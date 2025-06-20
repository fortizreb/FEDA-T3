#!/bin/bash
# Script para compilar, ejecutar pruebas y generar gráficos
# Uso: ./ejecutar_pruebas.sh [N]
# N = cantidad de iteraciones (por defecto 1)

N=${1:-1}

set -e

# Compilar
make clean && make

for ((i=1; i<=N; i++)); do
    echo "------ Iteración $i ------"
    ./test
    echo "------ Generando gráficos ------"
    python3 utilidades/graficos_generar.py
    echo "------ Iteración $i completa ------"
done

echo "Todas las pruebas y gráficos completados."
