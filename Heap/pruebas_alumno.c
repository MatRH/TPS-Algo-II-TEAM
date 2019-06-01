/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include "heap.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> //para el ssize_t

int comparar_clave(const char* clave1, const char* clave2){
	size_t len1 = strlen(clave1), len2 = strlen(clave2);
	if (len1 == len2) return 0;
	if (len1 > len2) return -1;
	else return 1;
}
/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_heap_vacio()
{
    heap_t* heap = heap_crear(comparar_clave);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap no hay tope", !heap_ver_max(heap));
    print_test("Prueba heap desencolar, es NULL", !heap_desencolar(heap));

    heap_destruir(heap);

		void* arreglo[0];

		heap_t* heap_arr = heap_arr_crear(arreglo, 0, comparar_clave);
		print_test("Prueba heap_arr crear heap vacio", heap);
		print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
		print_test("Prueba heap no hay tope", !heap_ver_max(heap));
		print_test("Prueba heap desencolar, es NULL", !heap_desencolar(heap));
}

static void prueba_heap_encolar()
{
    heap_t* heap = heap_crear(comparar_clave);

    char *clave1 = "perro";
    char *clave2 = "gato";
    char *clave3 = "vaca";


    print_test("Prueba heap insertar clave1", heap_encolar(heap, clave1));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap pertenece clave1, es true", heap_pertenece(heap, clave1));
    print_test("Prueba heap borrar clave1, es valor1", heap_borrar(heap, clave1) == valor1);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el heap) */
    print_test("Prueba heap insertar clave2", heap_encolar(heap, clave2, valor2));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap pertenece clave2, es true", heap_pertenece(heap, clave2));

    print_test("Prueba heap insertar clave3", heap_encolar(heap, clave3, valor3));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap pertenece clave3, es true", heap_pertenece(heap, clave3));

    heap_destruir(heap);
}

static void prueba_heap_reemplazar()
{
    heap_t* heap = heapheap_crear(comparar_clave);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba heap insertar clave1", heap_encolar(heap, clave1, valor1a));
    print_test("Prueba heap insertar clave2", heap_encolar(heap, clave2, valor2a));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

    print_test("Prueba heap insertar clave1 con otro valor", heap_encolar(heap, clave1, valor1b));
    print_test("Prueba heap insertar clave2 con otro valor", heap_encolar(heap, clave2, valor2b));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

    heap_destruir(heap);
}

static void prueba_heap_reemplazar_con_destruir()
{
    heap_t* heap = heap_crear(comparar_clave);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba heap insertar clave1", heap_encolar(heap, clave1, valor1a));
    print_test("Prueba heap insertar clave2", heap_encolar(heap, clave2, valor2a));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

    print_test("Prueba heap insertar clave1 con otro valor", heap_encolar(heap, clave1, valor1b));
    print_test("Prueba heap insertar clave2 con otro valor", heap_encolar(heap, clave2, valor2b));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

    /* Se destruye el heap (se debe liberar lo que quedó dentro) */
    heap_destruir(heap);
}

static void prueba_heap_borrar()
{
    heap_t* heap = heapheap_crear(comparar_clave);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba heap insertar clave1", heap_encolar(heap, clave1, valor1));
    print_test("Prueba heap insertar clave2", heap_encolar(heap, clave2, valor2));
    print_test("Prueba heap insertar clave3", heap_encolar(heap, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba heap pertenece clave3, es verdadero", heap_pertenece(heap, clave3));
    print_test("Prueba heap borrar clave3, es valor3", heap_borrar(heap, clave3) == valor3);
    print_test("Prueba heap borrar clave3, es NULL", !heap_borrar(heap, clave3));
    print_test("Prueba heap pertenece clave3, es falso", !heap_pertenece(heap, clave3));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

    print_test("Prueba heap pertenece clave1, es verdadero", heap_pertenece(heap, clave1));
    print_test("Prueba heap borrar clave1, es valor1", heap_borrar(heap, clave1) == valor1);
    print_test("Prueba heap borrar clave1, es NULL", !heap_borrar(heap, clave1));
    print_test("Prueba heap pertenece clave1, es falso", !heap_pertenece(heap, clave1));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);

    print_test("Prueba heap pertenece clave2, es verdadero", heap_pertenece(heap, clave2));
    print_test("Prueba heap borrar clave2, es valor2", heap_borrar(heap, clave2) == valor2);
    print_test("Prueba heap borrar clave2, es NULL", !heap_borrar(heap, clave2));
    print_test("Prueba heap pertenece clave2, es falso", !heap_pertenece(heap, clave2));
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap);
}

static void prueba_heap_clave_vacia()
{
    heap_t* heap = heapheap_crear(comparar_clave);

    char *clave = "", *valor = "";

    print_test("Prueba heap insertar clave vacia", heap_encolar(heap, clave, valor));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap pertenece clave vacia, es true", heap_pertenece(heap, clave));
    print_test("Prueba heap borrar clave vacia, es valor", heap_borrar(heap, clave) == valor);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap);
}

static void prueba_heap_valor_null()
{
    heap_t* heap = heapheap_crear(comparar_clave);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba heap insertar clave vacia valor NULL", heap_encolar(heap, clave, valor));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap pertenece clave vacia, es true", heap_pertenece(heap, clave));
    print_test("Prueba heap borrar clave vacia, es valor NULL", heap_borrar(heap, clave) == valor);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap);
}

static void prueba_heap_volumen(size_t largo, bool debug)
{
    heap_t* heap = heapheap_crear(comparar_clave);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el heap */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = heap_encolar(heap, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap almacenar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);


    if (debug) print_test("Prueba heap pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = heap_borrar(heap, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap borrar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    /* Destruye el heap y crea uno nuevo que sí libera */
    heap_destruir(heap);
    heap = heap_crear(comparar_clave);

    /* Inserta 'largo' parejas en el heap */
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = heap_encolar(heap, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    /* Destruye el heap - debería liberar los enteros */
    heap_destruir(heap);

}
/*
static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}
*/
/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_heap_alumno()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_reemplazar();
    prueba_heap_reemplazar_con_destruir();
    prueba_heap_borrar();
    prueba_heap_clave_vacia();
    prueba_heap_valor_null();
    prueba_heap_volumen(5000, true);
}

void pruebas_volumen_alumno(size_t largo)
{
    prueba_heap_volumen(largo, false);
}
