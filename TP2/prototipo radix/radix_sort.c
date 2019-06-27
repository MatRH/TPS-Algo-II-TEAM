#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tupla.h"
#include <string.h>
#define TAM 27 //Por las 26 letras del abecedario yankee mas el _ despues aumentalo a 62 para tener el cjto entero

//Declaraciones funciones auxiliares
size_t indexador(char caracter);
void counting_sort_cadenas(tupla_t** tuplas, tupla_t** tuplas_ordenado, size_t pos_iter_radix);
//void countin_sort_frec(tupla_t** tuplas, tupla_t** tuplas_ordenado);
void countin_sort_frec(tupla_t** tuplas_ordenado, tupla_t** resultado);
//tupla_t** radix_sort(tupla_t** tuplas);
tupla_t** radix_sort(tupla_t** tuplas, tupla_t** resultado);


int main(){ //Cero si todo anda bien
	//Creo tres tuplas
	tupla_t* tupla1 = tupla_crear("nara_c", 40); //long 6
	tupla_t* tupla2 = tupla_crear("seba", 41); //long 4
	tupla_t* tupla3 = tupla_crear("licha", 20); //long 5

	if (!tupla1 || !tupla2 || !tupla3){
		printf("Alguna de las tuplas se creo mal\n");
		return 1;
	}

	tupla_t** tuplas = malloc(sizeof(void*) * 4);
	if (!tuplas){
		printf("No hay arreglo de tuplas\n");
		return 1;
	} 

	tupla_t** resultado = malloc(sizeof(void*) * 4);
	resultado[3] = NULL;

	//Guardo las tuplas
	tuplas[0] = tupla1;
	tuplas[1] = tupla2;
	tuplas[2] = tupla3;
	tuplas[3] = NULL; //Fin del arreglo
	//Guarde las tuplas, ahora quiero ordenarlas
	//Ahora comienza la magia de querer ordenar creo
	tupla_t** tuplas_ordenado = radix_sort(tuplas, resultado);
	for (int i = 2; i >= 0; i--){  //queda ordenado de menor a mayor porque counting sort hace eso, mas facil hacer asi
		tupla_t* tupla_aux = tuplas_ordenado[i];
		printf("%ld - %s\n", tupla_frec(tupla_aux), tupla_tag(tupla_aux));
		tupla_destruir(tupla_aux);
	}
	free(tuplas);
	free(resultado);
	printf("Termine de ordenar todo\n");
	return 0;
}

size_t indexador(char caracter){
	/*Devuelve el indice que le corresponde al caracter leido del usuario*/
	//Esto es para que obtengas letra por letra
	if (caracter == '_') return 0;
	return (size_t)(caracter - 'a' + 1); //Asi si viene la a eso da cero mas 1 va a la posicion uno del arreglo
}

char obtener_caracter(char* usuario, size_t pos_iter_radix){
	//Devuelve el caracter a leer en la iteracion de radix sort
	int resta = (int)(strlen(usuario) - pos_iter_radix); //DEBUG ACA PUEDE FALLAR EL CASTEO
	return (resta > 0) ? usuario[resta] : usuario[0]; //Si devuelve cero es porque tenes cadenas de diferente largo y una ya termino
}

tupla_t** radix_sort(tupla_t** tuplas, tupla_t** resultado){  //DEBBUG, PODRIAS PASARLE DIRECTAMENTE EL ARREGLO TUPLAS_ORDENADO ASI NO CREAS NADA
	//Se encarga de hacer la magia
	tupla_t** tuplas_ordenado = malloc(sizeof(void*) * 4);
	tuplas_ordenado[3] = NULL; //Primero tenes basura, vemos que pasa en cada iteracion

	for (size_t i = 1; i < 7; i++){
		counting_sort_cadenas(tuplas, tuplas_ordenado, i);
	}
	size_t pos = 0;
	while(tuplas_ordenado[pos] != NULL){
		tupla_t* tuplita = tuplas_ordenado[pos++];
		printf("%s\n", tupla_tag(tuplita));
	}
	countin_sort_frec(tuplas_ordenado, resultado);
	free(tuplas_ordenado);
	return resultado;
}

void counting_sort_cadenas(tupla_t** tuplas, tupla_t** tuplas_ordenado, size_t pos_iter_radix){
	//Hace la segunda parte de magia
	int contador[TAM] = {0}; //Lo inicializo en cero
	int suma_acum[TAM] = {0}; //Tambien arranca en cero //yo pondria tam - 1 porque yo le agrego un cero, y tengo que leer uno menos, creo
	size_t pos_aux = 0;
	while (tuplas[pos_aux] != NULL){
		//Aca tengo que ir actualizando contador
		tupla_t* tupla = tuplas[pos_aux++];
		char* usuario = tupla_tag(tupla); //Obtengo el usuario
		size_t pos_cont = indexador(obtener_caracter(usuario, pos_iter_radix)); //Todo esto me da la posicion del contador
		if (pos_cont >= TAM) printf("Me dio mas grande la pos del contador, algo anda mal\n");
		contador[pos_cont]++; //Actualizo la cantidad
	}
	//LLEGADA A ESTA INSTANCIA SUPUESTAMENTE YA CONTE LOS CARACTERES PARA ESTA CIFRA SIGNIFICATIVA DADA POR POS_ITER_RADIX
	//AHORA LA FRECUENCIA ACUMULADA
	for (size_t i = 1; i < TAM; i++){
		int cantidad = contador[i - 1];
		suma_acum[i] = suma_acum[i-1] + cantidad; //DEBUG FIJATE UN CASO CON Z DESPUES QUE PASA
	}
	//Supuestamente ya tengo las frecuencias acumuladas, ahora viene la magia
	pos_aux = 0; //Por las dudas DEBUG
	while (tuplas[pos_aux] != NULL){
		tupla_t* tupla_aux = tuplas[pos_aux];
		char* usuario_aux = tupla_tag(tupla_aux);
		size_t posicion = indexador(obtener_caracter(usuario_aux, pos_iter_radix));
		int pos_resultado = suma_acum[posicion]++;
		tuplas_ordenado[pos_resultado] = tuplas[pos_aux++];
	}
	//A estas alturas ya ordene alfabeticamente, pero quedo todo guardado en el arreglo resultante

}

void countin_sort_frec(tupla_t** tuplas_ordenado, tupla_t** resultado){
	//Ordena por frecuencia
	int max = 41;
	int min = 20;
	int k =  1 + max - min; //21 asi si haces modulo a los valores te da bien
	int contador[k];
	int suma_acum[k];
	for (int i = 0; i < k - 1; i++){
		contador[i] = 0;
		suma_acum[i] = 0;
	}

	size_t pos_aux = 0;
	while (tuplas_ordenado[pos_aux] != NULL){
		tupla_t* tupla = tuplas_ordenado[pos_aux++];
		size_t frec = tupla_frec(tupla);
		contador[frec - (size_t)min]++; //acutalizo la frecuencia
	}
	//Aca ya tengo el contador ahora hago lo de sumas acumuladas
	for (int i = 1; i < k; i++){
		int cantidad = contador[i-1];
		suma_acum[i] = suma_acum[i - 1] + cantidad;
		printf("La sua acumulada %d es %d\n",i, suma_acum[i]);
	}

	//Tengo las sumas acumuladas
	pos_aux = 0;
	//while (tuplas_ordenado[pos_aux] != NULL){
	for (int i = 2; i >= 0; i--){
		tupla_t* tupla_aux = tuplas_ordenado[i]; //Otro arreglo mas se me hace una guasada
		size_t frec_aux = tupla_frec(tupla_aux); //40 seba 40 nara 20 licha
		printf("el resultado es %ld y la pos es %d\n", frec_aux - (size_t)min, suma_acum[frec_aux - (size_t)min] );
		int posicion = suma_acum[frec_aux - (size_t)min]++;
		resultado[posicion] = tuplas_ordenado[i];

	}


}

