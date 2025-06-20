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
- `python3`
- Dependencias de Python:
  - `pandas`
  - `matplotlib`
  - `python3-venv` (requerido para crear entornos virtuales)

### Instalación de requisitos del sistema (solo una vez)
Si se ejecuta en una máquina nueva o sin entorno configurado, instale lo necesario con:

```bash
sudo apt update
sudo apt install -y make g++ python3 python3-pip python3-venv
```

## Compilación y ejecución básica
Para compilar y ejecutar una vez el programa:

```bash
make clean && make
./test
python3 utilidades/graficos_generar.py
```

## Ejecución automatizada N veces
Para automatizar la ejecución (incluye creación de entorno virtual e instalación de dependencias):

```bash
./ejecutar_pruebas.sh [numero natural]
```

Ejemplo:
```bash
./ejecutar_pruebas.sh 3
```

## Salida esperada

- Archivos CSV en `resultados/` con tiempos de inserción y búsqueda.
- Imágenes en `graficos/` con comparaciones por estructura y tipo de clave.

## Consideraciones

- El archivo `data.csv` debe estar ubicado en la misma carpeta del ejecutable al momento de la ejecución.
- Cada nueva ejecución sobrescribe los contenidos anteriores de las carpetas `resultados/` y `graficos/`.

## Autor
Fabián Ortiz
