
"""Importante: En caso de recibir un error FileNotFoundError: [Errno 2] No such
 file or directory: './algopoli': './algopoli', tener en cuenta que para el caso
  de enviar código escrito en Python es necesario además indicar la ruta del
  intérprete. Esto puede hacerse agregando como primera línea del archivo principal
  (en el ejemplo, sería algopoli.py) la línea: #!/usr/bin/python3."""
  """Datos disponibles

Se cuenta con un set de datos1 de los mensajes, comunicaciones, encuentros, etc.
 entre los mencionados delincuentes (y agentes encubiertos). Solo se han dejado
 los números de identificación de dichos delincuentes, sin ningún dato personal.
 El formato del archivo es un tsv (los campos se separan por tabuladores):

id_vertice1	id_vertice2
id_vertice1	id_vertice3
...
id_vertice2	id_vertice1
id_vertice2	id_vertice3
...
id_vertice3	id_vertice5

Adicionalmente, se cuenta con el siguiente set mínimo de pruebas inventado,
por lo que no representa un escenario real donde valga la pena realizar todas
las operaciones pedidas, pero puede servir de prueba, y se utilizará este set
de datos para los ejemplos. Además se deja un script generador para que se puedan
realizar más sets de prueba (aleatorios).

Que un delincuente se comunique con otro no implica que también suceda la recíproca.
Se ha visto también que hay delincuentes que se han enviado mensajes a sí mismos.
 Téngase esto en cuenta a la hora de procesar los datos.
Aunque dos delincuentes se comuniquen 1, 2 o n veces, la comunicación sólo aparece
 una vez en el archivo (que quiere decir que se hace esa comunicación al menos una vez).
"""
import sys #para leer parametros de la linea de comando

def main():
    argumentos = sys.argv
    if (len(argumentos != 2)) print("Error: Cantidad de parametros incorrecta", file=sys.stderr)
    file_name = argumentos[1]
    input = fopen(file_name)
    #menu o algo por el estilo para que elijan el script a correr
    close(input)

def min_seguimientos()
def determinar_importantes()
def armar_persecucion()
def buscar_comunidades()
def buscar_ciclo()
def cconexas_fuertes()
main()

"""
Mínimos Seguimientos

    Comando: min_seguimientos.
    Parámetros: origen y destino.
    Utilidad: nos imprime una lista con los delincuentes (su código identificador) con los cuáles vamos del delincuente origen al delincuente destino de la forma más rápida. En caso de no poder hacer el seguimiento (i.e. no existe camino), imprimir Seguimiento imposible.
    Ejemplo:
    Entrada:

      min_seguimientos 10 4
      min_seguimientos 30 12

    Salida:

      10 -> 57 -> 4
      30 -> 36 -> 38 -> 20 -> 45 -> 12

Delincuentes más importantes

Usualmente nos gustaría determinar cuáles son los vértices más importantes en un grafo en función de su centralidad exacta. Teniendo en cuenta que se cuenta con demasiados delincuentes, el cálculo exacto de la centralidad puede consumir una cantidad excesiva de tiempo. Por lo tanto, se pide realizar una aproximación para determinar los delincuentes más importantes. Las formas posibles son:

    Betweeness Centrality, aproximado.
    PageRank.

Por lo tanto, el comando pedido debe ser:

    Comando: mas_imp.
    Parámetros: cant.
    Utildad: Imprime, de mayor a menor importancia, los cant delincuentes más importantes.
    Ejemplo:
    Entrada:

    mas_imp 10

    Salida:

    20, 89, 42, 3, 49, 47, 56, 28, 22, 8

Persecución rápida

    Comando: persecucion.
    Parámetros: delincuente1,delincuente2,...,delincuenteN y K.
    Utilidad: Dado cada uno de los delincuentes pasados (agentes encubiertos), obtener cuál es el camino más corto para llegar desde alguno de los delincuentes pasados por parámetro, a alguno de los K delincuentes más importantes. En caso de tener caminos de igual largo, priorizar los que vayan a un delincuente más importante.
    Ejemplo:
    Entrada:

    persecucion 10,14,17 5
    persecucion 19,11,7,12 3

    Salida:

    17 -> 35 -> 20
    19 -> 42

Comunidades

Para implementar esto, utilizaremos el algoritmo de Label Propagation para detectar comunidades.

    Comando: comunidades.
    Parámetros: n.
    Utilidad: Imprime un listado de comunidades de al menos n integrantes.
    Ejemplo:
    Entrada:

    comunidades 10

    Salida:

    Comunidad 1: 0, 39, 59, 1, 47, 62, 2, 20, 3, 37, 31, 96, 16, 32, 80, 14, 40, 13, 89, 64, 72, 21, 15, 50, 97, 4, 17, 67, 6, 74, 54, 73, 93, 11, 65, 57, 70, 75, 7, 29, 8, 19, 55, 69, 33, 78, 44, 84, 43, 9, 42, 10, 53, 58, 35, 48, 45, 12, 25, 52, 71, 66, 36, 41, 79, 99, 92, 28, 56, 23, 18, 91, 34, 86, 30, 81, 38, 51, 87, 88, 22, 46, 63, 24, 95, 82, 49, 26, 27, 83, 90, 68, 94, 98, 61, 85, 76, 77, 60

Tener en cuenta que siendo un archivo generado de forma aleatoria, los resultados obtenibles para este punto tienen muy poco sentido con la realidad.
Divulgación de rumor

    Comando: divulgar.
    Parámetros: delincuente y n.
    Utilidad: Imprime una lista con todos los delincuentes a los cuales les termina llegando un rumor que comienza en el delincuente pasado por parámetro, y a lo sumo realiza n saltos (luego, se empieza a tergiversar el mensaje), teniendo en cuenta que todos los delincuentes transmitirán el rumor a sus allegados.
    Ejemplo:
    Entrada

    divulgar 30 4
    divulgar 30 1

    Salida:

    36, 79, 84, 38, 71, 48, 13, 76, 77, 20, 64, 72, 57, 23, 7, 24, 85, 61, 47, 19, 25, 40, 37, 52, 56, 74, 66, 1, 18, 27, 26, 80, 62, 97, 86, 15, 53, 31, 78, 99, 81, 6, 29, 11, 33, 45, 51, 65, 87, 42, 50, 93, 41, 90, 4, 70, 92, 67, 95, 0, 82, 63, 60, 5, 9, 68, 59, 89, 34, 8, 14, 73, 28, 16, 49, 43, 83, 75, 39, 21, 32, 54, 55, 17, 91, 46
    36, 79, 84

Ciclo de largo n

    Comando: divulgar_ciclo
    Parámetros: delincuente y n.
    Utilidad: Permite encontrar un camino simple que empiece y termine en el delincuente pasado por parámetro, de largo n. En caso de no encontrarse un ciclo de ese largo y dicho comienzo, imprimir No se encontro recorrido.
    Ejemplo:
    Entrada:

    divulgar_ciclo 74 5
    divulgar_ciclo 19 11

    Salida:

    74 -> 21 -> 81 -> 18 -> 42 -> 74
    19 -> 34 -> 12 -> 33 -> 54 -> 28 -> 79 -> 71 -> 57 -> 41 -> 56 -> 19

Componentes Fuertemente Conexas

Se debe leer el apunte sobre componentes fuertemente conexas.

    Comando: cfc
    Parámetros: ninguno.
    Utilidad: Imprime cada conjunto de vértices entre los cuales todos están conectados con todos.
    Ejemplo:
    Entrada:

    cfc

    Salida:

    CFC 1: 10
    CFC 2: 77, 18, 73, 47, 91, 57, 30, 64, 82, 60, 85, 58, 22, 87, 50, 89, 14, 70, 32, 96, 37, 3, 29, 7, 40, 17, 4, 35, 53, 24, 75, 94, 43, 31, 78, 2, 52, 44, 45, 99, 69, 8, 16, 25, 13, 79, 15, 97, 20, 38, 36, 41, 46, 95, 1, 92, 23, 51, 62, 11, 68, 80, 93, 67, 5, 65, 83, 27, 28, 54, 33, 12, 34, 19, 56, 76, 84, 63, 81, 21, 74, 6, 90, 55, 66, 86, 98, 88, 42, 9, 61, 48, 49, 59, 26, 72, 71, 39, 0

"""
