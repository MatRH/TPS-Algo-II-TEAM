class Nodo:
	'''Crea la clase nodo.'''

	def __init__(self, dato = None, prox = None):
		'''Constructor de la clase Nodo.'''
		self.dato = dato
		self.prox = prox

	def __str__(self):
		'''Devuelve el dato que contiene el nodo pasado a cadena.'''
		return str(self.dato)

class Pila:
	'''Crea la clase pila'''

	def __init__(self):
		'''Constructor de la clase pila'''
		self.items = []

	def apilar(self,x):
		'''Agrega el elemento x a la pila'''
		self.items.append(x)

	def desapilar(self):
		'''Quita y devuelve el ultimo elemento de la pila'''
		if self.esta_vacia():
			raise Exception('La pila esta vacia')
		return self.items.pop()

	def esta_vacia(self):
		'''Devuelve True si la pila esta vacia, False en caso contrario'''
		return len(self.items) == 0

	def ver_tope(self):
		'''Devuelve el elemento que se encuentra en el tope de la pila, levanta una excepcion si esta vacia.'''
		if len(self.items) == 0:
			raise Exception('La pila esta vacia')
		return self.items[-1]

class Cola:
	'''Crea la clase cola con los atributos, FIFO'''

	def __init__(self):
		'''Constructor de la clase cola'''
		self.primero = None
		self.ultimo = None

	def ver_primero(self):
		'''Devuelve el primer elemento de la cola sin extraerlo'''
		if self.primero == None:
			raise Exception('La cola esta vacia')
		return self.primero.dato

	def encolar(self, elemento):
		'''Encola el elemento'''
		nodo = Nodo(elemento)
		if self.primero == None:
			self.primero = nodo
			self.ultimo = nodo
		else:
			self.ultimo.prox = nodo
			self.ultimo = nodo

	def desencolar(self):
		'''Quita el primer elemento de la cola y lo devuelve'''
		if not self.primero:
			raise Exception('La cola esta vacia')
		dato = self.primero.dato
		self.primero = self.primero.prox   #si la cola tiene un elemen y lo sacas,
		if not self.primero:              #ultimo apuntaria al que sacaste, y tiene que ser None
			self.ultimo = None
		return dato

	def esta_vacia(self):
		'''Devuelve True si la cola esta vacia, False en caso contrario.'''
		return self.primero == None
