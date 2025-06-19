import pandas as pd
import matplotlib.pyplot as plt
import os
import re

# Rutas:
ruta_resultados = "../resultados"
# Cambiar a ruta relativa si se ejecuta desde raíz del proyecto
if not os.path.isdir(ruta_resultados):
    ruta_resultados = "resultados"
ruta_salida = "./imagenes"

# Crear carpeta salida si no existe
os.makedirs(ruta_salida, exist_ok=True)

# Eliminar imágenes antiguas
for archivo in os.listdir(ruta_salida):
    if archivo.endswith(".png"):
        os.remove(os.path.join(ruta_salida, archivo))


# === Inserción ===
def graficar_insercion(df, nombre_limpio, estructura):
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
def graficar_busqueda(df, nombre_limpio, estructura):
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

# === Recorrer archivos de resultados ===
for archivo in os.listdir(ruta_resultados):
    print(f"Revisando archivo: {archivo}")
    if not archivo.endswith(".csv"):
        print("Se omite (no es .csv)")
        continue

    ruta = os.path.join(ruta_resultados, archivo)
    nombre_base = os.path.splitext(archivo)[0]

    match_insert = re.match(r"insert_(\w+)_.*\.csv", archivo)
    match_busqueda = re.match(r"busqueda_(\w+)_.*\.csv", archivo)

    try:
        df = pd.read_csv(ruta)
        print(f"CSV leído correctamente: {archivo}")

        if match_insert:
            estructura = match_insert.group(1)
            graficar_insercion(df, nombre_base, estructura)
        elif match_busqueda:
            estructura = match_busqueda.group(1)
            graficar_busqueda(df, nombre_base, estructura)
        else:
            print(f"Archivo con nombre no reconocido: {archivo}")

    except Exception as e:
        print(f"Error al procesar {archivo}: {e}")
