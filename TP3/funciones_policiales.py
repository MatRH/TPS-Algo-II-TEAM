import heapq #Para poder utilizar el heap de python
from collections import Counter # sirve para contar las apariciones de elementos en una lista
COEF_COMUNIDADES = 0.5 #coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCIÓN DE COMUNIDADES
COEF_RANK = 0.5#coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCION DE RANK

#Funciones policiales
def divulgar(grafo, delincuente, n):
    '''Imprime una lista con todos los delincuentes a los cuales les terminaria llegando un rumor
    que comienza en el delincuente pasado por parametro, y a lo sumo puede realizar n saltos'''
    camino = bfs(grafo, delincuente, None, n) #esto me da dos diccionarios, el de distancias no necesito
    separador = ', '
    print(separador.join(camino.keys)) #magia no se si anda

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

def min_seguimientos(grafo, origen, destino):
    '''Imprime una lista con los delincuentes con los cuales vamos del delincuente origen
    al delincuente destino de la forma mas rapida. En caso de no poder hacer el seguimiento
    se imprime Seguimiento imposible.'''
    camino, distancias = bfs(grafo, origen, destino)
    if destino not in camino:
        print("Seguimiento imposible")
    else:
        camino_final = construir_camino(grafo, destino) #Reconstruyo el camino
        imprimir_camino(camino_final, " -> ")

def persecucion(grafo, delincuentes, k):
    '''Obtiene el camino mas corto de uno de los delincuentes pasados por parametro a uno de los k mas importantes'''
    #Primero obtengo los delincuentes mas buscados
    mas_buscados = determinar_importantes(grafo, k) #Obtengo el diccionario de los k mas buscados quizas sea una lista
    min_dist = 0
    persecucion = [] #Lista utilizada para guardar como es la persecucion   #Me conviene que mas buscados sea un set asi es mas rapido buscasr
    camino_rapido, agente_requerido, delincuente = None, None, None

    for agente in delincuentes: #Son agentes en cubierto
        camino, distancia = bfs(grafo, agente, None, min_dist, mas_buscados) #Camino seria el diccionario de padres
        #Cuando llegas aca encontras el camino minimo a un chorro o no
        for thief in mas_buscados:
            if thief in camino.keys():
                if min_dist == 0 or distancia[thief] <= min_dist:   #FALTA AGREGAR QUE SI SON DISTANCIAS IGUALES, ME QUEDO CON EL QUE SEA EL CHORRO MAS IMPORTANTE
                    delincuente = thief
                    min_dist = distancia[thief]
                    camino_rapido = camino
                    agente_requerido = agente
                break #Creo que con este break salgo del for de mas adentro

    if delincuente != None:
        #Si el delincuente es distinto de None quiere decir que encontre un camino para llegar al chorro
        persecucion = construir_camino(camino_rapido, agente_requerido, delincuente)
    	imprimir_camino(persecucion, ' -> ')

    else print("Error: Seguimiento imposible")

def mas_imp(grafo, k):
	"""Imprime los k delincuentes mas importantes"""
	delincuentes_vip = determinar_importantes(grafo, k) #TE FALTA PASARLO A STRINGS, creo
	separador = ", "
	print(separador.join(delincuentes_vip))






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
