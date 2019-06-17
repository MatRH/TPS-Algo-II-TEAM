/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#ifndef TUPLA_H
#define TUPLA_H 

#include <stdbool.h>
#include <stddef.h>

typedef struct tupla tupla_t;

/*Crea la estructura tupla
 *En caso de error devuelve NULL
 */
tupla_t* tupla_crear(char* clave, size_t frec);

//void tupla_sort();

/*Destruye la estructura tupla
 *Pre: la estructura tupla fue inicializada
 *Post: la estructura tupla fue destruida
 */
void tupla_destruit(tupla_t* tupla);

#endif