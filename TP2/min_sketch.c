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
#include <stdbool.h>
#include "heap.h"
#include "min_sketch.h"
#include "tupla.h"
#include "pila.h"
typedef size_t (*funcion_de_hash)(char*); //para las funciones de hash

struct min_sketch{
	int *arr1;
	int *arr2;
	int *arr3;
	funcion_de_hash func_hash_1;
	funcion_de_hash func_hash_2;
	funcion_de_hash func_hash_3;
	int tam_arrays;
	heap_t* heap; //Con este sabes los k elementos
};
//Declaraciones funciones de hash
size_t hash_djb2(char *str);
size_t sdbm(char* str);
size_t hashCode(char* key)

//Declaraciones funciones auxiliares
void destruir_tupla_wrapper(void* elemento);
void inicializar_arreglos(int arr1[], int arr2[], int arr3[], int tamanio);
int obtener_minimo(int frec1, int frec2, int frec3);
void update_frequency(min_sketch_t* min_sketch, char* tweet, size_t* min_frec);
int func_cmp(void* a, void* b);
int exit_cmp(void* a, void* b);
/* *****************************************************************
 *            IMPLEMENTACION PRIMITIVAS DEL MIN_SKETCH
  * *****************************************************************/
min_sketch_t* min_sketch_crear(int tamanio){
	min_sketch_t* min_sketch = malloc(sizeof(min_sketch_t));
	if (!min_sketch) return NULL;

	heap_t* heap = heap_crear(func_cmp);
	if (!heap){
		free(min_sketch);
		return NULL;
	}
	tamanio *= 100; //Por lo dicho en los mails
	int *arr1 = malloc(sizeof(int) * tamanio);
	int *arr2 = malloc(sizeof(int) * tamanio);
	int *arr3 = malloc(sizeof(int) * tamanio);
	inicializar_arreglos(arr1, arr2, arr3, tamanio);

	min_sketch->heap = heap;
	min_sketch->func_hash_1 = &hash_djb2;
	min_sketch->func_hash_2 = &sdbm;
	min_sketch->func_hash_3 = &hashCode;
	min_sketch->arr1 = arr1;
	min_sketch->arr2 = arr2;
	min_sketch->arr3 = arr3;
	min_sketch->tam_arrays = tamanio;
	return min_sketch;

}

bool min_sketch_update(min_sketch_t* min_sketch, char* tweet, int k){
	size_t min_frec = 0;
	update_frequency(min_sketch, tweet, &min_frec);

	heap_t* heap = min_sketch->heap;
	//Ahora debo encolarlo
	if (!heap_actualizar(heap, tweet, min_frec)){
		//Si llego aqui, es porque ese tweet no se encontraba
		tupla_t* tupla = tupla_crear(tweet, min_frec);
		if (!tupla) return false;
		if (heap_cantidad(heap) == k){ //Veo si desencolo o no
			tupla_t* tope_heap = heap_ver_max(heap);
			if (exit_cmp(tope_heap, tupla) < 0){ //Sale el tweet con menor frecuencia
				tupla_destruir(heap_desencolar(heap)); //Encolo la nueva tupla mas abajo
			}
			else return true; 
		}
		//Encolo la tupla
		if (!heap_encolar(heap, tupla)){ //Fallo encolar
			tupla_destruir(tupla);
			return false;
		}
	}
	return true;
}

void min_sketch_print(min_sketch_t* min_sketch, int k){
	//Primero invertis todo lo que tenes en el heap, para eso usamos una pila
	pila_t* pila = pila_crear(k);
	//Desencolamos hasta que quede vacio
	heap_t* heap = min_sketch->heap;
	while (!heap_esta_vacio(heap)) pila_apilar(pila, heap_desencolar(heap));

	//Segundo, desapilamos e imprimimos
	while (!pila_esta_vacia(pila)){
		tupla_t* tupla = pila_desapilar(pila);
		size_t frec = tupla_frec(tupla);
		char* tweet = tupla_tag(tupla);
		fprintf(stdout, "\n%ld %s", frec, tweet);
		tupla_destruir(tupla);
	}
	fprintf(stdout, "\n");
	pila_destruir(pila);
}

void min_sketch_destruir(min_sketch_t* min_sketch){
	heap_t* heap = min_sketch->heap;
	heap_destruir(heap, destruir_tupla_wrapper);
	free(min_sketch->arr1);
	free(min_sketch->arr2);
	free(min_sketch->arr3);
	free(min_sketch);
}

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
  * *****************************************************************/
void destruir_tupla_wrapper(void* elemento){
	tupla_destruir((tupla_t*) elemento);
}

void inicializar_arreglos(int arr1[], int arr2[], int arr3[], int tamanio){
	//Inicializa en cero los arreglos pasaros por parametro
	for (int i = 0; i < tamanio; i++){
		arr1[i] = 0;
		arr2[i] = 0;
		arr3[i] = 0;		
	}
}

int exit_cmp(void* a, void* b){
	/*Funcion de comparacion para ver si hay que sacar un elemento
	 *del heap o no*/
	tupla_t* tupla1 = a;
	tupla_t* tupla2 = b;

	size_t frec1 = tupla_frec(tupla1);
	size_t frec2 = tupla_frec(tupla2);
	char* cad1 = tupla_tag(tupla1);
	char* cad2 = tupla_tag(tupla2);
	if (frec1 < frec2 || (frec1 == frec2 && strcmp(cad1, cad2) > 0)) return -1;
	return 1;

}

int obtener_minimo(int frec1, int frec2, int frec3){
	/*Obtiene el minimo de los tres valores pasados por parametro*/
	if (frec1 > frec2){ //El dos es mas chiquito
		return (frec2 > frec3) ?  frec3 : frec2;
	}
	//min1 era el mas chico, si da igual no pasa nada
	return (frec1 > frec3) ? frec3 : frec1;
}

void update_frequency(min_sketch_t* min_sketch, char* tweet, size_t* min_frec){
	/*Actualiza las frecuencias de los arreglos*/
	int tam_arrays = min_sketch->tam_arrays;
	//Aplico la funcion de hash al tweet y actualizo las frecuencias
	size_t pos1 = min_sketch->func_hash_1(tweet) % tam_arrays;
	size_t pos2 = min_sketch->func_hash_2(tweet) % tam_arrays;
	size_t pos3 = min_sketch->func_hash_3(tweet) % tam_arrays;

	int frec1 = ++min_sketch->arr1[pos1];
	int frec2 = ++min_sketch->arr2[pos2];
	int frec3 = ++min_sketch->arr3[pos3];

	int minn_frec =  obtener_minimo(frec1, frec2, frec3);
	*min_frec = (size_t) minn_frec;
}

int func_cmp(void* a, void* b){
	tupla_t* tupla1 = a;
	tupla_t* tupla2 = b;

	size_t frec1 = tupla_frec(tupla1);
	size_t frec2 = tupla_frec(tupla2);
	char* cad1 = tupla_tag(tupla1);
	char* cad2 = tupla_tag(tupla2);

	if (frec1 == frec2 && strcmp(cad1, cad2) == 0) return 0;

	if (frec1 > frec2 || (frec1 == frec2 && strcmp(cad1, cad2) < 0)) return -1;

	return 1;
}

/* *****************************************************************
 *                      FUNCIONES DE HASH
  * *****************************************************************/
size_t hash_djb2(char *str){
		unsigned long hash = 5381;
		int c;
		while ((c = *str++))
				hash = ((hash << 5) + hash) + (size_t)c; /* hash * 33 + c */

		return hash;
}

size_t sdbm(char* str){
    unsigned long hash = 0;
    int c;

    while ((c = *str++)){
	    hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return (size_t) hash;
}	

size_t hashCode(char* key){ //Jenkins
    size_t  hash, i;
    size_t lenght = strlen(key);
    for(hash = i = 0; i < lenght; ++i){
        hash += (size_t) key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

