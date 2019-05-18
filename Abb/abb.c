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
	if (!arbol->raiz){ //El arbol no contiene elementos, sera la raiz este nodo
		//Primero creo el nodo
		nodo_abb_t* nodo_abb = crear_nodo_abb(clave, dato);
		if (!nodo_abb) return false;
		arbol->raiz = nodo_abb;
		arbol->cant_elem++;
		return true;
	}
	//El abb no estaba vacio si llego a esta instancia
	if (/*aca falta algo que se fije si esta la calve*/){
		/*La funcion deberia hacer lo siguiente:
		 *Si esta, la reemplaza, por ende un bool replace tendria que tener
		 *que devuelva booleano asi sabemos si entrar aca o no*/
		//Cuando busque quizas si no esta deberia saber donde guardar, sino
		//estaria buscando dos veces esa posicion donde guardar, 

		arbol->cant_elem++;
		return true
	}
	//Si no entras al if de arriba, reemplazaste el elemento, entonces esa funcion devolveria true
	//con el operador ! seria false, y bueno aca esta el true que debes devolver
	return true;	

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
