/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#define _POSIX_C_SOURCE 200809L  //Para que ande strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "min_sketch.h"
#include "strutil.h"
#define CANT_PARAM 3
//Recibis n que es cantidad de lineas, y k el ranking

//Declaraciones funciones auxiliares
void imprimir_error(char* error_sms);
bool numeros_validos(const char* cad1, const char* cad2);
void leer_tweets(min_sketch_t* min_sketch, FILE* archivo, int n, int k);
void imprimir_tts(min_sketch_t* min_sketch, size_t indice, int k);

//Programa principal
int main(int argc, char const *argv[]){ //0 si anda bien 1 en caso contrario
	if (argc != 3){
		imprimir_error("Error: Cantidad erronea de parametros");
		return 1;
	}
	FILE* archivo = stdin;
	if (!archivo){
		imprimir_error("Error: archivo fuente inaccesible");
		return 1;
	}
	//Chequeo que los parametros referidos a los numeros (n y k) sean validos
	if (!numeros_validos(argv[1], argv[2])){
		imprimir_error("Error: Parametros erroneos");
		return 1;
	}
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);

	//Creo el min_sketch
	min_sketch_t* min_sketch = min_sketch_crear(k);
	if (!min_sketch){
		imprimir_error("Error: Falla durante la ejecucion");
		return 1;
	}
	//Si llegamos a esta instancia todo se creo en orden
	leer_tweets(min_sketch, archivo, n, k);
	min_sketch_destruir(min_sketch);
	return 0;
}

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
  * *****************************************************************/
void imprimir_error(char* error_sms){
	/*Imprime el mensaje de error pasado por parametro*/
	fprintf(stderr, "%s\n", error_sms);
}

bool numeros_validos(const char* cad1, const char* cad2){
	/*Recibe dos cadenas. Devuelve true si las cadenas son un numero,
  	false en caso contrario*/
  	bool valido = true;
 	for (size_t i = 0; i < strlen(cad1); i++) valido &= isdigit(cad1[i]) != 0;
 	for (size_t i = 0; i < strlen(cad2); i++) valido &= isdigit(cad2[i]) != 0;
  	return valido;
}

void leer_tweets(min_sketch_t* min_sketch, FILE* archivo, int n, int k){
	/*Lee los tweets del archivo pasado por parametro*/
	char* cadena = NULL;
	size_t num_bytes = 0, cont_lineas = 0, indice = 1;
	long int bytes_leidos;
	//Indice es para dar la salida pedida "--- indice"

	while ((bytes_leidos = getline(&cadena, &num_bytes, archivo)) != -1){ //Mietras no llegue al final del archivo leo lineas
		if (cont_lineas == n){ //Imprimo los TTs
			imprimir_tts(min_sketch, indice++, k);
			cont_lineas = 0;
		}
		if (cadena[bytes_leidos - 1] == '\n') cadena[--bytes_leidos] = '\0'; //Piso el \n
		char** strv = split(cadena, ','); //Creo un arreglo que tenga los tweets de esas lineas
		if (!strv){
			imprimir_error("Error: Falla durante la ejecucion");
			return; //El min_sketch pudo quedarse con cosas, chequear que se destruya bien
		}
		size_t pos_aux = 1; //Porque la posicion 0 es el usuario
		while (strv[pos_aux] != NULL){
			min_sketch_update(min_sketch, strv[pos_aux++], k); //Cuando creas a tupla haces una copia del tweet
		}
		free_strv(strv); 
		cont_lineas++; //Esto podrias hacer arriba cuando compares lineas
	}
	if (cont_lineas != 0){ //Significa que termine de leer pero no imprimi porque se termino el archivo antes
		imprimir_tts(min_sketch, indice, k);
	}

}

void imprimir_tts(min_sketch_t* min_sketch, size_t indice, int k){
	/*Imprime los TTs*/
	fprintf(stdout, "--- %ld", indice);
	min_sketch_print(min_sketch, k); //Se encarga de imprimir los K TTs
}


