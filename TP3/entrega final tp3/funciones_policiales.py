from collections import Counter #Sirve para contar las apariciones de elementos en una lista
from grafo_tours import label_propagation
from lib_fcion_poli import *
COEF_COMUNIDADES = 0.01 #Coeficiente para el cálculo de iteraciones en la función de comunidades

#Funciones policiales
def divulgar(grafo, delincuente, n):
    '''Imprime una lista con todos los delincuentes a los cuales les terminaria llegando un rumor
    que comienza en el delincuente pasado por parametro, y a lo sumo puede realizar n saltos'''
    destinatarios = divulgar_rumor(grafo, delincuente, n)
    destinatarios = [str(ladron) for ladron in destinatarios.keys() if ladron != delincuente] #El delincuente de donde inicio no debe figurar
    print(", ".join(destinatarios))

def buscar_comunidades(grafo, n):
    resultado = ""
    cant_vertices = grafo.cantidad_vertices()
    iteraciones  = int(cant_vertices * COEF_COMUNIDADES)
    labels = label_propagation(grafo, iteraciones)
    cuenta = Counter(labels.values()) #cuento cuantas veces aparece cada valor repetido
    num_comunidad = 1
    for etiqueta, cuenta in cuenta.items(): #tomo cada etiqueta y sus apariciones
        if cuenta >= n :#si la comunidad tiene al menos n miembros
            if num_comunidad > 1:
                resultado = resultado.rstrip(", ")
                resultado += '\n'
            resultado += ("Comunidad {}: ".format(num_comunidad)) #agrego al resultado el indice de la comunidad
            for vertice, label in labels.items():
                if etiqueta == label:
                    resultado += "{}, ".format(vertice)#agrego al resultado los vertices que pertenecen a la comunidad
            num_comunidad += 1
    print(resultado.rstrip(", "))

def min_seguimientos(grafo, origen, destino):
    '''Imprime una lista con los delincuentes con los cuales vamos del delincuente origen
    al delincuente destino de la forma mas rapida. En caso de no poder hacer el seguimiento
    se imprime Seguimiento imposible.'''
    camino = seguimiento(grafo, origen, destino)
    if camino == None:
        print("Seguimiento imposible")
    else:
        camino_final = construir_camino(camino, destino) #Reconstruyo el camino
        print(" -> ".join(camino_final))

def persecucion(grafo, agentes, k):
    '''Recibe por parametro un grafo, una lista agentes en cubierto,
    y un numero k que representa la cantidad de delincuentes importantes.
    Imprime la persecucion que sea mas rapida (camino minimo) desde uno de los
    agentes en cubierto hacia uno de los k delincuentes mas importantes'''
    camino, delincuente = persecucion_rapida(grafo, agentes, k)
    if delincuente != None:
        persecucion = construir_camino(camino, delincuente)
        print(" -> ".join(persecucion))
    else: print("Error: No se encontro camino hacia los mas buscados")

def mas_imp(grafo, k):
    """Imprime los k delincuentes mas importantes"""
    delincuentes_vip = [str(x) for x in determinar_importantes(grafo, k)[0]] #Lista por comprension asi quedan en formato str
    print(", ".join(delincuentes_vip))

def cfc(grafo):
    cfc = cfc_grafo(grafo)
    cont = 1
    for comp in cfc:
        print("CFC {}: {}".format(cont, ", ".join(comp)))
        cont += 1

def divulgar_ciclo(grafo, origen, n):
    '''Permite encontrar un camino simple que empiece y termine en el delincuente pasado
    por parámetro, de largo n. En caso de no encontrarse un ciclo de ese largo y dicho
    comienzo, imprime: No se encontro recorrido.'''
    if n > grafo.cantidad_vertices():
        print("No se encontro recorrido")
        return
    ciclo = ciclo_largo_n(grafo, origen, n)
    if ciclo == None:
        print("No se encontro recorrido")
    else:
        ciclo = [str(x) for x in ciclo] #Lo paso a strings para poder imprimir
        print(" -> ".join(ciclo))

#*************************************FUNCIONES AUXILIARES************************************************
def construir_camino(camino, delincuente):
    '''Reconstruye el camino desde el agente hacia el delincuente con la ayuda del diccionario pasado por parametro.
    Devuelve una lista con dicho camino'''
    path = []
    path.append(str(delincuente))
    while camino[delincuente] != None:
        path.append(str(camino[delincuente]))
        delincuente = camino[delincuente] #actualizo el delincuente, o sea, voy hacia atras

    return path[::-1] #Invierto la lista
