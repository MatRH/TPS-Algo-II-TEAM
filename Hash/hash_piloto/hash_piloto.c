/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#define _POSIX_C_SOURCE 200809L  //Para que ande strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#define TAMANIO_INICIAL (size_t) 53
#define FACTOR_DE_CARGA 1 //Probar distintos valores, segun teorica puede ser 2 o 3 tmb
#define FACTOR_REDUCCION 0.75

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

struct hash_iter{
	const hash_t* hash;
	lista_iter_t* iter_lista;
	size_t pos;
};

//HACER FUNCION PARA OBTENER POSICION Y LA LISTA
//Declaraciones funciones auxiliares:
lista_t* obtener_lista(const hash_t* hash, const char* clave);

nodo_hash_t* crear_nodo_hash(const char* clave, void* dato);

bool search_or_replace(const hash_t* hash, lista_t* lista, const char* clave, void* dato, bool replace);

bool rellenar_tabla(lista_t** tabla_hash, size_t tamanio);

bool guardar_elemento(hash_t* hash, lista_t* lista, const char* clave, void* dato);

nodo_hash_t* obtener_nodo(lista_t* lista, const char* clave, bool destroy);

size_t buscar_lista_no_vacia(const hash_t* hash, size_t pos);

bool hash_redimensionar(hash_t* hash, const size_t nueva_capacidad);

size_t hash_djb2(const char *str);
/* *****************************************************************
 *                  IMPLEMENTACION PRIMITIVAS DEL HASH
  * *****************************************************************/
hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	//Pido memoria para el struct
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;

	//Pido memoria para el arreglo de listas
	hash->tabla_hash = malloc(sizeof(void*) * TAMANIO_INICIAL);
	if (!hash->tabla_hash){
		free(hash);
		return NULL;
	}

	//funcion para rellenar de listas vacias todo
	if (!rellenar_tabla(hash->tabla_hash, TAMANIO_INICIAL)){ //Devuelve false en caso de error
		free(hash->tabla_hash);
		free(hash);
		return NULL;
	}

	hash->cant_elem = 0;
	hash->tam_tabla = TAMANIO_INICIAL;
	hash->funcion_destruc = destruir_dato;
	hash->funcion_hash = &hash_djb2; //FALTA LA FUNCION DE HASH
	return hash;

}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	//Crear funcion search_or_replace
	//necesitas un nodo asi contiene la clave y el valor
	//size_t pos_tabla = hash->funcion_de_hash(clave) % hash->tam_tabla; //obtengo la posicion del hash
	//lista_t* lista_actual = hash->tabla_hash[pos_tabla];
	size_t ocupada = hash->cant_elem / hash->tam_tabla;
	size_t nueva_capacidad = 2 * hash->tam_tabla - 1; //evito que de par, no se como hacer para que quede primo

	if (ocupada > FACTOR_DE_CARGA){
		if (!hash_redimensionar(hash, nueva_capacidad)) return false; //fallo redimensionar
	}

	lista_t* lista_actual = obtener_lista(hash, clave);
	if (lista_esta_vacia(lista_actual)){ //Se que el elemento no esta, guardo directamente
		return guardar_elemento(hash, lista_actual, clave, dato); //devolvera true o false
	}
	//Si no entras a ese if, la lista no esta vacia
	if (!search_or_replace(hash, lista_actual, clave, dato, true)){ // quiero reemplazar
		//Si entro aqui es que no esta la clave
		return guardar_elemento(hash, lista_actual, clave, dato);
	}
	return true;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant_elem;
}

void *hash_borrar(hash_t *hash, const char *clave){

	//Redimensiono para abajo
	float ocupada = (float)(hash->cant_elem / hash->tam_tabla); //chequear casteo
	size_t nueva_capacidad = hash->tam_tabla / 2; //no se como hacer que de primo
	if (ocupada <  FACTOR_REDUCCION && hash->cant_elem > 100){ //redimensiono //CHEQUEAR ESE 100
		if (!hash_redimensionar(hash, nueva_capacidad)) return NULL; //Fallo redimensionar
	}

	void* dato;
	lista_t* lista_actual = obtener_lista(hash, clave); //Obtengo la lista donde puede estar o no el elemento
	nodo_hash_t* nodo = obtener_nodo(lista_actual, clave, true); //puedo tener el nodo o algo NULL
	if (!nodo) return NULL; //El elemento que quiero borrar no estaba en el hash
	dato = nodo->dato;
	free(nodo->clave);
	free(nodo);
	hash->cant_elem--;
	return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	lista_t* lista_actual = obtener_lista(hash, clave);


	nodo_hash_t* nodo = obtener_nodo(lista_actual, clave, false);
	if (!nodo){
		printf("No obtuve nodo\n");
		return NULL; //La clave no estaba, caso contrario
	}
	return nodo->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	lista_t* lista_actual = obtener_lista(hash, clave);
	return search_or_replace(hash, lista_actual, clave, NULL, false);
}

void hash_destruir(hash_t* hash){
	//La funcion de destruccion puede ser NULL, RECORDA ESTO
	//2) Si tenes listas vacias borras directamente, recorda liberar la memoria para la estructura
	//y la memoria pedida para la tabla
	size_t pos_hash = 0;

	while (pos_hash < hash->tam_tabla){
		lista_t* lista_actual = hash->tabla_hash[pos_hash++];
		lista_iter_t* lista_iter = lista_iter_crear(lista_actual);

		while (!lista_iter_al_final(lista_iter)){
			//aca la lista no esta vacia tiene datos y cosas
			nodo_hash_t* nodo = lista_iter_ver_actual(lista_iter); //tengo un nodo ahora
			if (hash->funcion_destruc){
				//Si estoy aca la funcion no es NULL
				hash->funcion_destruc(nodo->dato);
			}
			free(nodo->clave); //viene de memoria dinamica la perra
			free(nodo); //liberas el nodo en el que estas parado
			lista_iter_avanzar(lista_iter);
		}
		lista_iter_destruir(lista_iter);
		lista_destruir(lista_actual, NULL); //porque la lista esta vacia en esta instancia
	}
	free(hash->tabla_hash); //porque pedi memoria para la tabla
	free(hash); //porque pedi memoria para el struct
}

/* *****************************************************************
 *            IMPLEMENTACION PRIMITIVAS DEL ITERADOR DEL HASH
  * *****************************************************************/
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;

	iter->hash = hash;
	size_t pos_ini = buscar_lista_no_vacia(hash, 0); //posicion de la primer lista no vacia
	iter->pos = pos_ini;
	if (pos_ini == hash->tam_tabla) iter->iter_lista = NULL; //todas las listas estan vacias

	else{
		iter->iter_lista = lista_iter_crear(hash->tabla_hash[pos_ini]);//crea iterador de la lista
		if (!iter->iter_lista){ //falló la creacion del iterador de la lista
			free(iter);
			return NULL;
		}
	}
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	size_t tam_tabla = iter->hash->tam_tabla;
	bool succes;
	//Casos:
	//0)estoy al final del hash
	if (tam_tabla == iter->pos) return false;
	//1)Estoy en una lista y el iterador no esta al final
	if (!lista_iter_al_final(iter->iter_lista)){
		succes = lista_iter_avanzar(iter->iter_lista);
		//NO DEVUELVO NADA PORQUE PUEDO CAER EN 2
		if(!succes) return false;
	}
	//2)Estoy en una lista pero el iterador esta al final
	if (lista_iter_al_final(iter->iter_lista)){
		lista_iter_destruir(iter->iter_lista); //destruyo el iterador de la lista
		iter->iter_lista = NULL;
		size_t pos_nueva = buscar_lista_no_vacia(iter->hash, iter->pos + 1); //así busco a partir de la siguiente posicion
		iter->pos = pos_nueva;
		if (pos_nueva != tam_tabla){ //significa que sigo dentro de los limites de la tabla
			iter->iter_lista = lista_iter_crear(iter->hash->tabla_hash[pos_nueva]);
			if (!iter->iter_lista) return false; //hubo problemas creando el iterador
		}
	}
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	size_t tam_tabla = iter->hash->tam_tabla;
	if (iter->pos == tam_tabla) return NULL;		//el iterador está al final del hash

	lista_iter_t* iter_lista = iter->iter_lista;
	nodo_hash_t* nodo = lista_iter_ver_actual(iter_lista);
	return (nodo->clave);
}

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

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
  * *****************************************************************/
lista_t* obtener_lista(const hash_t* hash, const char* clave){
	size_t pos_tabla = hash->funcion_hash(clave) % hash->tam_tabla;
	return hash->tabla_hash[pos_tabla]; //Puede devolver listas vacias eh
}

nodo_hash_t* crear_nodo_hash(const char* clave, void* dato){
	nodo_hash_t* nodo_hash = malloc(sizeof(nodo_hash_t));
	if (!nodo_hash) return NULL;

	nodo_hash->clave = strdup(clave);
	nodo_hash->dato = dato;
	return nodo_hash;
}

bool search_or_replace(const hash_t* hash, lista_t* lista, const char* clave, void* dato, bool replace){
	lista_iter_t* iter = lista_iter_crear(lista);
	if (!iter) return false;

	while (!lista_iter_al_final(iter)){
		nodo_hash_t* nodo = lista_iter_ver_actual(iter);
		if (!strcmp(nodo->clave, clave)){
			lista_iter_destruir(iter); //libero la memoria pedida para el iterador
			if (replace){  //si replace es false significa que queria buscar, asi que devuelve true
				if (hash->funcion_destruc){
					hash->funcion_destruc(nodo->dato);
				}
				nodo->dato = dato;
			}
			return true;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return false;
}

bool rellenar_tabla(lista_t** tabla_hash, size_t tamanio){
	/*A cada bloque de la tabla le agrega una lista vacia.
	Devuelve false en caso de error*/
	for (size_t i = 0; i < tamanio; i++){
		lista_t* lista = lista_crear();
		if (!lista){
			while (i > 0){ //si i == 0 entonces no hago nada, sino borro las listas que sobran
				i--;
				lista_destruir(tabla_hash[i], NULL); //tabla_hash[i] te va a dar una lista
			}
			return false;
		}
		tabla_hash[i] = lista;
	}
	return true;
}

bool guardar_elemento(hash_t* hash, lista_t* lista, const char* clave, void* dato){
	nodo_hash_t* nodo_nuevo = crear_nodo_hash(clave, dato);
	if (!nodo_nuevo) return false;

	bool estado = lista_insertar_primero(lista, nodo_nuevo); //puede devolver false
	if (!estado){ //fallo guardar el elemento
		if (hash->funcion_destruc) hash->funcion_destruc(nodo_nuevo->dato); //puede ser NULL la funcion
		free(nodo_nuevo->clave);
		free(nodo_nuevo); //destruis el nodo basicamente
		return false; //Porque fallo guardar
	} 
	hash->cant_elem++;
	return true;
}

nodo_hash_t* obtener_nodo(lista_t* lista, const char* clave, bool destroy){
	lista_iter_t* iter = lista_iter_crear(lista);
	if (!iter) return NULL;

	while (!lista_iter_al_final(iter)){
		nodo_hash_t* nodo = lista_iter_ver_actual(iter);
		if (!strcmp(nodo->clave, clave)){
			if (destroy) lista_iter_borrar(iter); //borro el elemento de la lista
			lista_iter_destruir(iter);
			return nodo;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;
}

size_t buscar_lista_no_vacia(const hash_t* hash, size_t pos){
	/*Busca una lista no vacia a partir de la posicion pasada por parametro.*/
	while (pos < hash->tam_tabla){
		if (!lista_esta_vacia(hash->tabla_hash[pos])) return pos;
		pos++;
	} //si no encuentra una si o si devuelve el valor del tam de la tabla
	return pos;
}

bool hash_redimensionar(hash_t* hash, const size_t nueva_capacidad){
	/*Redimensiona el hash, en caso de error devuelve false*/
	//1)Recorro cada posicion del hash viejo y hasheo cada elemento a la nueva tabla
	lista_t** nueva_tabla = malloc(sizeof(void*) * nueva_capacidad); //trata de que siempre sea primo
	if (!nueva_tabla) return false;

	//debo recorrerla y ponerle listas vacias
	if (!rellenar_tabla(nueva_tabla, nueva_capacidad)){ //si falla libero la memoria pedida
		free(nueva_tabla);
		return false;
	}
	size_t nueva_pos; //si trae problemas ponerlo dentro del while de abajo

	for (size_t i = 0; i < hash->tam_tabla; i++){ //debes liberar las listas vacias, sino las perdes 4ever
		lista_t* lista_actual = hash->tabla_hash[i];
		if (lista_esta_vacia(lista_actual)){ //Si esta vacia borro directamente
			lista_destruir(lista_actual, NULL);
			continue;
		}

		lista_iter_t* iter_lista = lista_iter_crear(lista_actual);
		if (!iter_lista){  //chequeo que se creo correctamente el iterador
			free(nueva_tabla); //deberia liberar todas las que puse hasta el momento, con las cosas que puse
			return false;
		}

		while (!lista_iter_al_final(iter_lista)){
			nodo_hash_t* nodo = lista_iter_ver_actual(iter_lista); //devuelve un nodo
			nueva_pos = hash->funcion_hash(nodo->clave) % nueva_capacidad; //obtengo la nueva posicion a la que ira
			if (!lista_insertar_primero(nueva_tabla[nueva_pos], nodo)){ //nueva_tabla[pos] devuelve una lista
				//si falla insertar, libero la memoria pedida para la nueva tabla
				free(nueva_tabla);
				return false;
			}
			lista_iter_avanzar(iter_lista);
		}
		lista_iter_destruir(iter_lista);
		lista_destruir(lista_actual, NULL);
	}
	free(hash->tabla_hash);  //cuando salis de este for ya copiaste todas las cosas a la nueva tabla de hash, elimina la vieja
	hash->tabla_hash = nueva_tabla;
	hash->tam_tabla = nueva_capacidad; //los otros miembros del struct no cambian
	return true;
}

/* *****************************************************************
 *                      FUNCIONES DE HASH
  * *****************************************************************/
size_t hash_djb2(const char *str){
		unsigned long hash = 5381;
		int c;
		while ((c = *str++))
				hash = ((hash << 5) + hash) + (size_t)c; /* hash * 33 + c */

		return hash;
}