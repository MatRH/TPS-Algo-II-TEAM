/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> //para el ssize_t
#include <wchar.h> //para el ssize_t

int comparar_clave(const char* clave1, const char* clave2){
    return strcmp(clave1, clave2);
}
bool what_does_the_fox_say(const char* clave, void* dato, void* extra){
  if(strcmp(clave, "fox")){
    char* says = dato;
    printf("%s\n", says);
    extra = dato;
    return false;
  }
  return true;
}

bool visitar(const char *clave, void *valor, void *extra){
    if (!clave) return false;
    printf("La clave es: %s\n", clave);
    return true;

}
/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio()
{
    printf("INICIO PRUEBAS ABB VACÍO\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_abb_insertar()
{
    printf("INICIO PRUEBAS ABB INSERTAR\n");
    abb_t* abb1 = abb_crear(comparar_clave, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb1, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb1) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb1, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb1, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb1, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb1) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb1, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb1) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb1, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb1, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb1, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb1) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb1, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb1, clave3));

    abb_destruir(abb1);
}

static void prueba_abb_reemplazar()
{
    printf("INICIO PRUEBAS ABB REEEMPLAZAR\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir()
{
    printf("INICIO PRUEBAS ABB REEMPLAZAR CON DESTRUIR\n");
    abb_t* abb = abb_crear(comparar_clave, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_borrar()
{
    printf("INICIO PRUEBAS ABB BORRAR\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia()
{
    printf("INICIO PRUEBAS ABB CLAVE VACÍA\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_iterador_interno()
{
    printf("INICIO PRUEBAS ABB ITERADOR INTERNO\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);
    char extra[100] = " ";

    char *clave1 = "dog", *valor1 = "woof";
    char *clave2 = "cat", *valor2 = "miau";
    char *clave3 = "bird", *valor3 = "tweet";
    char *clave4 = "mouse", *valor4 = "squeak";
    char *clave5 = "cow", *valor5 = "moo";
    char *clave6 = "frog", *valor6 = "croak";
    char *clave7 = "elephant", *valor7 = "toot";
    char *clave8 = "duck", *valor8 = "quack";
    char *clave9 = "fish", *valor9 = "blub";
    char *clave10 = "seal", *valor10 = "ow ow ow";
    char *clave11 = "fox", *valor11 = "RING DING DING DING DINGERINGEDING";

    abb_guardar(abb, clave1, valor1);
    abb_guardar(abb, clave2, valor2);
    abb_guardar(abb, clave3, valor3);
    abb_guardar(abb, clave4, valor4);
    abb_guardar(abb, clave5, valor5);
    abb_guardar(abb, clave6, valor6);
    abb_guardar(abb, clave7, valor7);
    abb_guardar(abb, clave8, valor8);
    abb_guardar(abb, clave9, valor9);
    abb_guardar(abb, clave10, valor10);
    abb_guardar(abb, clave11, valor11);

    abb_in_order(abb, what_does_the_fox_say, extra);
    print_test("Prueba iterador interno", strcmp(extra, valor11));

    abb_destruir(abb);

}

static void prueba_abb_valor_null()
{
    printf("INICIO PRUEBAS ABB CON VALOR NULL\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}


static void prueba_abb_iterador_externo()
{
  printf("INICIO PRUEBAS ABB ITERADOR EXTERNO\n");
  abb_t* abb = abb_crear(comparar_clave, NULL);

  char *clave1 = "dog", *valor1 = "woof";
  char *clave2 = "cat", *valor2 = "miau";
  char *clave3 = "bird", *valor3 = "tweet";
  char *clave4 = "mouse", *valor4 = "squeak";
  char *clave5 = "cow", *valor5 = "moo";
  char *clave6 = "frog", *valor6 = "croak";
  char *clave7 = "elephant", *valor7 = "toot";
  char *clave8 = "duck", *valor8 = "quack";
  char *clave9 = "fish", *valor9 = "blub";
  char *clave10 = "seal", *valor10 = "ow ow ow";
  char *clave11 = "fox", *valor11 = "RING DING DING DING DINGERINGEDING";

  abb_guardar(abb, clave1, valor1);
  abb_guardar(abb, clave2, valor2);
  abb_guardar(abb, clave3, valor3);
  abb_guardar(abb, clave4, valor4);
  abb_guardar(abb, clave5, valor5);
  abb_guardar(abb, clave6, valor6);
  abb_guardar(abb, clave7, valor7);
  abb_guardar(abb, clave8, valor8);
  abb_guardar(abb, clave9, valor9);
  abb_guardar(abb, clave10, valor10);
  abb_guardar(abb, clave11, valor11);

  abb_iter_t* iter = abb_iter_in_crear(abb);
  print_test("Creo el iterador", iter);
  print_test("El iterador no está al final", !abb_iter_in_al_final(iter));
  print_test("Puedo ver el actual", abb_iter_in_ver_actual(iter));
  print_test("Puedo avanzar", abb_iter_in_avanzar(iter));
  print_test("El iterador no está al final", !abb_iter_in_al_final(iter));
  print_test("Puedo ver el actual", abb_iter_in_ver_actual(iter));
  printf("Recorro hasta el final\n" );

  while (!abb_iter_in_al_final(iter)){
    print_test("Puedo avanzar", abb_iter_in_avanzar(iter));
  }
  print_test("El iterador está al final", abb_iter_in_al_final(iter));
  print_test("No puedo ver el actual", !abb_iter_in_ver_actual(iter));
  printf("Destruyo el iterador\n");
  abb_iter_in_destruir(iter);

  abb_destruir(abb);
}


static void prueba_abb_volumen(size_t largo, bool debug)
{
    printf("INICIO PRUEBAS ABB VOLÚMEN\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);

    const size_t largo_clave = 10;
    char** claves = malloc(sizeof(char*) * largo);

    unsigned* valores[largo];

    // Inserta 'largo' parejas en el abb
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        claves[i] = malloc(sizeof(char) * largo_clave);
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }
    //Desordeno el arreglo
    for (size_t i =0 ; i < largo; i++){
        size_t num_rnd = (size_t)rand() % largo;
        if (num_rnd == i) continue;

        unsigned* aux_valor = valores[i];
        valores[i] = valores[num_rnd];
        valores[num_rnd] = aux_valor;

        char* aux_clave = claves[i];
        claves[i] = claves[num_rnd];
        claves[num_rnd] = aux_clave;
    }
    //Guardo las claves
    for (unsigned i = 0; i < largo; i++){
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    // Verifica que devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    // Verifica que borre y devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        unsigned* borrado = abb_borrar(abb, claves[i]);
        ok = *borrado == *valores[i];
        if (!ok){
          printf("Clave guardada : %s\n", claves[i]);
          printf("Valor esperado : %u\n", *valores[i]);
          printf("Valor obtenido : %u\n", *borrado);
          break;
        };
    }

    if (debug) print_test("Prueba abb borrar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    // Destruye el abb y crea uno nuevo que sí libera
    abb_destruir(abb);
    //Libero la memoria pedida para las claves y valores
    for (size_t i = 0; i < largo; i ++){
        free(claves[i]);
        free(valores[i]);
    }
    free(claves);
}

/*static void iterar_externo_volumen(size_t largo)
{
    printf("INICIO PRUEBAS ABB ITERADOR VOLUMEN\n");
    abb_t* abb = abb_crear(comparar_clave, NULL);
    const size_t largo_clave = 10;
    char** claves = malloc(sizeof(char*) * largo);

    unsigned* valores[largo];

    // Inserta 'largo' parejas en el abb
    bool ok = true;
    bool debug = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        claves[i] = malloc(sizeof(char) * largo_clave);
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }
    //Desordeno el arreglo
    for (size_t i =0 ; i < largo; i++){
        size_t num_rnd = (size_t)rand() % largo;
        if (num_rnd == i) continue;

        unsigned* aux_valor = valores[i];
        valores[i] = valores[num_rnd];
        valores[num_rnd] = aux_valor;

        char* aux_clave = claves[i];
        claves[i] = claves[num_rnd];
        claves[num_rnd] = aux_clave;
    }
    //Guardo las claves
    for (unsigned i = 0; i < largo; i++){
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    // Verifica que devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);


    
    abb_in_order(abb, visitar, NULL);

    abb_destruir(abb);
    //Libero la memoria pedida para las claves y valores
    for (size_t i = 0; i < largo; i ++){
        free(claves[i]);
        free(valores[i]);
    }
    free(claves);
    print_test("Destrui el abb usado en PRUEBA VOLUMEN", true);
}*/

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_alumno()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_iterador_interno();
    prueba_abb_iterador_externo();
    prueba_abb_volumen(5000, true);
    //iterar_externo_volumen(1);
}

void pruebas_volumen_alumno(size_t largo)
{
    prueba_abb_volumen(largo, false);
}
