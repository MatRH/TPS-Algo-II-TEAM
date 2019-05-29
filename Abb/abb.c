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

struct abb{
	nodo_abb_t* raiz;
	abb_comparar_clave_t func_cmp;
	abb_destruir_dato_t func_destruc;
	size_t cant_elem;
}; //creo que no hace falta hacer typedef porque en el .h ya le hacen

//DECLARACIONES FUNCIONES AUXILIARES
nodo_abb_t* crear_nodo_abb(const char* clave, void* dato);
bool master_search(const abb_t* abb, const char* clave, void* dato, nodo_abb_t* nodo_aux, int mode);
bool buscar_clave(nodo_abb_t* raiz, const char* clave, nodo_abb_t* nodo_aux, abb_comparar_clave_t cmp);
size_t nodo_cant_hijos(nodo_abb_t* nodo);
void* nodo_destruir(nodo_abb_t* nodo);
nodo_abb_t* nodo_buscar_padre(abb_t* abb, const char* clave, bool *es_izq);
nodo_abb_t* buscar_padre_wrapper(nodo_abb_t* raiz, const char* clave, abb_comparar_clave_t cmp, bool* es_izq);
nodo_abb_t obtener_nieto(abb_t* abb, const char* clave);
nodo_abb_t* obtener_nieto_wrapper(nodo_abb_t* raiz, abb_comparar_clave_t cmp, const char* clave);
nodo_abb_t* buscar_reemplazante(abb, clave);
void asignar_padre(abb_t* abb, nodo_abb_t* padre, nodo_abb_t* nuevo_hijo, bool es_izq);
void destruir_wrapper(abb_t* arbol, nodo_abb_t* raiz);
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
void *abb_borrar(abb_t *arbol, const char *clave){ //clave del que voy a borrar
	if(!arbol) return NULL;
	nodo_abb_t* nodo;
	void* dato, dato_nodo;
	bool pertenece = master_search(arbol, clave, dato, nodo, 4); //El nodo que quiero borrar queda guardado

	if (pertenece){
		bool izq = true; //Variable para saber cuando busque al padre, si el hijo que voy a borrar es izq o derecho
		size_t cant_hijos = nodo_cant_hijos(nodo);
		nodo_abb_t* padre = nodo_buscar_padre(arbol, clave, &izq); //esta funcion tiene que tener cuidado si es la raiz
		//Acordate de restar uno a la cantidad en todos los casos
		if (!cant_hijos) asignar_padre(arbol, padre, NULL, izq);

		else if (cant_hijos == 1){ //Podriamos obtener el nieto, despues llamamos a la funcion de asignar y listo
			nodo_abb_t* nieto = obtener_nieto(arbol, clave); //Supongamos que anda
			asignar_padre(arbol, padre, nieto, izq);
		}
		else{				//0.0
			nodo_abb_t* reemplazante = buscar_reemplazante(arbol, clave);//implementar
			/*buscar reemplazante tiene que borrar el nodo reemplazante del abb, y devolverlo*/
			free(nodo->clave); //Libero la clave del nodo viejo
			nodo->clave = strdup(reemplazante->clave); //reemplazo la clave, pero hago una copia, porque nodo_destruir la libeta
			dato_nodo = nodo->dato; //me guardo el dato del nodo que voy a borrar
			nodo->dato = nodo_destruir(reemplazante);//reemplazo el dato y destruyo el nodo
			arbol->cant_elem--;
			return dato_nodo;
		}
		arbol->cant_elem--;
		dato_nodo = nodo_destruir(nodo);
		return dato_nodo;
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
	destruir_wrapper(arbol->raiz);
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

bool master_search(const abb_t* abb, const char* clave, void* dato, void* nodo_aux, int mode){ 
	//guardar: 1; obtener: 2; pertenece: 3; borrar: 4
	if (!abb || (!abb->cant_elem && mode == 3)) return false;
	nodo_abb_t* nodo = NULL;

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

void* nodo_destruir(nodo_abb_t* nodo){
	void* dato = nodo->dato;
	free(nodo->clave); //Libero la clave, porque es de memoria dinamica (strdup)
	free(nodo); //Libero la memoria pedida para nodo
	return dato;
}

nodo_abb_t* nodo_buscar_padre(abb_t* abb, const char* clave, bool *es_izq){
	/*Cuando busque el padre, ya voy a saber si el hijo que voy a borrar esta a la izq o der
	*Para el caso de sin hijos y con un hijo lo facilita bastante (creo), para el caso de dos hijos
	*tambien sirve, porque su hijo va a morir (drastico) y ser reemplazado, y como sabes si era izq o der
	*ya sabes a donde hacer que apunte.
	Supongo que pasarle puntero a bool va a andar, chequear despues*/
	if (abb->cant_elem == 1) return NULL; //el que quiero borrar es la raiz, no tiene padre

	return buscar_padre_wrapper(abb->raiz, clave, abb->func_cmp, es_izq);
}

nodo_abb_t* buscar_padre_wrapper(nodo_abb_t* raiz, const char* clave, abb_comparar_clave_t cmp, bool* es_izq){
	//No pongo el caso que no este porque ya buscamos antes si esta o no lo que quiero borrar
	nodo_abb_t* hijo_izq = raiz->izq;
	nodo_abb_t* hijo_der = raiz->der;

	if (hijo_izq && !cmp(clave, hijo_izq->clave)){
		*es_izq = true;
		return raiz; //esa raiz va a ser el padre
	}
	else if (hijo_der && !cmp(clave, hijo_der->clave)){
		*es_izq = false;
		return raiz;
	}
	else{ //tengo que seguir buscando, veo si voy pa' la der o izq
		if (cmp(clave, raiz->clave) > 0) return buscar_padre_wrapper(hijo_der, clave, cmp, es_izq);
		else return buscar_padre_wrapper(hijo_izq, clave, cmp, es_izq);
	} //Creo que va a andar
}

nodo_abb_t* obtener_nieto(abb_t* abb, const char* clave){
	/*Queres borrar la raiz que tiene un hijo, saber si es izq o derecho o si es la raiz ya sabes
	*Ya sabes que si o si tenes un hijo*/
	return obtener_nieto_wrapper(abb->raiz, abb->func_cmp, clave);
}

nodo_abb_t* obtener_nieto_wrapper(nodo_abb_t* raiz, abb_comparar_clave_t cmp, const char* clave){
	nodo_abb_t* hijo_izq = raiz->izq; //quizas se podria juntar con el otro wrapper, pero el cmp es !=
	nodo_abb_t* hijo_der = raiz->der;

	if (hijo_izq && !cmp(clave, raiz->clave)){
		return hijo_izq;
	}
	else if (hijo_der && !cmp(clave, raiz->clave)){
		return hijo_der;
	}
	else{ //tengo que seguir buscando, veo si voy pa' la der o izq
		if (cmp(clave, raiz->clave) > 0) return obtener_nieto_wrapper(hijo_der, cmp, clave);
		else return obtener_nieto_wrapper(hijo_izq, cmp, clave);
	} //Creo que va a andar

}

void asignar_padre(abb_t* abb, nodo_abb_t* padre, nodo_abb_t* nuevo_hijo, bool es_izq){
	//Caso 1: El padre es Null, o sea, lo que voy a borrar es la raiz, entonces, la nueva raiz es ese hijo (que puede
	//ser NULL en el caso de que lo que voy a borrar es la raiz y justo no tiene hijos)
	if (!padre){
		abb->raiz = nuevo_hijo; //Puede ser NULL como puede ser un nodo
		return;
	}
	//Caso 2: tengo padre, pero yo no tengo nuevo_hijo (caso cant_hijos == 0)
	//Caso 3: tengo padre, y tengo (caso cant_hijos == 1)
	else if (padre){
		if (!nuevo_hijo){
			if (izq) padre->izq = NULL; //El que voy a borrar esta a la izq de su padre, y a su vez no tiene hijos
			else padre->der = NULL;
			return;
		}
		//Tengo hijo
		if (izq) padre->izq = nuevo_hijo; //El que voy a borrar esta a la izq de su padre, y a su vez no tiene hijos
		else padre->der = nuevo_hijo;
		return;
	}
	return;
}

nodo_abb_t* buscar_reemplazante(abb_t* abb, const char* clave){ //Si uso esta es porque tengo dos hijos, busco sucesor

}

void destruir_wrapper(abb_t* arbol, nodo_abb_t* raiz){
	if (!raiz) return;

	destruir_wrapper(arbol, raiz->der);
	destruir_wrapper(arbol, raiz->izq);

	if (arbol->func_destruc) arbol->func_destruc(nodo_destruir(raiz));

	nodo_destruir(raiz);
	return;
}