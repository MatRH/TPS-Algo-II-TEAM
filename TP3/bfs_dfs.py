from tdas_auxiliares import Cola
import random
def bfs(grafo, origen, destino = None, n = 0, set_datos = None):
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
				if (set_datos != None and w in set_datos) or (destino != None and w == destino): #Condicion de corte extra
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
