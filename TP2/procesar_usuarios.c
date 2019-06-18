/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
/*
procesar_usuarios: este programa tendrá como objetivo contar la cantidad de
hashtags que usa cada usuario, leyendo cada línea del archivo pasado por parámetro.
Como se sabe que la cantidad de usuarios es mucho menor a la cantidad de TTs, y
que dicho archivo termina, consideramos que se puede almacenar en memoria todo
lo necesario para procesar la entrada.
El programa deberá procesar la entrada y luego deberá escribir por salida estándar
los usuarios y la cantidad de hashtags que utilizaron en tiempo lineal: O(u + t)
siendo u la cantidad de usuarios encontrados y t la cantidad de hashtags diferentes,
ordenados según ocurrencias. Los usuarios que tienen la misma cantidad de hashtags
tienen que ser impresos por orden alfabético (tener en cuenta que para ordenar
alfabéticamente a los usuarios, los nombres de los mismos no superan más de 15 caracteres).

Ejemplo de invocación: ~$ ./procesar_usuarios tweets.txt

Ejemplo de salida: El usuario Mile tiene 5 tags, Martín 3, Jorge 3, Javier 5,
Nacho 8, Cami 2, Ceci 5, Ezequiel 3, Matías 2, Dato 6, Anita 1, Gian 1

Se debe escribir por salida estándar:
  1: Anita, Gian
  2: Cami, Matias
  3: Ezequiel, Jorge, Martín
  5: Ceci, Javier, Mile
  6: Dato
  8: Nacho

En ambos casos, la entrada debe tener el siguiente formato:

	Usuario1,tag1,tag2,...
	Usuario2,tag3,tag4,...
	Usuario1,tag5,tag1,...

Donde cada línea corresponde a un tweet realizado por el usuario mencionado,
continuado por una los tags en dicho tweet. Por eso, un mismo usuario puede
aparecer más de una vez. No hay líneas que no contengan al menos un tag.
Atención: tener en cuenta que el archivo provisto por el curso, descomprimido,
460 Mb, y cuenta con más 19 millones de líneas. Se recomienda utilizar el comando
less de Unix para leerlo, ya que no lo carga en memoria: less tweets.txt.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#include "lista.h"
#include "hash.h"
#include "tupla.h"
#define MAX_LEN 150

typedef struct registro{
  char* usuario;
  lista_t* tweets;
}

hash_t procesar_usuarios(FILE* input);
void analizar_datos(hasht_t usuarios_procesados);
void imprimir_resultado(tuplas_t** tuplas);//función que imprime por pantalla los datos obtenidos

int main(int argc, char* argv[]){
    if(argc != 2){
      fprintf( stderr, "Error: Cantidad erronea de parametros");
      return 0;
    }
    FILE* input;
    input = argv[1];
    if(!input){
      fprintf( stderr, "Error: archivo fuente inaccesible");
      return 0;
    }
    hash_t usuarios_procesados = procesar_usuarios(input);
    if(usuarios_procesados) analizar_datos(usuarios_procesados);
    else fprintf( stderr, "Error: No se pudieron procesar los usuarios");
    return 0;
  }

hash_t procesar_usuarios(FILE* input){
  char linea[MAX_LEN];
  linea = fgets(linea, MAX_LEN, input);
  hash_t usuarios = hash_crear(hash_destruir); //usuarios va a ser un hash de hashes, donde cada usuario es una clave y un hash con los tweets el valor
  char* usuario;
  char* tweet;
  bool todo_ok = true;

  while(linea && todo_ok){
     char** strv = split(linea, ',');
     usuario = strv[0];

     if(!hash_pertenece(usuarios, usuario)){
       hash_t tweets = hash_crear(NULL);
       todo_ok = hash_guardar(usuarios, usuario, tweets);//guardo el usuario y creo el hash para sus tweets
     }

     if(!todo_ok) break;

     size_t i = 1;
     while(strv[i]){//guardo los tweets del usuario
       tweet = strv[i];
       todo_ok = hash_guardar(hash_obtener(usuarios, usuario), tweet);
       i++;
     }
  linea = fgets(linea, MAX_LEN, input);//tomo la siguiente linea del archivo
  }
  if (todo_ok) return usuarios;
  free_strv(strv);
  return NULL;
}

void analizar_datos(hasht_t usuarios_procesados){
  hash_iter_t iter = hash_iter_crear(usuarios_procesados);
  size_t num_usuario = 0;
  size_t len_tuplas = hash_cantidad(usuarios_procesados)+1);

  while(!hash_iter_al_final(iter)){//obtengo los usuarios y su frecuencia de tweets
    char* usuario = hash_iter_ver_actual(iter);
    hash_t tweets = hash_obtener(usuarios_procesados, usuario);
    size_t cantidad = hash_cantidad(tweets);
    hash_destruir(tweets);
    tupla_t* tupla = tupla_crear(usuario, cantidad);
    tupla_t** tuplas = malloc(sizeof(void*)*(len_tuplas);
    tuplas[hash_cantidad(usuarios_procesados)] = NULL; //para marcar el final del arreglo
    tuplas[num_usuario] = tupla;
    num_usuario++;
    hash_iter_avanzar(iter);
  }
  hash_iter_destruir(iter);

  ordernar_tuplas(tuplas, len_tuplas);//función que ordena un arreglo de tuplas primero según frecuencuencia y después alfabéticamente
  imprimir_resultado(tuplas, len_tuplas);//función que imprime por pantalla los datos obtenidos

  hash_destruir(usuarios_procesados);
}

void imprimimr_resultado(tupla_t** tuplas, size_t len){
  size_t pos = 0;//posicion sobre el arreglo de tuplas
  size_t cant_actual;//cantidad de usuarios guardados en usuarios
  size_t cant_anterior = 0; //cantidad de tweets de los usuarios guardados
  tupla_t tupla_actual;//pareja usuario, cantidad tweets que estoy evaluando
  size_t cant_usuarios = 0;//cantidad de usuarios guardados en el arreglo de usuarios
  char** usuarios = malloc(sizeof(char*)*len-1);//arreglo de usuarios

  while(tuplas[pos]){//mientras que tenga usuarios para procesar
    tupla_actual = tuplas[pos];
    cant_actual = tupla_frec(tupla_actual);
    usuario_actual = tupla_tag(tupla_actual);

    if(cant_actual > cant_anterior && usuarios[0]){
      char* str = join(usuarios, ', ');//cadena para mostrar por pantalla de todos los usuarios
      printf("%ld: %s\n", cant_actual, str); //imprimo los usuarios anteriores

      for(int i; i <= cant_usuarios; i++){ //borro los usuarios anteriores
        usuarios[i] = NULL;
      }
      cant_usuarios = 0; //reinicio la cantidad de usuarios guardados
      free(str); //libero str
    }

    usuarios[cant_usuarios] = usuario; //guardo el usuario actual
    cant_usuarios++;
    cant_anterior = cant_actual;//actualizo la cantidad de tweets de los usuarios que estoy almacenando
  }

  if(usuarios[0]){//si me quedan usuarios por imprimir
    printf("%ld: %s\n", cant_actual, join(usuarios, ', ')); //imprimo los usuarios anteriores
  }

  for(pos = 0; tuplas[pos]; pos++){//destruyo las tuplas
    tupla_destruir(tuplas[pos]);
  }

  free(usuarios);//libero el arreglo de usuarios
  free(tuplas);//libero el arreglo de tuplas
}
