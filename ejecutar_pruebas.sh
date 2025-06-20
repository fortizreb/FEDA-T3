#!/bin/bash
# Script para compilar, ejecutar pruebas y generar gráficos
# Uso: ./ejecutar_pruebas.sh [N]
# N = cantidad de iteraciones (por defecto 1)

N=${1:-1}

set -e

# Compilar
make clean && make

# Preparar entorno Python
if [ ! -x ./setup_entorno_py.sh ]; then
    chmod +x ./setup_entorno_py.sh
fi
./setup_entorno_py.sh

for ((i=1; i<=N; i++)); do
    echo "------ Iteración $i ------"
    ./test
    echo "------ Generando gráficos ------"
    source venv/bin/activate
    python utilidades/graficos_generar.py
    deactivate
    echo "------ Iteración $i completa ------"
done

echo "Todas las pruebas y gráficos completadas."
