/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//Declaraciones funciones auxiliares
bool condiciones_iniciales_abb(abb_t* arbol);

void pruebas_abb_alumno(void){
	printf("INICIO PRUEBAS ABB\n");

	abb_t* abb_1 = abb_crear(strcmp, NULL); //basica basica
	//print_test("La cantidad de elementos es cero", !abb_1->cant_elem);
	//print_test("No hay raiz", !abb_1->raiz);

	print_test("Se cumplen las condiciones iniciales del abb_1", condiciones_iniciales_abb(abb_1));
}

bool condiciones_iniciales_abb(abb_t* arbol){
	return arbol->cant_elem == 0 && !arbol->raiz;
}