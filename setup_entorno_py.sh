#!/bin/bash

# Nombre del entorno virtual
ENV_DIR="venv"

# Verificar que python3-venv esté instalado
if ! dpkg -s python3-venv >/dev/null 2>&1; then
    echo "Instalando paquete del sistema 'python3-venv'..."
    sudo apt update && sudo apt install -y python3-venv
fi

# Crear entorno virtual si no existe
if [ ! -d "$ENV_DIR" ]; then
    echo "Creando entorno virtual en '$ENV_DIR'..."
    python3 -m venv $ENV_DIR
    if [ $? -ne 0 ]; then
        echo "Error: No se pudo crear el entorno virtual."
        exit 1
    fi
fi

# Verificar que se haya creado correctamente
if [ ! -f "$ENV_DIR/bin/activate" ]; then
    echo "Error: El entorno virtual no contiene el archivo de activación."
    exit 1
fi

# Activar entorno virtual
echo "Activando entorno virtual..."
source $ENV_DIR/bin/activate

# Instalar dependencias requeridas
echo "Instalando dependencias..."
pip install --upgrade pip > /dev/null
pip install pandas matplotlib || {
    echo "Error: No se pudo instalar pandas o matplotlib."
    echo "Si estás en un entorno restringido, puedes usar:"
    echo "pip install pandas matplotlib --break-system-packages"
    exit 1
}

deactivate
echo "Entorno virtual preparado correctamente."
