/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "heap.h"
#define TAMANIO_INICIAL (size_t) 100

struct heap{
	void** datos;
	size_t cant_elem;
	size_t capacidad;
	cmp_func_t cmp;
};

//Declaraciones funciones auxiliares


/* *****************************************************************
 *                  IMPLEMENTACION PRIMITIVAS DEL HEAP
  * *****************************************************************/
heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	heap->datos = malloc(sizeof(void*) * TAMANIO_INICIAL);
	if (!heap->datos){
		free(heap);
		return NULL;
	}
	heap->cant_elem = 0;
	heap->capacidad = TAMANIO_INICIAL;
	heap->cmp = cmp;
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;
	//en la lista de correos dicen que hay que hacer una copia al parecer del arreglo
	heap->datos = heapify(arreglo, cmp); // heapify devuelve un arreglo, aplicas heapify desde la pos (i/2) - 1, no se si estos parametros estan bien
	heap->cant_elem = n;
	heap->capacidad = n + TAMANIO_INICIAL; //fijate esta despues
	heap->cmp = cmp;
	return heap;

}

size_t heap_cantidad(const heap_t *heap){
	return heap->cant_elem;
}

bool heap_esta_vacio(const heap_t *heap){
	return !heap->cant_elem;
}

void *heap_ver_max(const heap_t *heap){
	return (!heap->cant_elem) ? NULL : heap->datos[0];
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){ //La funcion de destruccion no es NULL
		for (size_t i = 0; i < heap->cant_elem; i++)
			destruir_elemento(heap->datos[i]);
	}
	free(heap->datos);
	free(heap);
}

//Funciones auxiliares