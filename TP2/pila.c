#include "pila.h"
#include <stdlib.h>
#define REDUCTO 1/2  //FACTOR DE DISMINUCION
#define ENGORGIO 2 //FACTOR DE INCREMENTO
#define CAPACIDAD_MINIMA 10
#define LIMITE_VACIO 4   //SI LA CAPACIDAD SE ENCUENTRA LLENA A MENOS DEL 1/LIMITE %

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear(){
	pila_t* pila = malloc(sizeof(pila_t));
	if (!pila) return NULL;
	pila->cantidad = 0;
	pila->datos = malloc(10*(sizeof(void*)));
	if (!(pila->datos)){
		free(pila);
		return NULL;
	}
	pila->capacidad = 10;
	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){
	return (pila->cantidad == 0);
}

//Cambia la capacidad de la pila, aumentando o disminuyendo la memoria que
//tiene disponible, devuelve false si la operacion de malloc fallo,
//caso contrario devuelve true.
//Pre: La pila fue creada
//Post: La capacidad de la pila es la ingresada, y el array datos tiene 
//tamaño capacidad * (sizeof(void*))
bool pila_redimensionar(pila_t* pila, size_t capacidad_nueva) {
    void** datos_nuevos = realloc(pila->datos, capacidad_nueva * sizeof(void*));
    if (!datos_nuevos)return false;

    pila->datos = datos_nuevos;
    pila->capacidad = capacidad_nueva;
    return true;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor){
	if (pila->cantidad == pila->capacidad){
		if(!(pila_redimensionar(pila, (pila->capacidad)* ENGORGIO)))return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	return pila->datos[pila->cantidad -1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	if (pila->capacidad > LIMITE_VACIO*(pila->cantidad) && (pila->capacidad) > CAPACIDAD_MINIMA){
		pila_redimensionar(pila, pila->capacidad/REDUCTO);
	}
	void* dato = pila_ver_tope(pila);
	pila->cantidad -= 1;
	return dato;
}	

// ...
