import heapq #Para poder utilizar el heap de python
from collections import Counter #Sirve para contar las apariciones de elementos en una lista
from tdas_auxiliares import Cola, Pila
from bfs import bfs, dfs
CANT_WALKS = 3600 #Cantidad de caminatas a realizar
LEN_WALKS = 15  #Longitud de las caminatas
COEF_COMUNIDADES = 0.01 #Coeficiente para el cálculo de iteraciones en la función de comunidades

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
    vertices = grafo.vertices() #lista de vertices en el grafo
    entran = grafo.dicc_entrantes() #diccionario {v:set de vertices entrantes a v}
    cant_vertices = len(vertices)
    iteraciones  = int(cant_vertices * COEF_COMUNIDADES)
    for v in vertices: #le asigno a cada vertice como etiqueta su posicion en la lista de vertices
        labels[v] = i
        i += 1
    for j in range(iteraciones):
        for vertice in vertices:
            entrantes_v = entran[vertice] #set de vertices entrantes a v
            if len(entrantes_v) == 0: continue #si el vertice está aislado lo salteo
            max = max_label(entrantes_v, labels)
            labels[vertice] = max
    cuenta = Counter(labels.values()) #cuento cuantas veces aparece cada valor repetido
    num_comunidad = 1
    for etiqueta, cuenta in cuenta.items(): #tomo cada etiqueta y sus apariciones
        if cuenta >= n :#si la comunidad tiene al menos n miembros
            if num_comunidad > 1:
                resultado = resultado.rstrip(", ")
                resultado += '\n'
            resultado += ("Comunidad {}: ".format(num_comunidad)) #agrego al resultado el indice de la comunidad
            for vertice, label in labels.items():
                if etiqueta == label:
                    resultado += "{}, ".format(vertice)#agrego al resultado los vertices que pertenecen a la comunidad
            num_comunidad += 1
    print(resultado.rstrip(", "))


def max_label(entrantes_v, labels):
    '''Dado un diccionario {vertice:label} y un set de vertices entrantes, Devuelve
    el label más grande de los vertices entrantes'''
    max = -1
    for entrante in entrantes_v:
        label = labels[entrante]
        if label > max: max = label
    return max

def min_seguimientos(grafo, origen, destino):
    '''Imprime una lista con los delincuentes con los cuales vamos del delincuente origen
    al delincuente destino de la forma mas rapida. En caso de no poder hacer el seguimiento
    se imprime Seguimiento imposible.'''
    dicc_verts = grafo.dicc_verts() #Devuelve un diccionario con los vertices
    if origen not in dicc_verts or destino not in dicc_verts:
        print("Seguimiento imposible")
    else:
        camino = bfs(grafo, origen, destino) #Me devuelve una tupla de elementos, solo necesito el primer diccionario para construir el camino
        if destino not in camino[0]:
            print("Seguimiento imposible")
        else:
            camino_final = construir_camino(camino[0], destino) #Reconstruyo el camino
            print(" -> ".join(camino_final))

def persecucion(grafo, agentes, k):
    '''Recibe por parametro un grafo, una lista agentes en cubierto,
    y un numero k que representa la cantidad de delincuentes importantes.
    Imprime la persecucion que sea mas rapida (camino minimo) desde uno de los
    agentes en cubierto hacia uno de los k delincuentes mas importantes'''
    mas_busc, mas_busc_dicc = determinar_importantes(grafo, k) #Devuelve una lista y un dicc
    min_dist = 0
    camino_min, delincuente = None, None #Delincuente es para saber hacia quien es la persecucion

    for agente in agentes:
        camino, dist, ladron = bfs(grafo, agente, None, min_dist, mas_busc_dicc)
        if ladron == None: continue #Desde el agente pasado no encontre un camino a uno de los chorros, continuo
        if min_dist == 0 or dist < min_dist or (dist == min_dist and mas_busc_dicc[ladron] > mas_busc_dicc[delincuente]):
            min_dist = dist
            camino_min = camino
            delincuente = ladron  #Guardas el delincuente al cual halle el camino minomo

    #Finalizado el for de agentes en cubierto debo reconstruir el camino
    if delincuente != None:
        persecucion = construir_camino(camino_min, delincuente)
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
    	comp = sorted(comp)
    	comp = [str(x) for x in comp]
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
    ranks = random_walk(grafo)
    heap = []
    resultado = []
    dicc_datos = {} #Contendra como clave al ladron y valor la importancia del mismo

    for vertice, rank in ranks.items():
        heap.append((rank, vertice))
    heapq.heapify(heap) #Me da un heap de minimos
    vip_thief = heapq.nlargest(cantidad, heap)  #Me devuelve los K mas buscados en una lista de tuplas
    for rank, thief in vip_thief:
        resultado.append(thief)  #Quedaran ordenados con los mas buscados al comienzo de la lista
        dicc_datos[thief] = rank

    return resultado, dicc_datos  #Ya me da ordenado de mayor a menor importancia

def random_walk(grafo):
    '''Realiza random walks sobre el grafo'''
    frec_verts = {} #Para guardar las frecuencias de los vertices
    origenes = grafo.obtener_vertice_random(CANT_WALKS)
    for origen in origenes:
        dfs(grafo, origen, frec_verts, LEN_WALKS)
    return frec_verts

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
            nueva_cfc.append(z)
        cfcs.append(nueva_cfc)

def divulgar_ciclo_wrapper(grafo, origen, adyacente, n, camino, rechazados):
    '''Devuelve True si se encontro un ciclo de largo n que comience en origen y finalice en el.
    False en caso contrario'''
    if n == 1:
    	if origen == adyacente:
    		return True
    	else:
    		return False

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
