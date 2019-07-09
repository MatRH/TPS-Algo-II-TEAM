import heapq #Para poder utilizar el heap de python
from collections import Counter # sirve para contar las apariciones de elementos en una lista
COEF_COMUNIDADES = 0.5 #coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCIÓN DE COMUNIDADES
COEF_RANK = 0.5#coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCION DE RANK
from tdas_auxiliares import Cola, Pila
#Funciones policiales
def divulgar(grafo, delincuente, n):
    '''Imprime una lista con todos los delincuentes a los cuales les terminaria llegando un rumor
    que comienza en el delincuente pasado por parametro, y a lo sumo puede realizar n saltos'''
    destinatarios = bfs(grafo, delincuente, None, n) #esto me da dos diccionarios, el de distancias no necesito
    resultado = [str(ladron) for ladron in destinatarios.keys() if ladron != delincuente] #Filtro porque el delincuente de donde arranco el rumor no debe figurar
    print(", ".join(resultado))

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
        camino_final = construir_camino(camino, destino) #Reconstruyo el camino
        print(" -> ".join(camino_final))

def persecucion(grafo, delincuentes, k):
    '''Recibe por parametro un grafo, una lista de delincuentes (agentes en cubierto),
    y un numero k que representa la cantidad de delincuentes importantes.
    Imprime la persecucion que sea mas rapida (camino minimo) desde uno de los
    agentes en cubierto hacia uno de los k delincuentes'''
    mas_buscados = determianr_importantes(grafo, k) #Devuelve un set con los mas importantes
    min_dist = 0
    camino_minimo, delincuente = None, None #Delincuente es para saber hacia quien es la persecucion
    for agente in delincuentes:
        camino, distancia, thief = bfs(grafo, agente, None, min_dist, mas_buscados)
        #Encontraste un camino a thief, te fijas si actualizas camino minimo
        if thief == None: continue #Desde el agente pasado no encontre un camino a uno de los chorros
        if min_dist == 0 or distancia < min_dist or distancia == min_dist and thie > delincuente:
            min_dist = distancia
            camino_minimo = camino
            delincuente = thief  #Guardas los datos de la persecucion hallada
    #Finalizado el for de agentes en cubierto debo reconstruir el camino
    if delincuente != None:
        persecucion = construir_camino(camino_minimo, delincuente)
        separador = " -> "
        print(separador.join(persecucion))
    else: print("Error: Seguimiento imposible")

def mas_imp(grafo, k):
	"""Imprime los k delincuentes mas importantes"""
	delincuentes_vip = [str(x) for x in determinar_importantes(grafo, k)] #Lista por comprension asi quedan en formato str
	separador = ", "
	print(separador.join(delincuentes_vip))

def cfc(grafo):
    visitados = set()
    orden = {}
    p = Pila()
    s = Pila()
    cfcs = []
    en_cfs = set()
    for v in grafo:
        if v not in visitados:
            orden[v] = 0
            dfs_cfc(grafo, v, visitados, orden, p, s, cfcs, en_cfs)
    return cfcs

def divulgar_ciclo(grafo, origen, n):
    '''Permite encontrar un camino simple que empiece y termine en el delincuente pasado
    por parámetro, de largo n. En caso de no encontrarse un ciclo de ese largo y dicho
    comienzo, imprimir No se encontro recorrido.'''
    separador = " -> "
    camino = [origen]
    visitados = set()
    rechazados = set()

    for w in grafo.adyacentes(origen):
        if divulgar_ciclo_wrapper(grafo, origen, w, n, visitados, camino, rechazados):
            print(separador.join(camino))
            return
    print("No se encontro recorrido")



#*************************************FUNCIONES AUXILIARES************************************************
def construir_camino(camino, delincuente):
    '''Reconstruye el camino desde el agente hacia el delincuente con la ayuda del diccionario pasado por parametro.
    Devuelve una lista con dicho camino'''
    path = []
    path.append(str(delincuente))
    while camino[delincuente] != None:
        path.append(str(camino[delincuente]))
        delincuente = camino[delincuente] #actualizo el delincuente, o sea, voy hacia atras

    return path[::-1] #Invierto la lista

def determinar_importantes(grafo, cantidad):
    ranks = pagerank(grafo)
    heap = []
    resultado = []
    for vertice, rank in ranks.items():
        heap.append((rank, vertice))

    heapq.heapify(heap) #Me da un heap de minimos
    vip_thief = heapq.nlargest(cantidad, heap)  #Me devuelve los K mas buscados en una lista de tuplas
    for rank, thief in vip_thief:
        resultado.append(thief)  #Quedaran ordenados con los mas buscados al comienzo de la lista

    return resultado[::-1] #Asi me quedan ordenados de menor a mayor en orden de busqueda

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

def dfs_cfc(grafo, v, visitados, orden, p, s, cfcs, en_cfs):
    visitados.agregar(v)
    s.apilar(v)
    p.apilar(v)
    for w in grafo.adyacentes(v):
        if w not in visitados:
            orden[w] = orden[v] + 1
            dfs_cfc(grafo, w, visitados, orden, p, s, cfcs, en_cfcs)
        elif w not in en_cfs:
            while orden[p.ver_tope()] > orden[w]:
                p.desapilar()

    if p.ver_tope() == v:
        p.desapilar()
        z = None
        nueva_cfc = []
        while z != v:
            z = s.desapilar()
            en_cfs.agregar(z)
            nueva_cfc.append(z)
        cfcs.append(nueva_cfc)

def divulgar_ciclo_wrapper(grafo, origen, adyacente, n, visitados, camino, rechazados):
    '''Devuelve True si se encontro un ciclo de largo n que comience en origen y finalice en el.
    False en caso contrario'''
    camino.append(adyacente)#cuando inicia agrega null?
    if len(camino) - 1 == n:
        if origen in grafo.adyacentes(adyacente):
            return True
        camino.pop()
        return False

    #Faltarian mas condiciones de poda

    for w in grafo.adyacentes(adyacente):
        if w in rechazados: continue
        divulgar_ciclo_wrapper(grafo, origen, w, n, visitados, camino, rechazados)

    if len(grafo.adyacentes(adyacente)) == 0: rechazados.add(adyacente)
    camino.pop() #Saco el vertice ya que no me manda por el camino correcto
    return False
