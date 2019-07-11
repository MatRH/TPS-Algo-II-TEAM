import heapq #Para poder utilizar el heap de python
from collections import Counter # sirve para contar las apariciones de elementos en una lista
COEF_COMUNIDADES = 0.0005 #coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCIÓN DE COMUNIDADES
COEF_RANK = 0.0005#coeficiente para calcular cuantas iteraciones realizar en base a la cantidad de vertices EN LA FUNCION DE RANK
from tdas_auxiliares import Cola, Pila
from bfs import bfs

#Funciones policiales
def divulgar(grafo, delincuente, n):
    '''Imprime una lista con todos los delincuentes a los cuales les terminaria llegando un rumor
    que comienza en el delincuente pasado por parametro, y a lo sumo puede realizar n saltos'''
    destinatarios = bfs(grafo, delincuente, None, n) #esto me da una tupla con dos diccionarios
    resultado = [str(ladron) for ladron in destinatarios[0].keys() if ladron != delincuente] #Filtro porque el delincuente de donde arranco el rumor no debe figurar
    print(", ".join(resultado))

def buscar_comunidades(grafo, n):
    resultado = ""
    labels = {}
    i = 0
    vertices = grafo.vertices()
    entran = grafo.dicc_entrantes()
    cant_vertices = len(vertices)
    iteraciones  = int(cant_vertices * COEF_COMUNIDADES)
    for v in vertices: #le asigno a cada vertice como etiqueta su posicion en la lista de vertices
        labels[v] = i
        i += 1
    for j in range(iteraciones):
        for vertice in vertices:
            entrantes_v = entran[vertice] #set de vertices entrantes a v
            if len(entrantes_v) == 0: continue
            max_label = max(entrantes_v)
            labels[vertice] = max_label
    cuenta = Counter(labels.values()) #cuento cuantas veces aparece cada valor repetido
    num_comunidad = 1
    for etiqueta, cuenta in cuenta.items(): #tomo cada etiqueta y sus apariciones
        if cuenta >= n :#si la comunidad tiene al menos n miembros
            resultado += ("Comunidad {}: ".format(num_comunidad)) #agrego al resultado el indice de la comunidad
            for vertice, label in labels.items():
                if etiqueta == label:
                    resultado += "{}, ".format(vertice)#agrego al resultado los vertices que pertenecen a la comunidad
            num_comunidad += 1
    print(resultado)

def min_seguimientos(grafo, origen, destino):
    '''Imprime una lista con los delincuentes con los cuales vamos del delincuente origen
    al delincuente destino de la forma mas rapida. En caso de no poder hacer el seguimiento
    se imprime Seguimiento imposible.'''
    camino, distancias, none = bfs(grafo, origen, destino)
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
    mas_buscados, mas_buscados_set = determinar_importantes(grafo, k) #Devuelve un set con los mas importantes
    min_dist = 0
    camino_minimo, delincuente = None, None #Delincuente es para saber hacia quien es la persecucion
    for agente in delincuentes:
        camino, distancia, thief = bfs(grafo, agente, None, min_dist, mas_buscados_set)
        #Encontraste un camino a thief, te fijas si actualizas camino minimo
        if thief == None: continue #Desde el agente pasado no encontre un camino a uno de los chorros
        if min_dist == 0 or distancia < min_dist or distancia == min_dist and mas_buscados.index(thief) < mas_buscados.index(delincuente):
            min_dist = distancia
            camino_minimo = camino
            delincuente = thief  #Guardas los datos de la persecucion hallada
    #Finalizado el for de agentes en cubierto debo reconstruir el camino
    if delincuente != None:
        persecucion = construir_camino(camino_minimo, delincuente)
        print(" -> ".join(persecucion))
    else: print("Error: Seguimiento imposible")

def mas_imp(grafo, k):
	"""Imprime los k delincuentes mas importantes"""
	delincuentes_vip = [str(x) for x in determinar_importantes(grafo, k)[0]] #Lista por comprension asi quedan en formato str
	print(", ".join(delincuentes_vip))

def cfc(grafo):
    visitados = set()
    orden = {}
    p = Pila()
    s = Pila()
    cfcs = []
    en_cfs = set()
    for v in grafo.vertices():
        if v not in visitados:
            orden[v] = 0
            dfs_cfc(grafo, v, visitados, orden, p, s, cfcs, en_cfs)
    cont_aux = 1
    for comp in cfcs:
        print("CFC {}: {}".format(cont_aux, ", ".join(comp)))
        cont_aux += 1

def divulgar_ciclo(grafo, origen, n):
    '''Permite encontrar un camino simple que empiece y termine en el delincuente pasado
    por parámetro, de largo n. En caso de no encontrarse un ciclo de ese largo y dicho
    comienzo, imprimir No se encontro recorrido.'''
    separador = " -> "
    camino = [origen]
    rechazados = set()

    for w in grafo.adyacentes(origen):
        if divulgar_ciclo_wrapper(grafo, origen, w, n, camino, rechazados):
            camino.append(origen)
            camino = [str(x) for x in camino]
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
    '''Determina los k (cantidad) delincuentes mas importantes y devuelve una lista y un set con ellos'''
    ranks = pagerank(grafo)
    heap = []
    resultado = []
    set_datos = set()
    for vertice, rank in ranks.items():
        heap.append((rank, vertice))

    heapq.heapify(heap) #Me da un heap de minimos
    vip_thief = heapq.nlargest(cantidad, heap)  #Me devuelve los K mas buscados en una lista de tuplas
    for rank, thief in vip_thief:
        resultado.append(thief)  #Quedaran ordenados con los mas buscados al comienzo de la lista
        set_datos.add(thief)


    return resultado, set_datos  #Ya me da ordenado de mayor a menor importancia

def pagerank(grafo):
    max_iter = int(grafo.cantidad_vertices()*COEF_RANK)
    #print ("Cantidad de iteraciones a realizar : {}".format(max_iter))
    page_rank = {}
    for vertice in grafo.vertices():
        page_rank[vertice] = 1/grafo.cantidad_vertices()
    for i in range(max_iter):
        #print("Número de iteración: {}".format(i))
        iter_rank = {}
        for vertice in grafo.vertices():
            adyacentes = grafo.adyacentes(vertice)
            cant_adyacentes = len(adyacentes)
            for adyacente in adyacentes:
                if adyacente not in iter_rank.keys(): iter_rank[adyacente] = 0
                iter_rank[adyacente] += page_rank[vertice]/cant_adyacentes #el rank en esta iteracion
        for vertice, rank in iter_rank.items():
            page_rank[vertice] = rank #guardo el resultado de la ultima iteracion
    return page_rank

def dfs_cfc(grafo, v, visitados, orden, p, s, cfcs, en_cfs):
    visitados.add(v)
    s.apilar(v)
    p.apilar(v)
    for w in grafo.adyacentes(v):
        if w not in visitados:
            orden[w] = orden[v] + 1
            dfs_cfc(grafo, w, visitados, orden, p, s, cfcs, en_cfs)
        elif w not in en_cfs:
            while orden[p.ver_tope()] > orden[w]:
                p.desapilar()

    if p.ver_tope() == v:
        p.desapilar()
        z = None
        nueva_cfc = []
        while z != v:
            z = s.desapilar()
            en_cfs.add(z)
            nueva_cfc.append(str(z))
        cfcs.append(nueva_cfc)

def divulgar_ciclo_wrapper(grafo, origen, adyacente, n, camino, rechazados):
    '''Devuelve True si se encontro un ciclo de largo n que comience en origen y finalice en el.
    False en caso contrario'''
    if n == 1 and origen == adyacente: return True #Para evitar agregarlo a la lista tres veces cuando nos piden largo 1
    if n != 1 and origen == adyacente: return False #Porque sino va a pasar dos veces por un vertice si esta dirigido a si mismo
    camino.append(adyacente)
    if len(camino) == n: #Siempre en la posicion n estara el ultimo vertice que tiene que tener como adyacente al origen
        if origen in grafo.adyacentes(adyacente):
            return True
        else:
            camino.pop() #Quito el elemento que agregue recien
            return False

    for w in grafo.adyacentes(adyacente):
        if w in rechazados or w in camino: continue #Si esta en rechazados o en el camino, lo descarto (no tendria camino simple)
        if not divulgar_ciclo_wrapper(grafo, origen, w, n, camino, rechazados):
            continue #si entro al if, significa que con ese vertice no llegue, pruebo con otro
        else: return True #Si encontre un camino el if de arriba dara True y por ende entrare a este else

    if len(grafo.adyacentes(adyacente)) == 0: rechazados.add(adyacente) #Agrego al set de rechazados
    camino.pop() #si Probe con todos los adyacentes y ninguno dio resultado saco el elemento de la lista y vuelvo para atras
    return False


'''Cosas que se me ocurren:
- Pasarse del largo -> Podar (1)
- Llegar al largo en cuestion y que no sea con el vértice destino (el mismo del origen) -> cortar (2)
- Llegamos al vértice destino fuera del largo n -> Cortar (3)'''
