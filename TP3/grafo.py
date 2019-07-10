# -*- coding: utf-8 -*-
class Grafo:
    'Implementación de un TDA grafo dirigido, pesado con Diccionarios de Diccionarios'
    """Hay un diccionario general donde cada clave es un vertice y cada valor es
    un diccionario, en cada uno de esos sub-diccionario cada clave es un vertice
    adyacente al vertice clave de su diccionario correspondiente y el valor el
    peso
    {
    Vertice1: {Vertice2 : pesov1-v2, Vertice3: pesov1-v3}
    Vertice2: {Vertice1 : pesov2-v1}
    Vertice3: {Vertice2: pesov3-v2} son aristas dirigidas
    }
     """
    def __init__(self): #método de creación
        self.vertices = {}
        self.cantidad_vertices = 0

    """Método para agregar un vértice, recibe el grafo, nombre del vértice y
    opcionalmente una lista de tuplas, donde cada tupla tiene por primer elemento
    el vertice de llegada y por segundo el peso"""
    def agregar_vertice(self, vertice, aristas = None):
        vertices = self.vertices
        if vertice not in vertices.keys(): #agrego el vértice si no existe
            adyacentes = {}
            vertices[vertice] = adyacentes
            self.cantidad_vertices+= 1

        adyacentes = vertices[vertice] #tomo sus aristas
        if (aristas):    #si tengo adyacentes los cargo
            for v_llegada, peso in aristas:
                adyacentes[v_llegada] = peso #si ya existía esa arista la sobreescribo

    def agregar_arista(self, salida, peso, llegada): #método para crear una arista entre dos vertices
        vertices = self.vertices
        if salida not in vertices.keys():
            adyacentes = {}
            vertices[salida] = adyacentes
            self.cantidad_vertices+= 1
        adyacentes = vertices[salida]
        adyacentes[llegada] = peso

    """Método para cargar varias aristas, donde aristas es una lista de listas
    de tres elementos, donde le primer elemento es el vertice de salida, el segundo
    el peso y el tercer elemento el vertice de llegada"""
    def agregar_aristas(self, aristas): #método para cargar varias aristas
        vertices = self.vertices
        for salida, peso, llegada in aristas:
            if salida not in vertices.keys():
                adyacentes = {}
                vertices[salida] = adyacentes
                self.cantidad_vertices+= 1
            adyacentes = vertices[salida]
            adyacentes[llegada] = peso

    def cantidad_vertices(self):#devuelve la cantidad de vertices
        return self.cantidad_vertices

    def vertice_pertenece(self, vertice):#devuevle true si el vertice pertenece al grafo
        return True if vertice in self.vertices.keys() else False

    def adyacentes(self, vertice):#devuelve una lista con los vertices adyacentes a vertice
        resultado = []
        vertices = self.vertices
        if vertice not in vertices.keys(): return resultado
        adyacentes = vertices[vertice]
        for adyacente in adyacentes.keys():
            resultado.append(adyacente)
        return resultado

    def existe_arista(self, salida, llegada):#devuelve true si existe la arista salida->llegada
        vertices = self.vertices
        if salida not in vertices.keys():
            return False
        return llegada in self.adyacentes(salida)

    def eliminar_arista(self, salida, llegada):#elimina la arista salida->llegada
        vertices = self.vertices
        if salida not in vertices.keys(): return
        adyacentes = vertices[salida]
        del adyacentes[llegada]
        #deberiamos borrar el vertice si queda solo???? en ese caso recordar actualizar la cantidad de vertices

    def eliminar_vertice(self, vertice):
        vertices = self.vertices
        if vertice not in vertices.keys(): return
        del vertices[vertice]
        self.cantidad_vertices -= 1

    def __str__(self):
        vertices = self.vertices
        for vertice in vertices:
            str += "{ " + vertice + " : "
            for  adyacente, peso in vertices[vertice].items():
                str += "( " + adyacente + " , " + peso + " ) "
            str += "}\n"
        return str

    def cantidad_aristas(self):#no se si vale la pena llevar la cuenta para que sea O(1) o recorrer en el momento
        cantidad = 0
        for vertice, adyacentes in self.vertices.items():
            cantidad += len(adyacentes.keys())
        return cantidad

    def vertices(self):#devuelve una lista con todos los vertices
        return self.vertices.keys()
