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
void swap(void** arr, size_t pos, size_t max);
void downheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp);
void** heapify(void** arr, cmp_func_t cmp, size_t n);
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
	heap->datos = heapify(arreglo, cmp, n); // heapify devuelve un arreglo, aplicas heapify desde la pos (i/2) - 1, no se si estos parametros estan bien
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
void** heapify(void** arr, cmp_func_t cmp, size_t n){
	//Aplicas downheap desde (n/2) - 1 para que sea O(n)
	size_t pos = (n / 2) - 1;
	downheap(arr, pos, n, cmp);
	return arr;
}

void downheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp){
	if (pos < 0) return;
	size_t max = pos;
	size_t hijo_izq = 2 * pos + 1;
	size_t hijo_der = 2 * pos + 2;

	if (hijo_izq < tam && cmp(arr[hijo_izq], arr[max]) > 0) max = hijo_izq;

	if (hijo_der < tam && cmp(arr[hijo_der], arr[max]) > 0) max = hijo_der;
	//Veo si tengo que swapear
	if (max != pos){
		swap(arr, pos, max);
		downheap(arr, max, tam, cmp); //no se que condicion de corte ponerle a este, llega a swapear y no sale mas
	} //Bah, sale pero no se si es O(n)

	downheap(arr, --pos, tam, cmp);
}

void swap(void** arr, size_t pos, size_t max){
	void* aux = arr[pos];
	arr[pos] = arr[max]; //La posicion en la que estaba ahora tiene el mayor elemento
	arr[max] = aux; //donde estaba el amyor elemento ahora esta el ex-bigger
}