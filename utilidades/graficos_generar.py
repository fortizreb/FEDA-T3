# Este archivo lo hice con LLM GPT-4 
# ya que no me acrodaba la sintaxis y no tenia mucho tiempo.

import pandas as pd
import matplotlib.pyplot as plt
import os
import re

# Rutas:
ruta_resultados = os.path.join(os.path.dirname(__file__), "..", "resultados")
if not os.path.isdir(ruta_resultados):
    ruta_resultados = os.path.join(os.path.dirname(__file__), "..", "..", "resultados")
ruta_salida = os.path.join(os.path.dirname(__file__), "..", "graficos")

# Crear carpeta salida si no existe
os.makedirs(ruta_salida, exist_ok=True)

# Eliminar imágenes antiguas
for archivo in os.listdir(ruta_salida):
    if archivo.endswith(".png"):
        os.remove(os.path.join(ruta_salida, archivo))


# === Inserción ===
def graficar_insercion_bst(df, nombre_limpio, estructura):
    print(f"Generando gráfico de inserción para {estructura}")
    plt.figure(figsize=(8, 5))
    for clave in df["clave"].unique():
        subset = df[df["clave"] == clave]
        plt.plot(subset["n_usuarios"], subset["tiempo_ms"], marker='o', label=clave)

    plt.title(f"Inserción en {estructura.upper()}")
    plt.xlabel("Cantidad de usuarios")
    plt.ylabel("Tiempo (ms)")
    plt.grid(True, alpha=0.3)
    plt.legend(title="Clave", loc="upper center", bbox_to_anchor=(0.5, -0.15), ncol=2)
    plt.tight_layout()

    path_img = os.path.join(ruta_salida, f"{nombre_limpio}.png")
    plt.savefig(path_img)
    plt.close()

# === Búsqueda ===
def graficar_busqueda_bst(df, nombre_limpio, estructura):
    print(f"Generando gráfico de búsqueda para {estructura} (líneas por clave y tipo)")
    resumen = df.groupby(["n_usuarios", "clave", "tipo"])["tiempo_ns"].mean().reset_index()

    plt.figure(figsize=(8, 5))
    for (clave, tipo), subset in resumen.groupby(["clave", "tipo"]):
        etiqueta = f"{clave} - {tipo}"
        plt.plot(subset["n_usuarios"], subset["tiempo_ns"], marker='o', label=etiqueta)

    plt.title(f"Búsqueda en {estructura.upper()}")
    plt.xlabel("Cantidad de usuarios")
    plt.ylabel("Tiempo (ns)")
    plt.grid(True, alpha=0.3)
    plt.legend(title="Clave - Tipo", loc="upper center", bbox_to_anchor=(0.5, -0.15), ncol=2)
    plt.tight_layout()

    path_img = os.path.join(ruta_salida, f"{nombre_limpio}.png")
    plt.savefig(path_img)
    plt.close()

# === Inserción HASH ===
def graficar_insercion_hash(df, nombre_limpio, tipo_hash):
    print(f"Generando gráfico de inserción para Hash {tipo_hash}")
    plt.figure(figsize=(8, 5))
    colores = {"id": "#1f77b4", "screenName": "#ff7f0e"}  # azul y naranja
    for clave in df["clave"].unique():
        subset = df[df["clave"] == clave]
        color = colores.get(clave, None)
        plt.plot(subset["n_usuarios"], subset["tiempo_ms"], marker='o', label=clave, color=color)
    plt.title(f"Inserción en Hash {tipo_hash.upper()}")
    plt.xlabel("Cantidad de usuarios")
    plt.ylabel("Tiempo (ms)")
    plt.grid(True, alpha=0.3)
    plt.legend(title="Clave", loc="upper center", bbox_to_anchor=(0.5, -0.15), ncol=2)
    plt.tight_layout()
    path_img = os.path.join(ruta_salida, f"{nombre_limpio}.png")
    plt.savefig(path_img)
    plt.close()

# === Búsqueda HASH ===
def graficar_busqueda_hash(df, nombre_limpio, tipo_hash):
    print(f"Generando gráfico de búsqueda para Hash {tipo_hash}")
    resumen = df.groupby(["n_usuarios", "clave", "tipo"])["tiempo_ns"].mean().reset_index()
    plt.figure(figsize=(8, 5))
    for (clave, tipo), subset in resumen.groupby(["clave", "tipo"]):
        etiqueta = f"{clave} - {tipo}"
        plt.plot(subset["n_usuarios"], subset["tiempo_ns"], marker='o', label=etiqueta)
    plt.title(f"Búsqueda en Hash {tipo_hash.upper()}")
    plt.xlabel("Cantidad de usuarios")
    plt.ylabel("Tiempo (ns)")
    plt.grid(True, alpha=0.3)
    plt.legend(title="Clave - Tipo", loc="upper center", bbox_to_anchor=(0.5, -0.15), ncol=2)
    plt.tight_layout()
    path_img = os.path.join(ruta_salida, f"{nombre_limpio}.png")
    plt.savefig(path_img)
    plt.close()

if __name__ == "__main__":
    import sys
    # Permite ejecutar el script desde cualquier carpeta
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

    # === Recorrer archivos de resultados ===
    for archivo in os.listdir(ruta_resultados):
        print(f"Revisando archivo: {archivo}")
        if not archivo.endswith(".csv"):
            print("Se omite (no es .csv)")
            continue

        ruta = os.path.join(ruta_resultados, archivo)
        nombre_base = os.path.splitext(archivo)[0]

        match_insert_hash = re.match(r"insert_hash_(abierto|cerrado)_.*\.csv", archivo)
        match_busqueda_hash = re.match(r"busqueda_hash_(abierto|cerrado)_.*\.csv", archivo)
        match_insert = re.match(r"insert_bst(?:_(\w+))?_.*\.csv", archivo)
        match_busqueda = re.match(r"busqueda_bst(?:_(\w+))?_.*\.csv", archivo)

        try:
            df = pd.read_csv(ruta)
            print(f"CSV leído correctamente: {archivo}")

            if match_insert_hash:
                tipo_hash = match_insert_hash.group(1)
                graficar_insercion_hash(df, nombre_base, tipo_hash)
            elif match_busqueda_hash:
                tipo_hash = match_busqueda_hash.group(1)
                graficar_busqueda_hash(df, nombre_base, tipo_hash)
            elif match_insert:
                estructura = match_insert.group(1) if match_insert.group(1) else "bst"
                graficar_insercion_bst(df, nombre_base, estructura)
            elif match_busqueda:
                estructura = match_busqueda.group(1) if match_busqueda.group(1) else "bst"
                graficar_busqueda_bst(df, nombre_base, estructura)
            else:
                print(f"Archivo con nombre no reconocido: {archivo}")

        except Exception as e:
            print(f"Error al procesar {archivo}: {e}")
