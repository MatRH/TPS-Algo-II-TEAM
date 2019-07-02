/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#include "lista.h"
#include "hash.h"
#include "tupla.h"

struct registro{
  char* usuario;
  lista_t* tweets;
};

hash_t* procesar_usuarios(FILE* input);
bool analizar_datos(hash_t* usuarios_procesados);
void imprimir_resultado(tupla_t** tuplas);//función que imprime por pantalla los datos obtenidos
void hash_destruir_w(void *hash){//un wrapper para poder pasarlo como funcion de destrucciond de dato
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
    bool todo_ok;
    if(usuarios_procesados) todo_ok = analizar_datos(usuarios_procesados);
    if(!usuarios_procesados || !todo_ok){
      fprintf(stderr, "%s\n", "Error: No se pudieron procesar los usuarios");
    }
    return 0;
  }

hash_t* procesar_usuarios(FILE* input){
  hash_t* usuarios = hash_crear(hash_destruir_w); //usuarios va a ser un hash de hashes, donde cada usuario es una clave y un hash con los tweets el valor
  if(!usuarios) return NULL;
  char* usuario;
  char* tweet;
  bool todo_ok = true;
  char** strv;
  char* linea = NULL;//para que getline se encargue de manejar la memoria
  size_t num_bytes = 0;//para que getline se encargue de manejar la memoria
  long int bytes_leidos;
  while((bytes_leidos = getline(&linea, &num_bytes, input)) != -1 && todo_ok){
     if (linea[bytes_leidos - 1] == '\n') linea[--bytes_leidos] = '\0'; //Piso el \n
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
  free(linea);
  if (todo_ok) return usuarios;
  else hash_destruir(usuarios);
  return NULL;
}

bool analizar_datos(hash_t* usuarios_procesados){
  hash_iter_t* iter = hash_iter_crear(usuarios_procesados);
  if (!iter) return false;

  size_t num_usuario = 0;
  size_t len_tuplas = hash_cantidad(usuarios_procesados) + 1; //Asi le ponemos el NULL al final
  tupla_t** tuplas = malloc((sizeof(void*))*(len_tuplas)); //Arreglo de tuplas
  if (!tuplas){
    hash_iter_destruir(iter);
    return false;
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
      return false;
    }
    tuplas[num_usuario++] = tupla; //Guardas la tupla
    hash_iter_avanzar(iter);
  }
  hash_iter_destruir(iter);
  //En estas instancias ya tenes el arreglo de tuplas y su longitud
  //tupla_t** tuplas_ordenadas;
  if (!ordernar_tuplas(tuplas, len_tuplas)) return false;

  imprimir_resultado(tuplas);//función que imprime por pantalla los datos obtenidos
  hash_destruir(usuarios_procesados);
  return true;
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
  printf("\n");
  free(tuplas);
}
