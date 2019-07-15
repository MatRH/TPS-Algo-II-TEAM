from tdas_auxiliares import Cola, Pila
import random
CANT_WALKS = 3600
LEN_WALKS = 15
def bfs(grafo, origen, destino = None, n = 0, dicc_datos = None):
	'''Realiza un recorrido BFS sobre el grafo pasado por parametro.
	Devuelve un diccionario con los padres de los vertices, sus respectivas distancias al origen,
	y el vertice que corta el ciclo, en caso de haber'''
	visitados = set()
	padre = {}	#diccionario {vertice:padre_de_vertice}
	distancia = {} #diccionario {vertice:distancia_al_origen}
	q = Cola()
	visitados.add(origen)
	padre[origen] = None
	distancia[origen] = 0
	q.encolar(origen)
	while not q.esta_vacia():
		vertice = q.desencolar()
		for w in grafo.adyacentes(vertice):
			if w not in visitados:
				padre[w] = vertice
				distancia[w] = distancia[vertice] + 1
				visitados.add(w)
				if (dicc_datos != None and w in dicc_datos) or (destino != None and w == destino): #Condicion de corte extra
					return padre, distancia[w], w
				if n == 0 or distancia[w] < n: #Tengo que chequear si ya me pase de la cantidad de saltos
					q.encolar(w)

	return padre, distancia, None

def dfs(grafo, origen, frecuencias, n):
	frecuencias[origen] = frecuencias.get(origen, 1) #Si no esta en el diccionario de frec, su frecuencia inicial es 1
	cont_walks = 0
	dfs_random(grafo, origen, frecuencias, n, cont_walks)
	return frecuencias

def dfs_random(grafo, vertice, frecuencias, n, cont):
	try: #Puede no tener adyacentes
		w = random.choice(grafo.adyacentes(vertice)) #Elijo un vertice adyacente de manera aleatoria
	except:
		return
	frecuencias[w] = frecuencias.get(w, 0) + 1 #Si no aparece en el dicc de frec, su frec sera 0 y le sumo 1, caso contrario actualizo el total
	cont += 1
	if cont < n:
		dfs_random(grafo, w, frecuencias, n, cont)

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
            nueva_cfc.append(str(z)) #Lo paso a str para evitar volver a recorrer todo y pasarlo luego
        cfcs.append(nueva_cfc)

def random_walk(grafo):
    '''Realiza random walks sobre el grafo'''
    frec_verts = {} #Para guardar las frecuencias de los vertices
    origenes = grafo.obtener_vertices_random(CANT_WALKS)
    for origen in origenes:
        dfs(grafo, origen, frec_verts, LEN_WALKS)
    return frec_verts

"""Recibe un grafo y una cantidad de iteraciones, y realiza una propagacion de
labels a lo largo de esta cantidad de iteraciones, devolviendo un diccionario
{vertice:label}"""
def label_propagation(grafo, iteraciones):
    labels = {}
    i = 0
    vertices = grafo.vertices() #lista de vertices en el grafo
    entran = grafo.dicc_entrantes() #diccionario {v:set de vertices entrantes a v}
    for v in vertices: #le asigno a cada vertice como etiqueta su posicion en la lista de vertices
        labels[v] = i
        i += 1
    for j in range(iteraciones):
        for vertice in vertices:
            entrantes_v = entran[vertice] #set de vertices entrantes a v
            if len(entrantes_v) == 0: continue #si el vertice está aislado lo salteo
            max = max_label(entrantes_v, labels)
            labels[vertice] = max
    return labels

def max_label(entrantes_v, labels):
    '''Dado un diccionario {vertice:label} y un set de vertices entrantes, Devuelve
    el label más grande de los vertices entrantes'''
    max = -1
    for entrante in entrantes_v:
        label = labels[entrante]
        if label > max: max = label
    return max

