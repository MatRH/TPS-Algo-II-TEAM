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

struct tupla{  //tupla = (frecuencia, tag)
	char* tag;  
	size_t frec;
};

tupla_t* tupla_crear(char* clave, size_t frec){
	tupla_t* tupla = malloc(sizeof(tupla_t));
	if (!tupla) return NULL;

	tupla->tag = strdup(clave);
	tupla->frec = frec;
	return tupla_t;
}

size_t tupla_frec(tupla_t* tupla){
	return tupla->frec;
}

void tupla_actualizar(tupla_t* tupla, size_t frec){
	tupla->frec = frec;
}

char* tupla_tag(tupla_t* tupla){
	return tupla->tag;
}

//tupla_sort(){}

void tupla_destruir(tupla_t* tupla){
	free(tupla->tag);
	free(tupla);
}