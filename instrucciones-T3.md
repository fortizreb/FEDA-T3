# Informe N°3: Tablas Hash vs Árboles de Búsqueda

## Descripción general

En este tercer informe compararemos el rendimiento de tablas hash y árboles de búsqueda sobre el dominio de usuarios de Twitter. Para la comparación utilizaremos el dataset "TwitterFriends" de Kaggle (https://www.kaggle.com/datasets/hwassner/TwitterFriends), el cual consiste de 40.000 registros de usuarios de X (ex Twitter). Pueden revisar los detalles del dataset, así como cargarlo para procesarlo, en la misma página de Kaggle.

Se solicita almacenar la información del dataset utilizando árboles binarios de búsqueda y tablas hash (similar a lo que revisaron en los laboratorios). Para ello, se deben considerar dos claves: `id` y `screenName`. En otras palabras, se pide diseñar e implementar estructuras de datos que utilicen `id` como clave, y un grupo similar que utilice `screenName` como clave.

Las estructuras de datos mínimas a considerar son:

- Árbol binario de búsqueda sin balanceo.
- Tabla hash con hashing abierto.
- Tabla hash con hashing cerrado. Para el caso de hashing cerrado, se deja a criterio del estudiante la selección de la estrategia para manejo de colisiones (linear probing, quadratic probing o double hashing).

## Secciones solicitadas

### 1. Introducción

Breve reseña de todo el trabajo realizado. Descripción a alto nivel del problema planteado y estructuras de datos utilizadas (implementadas y provistas por bibliotecas), herramientas, fuentes de datos y conclusiones preliminares.

### 2. Descripción de las estructuras de datos comparadas

Entrega de todos los códigos como adjuntos al documento o enlaces a dónde encontrarlos en el mismo. El código debe estar documentado y seguir algún estándar de codificación adoptado por el estudiante. Para cada estructura de datos, entregar una descripción general, indicar los costos de inserción, eliminación y búsqueda, y especificar los parámetros utilizados (función hash, número inicial de buckets, etc). Se debe destinar, como máximo, un párrafo por cada estructura de datos descrita. En dicho párrafo se pueden incluir links a sitios web y otras referencias bibliográficas.

### 3. Descripción del dataset y del ambiente de experimentación

Describir los métodos de lectura del dataset. Si fuese necesario realizar alguna modificación al dataset, especificar las modificaciones de manera justificada. Además, describir en detalle la máquina en la que se realizaron los experimentos (modelo del procesador, tamaño de la memoria RAM, tamaño de las memorias caché, etc).

### 4. Resultados experimentales

Comparar el rendimiento de las estructuras de datos usando evidencia experimental. Como mínimo, se pide entregar gráficas y/o tablas con las siguientes comparaciones:

- Tamaño de las estructuras de datos, expresadas en B, KB o MB.
- Rendimiento en la creación de la estructura de datos. Por cada estructura de datos, medir experimentalmente su escalabilidad en función de la cantidad de usuarios almacenados en la estructura. Por ejemplo, medir el tiempo que toma insertar los primeros 1.000, 5.000, 10.000, 15.000, etc.
- Comparar el rendimiento de las estructura de datos para realizar búsquedas. Para ello, se solicita realizar dos tipos de experimentos. En el primer tipo, se pide medir el tiempo de ejecutar varias búsquedas de usuarios que están almacenados (al menos 100 búsquedas). Para el segundo tipo de experimentos, se pide medir el tiempo de consultar por usuarios que no están almacenados en las estructuras de datos (al menos 100 búsquedas).

Cada medición experimental debe ser realizada utilizando las claves `id` y `screenName`. Se pide mostrar evidencia experimental de la clave más eficiente para cada estructura de datos.

### 5. Conclusiones

Describir los principales descubrimientos obtenidos. Hacer hincapié en las estructuras de datos más rápidas para cada tipo de experimento y en la comparación de los dos tipos de claves evaluadas.

## Entrega

- Puntos totales: 8
- Tipos de archivo permitidos: `.pdf`, `.cpp`, `.cc`, `.h`, `.hpp`, `.hh`
- Fecha de entrega: 19 de junio hasta las 23:59
- Disponible desde: 3 de junio a las 00:00

## Rúbrica de evaluación

**1. Introducción (0.5 pts):** Se introduce de manera correcta y coherente el tema a tratar. Se menciona el objetivo general de la tarea y una breve descripción de las conclusiones obtenidas.

**2. Descripción de las estructuras de datos (1.5 pts):** Se describen, como mínimo, las estructuras de datos comparadas: Árbol binario, tabla hash con hashing abierto, tabla hash con hashing cerrado. Cada estructura debe ser descrita en un párrafo, incluyendo referencias bibliográficas de ser necesario.

**3. Código y documentación (1 pts):** Se entrega el código fuente de las tres estructuras de datos y su documentación.

**4. Datasets y diseño experimental (1 pts):** Descripción del dataset, métodos de lectura/escritura y setup experimental.

**5. Exposición de resultados experimentales (3 pts):** Incluye una sección describiendo los experimentos realizados, resumiendo de manera adecuada los resultados obtenidos por medio de tablas y figuras.

**6. Conclusiones (1 pts):** Incluye una sección con conclusiones y reflexiones generadas a partir del desarrollo del trabajo.
