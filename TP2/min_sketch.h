#ifndef MIN_SKETCH_H
#define MIN_SKETCH_H //FIJATE SI SIRVE EL DOBLE GUION

#include <stdbool.h>
#include <stddef.h>

typedef struct min_sketch min_sketch_t;

/*Crea el min_sketch*/
min_sketch_t* crear_min_sketch(size_t tamanio);

/* Obtiene el valor de un elemento del min_sketch, 
 *si el min_sketch no posee elementos, devuelve NULL.
 * Pre: La estructura min_sketch fue inicializada
 */
void* min_sketch_get(min_sketch_t* min_sketch); //Opcion 1

/*Imprime los elementos que se encuentran en el min_sketch
 *Pre: la estructura min_sketch fue inicializada
 */
void min_sketch_print(min_sketch_t* min_sketch); //Opcion 2

/* Destruye la estructura liberando la memoria pedida.
 * Pre: La estructura min_sketch fue inicializada
 * Post: La estructura min_sketch fue destruida
 */
void destruir_min_sketch(min_sketch_t* min_sketch);




#endif
