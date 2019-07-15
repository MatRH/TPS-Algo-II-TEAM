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
