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

struct abb_iter{
	//COMPLETAR
	const abb_t* arbol;
	nodo_abb_t* actual;
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
void asignar_padre(abb_t* abb, family_t* flia);
void destruir_wrapper(abb_t* arbol, nodo_abb_t* raiz);
family_t* crear_familia(void);
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

	bool pertenece = master_search(arbol, clave, NULL, 4, flia); //El nodo que quiero borrar queda guardado

	if (pertenece){
		if (!flia->cant_hijos) asignar_padre(arbol, flia);

		else if (flia->cant_hijos == 1) asignar_padre(arbol, flia);

		else{				//0.0
			family_t* flia_reemplazante = crear_familia();
			if (!flia_reemplazante) return NULL; //Se podria hacer todo con una no se que conviene

			nodo_abb_t* reemplazante = buscar_reemplazante(flia->padre->der, flia_reemplazante);
			//Si todo funca, ya tenes el reemplazante
			//Como maximo el sucesor puede tener un hijo a su derecha
			if (flia_reemplazante->abuelo) flia_reemplazante->abuelo->izq = flia_reemplazante->hijo_der;

			/*buscar reemplazante tiene que borrar el nodo reemplazante del abb, y devolverlo*/
			free(flia->padre->clave); //Libero la clave del nodo viejo
			flia->padre->clave = strdup(reemplazante->clave); //reemplazo la clave, pero hago una copia, porque nodo_destruir la libeta

			dato_nodo = flia->padre->dato; //me guardo el dato del nodo que voy a borrar
			flia->padre->dato = reemplazante->dato; //Actualizo el dato del que "borre"
			free(reemplazante); //Ya termine de copiar las cosas del sucesor, libero la memoria pedida para este
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
/* *****************************************************************
 *     IMPLEMENTACION PRIMITIVAS DEL ITERADOR EXTERNO DEL ABB
  * *****************************************************************/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter_abb = malloc(sizeof(abb_iter_t));
	if (!iter_abb) return NULL;

	iter_abb->arbol = arbol;
	iter_abb->actual = arbol->raiz;
	return iter_abb;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	return (iter->actual) ? strdup(iter->actual->clave) : NULL;
	//Creo que tiene que devolver una copia no la original
	//que despues el usuario se encargue de borrarla
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return !iter->actual; //No se bien como funciona porque todas las hojas tienen NULL
}

void abb_iter_in_destruir(abb_iter_t* iter){
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
			/*//guardo el nuevo nodo
			nodo_abb_t* nuevo_nodo =  crear_nodo_abb(clave, dato);
			if (!nuevo_nodo) return false; //fallo la creacion del nodo

			//if (abb->func_cmp(flia->abuelo->clave, clave) > 0) flia->abuelo->izq = nuevo_nodo;
			if (flia->es_izq) flia->abuelo->izq = nuevo_nodo;
			else flia->abuelo->der = nuevo_nodo;
			return true;*/ //Como es const abb esto no se puede hacer aca, rompe, devolvemos false a proposito
			return false;
		case 2:
		case 3:
		case 4:
			return pertenece; //Los 3 casos devuelven lo mismo porque todo queda en flia
	}
	return false;
}

/*Un entero menor que 0 si la primera cadena es menor que la segunda.
Un entero mayor que 0 si la primera cadena es mayor que la segunda.
0 si ambas claves son iguales.*/ //Para guiarme si esta bien

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

void asignar_padre(abb_t* abb, family_t* flia){
	//El abuelo adopta a un nieto
	//Caso 1: No tenes padre, entonces queres borrar la raiz significa, si la raiz no tiene hijos el arbol queda vacio
	//Si tenes un hijo es izq O der, no tenes 2, te fijas cual es el que vive y esa es la nueva raiz
	if (!flia->abuelo){  //Asignar padre se llama si tenes cero o un hijo, ergo nunca vas a tener que buscar reemplazante aca
		if (flia->cant_hijos){
			abb->raiz = (flia->es_izq) ? flia->hijo_izq : flia->hijo_der;
			return;
		}
		abb->raiz = NULL;
		return;
	}
	//Caso 2: tengo padre, pero yo no tengo nuevo_hijo (caso cant_hijos == 0)
	//Caso 3: tengo padre, y tengo (caso cant_hijos == 1)
	if (flia->es_izq) flia->abuelo->izq = flia->hijo_izq; //El que voy a borrar esta a la izq o der de su padre
	else flia->abuelo->der = flia->hijo_der; //Nuevo_hijo puede ser NULL asi que cumpliria los casos
	return;
}

nodo_abb_t* buscar_reemplazante(nodo_abb_t* raiz, family_t* flia){ //Si uso esta es porque tengo dos hijos, busco sucesor
	/*Si o si tenes dos hijos si llamas a esta funzion, asi que raiz no va a ser NULL*/
	if (!raiz->izq){ //Encontre el sucesor si no puedo ir mas a la izq
		flia->hijo_der = raiz->der; //El sucesor como maximo tiene un hijo y es derecho si o si
		return raiz; //Donde estoy parado es el sucesor del que quiero borrar
	}
	flia->abuelo = raiz;
	return buscar_reemplazante(raiz->izq, flia);
}

void destruir_wrapper(abb_t* arbol, nodo_abb_t* raiz){ //CHEQUEAR
	if (!raiz) return;

	destruir_wrapper(arbol, raiz->der);
	destruir_wrapper(arbol, raiz->izq);

	if (arbol->func_destruc) arbol->func_destruc(nodo_destruir(raiz));

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
