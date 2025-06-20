# Informe T3: Tablas Hash vs Árbol Binario de Búsqueda

Este proyecto corresponde al Informe No. 3 de la asignatura de Fundamentos en Estructuras de Datos y Algoritmos. Su objetivo es comparar el rendimiento de tres estructuras de datos implementadas en C++ sobre un conjunto de 40.000 usuarios extraídos del dataset "TwitterFriends".

## Estructuras de datos implementadas

- Árbol Binario de Búsqueda (BST), no balanceado  
- Tabla Hash con hashing abierto (encadenamiento)  
- Tabla Hash con hashing cerrado (linear probing)

Cada estructura opera utilizando dos claves distintas: el identificador numérico ("id") y el nombre de usuario ("screenName").

## Requisitos

- Sistema operativo con terminal Bash (Linux o macOS)
- `g++` compatible con C++17
- `make`
- `python3` con las librerías:
  - `pandas`
  - `matplotlib`

Para instalar las dependencias en Python (si no las tiene):

```bash
pip install pandas matplotlib
```

## Compilación y ejecución básica
Para compilar y ejecutar una vez el programa:
```bash
make clean && make
./test
python3 utilidades/graficos_generar.py
```
Ejecución automatizada N veces:
```bash
./ejecutar_pruebas.sh [numero natural]
```

## Salida esperada
- Archivos CSV en resultados/ con tiempos de inserción y búsqueda.

- Imágenes en graficos/ con comparaciones por estructura y tipo de clave.

## Consideraciones

- El archivo data.csv debe estar ubicado en la misma carpeta del ejecutable al momento de la ejecución.

- Cada nueva ejecución sobrescribe los contenidos anteriores de las carpetas resultados/ y graficos/.

## Autor
Fabián Ortiz 