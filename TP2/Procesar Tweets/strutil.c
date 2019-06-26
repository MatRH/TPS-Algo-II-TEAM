/*
Alumno: Torresetti Lisandro
Padron: 99846
Corrector: Cano Matias
*/
#define _POSIX_C_SOURCE 200809L  //Para que ande strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

size_t obtener_longitud(char** str_vector);
size_t contar_separadores(const char* cadena, char sep, size_t* larger_str);

char *substr(const char *str, size_t n){
	size_t long_ideal = strlen(str);
	if (long_ideal >= n) long_ideal = n;

	char *cadena_resultante = malloc(sizeof(char) * (long_ideal + 1)); //para poner barra cero sumas 1
	if(!cadena_resultante) return NULL;

	strncpy(cadena_resultante, str, long_ideal); //copia n - 1 caracteres
	cadena_resultante[long_ideal] = '\0';
	return cadena_resultante;
}

char **split(const char *str, char sep){
	size_t long_str = strlen(str) + 1;
	size_t cad_mas_larga = 0;
	size_t long_ideal = contar_separadores(str, sep, &cad_mas_larga);

	//Creo el vector
	char** str_vector = malloc(sizeof(char*) * long_ideal);
	if (!str_vector) return NULL;

	char cadena_aux[cad_mas_larga + 1]; //memoria estatica

	size_t pos_vector = 0, pos_cad_aux = 0;
	for (size_t i = 0; i < long_str; ++i){
		if(str[i] == sep || i + 1 == long_str){
			cadena_aux[pos_cad_aux] = '\0'; 
			str_vector[pos_vector++] = strdup(cadena_aux);
			pos_cad_aux = 0;  
			continue;
		}
		cadena_aux[pos_cad_aux++] = str[i];
	}
	str_vector[pos_vector] = NULL;
	return str_vector;
}

char *join(char **strv, char sep){
	size_t long_requerida = obtener_longitud(strv);
	char* cadena_aux = malloc(sizeof(char) * long_requerida);
	if (!cadena_aux) return NULL;

	char *cadena = NULL;
	size_t pos_cad_aux = 0, pos_vector = 0, long_cadena = 0;
	
	while((cadena = strv[pos_vector]) != NULL){
		long_cadena  = strlen(cadena);
		for (size_t i = 0; i < long_cadena; i++) cadena_aux[pos_cad_aux++] = cadena[i];
		if (strv[++pos_vector] != NULL && sep != '\0') cadena_aux[pos_cad_aux++] = sep; 
		//La cadena que concatene no era la ultima
	}
	cadena_aux[pos_cad_aux] = '\0';
	return cadena_aux;
}

void free_strv(char *strv[]){
	size_t contador = 0;
	while(strv[contador] != NULL) free(strv[contador++]); //libero las cadenas
	free(strv); //libero el vector
}

//Funciones auxiliares
size_t contar_separadores(const char* cadena, char sep, size_t* mayor_long){
	/*Cuenta la cantidad de separadores que tiene la cadena, a su vez calcula
	la longitud de la maxima cadena posible formada*/
	size_t cant_sep = 2; //porque si pasan cadena vacia necesito uno para el \0 y otro para el NULL
	size_t long_max_aux = 0, long_cadena = strlen(cadena);
	
	for (size_t i = 0; i < long_cadena; i++){
		long_max_aux++;
		if(cadena[i] == sep){
			*mayor_long = (*mayor_long < long_max_aux) ? long_max_aux : *mayor_long;
			cant_sep++;
			long_max_aux = 0;
		}
		else if (i + 1 == long_cadena) *mayor_long = (*mayor_long < long_max_aux) ? long_max_aux : *mayor_long;
	}
	return cant_sep;
}

size_t obtener_longitud(char** vec_str){ 
	size_t long_total= 1; //Aunque sea tiene que ser uno para guardar el \0
	size_t pos_vector = 0;
	while(vec_str[pos_vector] != NULL) long_total += strlen(vec_str[pos_vector++]) + 1;
	return long_total;
}