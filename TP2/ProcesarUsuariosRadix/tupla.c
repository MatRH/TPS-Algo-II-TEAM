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
#include "lista.h"
#define	MAX_STR_LEN 15	//longitud maxima de las cadenas guardadas para ordenar
#define MAX_CANT_CARACT 63 //Solo usamos 63 caracteres ascii, 26 x 2 (para letras mayus y minus) + 10 numeros + _ = 63

struct tupla{  //tupla = (frecuencia, tag)
	char* tag;
	size_t frec;
};

//*************************Declaraciones funciones auxiliares**********************************
size_t indexador(char caracter);
void counting_sort_abc(tupla_t** tuplas, tupla_t** tuplas_ordenado, size_t pos_iter_radix);
void counting_sort_frec(tupla_t** tuplas, tupla_t** resultado, size_t max_frec, size_t min_frec, size_t len_tuplas);
tupla_t** radix_sort(tupla_t** tuplas, size_t len_tuplas, size_t max_frec, size_t min_frec);
char obtener_caracter(char* usuario, size_t pos_iter_radix);
//*********************************************************************************************

tupla_t* tupla_crear(const char* clave, size_t frec){
	tupla_t* tupla = malloc(sizeof(tupla_t));
	char* str_cpy = strdup(clave);
	if (!tupla || !str_cpy ) return NULL;
	tupla->tag = str_cpy;
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

void tupla_destruir(tupla_t* tupla){
	free(tupla->tag);
	free(tupla);
}

tupla_t** ordernar_tuplas(tupla_t** tuplas, size_t len){
	size_t max_frec = 0, frec = 0;
	size_t min_frec = tupla_frec(tuplas[0]);
	tupla_t* tupla;

	for (size_t i = 0; i < len; i++){ //OBTENGO MAXIMO Y MINIMO	O(len)
		tupla = tuplas[i];
		if (!tupla) break;
		frec = tupla_frec(tupla);
		if (frec < min_frec) min_frec = frec;
		if (frec > max_frec) max_frec = frec;
	} //Despues de este for ya tengo las frecuencias para el counting sort

	tupla_t** ordenado = radix_sort(tuplas, len, max_frec, min_frec); //Despues vemos como liberar memoria
	return (ordenado != NULL) ? ordenado : NULL; //Devuelvo el arreglo de tuplas ordenado si todo salio bien
}

//************************************FUNCIONES AUXILIARES*****************************************************
size_t indexador(char caracter){
	/*Recibe un caracter, devuelve el indice correspondiente en el arreglo de tam 63
	*El orden en el arreglo es el siguiente (segun la tabla ascii):
	*[0,9] -> numeros
	*[10, 35] -> letras mayusculas
	*[36] -> guion bajo
	*[37, 62] -> letras minusculas */
	//if (caracter == '_') return 36;
	if (caracter == '\0') return 0; //Para evitar problemas con cadenas de distitntas longitudes
	if (caracter >= 48 && caracter <= 57) return (size_t)(caracter - '0'); //asi te da del 0 al 9
	if (caracter >= 65 && caracter <= 90) return (size_t)(caracter + 10 - 'A'); //Asi me da el rango
	if (caracter >= 97 && caracter <= 122) return (size_t)(caracter + 37 - 'a');
	return 36; //Si no es ninguno de esos casos, significa que es el guion bajo
	

}

char obtener_caracter(char* usuario, size_t pos_iter_radix){
	//Devuelve el caracter mas significativo en la iteracion de radix que te encuentres,
	//que esta dada por pos_iter_radix
	return (pos_iter_radix < strlen(usuario)) ? usuario[pos_iter_radix] : '\0'; //Para solucionar problemas de cadenas de diferentes long, seria un padding hacia la izq
}

tupla_t** radix_sort(tupla_t** tuplas, size_t len_tuplas, size_t max_frec, size_t min_frec){
	/*Ordenamiento no comparativo utilizado para ordenar el arreglo de tuplas primero
	*alfabeticamente y luego por frecuencia. Como ordenamiento interno utilizara counting sort,
	*el ordenamiento no es in-place por lo que devuelve un arreglo de tuplas ordenado */
	tupla_t** ordenado_abc = malloc(sizeof(void*) * len_tuplas); //Utilizado para obtener el arreglo ordenado alfabeticamente
	if (!ordenado_abc) return NULL;
	tupla_t** resultado = malloc(sizeof(void*) * len_tuplas); //Utilizado para obtener el arreglo de tuplas ordenado por completo
	if (!resultado){
		free(ordenado_abc);
		return NULL;
	}
	ordenado_abc[len_tuplas - 1] = NULL;
	resultado[len_tuplas - 1] = NULL;
	for (int i = MAX_STR_LEN - 1; i >= 0; i --){ //Son 15 cifras significativas en este caso O(1)
		counting_sort_abc(tuplas, ordenado_abc, (size_t)i); //deberia ir haciendo que tuplas siempre quede modificado
		for (int j = 0; j < len_tuplas - 1; j++) tuplas[j] = ordenado_abc[j]; //Actualizo tuplas O(N)
	}
	counting_sort_frec(ordenado_abc, resultado, max_frec, min_frec, len_tuplas); //O(N)
	free(ordenado_abc); //El resultado final quedo guardado en resultado, se puede liberar
	return resultado;
}

void counting_sort_abc(tupla_t** tuplas, tupla_t** tuplas_ordenado, size_t pos_iter_radix){
	/*Ordena alfabeticamente el arreglo de tuplas pasado por parametro, el arreglo ordenado 
	*es guardado en tuplas_ordenado.*/
	int contador[MAX_CANT_CARACT] = {0}; //O(62) = O(1)
	int suma_acumulada[MAX_CANT_CARACT] = {0}; //Inicializo en cero ambos arreglos //O(62) = O(1)
	tupla_t* tupla_aux = NULL;
	char* usuario;
	size_t pos_aux = 0;
	size_t pos_contador = 0;
	//Primero cuento las frecuencias de las letras, y actualizo esas frecuencias en contador
	while (tuplas[pos_aux] != NULL){ //O(N)
		tupla_aux = tuplas[pos_aux++];
		usuario = tupla_tag(tupla_aux);
		pos_contador = indexador(obtener_caracter(usuario, pos_iter_radix)); //Obtengo la posicion a la que debe ir el caracter leido
		contador[pos_contador] ++; //Actualizo la cantidad de apariciones
	}
	//Segundo, actualizo el arreglo de sumas acumuladas
	for (size_t i = 1; i < MAX_CANT_CARACT; i++){  //O(62) = O(1)
		suma_acumulada[i] = suma_acumulada[i - 1] + contador[i - 1];
	}

	//Tercero, ordenamos esta tanda de caracteres
	pos_aux = 0; //No se por que anda, pero anda, despues pensalo bien, se soluciona con un if para saber cual usar
	while (tuplas[pos_aux] != NULL){ //O(N)
		tupla_aux = tuplas[pos_aux];
		usuario = tupla_tag(tupla_aux);
		pos_contador = indexador(obtener_caracter(usuario, pos_iter_radix));
		int pos_resultado = suma_acumulada[pos_contador]++;
		tuplas_ordenado[pos_resultado] = tuplas[pos_aux++];
	}
}

void counting_sort_frec(tupla_t** tuplas, tupla_t** resultado, size_t max_frec, size_t min_frec, size_t len_tuplas){
	int rango = 1 + (int)(max_frec - min_frec);
	int contador[rango], suma_acumulada[rango];
	for (int i = 0; i < rango; i ++ ){ //Inicializo los arreglos en cero  O(RANGO)
		contador[i] = 0;
		suma_acumulada[i] = 0;
	}
	size_t pos_aux = 0;
	tupla_t* tupla_aux = NULL;
	size_t frec;
	while (tuplas[pos_aux] != NULL){ //O(N)
		tupla_aux = tuplas[pos_aux++];
		frec = tupla_frec(tupla_aux);
		contador[frec - (int)min_frec]++; //Actualizo la cantidad de apariciones de las frecuencias
	}

	//Actualizo el arreglo de sumas acumuladas
	for (int i = 1; i < rango; i++){  //O(RANGO)
		suma_acumulada[i] = suma_acumulada[i - 1] + contador[i - 1];
	}
	pos_aux = 0;
	while (tuplas[pos_aux] != NULL){  //O(N)
		tupla_aux = tuplas[pos_aux];
		frec = tupla_frec(tupla_aux);
		int pos_resultado = suma_acumulada[frec - (int)min_frec] ++;
		resultado[pos_resultado] = tuplas[pos_aux++];
	}

}
