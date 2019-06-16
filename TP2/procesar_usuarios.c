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
    procesar_usuarios(input);
    return 0;
  }

void procesar_usuarios(FILE* input){

}
