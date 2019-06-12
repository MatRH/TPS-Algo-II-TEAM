/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*COUNT MIN SKETCH*/
/*MI IDEA PARA EL CONTADOR ES IMPLEMENTAR UNA CLASE QUE SEA COUNT MIN SKETCH O
ALGO POR EL ESTILO Y QUE ESE OBJETO TENGA ADENTRO LOS DOS ARREGLOS, LAS DOS
FUNCIONES DE HASHING Y NOSOTROS LE VAMOS APSANDO LAS COSAS QUE APARECEN*/

typedef size_t (*funcion_de_hash)(const char*);

typedef struct min_sketch{
  funcion_de_hash f1;
  funcion_de_hash f2;
  int* arr1;
  int* arr2;
}min_sketch_t;

bool min_sketch_update(min_sketch_t* min_sketch, char* tweet){
  /* 1)aplico f1 al tweet lo guardo en arr1
     2)voy a su posicion en arr1 y le sumo uno al valor
     3)repito para todas las f que tenga
*/
  funcion_de_hash f1 = min_sketch->f1;
  funcion_de_hash f2 = min_sketch->f2;
  int* arr1 = min_sketch->arr1;
  int* arr2 = min_sketch->arr2;
  size_t hash1 = f1(tweet);
  size_t hash2 = f2(tweet);

  arr1[hash1]++;
  arr2[hash2]++;
  return true;
}

char* min_sketch_get(min_sketch_t* min_sketch, int n){
    /* DEBERIA DEVOLVER LOS N TTS
    podriamos devolverlos en un strv como los del tp1*/
  int* arr1 = min_sketch->arr1;
  int* arr2 = min_sketch->arr2;
  for (int i; i < n, i++){
    /*acá la magia*/
    /*tiene que ser o(n log k), si le hacemos heapsort o heapify al arreglo ya
    no vamos a saber cuál es el tweet correspondiente al número*/
  }


}
