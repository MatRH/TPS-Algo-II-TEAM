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


def dfs(grafo, origen, distancia, n):
    distancia[origen] = 0
    cont = 1
    dfs_wrapper(grafo, origen, distancia, n, cont)
    return distancia

def dfs_wrapper(grafo, vertice, distancia, n, cont):
    for w in grafo.adyacentes(vertice):
        print("Iteracion numero {}".format(cont))
        cont += 1

        distancia[w] = distancia[vertice] + 1
        if distancia[w] == n: return
        dfs_wrapper(grafo, w, distancia, n, cont)
    return 