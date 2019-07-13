#!/usr/bin/python3
"""Importante: En caso de recibir un error FileNotFoundError: [Errno 2] No such
 file or directory: './algopoli': './algopoli', tener en cuenta que para el caso
  de enviar código escrito en Python es necesario además indicar la ruta del
  intérprete. Esto puede hacerse agregando como primera línea del archivo principal
  (en el ejemplo, sería algopoli.py) la línea: #!/usr/bin/python3."""
import sys #para leer parametros de la linea de comando
from grafo import Grafo
from funciones_policiales import *
COMANDOS = ["min_seguimientos", "persecucion", "mas_imp", "comunidades", "divulgar", "divulgar_ciclo", "cfc"]
sys.setrecursionlimit(1000000)

def main():
    argumentos = len(sys.argv) #Me da la cantidad de argumentos recibidos
    if argumentos != 2: raise Exception("Error: Cantidad de parametros incorrecta")
    file_name = sys.argv[1]
    with open(file_name, 'r') as datos_poli:
        grafo = Grafo()
        for linea in datos_poli:
            v, w = linea.split()
            grafo.agregar_vertice(int(v))
            grafo.agregar_vertice(int(w))
            grafo.agregar_arista(int(v), 0, int(w))
        #Grafo creado
        while True:
            try:
                comando = input() #Recibe el comando
                if not comando: break
            except EOFError: #Para evitar el Error de EOF en el corrector
                return
            comando = comando.split() #Separa por espacios en blanco
            if comando[0] not in COMANDOS:
                print("Error: Comando invalido")
                continue
            aplicar_comando(grafo, comando)

def aplicar_comando(grafo, comando):
    """Aplica el comando recibido por parametro al grafo"""
    index_comando = COMANDOS.index(comando[0])
    if index_comando == 0:
        if longitud_requerida(comando, 3):
            min_seguimientos(grafo, int(comando[1]), int(comando[2]))
        else: print("Error: Parametros invalidos")

    elif index_comando == 1:
        if longitud_requerida(comando, 3):
            delincuentes = comando[1].split(",")
            delincuentes = [int(x) for x in delincuentes] #La transformo para que sean ints, sino son todos str
            k = int(comando[2])
            persecucion(grafo, delincuentes, k)
        else: print("Error: Parametros invalidos")

    elif index_comando == 2:
        if longitud_requerida(comando, 2):
            mas_imp(grafo, int(comando[1]))
        else: print("Error: Parametros invalidos")

    elif index_comando == 3:
        if longitud_requerida(comando, 2):
            buscar_comunidades(grafo, int(comando[1]))
        else: print("Error: Parametros invalidos")


    elif index_comando == 4:
        if longitud_requerida(comando, 3):
            divulgar(grafo, int(comando[1]), int(comando[2]))
        else: print("Error: Parametros invalidos")

    elif index_comando == 5:
        if longitud_requerida(comando, 3):
            divulgar_ciclo(grafo, int(comando[1]), int(comando[2]))
        else: print("Error: Parametros invalidos")

    else:
        cfc(grafo)

def longitud_requerida(lista, long_requerida):
    '''Recibe por parametro una lista y un entero que representa la cantidad de parametros requeridos
    .Devuelve true en caso de que sea la longitud correcta false en caso contrario.'''
    return len(lista) >= long_requerida

main()
