from grafo_tours import bfs, random_walk, dfs_cfc
import heapq
from tdas_auxiliares import Cola, Pila
def divulgar_rumor(grafo, origen, n):
	'''Devuelve un diccionario con todos los delincuentes a los cuales les llego el rumor.
	El rumor puede dar n saltos como maximo.'''
	resultado = bfs(grafo, origen, None, n) #Me dara una tupla con los diccionarios de padres y distancias, necesito solo el primero
	return resultado[0]

def seguimiento(grafo, origen, destino):
	'''Devuelve un diccionario si encuentra un camino para llegar desde origen hacia destino.
	En caso de no encontrarlo devuelve None'''
	dicc_verts = grafo.dicc_verts() #Devuelve un diccionario con los vertices
	if origen not in dicc_verts or destino not in dicc_verts: return None

	camino = bfs(grafo, origen, destino)
	if destino not in camino[0]: return None
	return camino[0] #BFS devuelve dos diccionarios, y el delincuente al cual llegue, necesito el primero en este caso

def determinar_importantes(grafo, cantidad):
	'''Devuelve una lista y un dicc {delincuente : importancia} con los k (cantidad) delincuentes mas importantes.
	En el caso de la lista, el orden de importancia es de mayor a menor'''
	thiefs = random_walk(grafo)
	heap = []
	dicc_k_thiefs = {}
	list_k_thiefs = []

	for vertice, ranking in thiefs.items():
		heap.append((ranking, vertice))
	heapq.heapify(heap) #Me da un heap de minimos

	k_thiefs = heapq.nlargest(cantidad, heap) #Obtengo los k mas importantes
	for ranking, thief in k_thiefs:
		dicc_k_thiefs[thief] = ranking
		list_k_thiefs.append(thief)

	return list_k_thiefs, dicc_k_thiefs

def cfc_grafo(grafo):
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
            
    return cfcs

def persecucion_rapida(grafo, lista, k):
	'''Devuelve un diccionario con el camino minimo de uno de los elementos de las lista hacia
	uno de los k vertices mas importantes. En caso de caminos de igual longitud, se desempata
	por la importancia del vertice. En caso de no encontrarse camino con ningun elemento de la lista
	devuelve None'''
	top_k, top_k_dicc = determinar_importantes(grafo, k)
	min_dist = 0
	camino_min, delincuente = None, None #Delincuente es para saber hacia quien encontre el camino

	for agente in lista: #Son agentes en cubierto
		camino, dist, ladron = bfs(grafo, agente, None, min_dist, top_k_dicc)
		if ladron == None: continue #Con el agente no se encontro ningun camino a alguno de los k mas importantes
		if min_dist == 0 or dist < min_dist or (min_dist == dist and top_k_dicc[ladron] > top_k_dicc[delincuente]):
			min_dist = dist
			camino_min = camino
			delincuente = ladron

	if delincuente != None: return camino_min, delincuente
	return None, None

def ciclo_largo_n(grafo, origen, n):
	'''Busca un camino simple de largo n que empiece y finalice en el origen,
	en caso de encontrarlo lo devuelve, caso contrario devuelve None'''
	camino = [origen]
	dicc_camino = {} #Para saber si un vertice ya se encuentra en el camino
	dicc_camino[origen] = True
	rechazados = set() #Contendra los vertices rechazados, seria una optimizacion

	for adyacente in grafo.adyacentes(origen):
		if ciclo_largo_n_wrapper(grafo, origen, adyacente, n, camino, dicc_camino, rechazados):
			camino.append(origen)
			return camino

	return None

def ciclo_largo_n_wrapper(grafo, origen, vert, n, camino, dicc_camino, rechazados):
	if n == 1:
		if vert == origen: return True
		return False
	if n != 1 and origen == vert: return False #Porque sino puede llegar a pasar dos veces por un mismo vert si esta dirigio a si mismo

	camino.append(vert)
	dicc_camino[vert] = True

	if n == len(camino):
		if origen in grafo.adyacentes(vert): return True
		camino.pop()
		dicc_camino[vert] = False
		return False

	for w in grafo.adyacentes(vert):
		dicc_camino[w] = dicc_camino.get(w, False) #Puede pasar que no este en el dicc, primero verifico, sino su valor es False
		if w in rechazados or dicc_camino[w]: continue #Al hacer la linea anterior me aseguro de que dicc_camino[w] no lanza error
		if not ciclo_largo_n_wrapper(grafo, origen, w, n, camino, dicc_camino, rechazados):
			continue #Si entro a este bloque, significa que no llegue al resultado, sigo probando

		else: return True
	#Si salgo del bloque for, significa que no llegue al resultado, debo volver para atras
	if len(grafo.adyacentes(vert)) == 0: rechazados.add(vert)
	camino.pop() #Saco el ultimo elemento que agregue al camino
	dicc_camino[vert] = False
	return False
