/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#ifndef MIN_SKETCH_H
#define MIN_SKETCH_H 

#include <stdbool.h>
#include <stddef.h>

typedef struct min_sketch min_sketch_t;

/*Crea el min_sketch*/
min_sketch_t* min_sketch_crear(int tamanio);

/*Imprime los elementos que se encuentran en el min_sketch
 *Pre: la estructura min_sketch fue inicializada
 */
void min_sketch_print(min_sketch_t* min_sketch, int k);

/*Actualiza las frecuencias en los arreglos de min_sketch
 *Pre: La estructura min_sketch fue inicializada
 */
bool min_sketch_update(min_sketch_t* min_sketch, char* tweet, int k);

/* Destruye la estructura liberando la memoria pedida.
 * Pre: La estructura min_sketch fue inicializada
 * Post: La estructura min_sketch fue destruida
 */
void min_sketch_destruir(min_sketch_t* min_sketch);

#endif