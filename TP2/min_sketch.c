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
//size_t hash_func(char* key);
size_t sdbm(char* str);



//Declaraciones funciones auxiliares
void destruir_tupla_wrapper(void* elemento);
void inicializar_arreglos(int arr1[], int arr2[], int arr3[], int tamanio);
int obtener_minimo(int frec1, int frec2, int frec3);
void update_frequency(min_sketch_t* min_sketch, char* tweet, size_t* min_frec);
int func_cmp(void* a, void* b);
bool chequear_ceros(int arr1[], int arr2[], int arr3[], int tamanio);

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
	int arr1[tamanio], arr2[tamanio], arr3[tamanio];
	inicializar_arreglos(arr1, arr2, arr3, tamanio);
	if (!chequear_ceros(arr1, arr2, arr3, tamanio)) printf("No son todos cero\n");
	else printf("Son todos ceros\n");

	min_sketch->heap = heap;
	min_sketch->func_hash_1 = &hash_djb2;
	min_sketch->func_hash_2 = &sdbm;
	min_sketch->func_hash_3 = &hash_djb2;
	min_sketch->arr1 = arr1;
	min_sketch->arr2 = arr2;
	min_sketch->arr3 = arr3;
	min_sketch->tam_arrays = tamanio;
	return min_sketch;

}

bool min_sketch_update(min_sketch_t* min_sketch, char* tweet, int k){
	size_t min_frec = 0;
	update_frequency(min_sketch, tweet, &min_frec);
	printf("La frecuencia minima es %ld\n", min_frec);

	heap_t* heap = min_sketch->heap;
	//Ahora debo encolarlo
	if (!heap_actualizar(heap, tweet, min_frec)){
		//Si llego aqui, es porque ese tweet no se encontraba
		tupla_t* tupla = tupla_crear(tweet, min_frec);
		if (!tupla) return false;
		if (heap_cantidad(heap) == k){ //Veo si desencolo o no
			tupla_t* tope_heap = heap_ver_max(heap);
			if (func_cmp(tope_heap, tupla) < 0){ //Sale el tweet con menor frecuencia
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
	//Primero invertis todo lo que tenes en el heap
	pila_t* pila = pila_crear(k);
	//Segundo, desencolamos hasta que quede vacio
	heap_t* heap = min_sketch->heap;
	while (!heap_esta_vacio(heap)) pila_apilar(pila, heap_desencolar(heap));

	//Tercero, desapilamos e imprimimos
	size_t frec_aux = 0;
	while (!pila_esta_vacia(pila)){
		tupla_t* tupla = pila_desapilar(pila);
		size_t frec = tupla_frec(tupla);
		char* tweet = tupla_tag(tupla);

		//printf("\nLa frecuencia es: %ld\n", frec);
		//printf("\nEl tweet es %s\n", tweet);
		//if (strcmp(tweet, "niley") == 0) printf("La frecuencia es %ld\n", frec);
		(frec_aux != frec) ? fprintf(stdout, "\n%ld ", frec) : fprintf(stdout, "%s", ", "); //No se que formato debe tener
		frec_aux = frec;
		fprintf(stdout, "%s", tweet);
		tupla_destruir(tupla);
	}
	fprintf(stdout, "\n");
	pila_destruir(pila);
}

void min_sketch_destruir(min_sketch_t* min_sketch){
	heap_t* heap = min_sketch->heap;
	heap_destruir(heap, destruir_tupla_wrapper);
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

int obtener_minimo(int frec1, int frec2, int frec3){
	/*Obtiene el minimo de los tres valores pasados por parametro*/
	printf("busco el minimo entre %d, %d, %d\n", frec1, frec2, frec3);
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

	printf("El tam de los arrays es %d\n", tam_arrays);
	printf("Hasheaos a las posiciones: %ld, %ld, %ld\n",pos1, pos2, pos3);

	int frec1 = ++min_sketch->arr1[pos1];
	int frec2 = ++min_sketch->arr2[pos2];
	int frec3 = ++min_sketch->arr3[pos3];

	int minn_frec =  obtener_minimo(frec1, frec2, frec3);
	printf("LA FRECUENCIA MINIMA ESSSSSSSS: %d\n", minn_frec);
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

bool chequear_ceros(int arr1[], int arr2[], int arr3[], int tamanio){
	bool ok1 = true, ok2 = true, ok3 = true;
	for (int i = 0; i < tamanio; i ++){
		ok1 &= arr1[i] == 0;
		ok2 &= arr2[i] == 0;
		ok3 &= arr3[i] == 0;
	}
	return ok1 && ok2 && ok3;
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

/*public override int GetHashCode(){
int hashVal = 0;
for (int i = 0; i < str.Length; i++)
hashVal = 26 * hashVal + str[i];
return hashVal;}*/

/*size_t hash_func(char* key) {
	long hashVal = 0;
	while (*key != '\0') {
		hashVal = (hashVal << 4) + *(key++);
		long g = hashVal & 0xF0000000L;
		if (g != 0) hashVal ^= g >> 24;
		hashVal &= ~g;
	}
	return (size_t) hashVal;
}	*/
size_t sdbm(char* str){
    unsigned long hash = 0;
    int c;

    while ((c = *str++)){
	    hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return (size_t) hash;
}	



