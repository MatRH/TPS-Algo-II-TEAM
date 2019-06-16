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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "min_sketch.h"


//Declaraciones funciones auxiliares
bool numeros_validos(const char* cad1, const char* cad2);
char **split(const char *str, char sep);
void free_strv(char *strv[]);
/*Asimismo, se pide que se imprima la frecuencia estimada para poder realizar
durante la corrección una comparación con la frecuencia exacta. La salida debe
ser como se indica a continuación, con separadores numerados entre cada grupo
de TT; por ejemplo, para una entrada de 9 líneas, con n=4 y k=2 la salida sería
del estilo:

  --- 1
  7 pesquisa
  2 zombietalk
  --- 2
  14 niley
  3 squarespace
  --- 3
  1 charlotte

*/

int main(int argc, char* argv[]){
    if(argc != 3){
      fprintf(stderr, "%s\n", "Error: Cantidad erronea de parametros");
      return 1;
    }
    FILE* archivo = stdin;
    if(!archivo){
      fprintf(stderr, "%s\n", "Error: archivo fuente inaccesible");
      return 1;
    }
    //El programa deberá imprimir por salida estándar (stdout) el histórico
    //de los k TTs aproximados cada n lineas
    if (!numeros_validos(argv[1], argv[2])){
      fprintf(stderr, "%s\n", "Error: Parametros errones");
      return 1;
    }
    size_t k = atoi(argv[1]);
    size_t n = atoi(argv[2]);
    //Si hasta aca no hubo errores tenemos todo para comenzar a hacer la magia de procesar tweets
    min_sketch_t* min_sketch = min_sketch_crear(k * 100); //Lo de 100 creo que dijo Dato en un mail no puedo chequear ahora
    if (!min_sketch) return 1;
    //Aca tiene que leer cada n lineas e imprimir las k mejores del momento
    size_t num_bytes = 0, bytes_leidos, cont_lineas = 0, linea = 1; //Linea es usado para que me de la salida que piden
    char* cadena = NULL;

    while(!feof(archivo)){
      if (cont_lineas++ == n){
        fprintf(stdout, "\n--- %ld", linea++);
        min_sketch_print(min_sketch);
        cont_lineas = 1;
      }
      getline(&cadena, &num_bytes, archivo);
      char** strv = split(cadena, ','); //Esta modificado para que no le de bola a los espacios
      if (!strv){
        min_sketch_destruir(min_sketch); //hay que modificarla
        return 1; //No se si hay que imprimir un error
      }
      size_t pos_aux = 0;
      while (strv[pos_aux] != NULL) min_sketch_update(min_sketch, strv[pos_aux++], k);
      free_strv(strv);
    }
    //printf("\n"); //No se si faltara este printf
    return 0;
}

//Funciones auxiliares
bool numeros_validos(const char* cad1, const char* cad2){
  /*Recibe dos cadenas. Devuelve true si las cadenas son un numero,
  false en caso contrario*/
  bool valido = true;
  for (size_t i = 0; i < strlen(cad1); i++) valido &= isdigit(cad1[i]) != 0;
  for (size_t i = 0; i < strlen(cad2); i++) valido &= isdigit(cad2[i]) != 0;
  return valido;
}

char **split(const char *str, char sep){
  size_t long_str = strlen(str) + 1; //La tenes podes pasarla por parametro
  size_t cad_mas_larga = 0;
  size_t long_ideal = contar_separadores(str, sep, &cad_mas_larga);

  //Creo el vector
  char** str_vector = malloc(sizeof(char*) * long_ideal);
  if (!str_vector) return NULL;

  char cadena_aux[cad_mas_larga + 1]; //memoria estatica

  size_t pos_vector = 0, pos_cad_aux = 0;
  for (size_t i = 0; i < long_str; ++i){
    if (str[i] == ' ' && i + 1 != long_str) continue; //Asi no me copia los espacios
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

void free_strv(char *strv[]){
  size_t contador = 0;
  while(strv[contador] != NULL) free(strv[contador++]); //libero las cadenas
  free(strv); //libero el vector
}

