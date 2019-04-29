/*
Alumnos: Ruiz Huidobro Matias, Torresetti Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: ...
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#define TAMANIO_INICIAL 53

typedef struct nodo_hash{
	void* dato;
	const char* clave;
}nodo_hash_t;

typedef size_t (*funcion_de_hash)(const char*);

struct hash{
	lista_t** tabla_hash;
	size_t cant_elem;
	size_t tam_tabla;
	hash_destruir_dato_t funcion_destruc;
	funcion_de_hash funcion_hash;
};

size_t hash_djb2(const char *str); //funcion de HASH_H
nodo_hash_t* crear_nodo(void* dato, const char* clave);
bool guardar_elemento(hash_t* hash, lista_t* lista, nodo_hash_t* nodo);
bool buscar_clave(lista_t* lista, const char* clave, void* dato, bool reemplazar);

//struct hash_iter
//...

/* *****************************************************************
 *                  IMPLEMENTACION PRIMITIVAS DEL HASH
  * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;

	hash->tabla_hash = malloc(sizeof(lista_t) * TAMANIO_INICIAL);
	if (!hash->tabla_hash){
		free(hash);
		return NULL;
	}
	/*creo que falta una funcion que recorra la lista creada y ponga listas vacias

	o podriamos hacer que al agregar un dato si no tiene lista que la cree, para
	no desperdiciar memoria*/

	hash->cant_elem = 0;
	hash->tam_tabla = TAMANIO_INICIAL;
	hash->funcion_destruc = destruir_dato;
	hash->funcion_hash = &hash_djb2;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	//Aplico la funcion de hash a la clave para saber en que posicion de la lista guardo
	lista_t* lista_hash = hash->tabla_hash[hash->funcion_hash(clave)]; //obtenes en que lista vas a guardar
	if (lista_esta_vacia(lista_hash)){ //si da true guardas directamente
		nodo_hash_t* nodo_hash = crear_nodo(dato, clave);
		if (!nodo_hash) return false;
		return guardar_elemento(hash, lista_hash, nodo_hash);
	}
	//Busco si la clave esta en la lista
	if(!buscar_clave(lista_hash, clave, dato, true)){ //si esta quiero reemplazar por eso el true
		nodo_hash_t* nodo_hash = crear_nodo(dato, clave);
		if (!nodo_hash) return false;
		return guardar_elemento(hash, lista_hash, nodo_hash);
	}
}

void *hash_borrar(hash_t *hash, const char *clave){
	lista_t* lista_hash = hash->tabla_hash[hash->funcion_hash(clave)];
	if (!lista_hash) return NULL; //la lista esta vacia, por ende no va a estar

	void* dato;
	nodo_hash_t* nodo;
	lista_iter_t* iter = lista_iter_crear(lista_hash);
	while (!lista_iter_al_final(iter)){
		if(!strcmp(((nodo_hash_t*)lista_iter_ver_actual(iter))->clave, clave)){
			nodo = lista_iter_borrar(iter); //devuelve un nodo_hash_t
			dato = nodo->dato;
			free(nodo);
			lista_iter_destruir(iter);
			return dato;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos_hash = hash->funcion_hash(clave); //obtengo la posicion de la tabla donde debo buscar
	bool pertenece = buscar_clave(hash->tabla_hash[pos_hash], clave, NULL, false); //devuelve truee si esta
	return pertenece;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant_elem;
}

/* *****************************************************************
 *            IMPLEMENTACION PRIMITIVAS DEL ITERADOR DEL HASH
  * *****************************************************************/



//Funciones auxiliares
nodo_hash_t* crear_nodo(void* valor, const char* clave){
	/*Crea un nodo. Si falla el proceso de pedir memoria devuelve NULL.
	Caso contrario devuelve un nodo*/
	nodo_hash_t *nodo_nuevo = malloc(sizeof(nodo_hash_t));

 	if (!nodo_nuevo) return NULL;

 	nodo_nuevo->dato = valor;
 	nodo_nuevo->clave = clave;
 	return nodo_nuevo;
}

bool guardar_elemento(hash_t* hash, lista_t* lista, nodo_hash_t* nodo){
	bool estado = lista_insertar_primero(lista, nodo); //da true o false
	if (estado) hash->cant_elem ++; //aumento la cantidad de elementos
	return estado;
}

bool buscar_clave(lista_t* lista, const char* clave, void* dato, bool reemplazar){
	/*Si la clave no esta devuelve false. En caso de que este
	la actualiza y devuelve true.*/
	nodo_hash_t* nodo;
	lista_iter_t* iter = lista_iter_crear(lista);
	while (!lista_iter_al_final(iter)){
		nodo = lista_iter_ver_actual(iter); //te devuelve un nodo_hash_t
		if (!strcmp(clave, nodo->clave)){ //si da cero son iguales
			if (reemplazar){
				nodo->clave = clave;
				nodo->dato = dato;
			}
			return true;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return false;
}
/******************************************************************************
*							FUNCION DE HASHING 																							*
******************************************************************************/

size_t hash_djb2(const char *str){
		unsigned long hash = 5381;
		int c;
		while (c = *str++)
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
}
