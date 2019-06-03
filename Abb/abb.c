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
#include "pila.h"
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
};

struct abb_iter{
	const abb_t* arbol;
	pila_t* stack;
};

typedef struct family{
	nodo_abb_t* abuelo;
	nodo_abb_t* padre; //Padre es para el que llamé
	nodo_abb_t* hijo_izq;
	nodo_abb_t* hijo_der;
	size_t cant_hijos;
	bool es_izq; //Para facilitar el tema de borrar, asi se si el abuelo tiene el hijo a la izq o der
} family_t;

//DECLARACIONES FUNCIONES AUXILIARES
nodo_abb_t* crear_nodo_abb(const char* clave, void* dato);
bool master_search(const abb_t* abb, const char* clave, void* dato, int mode, family_t* flia);
bool buscar_clave(nodo_abb_t* raiz, const char* clave, family_t* flia, int mode, abb_comparar_clave_t cmp);
void nodo_cant_hijos(nodo_abb_t* nodo, family_t* flia);
void* nodo_destruir(nodo_abb_t* nodo);
nodo_abb_t* buscar_reemplazante(nodo_abb_t* raiz, family_t* flia);
void reorganizar_flia(abb_t* abb, family_t* flia, family_t* flia_reemplazante);
void destruir_wrapper(abb_t* arbol, nodo_abb_t* raiz);
family_t* crear_familia(void);
void abb_in_order_root(nodo_abb_t* raiz, bool visitar(const char *, void *, void *), void* extra);
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
	family_t* flia = crear_familia();
	if (!flia) return false;

	bool guardado = master_search(arbol, clave, dato, 1, flia);
	if (!guardado && !flia->padre){ //!padre significa que no estaba y !guardado que no lo guarde
		nodo_abb_t* nuevo_nodo =  crear_nodo_abb(clave, dato);
		if (!nuevo_nodo){
			free(flia);
			return false; //fallo la creacion del nodo
		}

		if (!arbol->cant_elem) arbol->raiz = nuevo_nodo; //No tenia nada es la raiz

		else{ //El arbol no esta vacio
			if (flia->es_izq) flia->abuelo->izq = nuevo_nodo; //porque flia->padre es null
			else flia->abuelo->der = nuevo_nodo;
		}
		arbol->cant_elem++; //Si guardado es true el proceso funciono, y si no tengo un padre
		free(flia);
		return true;
	}
	//Si llego aca es porque guardado es true seguramente, entonces lo actualice, asi que libero memoria y chau
	free(flia);
	return guardado; //cambiale el nombre muy pedorro este
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	family_t* flia = crear_familia();
	if (!flia) return false;
	bool pertenece = master_search(arbol, clave, NULL, 3, flia);
	free(flia);  //aca pasarle flia es al pedo, pero habria que modificar mucho si no le pasamos
	return pertenece;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	void* dato = NULL; //de entrada suponemos que no esta
	family_t* flia = crear_familia();
	if (!flia) return NULL;

	bool obtener = master_search(arbol, clave, NULL, 2, flia);

	if (obtener) dato = flia->padre->dato; //Obtener da true, asi que padre es distinto de NULL
	free(flia);
	return dato;
}

void *abb_borrar(abb_t *arbol, const char *clave){ //clave del que voy a borrar
	if(!arbol) return NULL;
	void* dato_nodo = NULL;
	family_t* flia = crear_familia();
	if (!flia) return NULL;

	bool pertenece = master_search(arbol, clave, NULL, 4, flia); //El nodo que quiero borrar queda guardado en padre SIEMPRE

	if (pertenece){
		if (!flia->cant_hijos) reorganizar_flia(arbol, flia, NULL);

		else if (flia->cant_hijos == 1) reorganizar_flia(arbol, flia, NULL);

		else{				//La cantidad de hijos es 2 si llego aca
			family_t* flia_reemplazante = crear_familia();
			if (!flia_reemplazante){
				free(flia);
				return NULL;
			}
			nodo_abb_t* reemplazante = buscar_reemplazante(flia->padre->der, flia_reemplazante);
			//Como maximo el sucesor puede tener un hijo a su derecha
			reorganizar_flia(arbol, flia, flia_reemplazante);

			free(flia->padre->clave); //Libero la clave del nodo viejo
			flia->padre->clave = strdup(reemplazante->clave); //reemplazo la clave, pero hago una copia, porque nodo_destruir la libeta
			dato_nodo = flia->padre->dato; //me guardo el dato del nodo que voy a borrar
			flia->padre->dato = nodo_destruir(reemplazante); //Actualizo el dato del que "borre", nodo destruir libera la clave y la memoria pedida para el nodo
			free(flia_reemplazante);
		}
		arbol->cant_elem--;
		if (flia->cant_hijos != 2) dato_nodo = nodo_destruir(flia->padre); //Porque si es 2 lo obtuve arriba
		free(flia);
		return dato_nodo;
	}
	free(flia);
	return NULL;
}

void abb_destruir(abb_t *arbol){
	if(!arbol) return;
	destruir_wrapper(arbol, arbol->raiz);
	free(arbol);
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cant_elem;
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (!arbol) return;
	abb_in_order_root(arbol->raiz, visitar, extra);
	return;
}

void abb_in_order_root(nodo_abb_t* raiz, bool visitar(const char *, void *, void *), void* extra){
	if(!raiz) return;
	abb_in_order_root(raiz->izq, visitar,extra );
	bool go = visitar(raiz->clave, raiz->dato, extra);
	if(!go) return;
	abb_in_order_root(raiz->der, visitar, extra );
	return;
}

/* *****************************************************************
 *     IMPLEMENTACION PRIMITIVAS DEL ITERADOR EXTERNO DEL ABB
  * *****************************************************************/
/*FUNCION AUXILIAR*/
void apilar_izquierdos(nodo_abb_t* raiz, pila_t* stack){
	if(!raiz || !stack) return;
	pila_apilar(stack, raiz);
	apilar_izquierdos(raiz->izq, stack);
}

/*PRIMITIVAS*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter_abb = malloc(sizeof(abb_iter_t));
	if (!iter_abb) return NULL;

	iter_abb->arbol = arbol;
	iter_abb->stack = pila_crear();
	apilar_izquierdos(arbol->raiz, iter_abb->stack);

	return iter_abb;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	nodo_abb_t* tope = pila_ver_tope(iter->stack);
	return (tope) ? strdup(tope->clave) : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->stack);
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)) return false;
	nodo_abb_t* tope = pila_desapilar(iter->stack);
	if (tope->der)  pila_apilar(iter->stack, tope->der);//apilo el derecho si tiene
	apilar_izquierdos(tope->izq, iter->stack);//apilo todos los hijos izquierdos
	return true;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->stack);
	free(iter);
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

void nodo_cant_hijos(nodo_abb_t* nodo, family_t* flia){
	flia->hijo_der = nodo->der;
	flia->hijo_izq = nodo->izq;
	if (flia->hijo_izq)  flia->cant_hijos++;
	if (flia->hijo_der) flia->cant_hijos++;
	return;
}

bool master_search(const abb_t* abb, const char* clave, void* dato, int mode, family_t* flia){
	/*Modos:
	*Guardar = 1, Obtener = 2, Pertenece = 3, Borrar = 4
	*/
	if (!abb || (!abb->cant_elem && mode > 1)) return false; //si no tengo elementos para los casos 2,3,4 al pedo buscar

	bool pertenece = buscar_clave(abb->raiz, clave, flia, mode, abb->func_cmp);

	switch(mode){ //todo queda guardado en la flia ahora supuestamente
		case 1:
			if (pertenece){ //reemplazo
				if (abb->func_destruc){ //hay func de destruccion
					abb->func_destruc(flia->padre->dato); //destruyo el dato viejo
				}
				flia->padre->dato = dato; //lo reemplazo por el nuevo
				return true;
			}
			return false;
		case 2:
		case 3:
		case 4:
			return pertenece; //Los 3 casos devuelven lo mismo porque todo queda en flia
	}
	return false;
}

bool buscar_clave(nodo_abb_t* raiz, const char* clave, family_t* flia, int mode, abb_comparar_clave_t cmp){
	if (!raiz){
		flia->padre = NULL; //No esta el que quiero borrar
		return false; //no hay arbol
	}
	flia->padre = raiz;

	if (cmp(clave, raiz->clave) > 0){
		flia->abuelo = raiz;
		flia->es_izq = false;
		return buscar_clave(raiz->der, clave, flia, mode, cmp); //Voy a la derecha del abb
	}
	else if (cmp(clave, raiz->clave) < 0){
		flia->abuelo = raiz;
		flia->es_izq = true;
		return buscar_clave(raiz->izq, clave, flia, mode, cmp);//Voy a la izquiera
	}
	//Si llego hasta aca es que la funcion de comparacion dio cero, veo el tema de hijos
	if (mode == 4) nodo_cant_hijos(raiz, flia); //Solo si estas en modo borrar te interesa la cantidad de hijos
	return true; //Seria el caso de que cmp de cero
}

void* nodo_destruir(nodo_abb_t* nodo){
	void* dato = nodo->dato;
	free(nodo->clave); //Libero la clave, porque es de memoria dinamica (strdup)
	free(nodo); //Libero la memoria pedida para nodo
	return dato;
}

void reorganizar_flia(abb_t* abb, family_t* flia, family_t* flia_reemplazante){
	nodo_abb_t* nieto = NULL;

	if (flia_reemplazante) nieto = flia_reemplazante->hijo_der; //Puede ser NULL como no

	else nieto = (flia->hijo_izq) ? flia->hijo_izq : flia->hijo_der; //Puede quedarte un nodo o NULL

	if (!flia_reemplazante){
		if (!flia->abuelo) abb->raiz = nieto;
		else{
			if (flia->es_izq) flia->abuelo->izq = nieto;
			else flia->abuelo->der = nieto;
		}
	}
	else{
		if (!flia_reemplazante->abuelo) flia->padre->der = nieto;
		
		else flia_reemplazante->abuelo->izq = nieto;
	}
}

nodo_abb_t* buscar_reemplazante(nodo_abb_t* raiz, family_t* flia){ //Si uso esta es porque tengo dos hijos, busco sucesor
	/*Si o si tenes dos hijos si llamas a esta funcion, asi que raiz no va a ser NULL*/
	if (raiz->izq == NULL){ //Encontre el sucesor si no puedo ir mas a la izq
		flia->hijo_der = raiz->der; //El sucesor como maximo tiene un hijo y es derecho si o si
		return raiz; //Donde estoy parado es el sucesor del que quiero borrar
	}
	flia->abuelo = raiz;
	return buscar_reemplazante(raiz->izq, flia);
}

void destruir_wrapper(abb_t* arbol, nodo_abb_t* raiz){
	if (!raiz) return;

	destruir_wrapper(arbol, raiz->izq);
	destruir_wrapper(arbol, raiz->der);

	if (arbol->func_destruc) arbol->func_destruc(nodo_destruir(raiz)); //Nodo destruir libera la clave y la memoria del nodo

	else nodo_destruir(raiz);
	return;
}

family_t* crear_familia(void){
	family_t* familia = malloc(sizeof(family_t));
	if (!familia) return NULL;
	familia->abuelo = NULL;
	familia->padre = NULL;
	familia->hijo_izq = NULL;
	familia->hijo_der = NULL;
	familia->cant_hijos = 0;
	familia->es_izq = true;
	return familia;
}
