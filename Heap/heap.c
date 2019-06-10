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
#define TAMANIO_INICIAL (size_t) 50 //La cambie para probar mas facil las de volumen DEBUG

struct heap{
	void** datos;
	size_t cant_elem;
	size_t capacidad;
	cmp_func_t cmp;
};

//Declaraciones funciones auxiliares
void swap(void** arr, size_t pos1, size_t pos2);
void downheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp);
void** heapify(void** arr, cmp_func_t cmp, size_t n);
void upheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp);
bool heap_redimensionar(heap_t* heap_viejo);
void copiar_arreglo(void** origen, size_t largo, void** destino);
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
	size_t capacidad = 1 + n + n / 2; //Porque si te pasan n == 0 explota, nos garantiza que algo se guarda
	void **arr = malloc(sizeof(void*) * capacidad);
	if (!arr) return NULL;
	copiar_arreglo(arreglo, n, arr); //Copia el arreglo pasado por parametro al arreglo de destino que hemos creado

	heap->datos = heapify(arr, cmp, n); // heapify devuelve un arreglo, aplicas heapify desde la pos (i/2) - 1,
	heap->cant_elem = n;
	heap->capacidad = capacidad; //para que tenga espacio libre extra, proporcional al asignado
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
	return (!heap || !heap->cant_elem) ? NULL : heap->datos[0];
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){ //La funcion de destruccion no es NULL
		for (size_t i = 0; i < heap->cant_elem; i++) destruir_elemento(heap->datos[i]);
	}
	free(heap->datos);
	free(heap);
}

bool heap_encolar(heap_t *heap, void *elem){
	if (heap->cant_elem == heap->capacidad){
		if(!heap_redimensionar(heap)) return false;
	}
	heap->datos[heap->cant_elem] = elem; 
	upheap(heap->datos, heap->cant_elem++, heap->capacidad, heap->cmp);
	return true;
}

void *heap_desencolar(heap_t *heap){
	if(!heap || !heap->cant_elem) return NULL; 
	void** arr = heap->datos;
	void* tope = arr[0];
	//cambio la raiz por el último
	swap(arr, 0, --heap->cant_elem);
	downheap(arr, 0, heap->cant_elem, heap->cmp);
	return tope;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	if (cant < 1) return;  //Porque si esta vacio o tiene un elemento ya esta ordenado
	elementos = heapify(elementos, cmp, cant); //Primer paso, darle forma de heap al arreglo
	//Segundo paso, iteramos y swapeamos la raiz con el "ultimo relativo"
	size_t ultimo_relativo = cant - 1;
	while (ultimo_relativo){
		swap(elementos, 0, ultimo_relativo); //Intercambiamos el mayor con el ultimo elemento del arreglo
		downheap(elementos, 0, --ultimo_relativo, cmp); //Reacomodas el heap, pero antes actualizas ultimo relativo
	}

}

//Funciones auxiliares
void** heapify(void** arr, cmp_func_t cmp, size_t n){
	//Aplicas downheap desde (n/2) - 1 hacia arriba
	size_t pos = (n / 2) - 1;
	downheap(arr, pos, n, cmp);
	return arr;
}

bool heap_redimensionar(heap_t* heap){ //O(n)
	heap->capacidad *= 2;
	void** datos_nuevo = realloc(heap->datos, heap->capacidad * sizeof(void*));  //hace la magia
	if (!datos_nuevo){
		free(heap); //Libero la memoria pedida para heap cuando lo cree
		return false; //Los datos se liberan solos al fallar
	}
	heap->datos = datos_nuevo;
	return true;
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
		swap(arr, pos, max);
		downheap(arr, max, tam, cmp); //hago downheap desde donde deje al padre
	}
	if (pos == 0) return;
	downheap(arr, --pos, tam, cmp);
}

void upheap(void** arr, size_t pos, size_t tam, cmp_func_t cmp){
	if (pos == 0 || pos > tam) return;
	size_t padre = (pos - 1 == 1) ? 1 : (pos - 1) / 2; //Porque cuando pasas 3 cosas, (2-1) / 2 da cero y esta mal, tenes que comparar con padre 1 al que esta en la pos 2 

	if (cmp(arr[padre], arr[pos]) < 0){
		swap(arr, pos, padre);
		upheap(arr, padre, tam, cmp); 
	}
} 

void swap(void** arr, size_t pos1, size_t pos2){
	void* aux = arr[pos1];
	arr[pos1] = arr[pos2]; //Cambio la posicion del primero
	arr[pos2] = aux; //cambio la posicion del segundo
}

void copiar_arreglo(void** origen, size_t largo, void** destino){
	/*Copia los datos del arreglo origen en el arreglo destino*/
	if (!largo) destino[0] = NULL;
	for (size_t i = 0; i < largo; i++) destino[i] = origen[i];
}