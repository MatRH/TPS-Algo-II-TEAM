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
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#include "lista.h"
#include "hash.h"
#include "tupla.h"
#define MAX_LEN 150

struct registro{
  char* usuario;
  lista_t* tweets;
};

hash_t* procesar_usuarios(FILE* input);
void analizar_datos(hash_t* usuarios_procesados);
void imprimir_resultado(tupla_t** tuplas);//función que imprime por pantalla los datos obtenidos
void hash_destruir_w(void *hash){
	hash_destruir(hash);
}

int main(int argc, char* argv[]){
    if(argc != 2){
      fprintf( stderr, "Error: Cantidad erronea de parametros");
      return 0;
    }
    FILE* input;
    const char* input_file = argv[1];
    input = fopen(input_file, "r");
    if(!input){
      fprintf( stderr, "Error: archivo fuente inaccesible");
      return 0;
    }
    hash_t* usuarios_procesados = procesar_usuarios(input);
    fclose(input);
    if(usuarios_procesados) analizar_datos(usuarios_procesados);
    else fprintf(stderr, "%s\n", "Error: No se pudieron procesar los usuarios");
    return 0;
  }

hash_t* procesar_usuarios(FILE* input){
  char linea[MAX_LEN];
  hash_t* usuarios = hash_crear(hash_destruir_w); //usuarios va a ser un hash de hashes, donde cada usuario es una clave y un hash con los tweets el valor
  char* usuario;
  char* tweet;
  bool todo_ok = true;
  char** strv;
  while(fgets(linea, MAX_LEN, input) && todo_ok){
     strv = split(linea, ',');
     usuario = strv[0];

     if(!hash_pertenece(usuarios, usuario)){
       hash_t* tweets = hash_crear(NULL);
       todo_ok = hash_guardar(usuarios, usuario, tweets);//guardo el usuario y creo el hash para sus tweets
     }

     if(!todo_ok) break;

     size_t i = 1;
     while(strv[i]){//guardo los tweets del usuario
       tweet = strv[i];
       todo_ok = hash_guardar(hash_obtener(usuarios, usuario), tweet, NULL);
       i++;
     }
     free_strv(strv);
  }
  if (todo_ok) return usuarios;
  return NULL;
}

void analizar_datos(hash_t* usuarios_procesados){
  hash_iter_t* iter = hash_iter_crear(usuarios_procesados);
  if (!iter) return;

  size_t num_usuario = 0;
  size_t len_tuplas = hash_cantidad(usuarios_procesados) + 1; //Asi le ponemos el NULL al final
  tupla_t** tuplas = malloc((sizeof(void*))*(len_tuplas)); //Arreglo de tuplas
  if (!tuplas){
    hash_iter_destruir(iter);
    return;
  }
  tuplas[len_tuplas - 1] = NULL; //Para indicar el final del arreglo
  tupla_t* tupla;

  while(!hash_iter_al_final(iter)){//obtengo los usuarios y su frecuencia de tweets
    const char* usuario = hash_iter_ver_actual(iter);
    hash_t* tweets = hash_obtener(usuarios_procesados, usuario);
    size_t cantidad = hash_cantidad(tweets); //Obtengo la frecuencia de tweets de ese usuario
    tupla = tupla_crear(usuario, cantidad);
    if (!tupla){
      hash_iter_destruir(iter);
      return;
    }
    //tuplas[hash_cantidad(usuarios_procesados)] = NULL; //para marcar el final del arreglo
    tuplas[num_usuario++] = tupla; //Guardas la tupla
    hash_iter_avanzar(iter);
  }
  hash_iter_destruir(iter);
  //En estas instancias ya tenes el arreglo de tuplas y su longitud
  tupla_t** tuplas_ordenadas;
  tuplas_ordenadas = ordernar_tuplas(tuplas, len_tuplas);//función que ordena un arreglo de tuplas primero según frecuencuencia y después alfabéticamente
  if (!tuplas_ordenadas){
    printf("Algo fallo\n"); //Habria que liberar cosas si falla quizas
  }

  imprimir_resultado(tuplas_ordenadas);//función que imprime por pantalla los datos obtenidos
  hash_destruir(usuarios_procesados);
  free(tuplas);

}

void imprimir_resultado(tupla_t** tuplas){
  size_t cant_actual; //Cant de tweets del usuario actual
  size_t cant_anterior = 0; //Cantidad de tweets del usuario anterior, inicializa en cero asi se usa de flag
  size_t pos_aux = 0; //Posicion para moverme por el arreglo de tuplas
  tupla_t* tupla_aux = NULL;
  char* usuario;
  while (tuplas[pos_aux] != NULL){
    tupla_aux = tuplas[pos_aux++];
    usuario = tupla_tag(tupla_aux);
    cant_actual = tupla_frec(tupla_aux);
    if (!cant_anterior) fprintf(stdout, "%ld: ", cant_actual);
    else{
      (cant_actual != cant_anterior) ? fprintf(stdout, "\n%ld: ", cant_actual) : fprintf(stdout, "%s", ", ");
    } //Para obtener el formato pedido
    cant_anterior = cant_actual; //actualizo
    fprintf(stdout, "%s", usuario);
    tupla_destruir(tupla_aux);
  }
  printf("\n"); //Este \n es para generar la salida pedida, quizas no es necesario
  free(tuplas);
}
