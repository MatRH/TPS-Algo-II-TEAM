#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La cola está planteada como una cola de punteros genéricos. */

typedef struct nodo{
	struct nodo* prox;
	void* dato;
}nodo_t;

struct lista{
	nodo_t* prim;
	nodo_t* ult;
	size_t cant;	
};

typedef struct lista_iter{
	struct lista* lista;
	nodo_t* actual;
	nodo_t* anterior;
	size_t posicion;
}lista_iter_t;


/* ******************************************************************
 *                		FUNCIONES AUXILIARES
 * *****************************************************************/

//Crea un nuevo nodo que tiene como dato el valor ingresado, y su siguente es NULL.
//Devuelve NULL en caso de error.
//Devuelve puntero al nodo si este pudo ser creado.
nodo_t* crear_nodo(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Devuelve NULL en caso de error.
// Post: devuelve una nueva lista vacia.
lista_t* lista_crear(void){
	lista_t* lista =  malloc(sizeof(lista_t));
	if (!lista) return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->cant = 0;
	return lista;
}

//Devuelve TRUE si la lista esta vacia, FALSE si contiene algun elemento.
//Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista){
	return (!(lista->prim) && !(lista->ult));
}

//Devuelve el largo de la lista.
//Pre: La lista fue creada.
size_t lista_largo(const lista_t *lista){
	return lista->cant;
}


//Devuelve FALSE en caso de error, devuelve TRUE si el elemento pudo ser insertado.
//Pre: La lista fue creada.
//Post: El elemento ingresado es el primero de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo = crear_nodo(dato);
	if (!nodo) return false;
	if (lista_esta_vacia(lista)) lista->ult = nodo;
	else{
		nodo_t* nodo_viejo = lista->prim;
		nodo->prox = nodo_viejo;	
	}
	lista->prim = nodo;
	lista->cant ++;
	return true;
}

//Devuelve FALSE en caso de error, devuelve TRUE si el elemento pudo ser insertado.
//Pre: La lista fue creada.
//Post: El elemento ingresado es el ultimo de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo = crear_nodo(dato);
	if(!nodo) return false;
	if (lista_esta_vacia(lista)) lista->prim = nodo;
	else{
		nodo_t* nodo_viejo = lista->ult;
		nodo_viejo->prox = nodo;
	}
	lista->ult = nodo;
	lista->cant ++;
	return true;
}

//Devuelve un puntero al primer elemento de la lista.
//Si la lista no tiene elementos devuelve NULL.
//Pre: La lista fue creada.
//Post: El primer elemento es eliminado de la lista.
void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	void* dato = lista->prim->dato;
	nodo_t* nodo = lista->prim;
	if (lista_largo(lista) == 1){
		lista->prim = NULL;
		lista->ult = NULL;
	}
	else{
		lista->prim = lista->prim->prox;

	}
	free(nodo);
	lista->cant --;
	return dato;
}

//Devuelve un puntero al primer elemento de la lista.
//Devuelve NULL si la lista no contiene elementos.
//Pre: La lista fue creada
void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}

//Devuelve un puntero al ultimo elemento de la lista.
//Devuelve NULL si la lista no contiene elementos.
//Pre: La lista fue creada
void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ult->dato;

}

//Destruye la lista. Si se le pasa la funcion destuir_dato(), esta funcion es
//aplicada a todos los datos de la lista.
//Pre: La lista fue creada
void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while(!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato) destruir_dato(dato);
	}
	free(lista);
	return;
}

/* *****************************************************************
 *                      PRIMITIVAS ITERADORES
 * *****************************************************************/


//ITERADOR INTERNO:
//
//Recibe una lista, una funcion visitar y un puntero extra, el recorrido sobre
//la lista finaliza cuando la funcion visitar devuelve false, aplica la funcion
//visitar a todos los datos de la lista, comenzando por el primero y avanzando hacia
// el final. Cuando la funcion visitar sea llamada se le pasara el puntero extra por
//parametro.
//PRE: La lista fue creada y se le paso una funcion visitar valida.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if (lista_esta_vacia(lista)) return;
	nodo_t* actual = lista->prim;
	bool deberia_visitar = visitar(actual->dato, extra);
	while (actual->prox && deberia_visitar){
		nodo_t* prox = actual->prox;
		deberia_visitar = visitar(prox->dato, extra);
		actual = prox;
	}
	return;
}

 //ITERADOR EXTERNO:
//
//Crea un iterador para la lista pasada por parametro.
//Pre: La lista debe haber sido creada.
lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter) return NULL;
	iter->lista = lista;
	iter->actual = iter->lista->prim;
	iter->anterior = NULL;
	iter->posicion = 0;
	return iter;
}

//Avanza a la siguiente posicion de la lista, devuelve false si esta vacia.
bool lista_iter_avanzar(lista_iter_t *iter){
	if((lista_esta_vacia(iter->lista) || lista_iter_al_final(iter))) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	iter->posicion ++;
	return true;
}

//Deveulve el dato almacenado en la posicion actual de la lista.
//Devuelve NULL si la lista esta vacia.
void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (lista_esta_vacia(iter->lista) || (lista_iter_al_final(iter))) return NULL;
	return (iter->actual->dato);
}

//Devuelve true si el iterador se encuentra en la ultima posicion de la lista.
bool lista_iter_al_final(const lista_iter_t *iter){
	if(iter->actual)return false;
	return true;
}

//Destruye el iterador.
//PRE: el iterador fue creado y no destruido previamente.
void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

//Inserta un dato en la posicion actual de la lista, moviendo el dato que que
//ocupaba esa posicion y los siguientes hacia el fondo de la lista.
bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t* nodo = crear_nodo(dato);
	if (!nodo) return false;
	nodo->prox = iter->actual;

	if(iter->anterior){
		iter->anterior->prox = nodo;
	} 
	if(iter->posicion == 0) iter->lista->prim = nodo;
	if(lista_iter_al_final(iter)) iter->lista->ult = nodo;
	if(lista_esta_vacia(iter->lista)){
		iter->lista->prim = nodo;
		iter->lista->ult = nodo;
	}
	iter->actual = nodo;
	iter->lista->cant ++;
	return true;
}

//Borra el elemento sobre el que esta parado el iterador, devuelve un puntero
//al dato guardado. Devuelve NULL en caso de que la lista no tenga elementos.
void *lista_iter_borrar(lista_iter_t *iter){
	if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) return NULL;
	void* dato = iter->actual->dato;
	nodo_t* nodo = iter->actual;
	if(iter->anterior) iter->anterior->prox = iter->actual->prox;
	if(iter->posicion == lista_largo(iter->lista)-1){
		iter->lista->ult = iter->anterior;
	}
	if (iter->posicion == 0){
		iter->lista->prim = iter->actual->prox;
	}
	iter->actual = iter->actual->prox;
	iter->lista->cant --;
	free(nodo);
	return dato;

}

 







/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_alumno(void);

