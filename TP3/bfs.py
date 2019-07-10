from tdas_auxiliares import Cola
def bfs(grafo, origen, destino = None, n = 0, set_datos = None):
	'''Realiza un recorrido BFS sobre el grafo pasado por parametro.
	Devuelve un diccionario con los padres de los vertices, sus respectivas distancias al origen,
	y el vertice que corta el ciclo, en caso de haber'''
	visitados = set()
	padre = {}
	distancia = {}
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
				if (set_datos != None and w in set_datos) or (destino != None and w == destino): #Condicion de corte extra
					return padre, distancia[w], w
				if n == 0 or distancia[w] < n: #Tengo que chequear si ya me pase de la cantidad de saltos
					q.encolar(w)

	return padre, distancia, None