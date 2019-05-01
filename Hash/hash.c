/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#define TAMANIO_INICIAL 53

typedef struct nodo_hash{
	void* dato;
	char* clave;
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
nodo_hash_t* crear_nodo(void* dato, char* clave);
bool guardar_elemento(hash_t* hash, lista_t* lista, nodo_hash_t* nodo);
bool buscar_clave(const hash_t* hash, size_t indice_tabla, const char* clave, void* dato, bool reemplazar);
char *strdup(const char *s);
struct hash_iter{
	hash_t* hash;
	lista_iter_t* iter_lista;
	size_t pos;
};
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
	//lista_t* lista_hash = hash->tabla_hash[(hash->funcion_hash(clave)) % hash->tam_tabla];
	size_t indice = (hash->funcion_hash(clave)) % hash->tam_tabla;//obtenes en que lista vas a guardar
	char* copia_clave = strdup(clave);
	lista_t* lista_hash = hash->tabla_hash[indice];
		if (lista_esta_vacia(lista_hash)){ //si da true guardas directamente
		nodo_hash_t* nodo_hash = crear_nodo(dato, copia_clave);
		if (!nodo_hash) return false;
		return guardar_elemento(hash, lista_hash, nodo_hash);
	}
	//Busco si la clave esta en la lista
	if(!buscar_clave(hash, indice, copia_clave, dato, true)){ //si esta quiero reemplazar por eso el true
		nodo_hash_t* nodo_hash = crear_nodo(dato, copia_clave);
		if (!nodo_hash) return false;
		return guardar_elemento(hash, lista_hash, nodo_hash);
	}
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	lista_t* lista_hash = hash->tabla_hash[(hash->funcion_hash(clave)) % hash->tam_tabla];
	if (!lista_hash) return NULL; //la lista esta vacia, por ende no va a estar

	void* dato;
	nodo_hash_t* nodo;
	lista_iter_t* iter = lista_iter_crear(lista_hash);
	while (!lista_iter_al_final(iter)){
		if(!strcmp(((nodo_hash_t*)lista_iter_ver_actual(iter))->clave, clave)){
			nodo = lista_iter_borrar(iter); //devuelve un nodo_hash_t
			dato = nodo->dato;
			free(nodo->clave);//libero la copia de la clave
			free(nodo);
			lista_iter_destruir(iter);
			return dato; //el usuario se encarga de liberar esto
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos_hash = (hash->funcion_hash(clave)) % hash->tam_tabla; //obtengo la posicion de la tabla donde debo buscar
	bool pertenece = buscar_clave(hash, pos_hash, clave, NULL, false); //devuelve truee si esta
	return pertenece;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant_elem;
}

/* *****************************************************************
 *            IMPLEMENTACION PRIMITIVAS DEL ITERADOR DEL HASH
  * *****************************************************************/
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;

	iter->hash;
	size_t pos_ini = buscar_lista_no_vacia(hash, 0);
	iter->pos = pos_ini;
	if (pos_ini == hash->tam_tabla) iter->iter_lista = NULL; //todas las listas estan vacias

	else{
		iter->iter_lista = lista_iter_crear(hash->tabla_hash[pos_ini]);
		if (!iter->iter_lista){ //fallo la creacion del iterador de la lista
			free(iter);
			return NULL;
		}
	}
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	if (iter->hash->tam_tabla == iter->pos) return false; //estoy al final del hash

	//Casos:
	//1)Estoy en una lista y el iterador no esta al final
	if (!lista_iter_al_final(iter->iter_lista)){
		return lista_iter_avanzar(iter->iter_lista);
	}
	//2)Estoy en una lista pero el iterador esta al final
	lista_iter_destruir(iter->iter_lista); //destruyo el iterador
	iter->iter_lista = NULL;
	size_t pos_nueva = buscar_lista_no_vacia(iter->hash, iter->pos + 1); //asi busco a partir de la siguiente posicion
	iter->pos = pos_nueva;
	if (pos_nueva != iter->has->tam_tabla){ //significa que sigo dentro de los limites de la tabla, si no entrp a este if llegue al final del hash
		iter->iter_lista = lista_iter_crear(iter->hash->tam_tabla[pos_nueva]);
		if (!iter->iter_lista) return false; //hubo problemas creando el iterador
	}
	return true
}

//const char *hash_iter_ver_actual(const hash_iter_t *iter)

bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->hash->tam_tabla == iter->pos;
}

void hash_iter_destruir(hash_iter_t* iter){
	if (iter->hash->tam_tabla == iter->pos) free(iter); //se que no tengo iter_lista en esa posicion
	else{
		lista_iter_destruir(iter->iter_lista);
		free(iter);
	}
}


//Funciones auxiliares
nodo_hash_t* crear_nodo(void* valor, char* clave){
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

bool buscar_clave(const hash_t* hash, size_t indice_tabla, const char* clave, void* dato, bool reemplazar){
	/*Si la clave no esta devuelve false. En caso de que este
	la actualiza y devuelve true.*/
	lista_t* lista = hash->tabla_hash[indice_tabla];
	nodo_hash_t* nodo;
	lista_iter_t* iter = lista_iter_crear(lista);
	while (!lista_iter_al_final(iter)){
		nodo = lista_iter_ver_actual(iter); //te devuelve un nodo_hash_t
		if (!strcmp(clave, nodo->clave)){ //si da cero son iguales
			if (reemplazar){ //la clave sigue siendo la misma, destruyo dato y guardo el nuevo
				hash->funcion_destruc(nodo->dato); //destrui el dato viejo
				nodo->dato = dato; //guardo el dato actualizado
			}
			return true;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return false;
}

size_t buscar_lista_no_vacia(const hash_t* hash, size_t pos){
	/*Busca una lista no vacia a partir de la posicion pasada por parametro.*/
	while (pos < hash->tam_tabla){
		if (!lista_esta_vacia(hash->tabla_hash[pos])) return pos;
		pos++;
	} //si no encuentra una si o si devuelve el valor del tam de la tabla
	return pos;
}

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}
/******************************************************************************
*							FUNCION DE HASHING 																							*
******************************************************************************/

size_t hash_djb2(const char *str){
		unsigned long hash = 5381;
		int c;
		while ((c = *str++))
				hash = ((hash << 5) + hash) + (size_t)c; /* hash * 33 + c */

		return hash;
}
