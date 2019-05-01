#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

struct lista;
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacia.
lista_t* lista_crear(void);

//Devuelve TRUE si la lista esta vacia, FALSE si contiene algun elemento.
//Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

//Devuelve FALSE en caso de error, devuelve TRUE si el elemento pudo ser insertado.
//Pre: La lista fue creada.
//Post: El elemento ingresado es el primero de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Devuelve FALSE en caso de error, devuelve TRUE si el elemento pudo ser insertado.
//Pre: La lista fue creada.
//Post: El elemento ingresado es el ultimo de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Devuelve un puntero al primer elemento de la lista.
//Si la lista no tiene elementos devuelve NULL.
//Pre: La lista fue creada.
//Post: El primer elemento es eliminado de la lista.
void *lista_borrar_primero(lista_t *lista);

//Devuelve un puntero al primer elemento de la lista.
//Devuelve NULL si la lista no contiene elementos.
//Pre: La lista fue creada
void *lista_ver_primero(const lista_t *lista);

//Devuelve un puntero al ultimo elemento de la lista.
//Devuelve NULL si la lista no contiene elementos.
//Pre: La lista fue creada
void *lista_ver_ultimo(const lista_t* lista);

//Devuelve el largo de la lista.
//Pre: La lista fue creada.
size_t lista_largo(const lista_t *lista);

//Destruye la lista. Si se le pasa la funcion destuir_dato(), esta funcion es
//aplicada a todos los datos de la lista.
//Pre: La lista fue creada
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* *****************************************************************
 *                      PRIMITIVAS ITERADORES
 * *****************************************************************/

 //ITERADOR INTERNO:
//Recibe una lista, una funcion visitar y un puntero extra, el recorrido sobre
//la lista finaliza cuando la funcion visitar devuelve false, aplica la funcion
//visitar a todos los datos de la lista, comenzando por el primero y avanzando hacia
// el final. Cuando la funcion visitar sea llamada se le pasara el puntero extra por
//parametro.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

 //ITERADOR EXTERNO:
//
//Crea un iterador para la lista pasada por parametro.
//Pre: La lista debe haber sido creada.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza a la siguiente posicion de la lista, devuelve false en caso de que no
//haya siguiente. Devuelve true si pudo avanzar.
bool lista_iter_avanzar(lista_iter_t *iter);

//Deveulve el dato almacenado en la posicion actual de la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve true si el iterador se encuentra en la ultima posicion de la lista.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador.
//PRE: el iterador fue creado y no destruido previamente.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta un dato en la posicion actual de la lista, moviendo el dato que que
//ocupaba esa posicion y los siguientes hacia el fondo de la lista.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);


void *lista_iter_borrar(lista_iter_t *iter);




/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_alumno(void);

#endif // LISTA_H
