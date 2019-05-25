/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#define _POSIX_C_SOURCE 200809L  //Para incluir strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"

typedef struct nodo_abb{
	struct nodo_abb* izq;
	struct nodo abb* der;
	char* clave;
	void* dato;
} nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	abb_comparar_clave_t func_cmp;
	abb_destruir_dato_t func_destruc;
	size_t cant_elem;
};

//DECLARACIONES FUNCIONES AUXILIARES
nodo_abb_t* crear_nodo_abb(const char* clave, void* dato);

/* *****************************************************************
 *                  IMPLEMENTACION PRIMITIVAS DEL ABB
  * *****************************************************************/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t)); //Pido memoria para la estructura
	if (!abb) return NULL;
	abb->func_cmp = cmp;
	abb->func_destruc = destruir_dato;
	abb->cant_elem = 0;
	abb->raiz = NULL;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	//Si no me equivoco con el seguimiento que hice, este master search funciona bien si el arbol esta vacio
	//o si ya tiene cosas
	return master_search(arbol, clave, dato, 1); //En la funcion se actualiza la cant de elem si es que no estaba
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	return master_search(arbol, clave, NULL, 3);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_abb_t* nodo;
	return (master_search(arbol, clave, nodo, 2)) ? nodo->dato : NULL;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cant_elem;
}

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
  * *****************************************************************/
nodo_abb_t* crear_nodo_abb(const char* clave, void* dato){
	nodo_abb_t* nodo_abb = malloc(sizeof(nodo_abb_t));
		if (!nodo_abb) return NULL;

		nodo_abb->clave = strdup(clave);
		nodo_abb->dato = dato;
		nodo_abb->izq = NULL;
		nodo_abb->der = NULL;
		return nodo_abb;
}

bool master_search(abb_t* abb, const char* clave, void* dato, int modo){ //guardar: 1  obtener: 2 pertenece: 3
	if (!abb || (!abb->cant_elem && modo == 3)) return false; //bien para los tres casos
	nodo_abb_t* nodo = NULL;

	//if (modo == 3 && !abb->cant_elem) return false; quedo contemplado con el || de arriba

	bool pertenece = buscar_clave(abb->raiz, clave, nodo, abb->func_cmp);

	switch(modo){
		case 1: 
			if (pertenece){ //reemplazo
				if (abb->func_destruc){ //hay func de destruccion
					abb->func_destruc(nodo->dato); //destruyo el dato viejo
				}
				nodo->dato = dato; //lo piso
				return true;
			}
			//guardo el nuevo nodo
			nodo_abb_t* nuevo_nodo =  crear_nodo_abb(clave, dato);
			if (!nuevo_nodo) return false; //fallo la creacion del nodo

			if (abb->func_cmp(nodo->clave, clave) > 0) nodo->izq = nuevo_nodo;
			else nodo->der = nuevo_nodo;
			abb->cant_elem++; //actualizo la cantidad de elementos
			return true;

		case 2:
			if (pertenece){
				dato = nodo->dato;
				return true;
			}
			dato = NULL;
			return false;

		case 3: return pertenece;
	}

	/*if (modo == 3) return pertenece;

	if (modo == 2){
		if (pertenece){
			dato = nodo->dato;
			return true;
		}
		dato = NULL;
		return false;
	}
	//A esta instancia modo 1 si o si
	if (pertenece){ //reemplazo
		if (abb->func_destruc){ //hay func de destruccion
			abb->func_destruc(nodo->dato); //destruyo el dato viejo
		}
		nodo->dato = dato; //lo piso
		return true;
	}
	//guardo el nuevo nodo
	nodo_abb_t* nuevo_nodo =  crear_nodo_abb(clave, dato);
	if (!nuevo_nodo) return false; //fallo la creacion del nodo

	if (abb->func_cmp(nodo->clave, clave) > 0) nodo->izq = nuevo_nodo;
	else nodo->der = nuevo_nodo;
	return true;*/
}

bool buscar_clave(nodo_abb_t* raiz, const char* clave, nodo_abb_t* nodo_aux, abb_comparar_clave_t cmp){
	if (!raiz) return false;
	nodo_aux = raiz;
	if (cmp(clave, raiz->clave) > 0) return buscar_clave(raiz->der, clave, nodo_aux, cmp); //Voy a la derecha del abb

	else if (cmp(clave, raiz->clave) < 0) return buscar_clave(raiz->izq, clave, nodo_aux, cmp);

	return true; //Seria el caso de que cmp de cero
}
