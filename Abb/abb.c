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
#include <stdbool.h>
#include <stdlib.h>

typedef struct nodo_abb{
	struct nodo_abb* izq;
	struct nodo_abb* der;
	char* clave;
	void* dato;
} nodo_abb_t;

typedef struct abb{
	nodo_abb_t* raiz;
	abb_comparar_clave_t func_cmp;
	abb_destruir_dato_t func_destruc;
	size_t cant_elem;
}abb_t;

//DECLARACIONES FUNCIONES AUXILIARES
nodo_abb_t* crear_nodo_abb(const char* clave, void* dato);
bool master_search(const abb_t* abb, const char* clave, void* dato, nodo_abb_t* nodo_aux, int mode);
bool buscar_clave(nodo_abb_t* raiz, const char* clave, nodo_abb_t* nodo_aux, abb_comparar_clave_t cmp);
size_t nodo_cant_hijos(nodo_abb_t* nodo);
void* nodo_destruir(nodo_abb_t* nodo, abb_destruir_dato_t func_destruc);
nodo_abb_t* nodo_buscar_padre(abb_t* abb, nodo_abb_t* nodo);
void adoptar_nieto(abb_t* abb, nodo_abb_t* padre, const char* clave_hijo);

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
	return master_search(arbol, clave, dato, NULL, 1); //En la funcion se actualiza la cant de elem si es que no estaba
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	return master_search(arbol, clave, NULL, NULL, 3);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_abb_t* nodo = NULL;
	return (master_search(arbol, clave, nodo,NULL, 2)) ? nodo->dato : NULL;
}

/* Borra un elemento del abb y devuelve el dato asociado. Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave){
	if(!arbol) return NULL;
	nodo_abb_t* nodo;
	void* dato;
	bool pertenece = master_search(arbol, clave, dato, nodo, 4);

	if (pertenece){
		size_t cant_hijos = nodo_cant_hijos(nodo);
		if (!cant_hijos){
			void* dato = nodo_destruir(nodo, func_destruc);
			return dato;
		}
		nodo_abb_t* padre = nodo_buscar_padre(abb, clave);
		if (cant_hijos == 1){
			adoptar_nieto(abb, padre, clave);//implementar
			return dato;
		}
		if (cant_hijos == 2){				//0.0
			nodo_abb_t* reemplazante = buscar_reemplazante(abb, clave);//implementar
			/*buscar reemplazante tiene que borrar el nodo reemplazante del abb, y devolverlo*/
			nodo->clave = reemplazante->clave; //reemplazo la clave
			abb_destruir_dato_t destruir_dato = abb->func_destruc;
			dato = nodo->dato;
			//nodo destruir devuelve el dato
			nodo->dato = nodo_destruir(reemplazante);//reemplazo el dato y destruyo el nodo
			return dato;
		}
	}
	return NULL;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *arbol){
	//implementar
	if(!arbol) return;
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

size_t nodo_cant_hijos(nodo_abb_t* nodo){
	size_t cant = 0;
	if (nodo->izq)  cant++;
	if (nodo->der) cant++;
	return cant;
}

bool master_search(const abb_t* abb, const char* clave, void* dato, void* nodo_aux, int mode){ //guardar: 1  obtener: 2 pertenece: 3
	if (!abb || (!abb->cant_elem && mode == 3)) return false; //bien para los tres casos
	nodo_abb_t* nodo = NULL;

	//if (mode == 3 && !abb->cant_elem) return false; quedo contemplado con el || de arriba

	bool pertenece = buscar_clave(abb->raiz, clave, nodo, abb->func_cmp);

	switch(mode){
		case 1://guardar
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
			//abb->cant_elem ++; //explota, hay que hacerlo afuera parece que
			return true;

		case 2://obtener
			if (pertenece){
				dato = nodo->dato;
				return true;
			}
			dato = NULL;
			return false;

		case 3: return pertenece;

		case 4:
			nodo_aux = nodo;
			return pertenece;
	}
	return false;
}
/*Si encuentra la clave devuelve el true y el nodo en aux, si no la encuentra
 devuelve false y el padre supuesto*/
bool buscar_clave(nodo_abb_t* raiz, const char* clave, nodo_abb_t* nodo_aux, abb_comparar_clave_t cmp){
	if (!raiz) return false;
	nodo_aux = raiz;
	if (cmp(clave, raiz->clave) > 0) return buscar_clave(raiz->der, clave, nodo_aux, cmp); //Voy a la derecha del abb

	else if (cmp(clave, raiz->clave) < 0) return buscar_clave(raiz->izq, clave, nodo_aux, cmp);//Voy a la izquiera

	return true; //Seria el caso de que cmp de cero
}
