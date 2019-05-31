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
void swap(void** arr, size_t pos1, size_t pos2);
void downheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp);
void downheap_all(void** arr, size_t pos, size_t tam, cmp_func_t cmp);
void** heapify(void** arr, cmp_func_t cmp, size_t n);
void upheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp);
bool heap_redimensionar(heap_t* heap_viejo);
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
	heap->capacidad = n + n/2; //para que tenga espacio libre extra, proporcional al asignado
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
	if(!heap) return NULL;
	return (!heap->cant_elem) ? NULL : heap->datos[0];
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){ //La funcion de destruccion no es NULL
		for (size_t i = 0; i < heap->cant_elem; i++){
			destruir_elemento(heap->datos[i]);
			free(heap->datos); //solo borro el array de datos si puedo borrar los datos
		}
	}
	free(heap);
}

bool heap_encolar(heap_t *heap, void *elem){
	size_t cantidad = heap->cant_elem;
	if (cantidad == heap->capacidad){
		if(!heap_redimensionar(heap)) return false;
	}
	(heap->datos)[cantidad] = elem;
	heap->cant_elem++;
	upheap(heap->datos, cantidad, heap->capacidad, heap->cmp);
	return true;
}

void *heap_desencolar(heap_t *heap){
	if(!heap || !heap->cant_elem) return NULL;
	void** arr = heap->datos;
	void* tope = arr[0];
	//cambio la raiz por el último
	swap(arr, 0, heap->cant_elem);
	heap->cant_elem--; //el próximo que encole va a pisar al viejo
	downheap(arr, 0, heap->cant_elem, heap->cmp);
	return tope;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	elementos = heapify(elementos, cmp, cant);
}

//Funciones auxiliares
void** heapify(void** arr, cmp_func_t cmp, size_t n){
	//Aplicas downheap desde (n/2) - 1 hacia arriba
	size_t pos = (n / 2) - 1;
	downheap_all(arr, pos, n, cmp);
	return arr;
}

bool heap_redimensionar(heap_t* heap_viejo){ //O(n)
	heap_t* heap_nuevo = heap_crear_arr(heap_viejo->datos, heap_viejo->cant_elem, heap_viejo->cmp);
	if (!heap_nuevo) return false;
	heap_viejo->datos = heap_nuevo->datos; //me llevo el arreglo de datos
	heap_viejo->capacidad = heap_nuevo->capacidad;//actualizo la capacidad
	heap_destruir(heap_nuevo, NULL); //destruyo la estructura pero no los datos
	return true;
}

void downheap_all(void** arr, size_t pos, size_t tam, cmp_func_t cmp){
	if (pos >= tam) return;
	size_t max = pos;
	size_t hijo_izq = 2 * pos + 1;
	size_t hijo_der = 2 * pos + 2;

	if (hijo_izq < tam && cmp(arr[hijo_izq], arr[max]) > 0) max = hijo_izq;

	if (hijo_der < tam && cmp(arr[hijo_der], arr[max]) > 0) max = hijo_der;
	//Veo si tengo que swapear
	if (max != pos){
	 	max = (cmp(arr[hijo_izq], arr[hijo_der]) > 0) ? hijo_der : hijo_izq; //me quedo con el hijo más grande
		swap(arr, pos, max);
		downheap_all(arr, max, tam, cmp); //hago downheap desde donde deje al padre
	}
	if (pos == 0) return;
	downheap_all(arr, --pos, tam, cmp);
}

void downheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp){
	if (pos >= tam) return;
	size_t max = pos;
	size_t hijo_izq = 2 * pos + 1;
	size_t hijo_der = 2 * pos + 2;

	if (hijo_izq < tam && cmp(arr[hijo_izq], arr[max]) > 0) max = hijo_izq;

	if (hijo_der < tam && cmp(arr[hijo_der], arr[max]) > 0) max = hijo_der;
	//Veo si tengo que swapear
	if (max != pos){
	 	max = (cmp(arr[hijo_izq], arr[hijo_der]) > 0) ? hijo_der : hijo_izq; //me quedo con el hijo más grande
		swap(arr, pos, max);
		downheap(arr, max, tam, cmp); //hago downheap desde donde deje al padre
	}
}

void upheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp){
	if (pos == 0 || pos > tam) return;
	size_t padre = (pos -1) /2;

	if (cmp(arr[padre], arr[pos]) > 0){
		swap(arr, pos, padre);
		upheap(arr, padre, tam, cmp); //hago upheap desde donde deje al padre
	}
}

void swap(void** arr, size_t pos1, size_t pos2){
	void* aux = arr[pos1];
	arr[pos1] = arr[pos2]; //Cambio la posicion del primero
	arr[pos2] = aux; //cambio la posicion del segundo
}
