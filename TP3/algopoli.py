#!/usr/bin/python3
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
import heapq #Para poder utilizar el heap de python
from collections import Counter # sirve para contar las apariciones de elementos en una lista
COEF_COMUNIDADES = 0.5 #coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCIÓN DE COMUNIDADES
COEF_RANK = 0.5#coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCION DE RANK
COMANDOS = ["min_seguimientos", "persecucion", "mas_imp", "comunidades", "divulgar", "divulgar_ciclo", "cfc"]

def main():
    argumentos = len(sys.argv) #Me da la cantidad de argumentos recibidos
    if argumentos != 2: raise Exception("Error: Cantidad de parametros incorrecta")
    file_name = sys.argv[1]
    with open(file_name, 'r') as datos_poli:
        grafo = Grafo()
        for linea in datos_poli:
            v, w = linea.split()
            grafo.agregar_vertice(int(v))
            grafo.agregar_vertice(int(w))
            grafo.agregar_arista(v, 0, w)
        #Grafo creado
        while True:
            comando = input() #Recibe el comando
            if not comando: break
            comando = comando.split() #Separa por espacios en blanco
            if comando[0] not in COMANDOS:
                print("Error: Comando invalido")
                continue
            aplicar_comando(grafo, comando)

def aplicar_comando(grafo, comando):
    """Aplica el comando recibido por parametro al grafo"""
    index_comando = COMANDOS.index(comando[0])
    if index_comando == 0:
        if longitud_requerida(comando, 3):
            min_seguimientos(grafo, int(comando[1]), int(comando[2]))
        else print("Error: Parametros invalidos")

    elif index_comando == 1:
        if longitud_requerida(comando, 3):
            delincuentes = comando[1:-1] #Hago un slice para tener los delincuentes (agentes en cubierto)
            delincuentes = [int(x) for x in delincuentes] #La transformo para que sean ints, sino son todos str
            k = int(comando[-1])
            persecucion(grafo, delincuentes, k)
        else print("Error: Parametros invalidos")

    elif index_comando == 2:
        if longitud_requerida(comando, 2):
            mas_imp(grafo, int(comando[1]))
        else print("Error: Parametros invalidos")

    elif index_comando == 3:
        if longitud_requerida(comando, 2):
            buscar_comunidades(grafo, int(comando[1]))
        else print("Error: Parametros invalidos")


    elif index_comando == 4:
        if longitud_requerida(comando, 3):
            divulgar(grafo, int(comando[1]), int(comando[2]))
        else print("Error: Parametros invalidos")

    elif index_comando == 5:
        if longitud_requerida(comando, 3):
            divulgar_ciclo(grafo, int(comando[1]), int(comando[2]))
        else print("Error: Parametros invalidos")

    else:
        cfc(grafo)

def longitud_requerida(lista, long_requerida):
    '''Recibe por parametro una lista y un entero que representa la cantidad de parametros requeridos
    .Devuelve true en caso de que sea la longitud correcta false en caso contrario.'''
    return len(lista) >= long_requerida

"""
Mínimos Seguimientos
Comando: min_seguimientos.
Parámetros: origen y destino.
Utilidad: imprime una lista con los delincuentes (su código identificador) con
los cuáles vamos del delincuente origen al delincuente destino de la forma más
rápida. En caso de no poder hacer el seguimiento (i.e. no existe camino),
imprimir Seguimiento imposible.
Salida
      10 -> 57 -> 4
      30 -> 36 -> 38 -> 20 -> 45 -> 12
"""
"""def min_seguimientos(grafo, salida, fin):
    camino = dijstra_no_pesado(grafo, salida) #devuelve un diccionario, {llegada: via}, devuelve solo los vertices a los que puedo llegar desde salida
    voy_por = None
    if fin not in camino.keys(): #si no puedo llegar al final vuelvo
        print("Seguimiento imposible")
        return
    voy_por = camino[fin]
    camino_final = Pila()
    while voy_por:
        camino_final.apilar(voy_por)
        voy_por = camino[voy_por]
    #ahora tengo una pila con el camino
    representacion = "{}".format(salida)
    while !camino_final.esta_vacia():
        voy_por = camino_final.desapilar()
        representacion += " -> {}".format(voy_por)
    print(representacion + " -> {}".format(fin))

def dijkstra_no_pesado(grafo, origen):
  visitados = {}
  distancia = {}
  padre = {}
  padre[origen]=None
  for w in grafo.vertices():
      distancia[w] = INF
  distancia[origen] = 0
  q = Cola()
  q.encolar(origen, 0)
  while !q.esta_vacia():
      v, dist = q.desencolar()
      visitados[v] = True
      for w in grafo.adyacentes(v):
          if !visitados[w] && dist + 1 < distancia[w]:
              padre[w] = v
              distancia[w] = distancia[v] + 1
              q.encolar[w, distancia[w]]
  return distancia, padre
"""
"""
Delincuentes más importantes

Usualmente nos gustaría determinar cuáles son los vértices más importantes en un
grafo en función de su centralidad exacta. Teniendo en cuenta que se cuenta con
demasiados delincuentes, el cálculo exacto de la centralidad puede consumir una
cantidad excesiva de tiempo. Por lo tanto, se pide realizar una aproximación para
determinar los delincuentes más importantes. Las formas posibles son:

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
"""
def determinar_importantes(grafo, cantidad):
    ranks = pagerank(grafo)
    heap = []
    for vertice, rank in ranks.items():
        heappush(heap, (rank, vertice))
        #un heap de minimos y me quedo con los cantidad maximos
    page_rank = heap[cantidad : ] #Slice para tener los k mas importantes, sigue siendo lista de tuplas, lo transformo en lista
    page_rank = [delincuente for i, delincuente in page_rank] #Lista por comprension, creo que anda
    return page_rank

def pagerank(grafo):
    max_iter = grafo.cantidad()*COEF_RANK
    page_rank = {}
    for vertice in grafo.vertices():
        page_rank[vertice] = 1/grafo.cantidad()
    for i in range(max_iter):
        iter_rank = {}
        for vertice in grafo.vertices():
            adyacentes = grafo.adyacentes(vertice)
            cant_adyacentes = len()
            for adyacente in adyacentes:
                if adyacente not in iter_rank.keys(): iter_rank[adyacente] = 0
                iter_rank[adyacente] += page_rank[vertice]/cant_adyacentes #el rank en esta iteracion
        for vertice, rank in iter_rank.items():  #No seria .items() ?
            page_rank[vertice] = rank #guardo el resultado de la ultima iteracion
    return page_rank

def buscar_ciclo():
def cconexas_fuertes():
main()


"""
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
"""
def persecucion(grafo, delincuentes, k):
    '''Obtiene el camino mas corto de uno de los delincuentes pasados por parametro a uno de los k mas importantes'''
    #Primero obtengo los delincuentes mas buscados
    mas_buscados = determinar_importantes(grafo, k) #Obtengo el diccionario de los k mas buscados quizas sea una lista
    min_dist = 0
    persecucion = [] #Lista utilizada para guardar como es la persecucion

    for agente in delincuentes: #Son agentes en cubierto
        camino, distancia = bfs(grafo, agente, 0, mas_buscados) #Camino seria el diccionario de padres
        delincuente = None #Me fijo el camino a que delincuente encontre
        for thief in mas_buscados:
            if thief in camino.keys():
                if min_dist == 0 or distancia[thief] <= min_dist and : #Si pasa esto, encontre un agente que tiene una persecucion faster
                    delincuente = thief
                    min_dist = distancia[thief]
                break #Creo que con este break salgo del for de mas adentro
        #Me fijo si encontre un camino mas corto, para ello delincuente tiene que ser != None
        if delincuente != None:
            #Si el delincuente es distinto de None quiere decir que encontre una mejor persecucion
            persecucion = construir_camino(camino, agente, delincuente)


    #Aca ya tendria el camino minimo
    if len(persecucion) == 0:
        print("Error: Persecución imposible")
        return
    imprimir_camino(persecucion, ' -> ')


def divulgar(grafo, delincuente, n):
    '''Imprime una lista con todos los delincuentes a los cuales les terminaria llegando un rumor
    que comienza en el delincuente pasado por parametro, y a lo sumo puede realizar n saltos'''
    camino = bfs(grafo, delincuente, n) #esto me da dos diccionarios, el de distancias no necesito
    separador = ', '
    print(separador.join(camino.keys)) #magia no se si anda

"""

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
"""
def buscar_comunidades(grafo, n):
    resultado = ""
    labels = {}
    i = 0
    vertices = grafo.vertices()
    cantidad_vertices = len(vertices)
    iteraciones  = cantidad_vertices * COEF_COMUNIDADES
    for v in vertices: #le asigno a cada vertice como etiqueta su posicion en la lista de vertices
        label[v] = i
        i += 1
    for j in range(iteraciones):
        for vertice in vertices:
            entrantes_v = grafo.entrantes(v) #primiva que devuelve una lista de los vertices que tienen como adyacente a v
            max_label = max(entrantes_v)
            labels[v] = max_label
    cuenta = Counter(label.values()) #cuento cuantas veces aparece cada valor repetido
    num_comunidad = 0
    for etiqueta, cuenta in cuenta.items(): #tomo cada etiqueta y sus apariciones
        if cuenta >= n :#si la comunidad tiene al menos n miembros
            resultado += ("Comunidad {}: ".format(num_comunidad)) #agrego al resultado el indice de la comunidad
            for vertice, label in labels:
                if etiqueta == label:
                    resultado += "{}, ".format(vertice)#agrego al resultado los vertices que pertenecen a la comunidad
            num_comunidad++
    print(resultado)



"""
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
"""

"""
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







#*************************************FUNCIONES AUXILIARES************************************************
def construir_camino(camino, agente, delincuente):
    '''Reconstruye el camino desde el agente hacia el delincuente con la ayuda del diccionario pasado por parametro.
    Devuelve una lista con dicho camino'''
    path = []
    path.append(delincuente)
    while camino[delincuente] != None:
        path.append(camino[delincuente])
        delincuente = camino[delincuente] #actualizo el delincuente, o sea, voy hacia atras

    return path[::-1] #Invierto el orden para tener el orden correcto

def imprimir_camino(lista, formato):
    '''Recibe una lista, y un formato, que es como separar a los elementos de la lista'''
    #debo mapear
    lista = [str(x) for x in lista] #Creo que asi se pasaba a cadenas todos los elementos que contiene
    print(formato.join(lista))
