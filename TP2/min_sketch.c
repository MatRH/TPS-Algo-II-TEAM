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
#include "heap.h"
#include "min_sketch.h"

/*Adeodato Simó dato@fi.uba.ar a través de googlegroups.com

vie., 14 jun. 0:43 (hace 2 días)

para fiuba-7541rw-alu

TL;DR: largo sugerido: n*100.

El TAD count min sketch tiene dos “variables” o parámetros: la cantidad de arreglos (o, lo que es lo mismo, la cantidad de funciones hash usadas); y el largo de los arreglos.

La cantidad de funciones hash suele ser fija, entre 2 y 3. No obstante, no hay nada que impidiera tomar un arreglo de funciones en el constructor. (No es necesario que lo hagan.)

En cuanto al largo de los arreglos: a diferencia de —por ejemplo— el TAD hash, el cual redimensiona su arreglo según se añaden elementos, el count min sketch jamás se redimensiona (no solo es una de sus propiedades, es que, además, es físicamente imposible hacerlo). Por tanto, el largo de los arreglos se fija en el constructor. ¿A qué valor fijar el largo en el constructor?

Para fijarlo en el constructor, hay dos opciones: usar un número fijo, siempre el mismo (por ejemplo, 10.000); o recibir el largo como parámetro. Esto último es lo más razonable, porque sólo quien crea y usa el TAD sabe la magnitud de datos que va a procesar.

Es todo un tema, como usuario del TAD count min sketch, qué largo elegir, pues entran varios factores en juego. Entonces les propongo las siguientes recomendaciones para el TP2:

    cada vez que llamen al constructor del count min sketch (básicamente, una vez al principio del programa) estaría bien pasar como largo el valor n * 100. (Si ya usan otro valor y las pruebas aproximadas pasan okay, desde luego lo pueden dejar.)
    “las pruebas aproximadas pasan okay” si los valores de similitud están entre 0.9 y 1.0, y no hay preponderancia de 1.0.
    les recomiendo que tomen el largo en el constructor, o sea que no usen un número fijo como 10.000. Les va a llevar menos tiempo implementar el cambio al constructor, que encontrar un valor fijo tal que las pruebas aproximadas pasen okay.

Cualquier aclaración que necesiten, no duden en responder a este correo.

-d */
typedef size_t (*funcion_de_hash)(const char*);

typedef struct tupla{  //tupla = (frecuencia, tag)
	const char* tag;  //No se si es const pero bueno
	size_t frec;
} tupla_t;

struct min_sketch{ //El typedef esta hecho en el .h
  	funcion_de_hash f1;
  	funcion_de_hash f2;
  	funcion_de_hash f3;
  	int* arr1;
  	int* arr2;
  	int* arr3;
  	heap_t* heap;
};
//Declaraciones funciones auxiliares
int func_cmp(void* a, void* b);
tupla_t* crear_tupla(size_t frec, const char* tweet);
void destruir_tupla(tupla_t* tupla);
void update_frequency(min_sketch_t* min_sketch, const char *tweet, size_t* min);
size_t obtener_minimo(int min1, int min2, int min3);
/* *****************************************************************
 *            IMPLEMENTACION PRIMITIVAS DEL MIN_SKETCH
  * *****************************************************************/
min_sketch_t* crear_min_sketch(size_t tamanio){
	min_sketch_t* min_sketch = malloc(sizeof(min_sketch_t*));
	if (!min_sketch) return NULL;

	heap_t* heap = heap_crear(func_cmp);
	if (!heap){
		free(min_sketch);
		return NULL;
	}
	int arr1[tamanio] = {0}, arr2[tamanio] = {0}, arr3[tamanio] = {0};
	min_sketch->f1 = ;
	min_sketch->f2 = ;  //FALTAN LAS FUNCIONES DE HASH
	min_sketch->f3 = ;
	min_sketch->arr1 = arr1;
	min_sketch->arr2 = arr2;
	min_sketch->arr3 = arr3;
	min_sketch->heap = heap;
	return min_sketch;
}

void destruir_min_sketch(min_sketch_t* min_sketch){
	heap_destruir(min_sketch->heap, NULL); //Porque queda vacio despues de imrpimir los k mejores
	free(min_sketch);
}

void* min_sketch_get(min_sketch_t* min_sketch){
	heap_t* heap = min_sketch->heap;
	return (!heap_cantidad(heap)) ? NULL : heap_desencolar(heap);
}

void min_sketch_print(min_sketch_t* min_sketch){
	heap_t* heap = min_sketch->heap;
	pila_t* pila = pila_crear();
	while (heap_cantidad(heap)) pila_apilar(pila, heap_desencolar(heap)); //Me deja en orden de frec de mayor a menor y a su vez ordenado alfabeticamente

	//La pila se queda con todas las cosas ordenadas como debe ser
	//Todo el formato nos encargamos en procesar tweets, aca solo imprimimos las lineas
	size_t frec = 0; //FLAG
	while(!pila_esta_vacia(pila)){
		tupla_t* tupla = pila_desencolar(pila);
		(frec != tupla->frec) ? fprintf(stdout, "\n%ld ", tupla->frec): fprintf(stdout, "%s", ", ");
		//Imprimimos la cadena ahora
		fprintf(stdout, "%s", tupla->tag);
		destruir_tupla(tupla); //No se si tiene memoria dinamica o que onda todavia
	}
	//Salimos del while. Los \n se encarga procesar_tweets
}

bool min_sketch_update(min_sketch_t* min_sketch, const char* tweet, size_t k){ //K es la cant de TTs que quiero
	size_t min = 0; //Aqui quedara guardada la minima frecuencia de los 3 arreglos
	update_frequency(min_sketch, tweet, &min);
	tupla_t* tupla = crear_tupla(min, tweet);
	if (!tupla) return false;

	heap_t* heap = min_sketch->heap;
	//Si esta vacia encola directamente
	if (!heap_cantidad(heap)){
		heap_encolar(heap, tupla);
		return true;
	}
	if (!heap_actualizar(heap, tweet)){ //Significa que la clave no estaba en el heap
		if (heap_cantidad(heap) == k){ //Tengo que ver si desencolo o no
			tupla_t* tope_heap = heap_ver_max(heap);
			if (min > tope_heap->frec || (min == tope_heap->frec && strcmp(tope_heap->tag, tweet) > 0)){
				destruir_tupla(heap_desencolar(heap));
			}
			else return true; //Si no se da ninguna condicion anterior no debo hacer nada
		}
		//Si la cantidad de elementos es menor a k, puedo encolar libremente, porque no tengo los k mejores en el heap
		//Encolo la tupla creada anteriormente
		heap_encolar(heap, tupla);
		return true;
	}
}

/* *****************************************************************
 *                       FUNCIONES AUXILIARES
  * *****************************************************************/
int func_cmp(void* a, void* b){
	tupla_t* tupla1 = a;
	tupla_t* tupla2 = b;  //Casos 1 > 2 en frec, no se comparan los tweets, 1 == 2 --> comparo tweets, 1 < 2 queda 1 como minimo
	if (tupla1->frec > tupla2->frec) return -1;
	if ((tupla1->frec == tupla2->frec) && strcmp(tupla1->tag, tupla2->tag) > 0) return -1
	return 1;
}
tupla_t* crear_tupla(size_t frec, const char* tweet){
	tupla_t* tupla = malloc(sizeof(tupla_t));
	if (!tupla) return NULL;
	tupla->tag = strdup(tweet);
	tupla->frec = frec;
	return tupla;

}
void destruir_tupla(tupla_t* tupla){ //SOLO SE USA SI USAMOS LA OPCION 2, CASO CONTRARIO DEBE APARECER EN PROCESAR TWEETS
	free(tupla->tag);
	free(tupla);
}

void update_frequency(min_sketch_t* min_sketch, const char *tweet, size_t* min){
	/*Actualiza las frecuencias en los arreglos, a su vez calcula cual es el minimo
	valor entro estos tres arreglos*/
	size_t pos1 = min_sketch->f1(tweet);
	size_t pos2 = min_sketch->f2(tweet);
	size_t pos3 = min_sketch->f3(tweet);

	int min1 = min_sketch->arr1[pos1] ++; //Aumento la frecuencia una unidad para todos los arreglos
	int min2 = min_sketch->arr2[pos2] ++;
	int min3 = min_sketch->arr3[pos3] ++;

	*min = obtener_minimo(min1, min2, min3);
}

size_t obtener_minimo(int min1, int min2, int min3){
	/*Obtiene el minimo de los tres valores pasados por parametro*/
	if (min1 > min2){ //El dos es mas chiquito
		return (min2 > min3) ? (size_t) min3 : (size_t) min2;
	}
	//min1 era el mas chico, si da igual no pasa nada
	return (min1 > min3) ? (size_t) min3 : (size_t) min1;
}
