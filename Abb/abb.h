/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *           DECLARACION PRIMITIVAS DEL ABB
  * *****************************************************************/

/*Crea el abb. 
*Devuelve NULL en caso de error
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado. Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al abb.
 * Pre: La estructura abb fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: La estructura abb fue inicializada
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *arbol);

/* *****************************************************************
 *      DECLARACION PRIMITIVAS DEL ITERADOR EXTERNO DEL ABB
  * *****************************************************************/

/*Crea un iterador para el ABB.
*Post: devuelve un iterador.
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*Avanza una posicion el iterador, devuelve false en caso de que el iterador
*se encuentre al final del abb.
*Pre: el abb y el iterador fueron creados.
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*Devuelve la clave del nodo al cual esta apuntado el iterador.
*Pre: el abb y el iterador fueron creados.
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*Devuelve true si el iterador se encuentra al final del abb,
*false en caso contrario.
*Pre: el iterador fue creado.
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*Destruye el iterador
*Pre: el iterador fue creado
*/
void abb_iter_in_destruir(abb_iter_t* iter);

/*Iterador interno, que funciona usando la función de callback 
*“visitar” que recibe la clave, el valor y un puntero extra,
*y devuelve true si se debe seguir iterando, false en caso contrario
*/

//Iterador interno
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

void pruebas_abb_alumno(void);

#endif // ABB_H