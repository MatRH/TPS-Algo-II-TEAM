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

struct tupla{  //tupla = (frecuencia, tag)
	char* tag;
	size_t frec;
};

tupla_t* tupla_crear(const char* clave, size_t frec){
	tupla_t* tupla = malloc(sizeof(tupla_t));
	if (!tupla) return NULL;

	tupla->tag = strdup(clave);
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

/*ORDENA un arreglo de tuplas, primero segun su frecuencia y luego
alfabeticamente*/
/*
1) Ordeno el arreglo con radix sort,	O(len)
2) Recorro una vez el arreglo para encontrar el rango de frecuencias O(len)
3) Creo (Max-min) baldes, O(M-m)
4) Recorro una vez el arreglo y separo cada tupla en su balde correspondiente
según su frecuencia						O(len)

*/
tupla_t** ordenar_abc(tupla_t** tuplas,size_t len);

tupla_t** ordernar_tuplas(tupla_t** tuplas, size_t len){
	size_t max_frec = 0;
	size_t min_frec = tupla_frec(tuplas[0]);
	tupla_t* tupla;
	size_t pos_baldes;
	size_t frec;
	tuplas = ordenar_abc(tuplas, len);
	if (!tuplas) return NULL;

	for (size_t i = 0; i < len; i++){ //OBTENGO MAXIMO Y MINIMO	O(len)
		tupla = tuplas[i];
		frec = tupla_frec(tupla);
		if (frec < min_frec) min_frec = frec;
		if (frec > max_frec) max_frec = frec;
	}
	size_t rango = max_frec - min_frec;

	void*** baldes = malloc(sizeof(void*)*rango);
	size_t tams[rango];

	for (size_t i = 0; i < len; i++){//BUSCO EL LARGO DE CADA BALDE
		tupla = tuplas[i];
		frec = tupla_frec(tupla);
		pos_baldes = frec - min_frec;
		tams[pos_baldes]++;
	}

	for (size_t i = 0; i < rango; i++){
		void** balde = malloc(sizeof(void*)*tams[i]);//creo cada balde con su tamaño correspondiente
		baldes[i] = balde;
		tams[i] = 0; //inicializo en 0 el arreglo de tamaños para reutilizarlo más adelante
	}

	//AHORA TENGO UN ARREGLO DE BALDES, DONDE CADA BALDE TIENE SUFICIENTE ESPACIO PARA GUARDAR A TODOS LOS DE SU FRECUENCIA
	size_t pos_balde;
	for (size_t i = 0; i < len; i++ ){//SEPARO CADA TUPLA SEGÚN SU FRECUENCIA, RESPETANDO EL ORDEN RELATIVO QUE TENIAN
		tupla = tuplas[i];
		frec = tupla_frec(tupla);
		pos_baldes = frec - min_frec; //asi min es el 0, min +1 en el 1 elemento etc
		pos_balde = tams[pos_baldes]; //posicion dentro del arreglo de su balde correspondiente
		baldes[pos_baldes][pos_balde] = tupla;//guardo la tupla en su balde correspondiente, en su posicion correspondiente
		tams[pos_baldes]++;//para saber donde va la siguiente tupla en su correspondiente balde
	}

	//AHORA TENGO BALDES SEGÚN FRECUENCIA, DONDE EN CADA BALDE LAS TUPLAS ESTÁN ORDENADAS ALFABÉTICAMENTE
	tupla_t** ordenado = malloc(sizeof(void*)*len);
	size_t pos = 0;
	for (size_t balde_actual = 0; balde_actual < rango; balde_actual++){
		pos_balde = 0;
		while ( tams[balde_actual]){
			pos_balde++; //posicion en el balde actual
			ordenado[pos] = baldes[balde_actual][pos_balde]; //pongo la tupla en su posicion correspondiente en el arreglo
			pos++; //posicion de la tupla en el arreglo ordenado
			tams[balde_actual]--;
		}
		free(baldes[balde_actual]);//libero el balde vacio
	}
	free(baldes);//libero el arreglo de baldes
	return (ordenado);
}

tupla_t** ordenar_abc(tupla_t** tuplas,size_t len){
	tupla_t* tupla;
	int max = 0;
	char* usuario;
	for (int i = 0; i < len; i++){	//busco el maximo valor ascii de las cadenas
		tupla = tuplas[i];
		usuario = tupla_tag(tupla);
		size_t pos_caracter = 0;
		char caracter = usuario[pos_caracter];
		while(caracter){
			if ((int)caracter > max) max = (int)caracter;
			caracter = usuario[pos_caracter++];
		}
	}

	//AHORA TENGO EL MAXIMO VALOR ASCII, SE CUANTOS "BALDES" hacer
	void* baldes[max]; //creo un arreglo de punteros de longitud max
	for (int i = 0; i < max; i++){	//creo los baldes
		lista_t* balde = lista_crear();	//malloc
		baldes[i] = balde;
	}

	//AHORA TENGO UN ARREGLO DE LISTAS DONDE CADA LISTA CORRESPONDE A UN VALOR ASCII
	size_t len_usuario;
	int ascii_char;
	lista_t* balde;
	for (int i = 0; i < len; i++){ //recorro las tuplas para cargarlas al arreglo
		tupla = tuplas[i];	//tomo la tupla
		usuario = tupla_tag(tupla);	//tomo el usuario de la tupla
		len_usuario = strlen(usuario);
		ascii_char = (int)usuario[len_usuario];	//tomo el valor ascii del ultimo caracter del usuario
		balde = baldes[ascii_char]; //tomo el balde correspondiente al caracter
		lista_insertar_ultimo(balde, tupla); //guardo la tupla en la lista que le corresponde
	}

	//AHORA TENGO TODOS LOS USUARIOS CARGADOS EN EL ARREGLO DE LISTAS
	tupla_t** ordenados = malloc(sizeof(tupla_t*)*len); //creo un arreglo de tuplas nuevo
	size_t guardados = 0;
	for(int i = 0; i < MAX_STR_LEN -1; i++){	//itero por la longitud de las cadenas -1 porque ya hice la primer iteracion
		for (int n = 0; n < max; n++){	//recorro todas las listas
			balde = baldes[n];
			while (!lista_esta_vacia(balde)){	//vacio cada balde
				tupla = lista_ver_primero(balde);
				lista_borrar_primero(balde);
				ordenados[guardados] = tupla;
				guardados++;
			}
		}
		for (int j = 0; j < len; j++){
			tupla = ordenados[j];	//tomo la tupla
			usuario = tupla_tag(tupla);	//tomo el usuario de la tupla
			len_usuario = strlen(usuario);
			if (len_usuario < i) len_usuario = 0; //para las palabras con menos de 15 caracteres
			ascii_char = (int)usuario[len_usuario];	//tomo el valor ascii del ultimo caracter del usuario
			balde = baldes[ascii_char]; //tomo el balde correspondiente al caracter
			lista_insertar_ultimo(balde, tupla); //guardo la tupla en la lista que le corresponde
		}
		guardados = 0;
	}
	for (int n = 0; n < max; n++){	//recorro todas las listas
		balde = baldes[n];
		while (!lista_esta_vacia(balde)){	//vacio cada balde
			tupla = lista_ver_primero(balde);
			lista_borrar_primero(balde);
			ordenados[guardados] = tupla;
			guardados++;
		}
	}
	//AHORA EN ORDENADOS TENGO LAS TUPLAS ORDENADAS ALFABÉTICAMENTE

	for (int n = 0; n < max; n++){//libero la memoria ocupada por el arreglo de listas
		balde = baldes[n];
		lista_destruir(balde, NULL);
	}
	free(baldes);
	free(tuplas); //libero el arreglo de tuplas original

	return ordenados;

}
