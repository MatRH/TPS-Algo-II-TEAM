import queue
def bfs(grafo, origen, n = 0, lista = None):
	'''Realiza un recorrido BFS sobre el grafo pasado por parametro.
	El argumento n es opcional al igual que la lista, y esta referido a la distancia maxima
	que puedo estar respecto al origen (o sea es la cantidad de saltos), y a la lista de los delincuentes mas buscados.
	Devuelve un diccionario con los padres de los vertices y sus respectivas distancias al origen'''
	visitados = {}
	padre = {}
	distancia = {}
	q = queue.Queue() #Crea una cola de tamanio infinito, se lo podria mejorar para que tenga la cantidad de vertices, pero nose
	visitados[origen] = True
	padre[origen] = None
	distancia[origen] = 0
	q.put(origen)
	while not q.empty():
		vertice = q.get()
		for w in grafo.adyacentes(vertice):
			if w not in visitados:
				padre[w] = vertice
				distancia[w] = distancia[vertice] + 1
				visitados[w] = True 
				if lista != None and w in lista: #Condicion de corte extra
					return padre, distancia
				if n == 0 or distancia[w] < n: #Tengo que chequear si ya me pase de la cantidad de saltos
					q.put(w)

	return padre, distancia