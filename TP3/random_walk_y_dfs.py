import random
CANT_WALKS = 5000
LEN_WALKS = 50
def random_walk(grafo):
    '''Realiza un random walk sobre el grafo'''
    frec_verts = {} #Para guardar las frecuencias de los vertices
    resultado = []
    cont = 1
    for _ in range(CANT_WALKS):
        print("Iteracion de walks: {}".format(cont))
        cont += 1
        origen = grafo.obtener_vertice_random()
        print("Obtuve un vertice random")
        print(type(origen))
        print(origen)
        dfs(grafo, origen, frec_verts, LEN_WALKS)

    return frec_verts

def dfs(grafo, origen, frecuencias, n):
    frecuencias[origen] = frecuencias.get(origen, 1) #Si no esta en el diccionario de frec, su frecuencia inicial es 1
    cont_walks = 0
    dfs_random(grafo, origen, frecuencias, n, cont_walks)
    return frecuencias

"""def dfs_wrapper(grafo, vertice, distancia, n, cont):
    for w in grafo.adyacentes(vertice):
        print("Iteracion numero {}".format(cont))
        cont += 1

        distancia[w] = distancia[vertice] + 1
        if distancia[w] == n: return
        dfs_wrapper(grafo, w, distancia, n, cont)
    return """

def dfs_random(grafo, vertice, frecuencias, n, cont):
    try: #Puede no tener adyacentes
        w = random.choice(grafo.adyacentes(vertice)) #Elijo un vertice adyacente de manera aleatoria
    except:
        return frecuencias
    frecuencias[w] = frecuencias.get(w, 0) + 1 #Si no aparece en el dicc de frec, su frec sera 0 y le sumo 1, caso contrario actualizo el total
    cont += 1
    if cont < n:
        dfs_random(grafo, w, frecuencias, n, cont)