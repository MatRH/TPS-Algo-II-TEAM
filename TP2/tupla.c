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
#include "tupla.h"

struct tupla{  //tupla = (frecuencia, tag)
	char* tag;
	size_t frec;
};

tupla_t* tupla_crear(char* clave, size_t frec){
	tupla_t* tupla = malloc(sizeof(tupla_t));
	if (!tupla) return NULL;

	tupla->tag = strdup(clave);
	tupla->tag = clave;
	tupla->frec = frec;
	return tupla;
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

/*ORDENA IN PLACE un arreglo de tuplas, primero segun su frecuencia y luego
alfabeticamente
void ordernar_tuplas(tupla_t** tuplas, size_t len){
	size_t max_frec = 0;.
	tupla_t tupla;
	size_t capacidad = len/2;
	size_t* usuarios_por_frecuencia = malloc(sizeof(size_t)*capacidad);
	bool todo_ok = true;
	size_t frec;
	for (int i;todo_ok && i < len, i++){//recorro una vez para ver el rango de los datos
		tupla = tuplas[i];
		size_t frec = tupla_frec(tupla);
		if(frec > max_frec) max_frec = frec;
		if(frec > capacidad){
			capacidad = capacidad*2;
			usuarios_por_frecuencia = realloc(usuarios_por_frecuencia, capacidad);
			if(!usuarios_por_frecuencia) todo_ok = false;
		}
		usuarios_por_frecuencia[frec]++;
	}
	if(!todo_ok return);
	void*** frecuencias = malloc(sizeof(void*)*max_frec);
	for (int n; n <= max_frec; n++){//creo los baldes
		if (!frecuencias[n]){
			tuplas_t** balde = malloc(sizeof(void*)*usuarios_por_frecuencia[n]);
			frecuencias[n] = balde;
		}
	}
	for(int i; i<len; i++){ //ordeno las tuplas por frecuencias en sus baldes
		tupla = tuplas[i];
		frec = tupla_frec(tupla);
		size_t n = 0;
		while(frecuencias[frec][n]){
			n++;
		}
		frecuencias[frec][n] = tupla;
	}
	for (int n; n<= max_frec; n++){

	}
}*/
