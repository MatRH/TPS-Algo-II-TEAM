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

/*DECLARACIONES DE FUNCIONES AUZILIARES*/
size_t hash_djb2(const char *str); //funcion de HASH_H
nodo_hash_t* crear_nodo_hash(void* dato, char* clave);
bool guardar_elemento(hash_t* hash, lista_t* lista, nodo_hash_t* nodo);
bool acceder_clave(const hash_t* hash, size_t indice_tabla, const char* clave, void** dato, bool reemplazar, bool obtener);
char *strdup(const char *s);
size_t buscar_lista_no_vacia(const hash_t* hash, size_t pos);
bool poner_listas_vacias(lista_t** tabla_hash, size_t tam_hash);
bool hash_redimensionar(hash_t* hash, const size_t nueva_capacidad);
/* *****************************************************************
 *                  IMPLEMENTACION PRIMITIVAS DEL HASH
  * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;

	hash->tabla_hash = malloc(sizeof(void*) * TAMANIO_INICIAL);
	if (!hash->tabla_hash){
		free(hash);
		return NULL;
	}
	//bool estado = poner_listas_vacias(hash->tabla_hash, TAMANIO_INICIAL);
	if (!poner_listas_vacias(hash->tabla_hash, TAMANIO_INICIAL)){
		free(hash->tabla_hash);
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
	//DEBUG
	//printf("Guardando en el hash con la clave '%s'\n", clave);
	//printf("Guardando el Dato: '%p'\n", dato);

	size_t ocupada = hash->cant_elem / hash->tam_tabla;
	size_t nueva_capacidad = 2 * hash->tam_tabla - 1; //evito que de par, no se como hacer para que quede primo

	//DEBUG
	//printf("Ocupadas: '%ld' Factor de Carga: %d \n", ocupada, FACTOR_DE_CARGA);

	if (ocupada > FACTOR_DE_CARGA){
		if (!hash_redimensionar(hash, nueva_capacidad)) return false;
	}

	size_t indice = (hash->funcion_hash(clave)) % hash->tam_tabla;//obtenes en que lista vas a guardar
	char* copia_clave = strdup(clave);
	lista_t* lista_hash = hash->tabla_hash[indice];

	//DEBUG
	//printf("Indice: '%ld'\n", indice);

	if (lista_esta_vacia(lista_hash)){ //si da true guardas directamente

		//DEBUG
		//printf("La lista está vacía antes de guardar\n");

		nodo_hash_t* nodo_hash = crear_nodo_hash(dato, copia_clave);
		if (!nodo_hash) return false;
		return guardar_elemento(hash, lista_hash, nodo_hash);
		}

		//Busco si la clave esta en la lista
	if(!acceder_clave(hash, indice, copia_clave, &dato, true, false)){ //si esta quiero reemplazar por eso el true

		//DEBUG
		//printf("La lista NO está vacía antes de guardar\n");

		nodo_hash_t* nodo_hash = crear_nodo_hash(dato, copia_clave);
		if (!nodo_hash) return false;
		return guardar_elemento(hash, lista_hash, nodo_hash);
	}
		return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	lista_t* lista_hash = hash->tabla_hash[(hash->funcion_hash(clave)) % hash->tam_tabla];
	if (!lista_hash) return NULL; //la lista esta vacia, por ende no va a estar

	//Redimensiono para abajo
	float ocupada = (float)(hash->cant_elem / hash->tam_tabla); //chequear casteo
	size_t nueva_capacidad = hash->tam_tabla / 2; //no se como hacer que de primo
	if (ocupada <  FACTOR_REDUCCION && hash->cant_elem > 100){ //redimensiono //CHEQUEAR ESE 100
		if (!hash_redimensionar(hash, nueva_capacidad)) return NULL;
	}

	void* dato;
	nodo_hash_t* nodo;
	lista_iter_t* iter = lista_iter_crear(lista_hash);

	while (!lista_iter_al_final(iter)){
		nodo = lista_iter_ver_actual(iter); //te devuelve un nodo
		if (!strcmp(nodo->clave, clave)){ //encontre al que quiero borrar
			lista_iter_borrar(iter); //borro el nodo de la lista
			dato = nodo->dato;
			free(nodo->clave);
			free(nodo);
			lista_iter_destruir(iter);
			hash->cant_elem--;
			return dato;
		}
		lista_iter_avanzar(iter);
	}//lista iter borrar devuelve un void* , me podria dar el nodo y listo
	//si salgo del while el dato no estaba, debo devolver NULL
	lista_iter_destruir(iter);
	return NULL;


	/*while (!lista_iter_al_final(iter)){
		if(!strcmp(((nodo_hash_t*)lista_iter_ver_actual(iter))->clave, clave)){
			nodo = lista_iter_ver_actual(iter); //devuelve un nodo_hash_t
			dato = nodo->dato;
			free(nodo->clave);//libero la copia de la clave
			free(nodo);
			lista_iter_destruir(iter);
			hash->cant_elem--; //actualizo cantidad de elementos
			return dato; //el usuario se encarga de liberar esto
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;*/
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos_hash = (hash->funcion_hash(clave)) % hash->tam_tabla; //obtengo la posicion de la tabla donde debo buscar
	bool pertenece = acceder_clave(hash, pos_hash, clave, NULL, false, false); //devuelve truee si esta
	return pertenece;
}

void *hash_obtener(const hash_t *hash, const char *clave){
 	size_t pos_hash = (hash->funcion_hash(clave)) % hash->tam_tabla; //obtengo la posicion de la tabla donde debo buscar
	void** dato = malloc(sizeof(void*));

	//DEBUG
	//printf("Entrada a la función HASH OBTENER\n");
	//printf("Obteniendo del hash con la clave '%s'\n", clave);
	//printf("Posicion en el hash '%ld'\n", pos_hash);

	bool succes = acceder_clave(hash, pos_hash, clave, dato, false, true); //devuelve true si está

	//DEBUG
	//if(succes)printf("Se encontró la clave\n");
	if(!succes){
		//printf("NO se encontró la clave\n");
		free(*dato);
		return NULL;
		//printf("Dato obtenido: '%p'\n", *dato);
	}
	return *dato;
	free(dato); //este free no puede estar
 }

size_t hash_cantidad(const hash_t *hash){
	return hash->cant_elem;
}

void hash_destruir(hash_t *hash){
	printf("INICIO DESTRUCCION DEL HASH\n"); //DEBUG
	size_t pos = 0;
	while(pos < hash->tam_tabla){ //recorro la tabla
		lista_t* lista_actual = hash->tabla_hash[pos];
		lista_iter_t* lista_iter = lista_iter_crear(lista_actual);

		while (!lista_iter_al_final(lista_iter)){ //recorro la lista
			nodo_hash_t* nodo_actual = lista_iter_ver_actual(lista_iter);
			if (hash->funcion_destruc){
				hash->funcion_destruc(nodo_actual->dato); //aplico la funcion al dato
				//printf("Destrui el dato\n"); //DEBUG
			} //la funcion de destruccion puede ser NULL
			free(nodo_actual->clave); //borro la copia de la clave
			free(nodo_actual);			//borro el nodo
			lista_iter_avanzar(lista_iter); //Avanzo a la prox posicion
		}
		lista_iter_destruir(lista_iter); //borro el iterador
		lista_destruir(lista_actual, NULL);	 //borro la lista ahora vacia
		pos++;
	}
	printf("Sali del while de destruccion\n");
	free(hash->tabla_hash); //liberamos la tabla
	free(hash);		//borro el hash
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

//Funciones auxiliares
nodo_hash_t* crear_nodo_hash(void* valor, char* clave){
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

/*Recibe un puntero a hash, el índice de la tabla en la que debe buscar,
una clave, un puntero a un dato, y un bool reemplazar y bool obtener.
Si reemplazar es true -> actualiza la copia_clave
Si obtener es true -> devuelve el dato asociado a la clave en el puntero pasado
*/
bool acceder_clave(const hash_t* hash, size_t indice_tabla, const char* clave, void** dato, bool reemplazar, bool obtener){
	/*Si la clave no esta devuelve false. En caso de que este
	la actualiza y devuelve true.*/
	lista_t* lista = hash->tabla_hash[indice_tabla];
	nodo_hash_t* nodo;
	lista_iter_t* iter = lista_iter_crear(lista);

	//DEBUG
	//printf("Entrada a la función acceder clave\n");
	//printf("Accediendo a la clave '%s'\n", clave );
	//printf("Ubicada en el indice '%ld'\n", indice_tabla );
	//if(obtener) printf("Modo Obtener\n" );
	//if(reemplazar) printf("Modo Reemplazar\n");

	while (!lista_iter_al_final(iter)){
		nodo = lista_iter_ver_actual(iter); //te devuelve un nodo_hash_t

		//DEBUG
		//printf("El iterador de la lista no está al final\n" );

		if (!strcmp(clave, nodo->clave)){ //si da cero son iguales

			//DEBUG
			//printf("Clave encontrada\n" );

			if (reemplazar){ //la clave sigue siendo la misma, destruyo dato y guardo el nuevo

				//DEBUG
				//printf("Reemplazando\n" );
				if (hash->funcion_destruc){ //puede ser la funcion NULL
					hash->funcion_destruc(nodo->dato); //destrui el dato viejo
					//printf("Destrui el dato\n");  //DEBUG
				}
				nodo->dato = *dato; //guardo el dato actualizado
			}
			if (obtener){ //apunto dato al dato guardado
				*dato = nodo->dato;

				//DEBUG
				//printf("Obteniendo\n" );
				//printf("Dato obtenido: %p\n", nodo->dato);
				//printf("Dato devuelto: %p\n", *dato);


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

bool poner_listas_vacias(lista_t** tabla_hash, size_t tam_hash){
	/*A cada bloque de la tabla le agrega una lista vacia.
	Devuelve false en caso de error*/
	for (size_t i = 0; i < tam_hash; i++){
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

bool hash_redimensionar(hash_t* hash, const size_t nueva_capacidad){
	/*Redimensiona el hash, en caso de error devuelve false*/
	//1)Recorro cada posicion del hash viejo y hasheo cada elemento a la nueva tabla
	lista_t** nueva_tabla = malloc(sizeof(void*) * nueva_capacidad); //trata de que siempre sea primo
	if (!nueva_tabla) return false;

	//debo recorrerla y ponerle listas vacias
	if (!poner_listas_vacias(nueva_tabla, nueva_capacidad)){ //si falla libero la memoria pedida
		free(nueva_tabla);
		return false;
	}
	size_t nueva_pos; //si trae problemas ponerlo dentro del while de abajo

	for (size_t i = 0; i < hash->tam_tabla; i++){ //debes liberar las listas vacias, sino las perdes 4ever
		lista_t* lista_actual = hash->tabla_hash[i];
		if (!lista_actual){
			lista_destruir(lista_actual, NULL);
			continue;
		}

		lista_iter_t* iter_lista = lista_iter_crear(lista_actual);
		if (!iter_lista){  //chequeo que se creo correctamente el iterador
			free(nueva_tabla);
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
	}
	free(hash->tabla_hash);  //cuando salis de este for ya copiaste todas las cosas a la nueva tabla de hash, elimina la vieja
	hash->tabla_hash = nueva_tabla;
	hash->tam_tabla = nueva_capacidad; //los otros miembros del struct no cambian
	return true;
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
