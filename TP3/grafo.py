# -*- coding: utf-8 -*-
import random
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
        self.verts = {}
        self.cant_vertices = 0

    """Método para agregar un vértice, recibe el grafo, nombre del vértice y
    opcionalmente una lista de tuplas, donde cada tupla tiene por primer elemento
    el vertice de llegada y por segundo el peso"""
    def agregar_vertice(self, vertice, aristas = None):
        #vertices = self.verts
        if vertice not in self.verts: #agrego el vértice si no existe
            adyacentes = set()
            #vertices[vertice] = adyacentes
            self.verts[vertice] = adyacentes
            self.cant_vertices+= 1

        adyacentes = self.verts[vertice] #tomo sus aristas
        if (aristas):    #si tengo adyacentes los cargo
            for v_llegada in aristas:
                adyacentes.add(v_llegada) #si ya existía esa arista la sobreescribo

    """def agregar_arista(self, salida, peso, llegada): #método para crear una arista entre dos vertices
        #vertices = self.verts
        if salida not in self.verts:
            adyacentes = set()
            self,verts[salida] = adyacentes
            self.cant_vertices+= 1
        adyacentes = vertices[salida]
        adyacentes.add(llegada)"""

    def agregar_arista(self, salida, llegada): #método para crear una arista entre dos vertices
        vertices = self.verts
        if salida not in vertices.keys():
            adyacentes = set()
            vertices[salida] = adyacentes
            self.cant_vertices+= 1
        if llegada not in vertices.keys():
            adyacentes = set()
            vertices[llegada] = adyacentes
            self.cant_vertices+= 1
        adyacentes = vertices[salida]
        adyacentes.add(llegada)

    """Método para cargar varias aristas, donde aristas es una lista de listas
    de tres elementos, donde le primer elemento es el vertice de salida, el segundo
    el peso y el tercer elemento el vertice de llegada"""
    def agregar_aristas(self, aristas): #método para cargar varias aristas
        vertices = self.verts
        for salida, peso, llegada in aristas:
            if salida not in vertices.keys():
                adyacentes = set()
                vertices[salida] = adyacentes
                self.cant_vertices+= 1
            adyacentes = vertices[salida]
            adyacentes.add(llegada)

    def cantidad_vertices(self):#devuelve la cantidad de vertices
        return self.cant_vertices

    def vertice_pertenece(self, vertice):#devuevle true si el vertice pertenece al grafo
        return True if vertice in self.verts else False

    def adyacentes(self, vertice):#devuelve una lista con los vertices adyacentes a vertice
        resultado = []
        #vertices = self.verts
        #if vertice not in vertices.keys(): return resultado
        if vertice not in self.verts: return resultado
        #adyacentes = vertices[vertice]
        adyacentes = self.verts[vertice]
        for adyacente in adyacentes:
            resultado.append(adyacente)
        return resultado

    def existe_arista(self, salida, llegada):#devuelve true si existe la arista salida->llegada
        #vertices = self.verts
        #if salida not in vertices.keys():
        if salida not in self.verts:
            return False
        return llegada in self.adyacentes(salida)

    def eliminar_arista(self, salida, llegada):#elimina la arista salida->llegada
        #vertices = self.verts
        if salida not in self.verts: return
        if llegada not in self.verts: return
        adyacentes = self.verts[salida]
        adyacentes.remove(llegada)

    def eliminar_vertice(self, vertice):
        #vertices = self.verts
        #if vertice not in vertices.keys(): return #si el vertice no existe vuelvo
        if vertice not in self.verts: return
        for vert, adyacentes in self.verts.items():   #elimino las aristas al vertice
            if vertice in adyacentes:
                adyacentes.remove(vertice)
        del vertices[vertice]
        self.cant_vertices -= 1

    """Devuelve un diccionario, {vertice:set de vertices entrantes} con todos los
    vertices del grafo y los vertices que con aristas entrantes a ellos"""
    def dicc_entrantes(self):
        grafo = self.verts
        entran = {}
        for vertice, adyacentes in grafo.items():
            if vertice not in entran: #agrego el vertice
                desde = set()
                entran[vertice] = desde
            for adyacente in adyacentes:
                if adyacente not in entran: #agrego el vertice
                    desde = set()
                    entran[adyacente] = desde
                entran[adyacente].add(vertice) #agrego el vertice al set de entrantes a adyacente
        return entran


    def __str__(self):
        vertices = self.verts
        for vertice in vertices:
            str += "{ " + vertice + " : "
            for  adyacente, peso in vertices[vertice].items():
                str += "( " + adyacente + " , " + peso + " ) "
            str += "}\n"
        return str

    def cantidad_aristas(self):#no se si vale la pena llevar la cuenta para que sea O(1) o recorrer en el momento
        cantidad = 0
        for vertice, adyacentes in self.verts.items():
            cantidad += len(adyacentes)
        return cantidad

    def entrantes(self, v):
        grafo = self.verts
        entran = set()
        for vertice, adyacentes in grafo.items():
            if v in adyacentes.keys():
                entran.add(vertice)
        return entran

    def vertices(self):#devuelve una lista con todos los vertices
        return self.verts.keys()

    def obtener_vertice_random(self, cant): #Devuelve un vertice random del grafo
        try:
            return random.choices(list(self.verts), k=cant)
        except:
            print("El grafo no contiene vertices")

    def dicc_verts(self): #Devuelve un diccionario con los vertices
        return self.verts
