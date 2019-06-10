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

bool cond_inic_heap(heap_t* heap){
	return !heap_cantidad(heap) && heap != NULL && heap_esta_vacio(heap);
}
int heap_strcmp(const void *a, const void* b){
	return strcmp(a,b);
}
int heap_numcmp(const void *x, const void* y){
	const int* num1 = x;
	const int* num2 = y;

	if (*num1 == *num2) return 0;

	return (*num1 > *num2) ? 1 : -1;
}

bool cond_inic_heap_arr(heap_t* heap_arr, size_t n){
	return heap_cantidad(heap_arr) == n && heap_arr != NULL;
}

static void pruebas_heap_vacio(){
	printf("INICIO PRUEBAS HEAP VACIO\n");

	heap_t* heap = heap_crear(heap_strcmp);
	print_test("El heap cumple las condiciones iniciales", cond_inic_heap(heap));
	print_test("Desencolar devuelve NULL", !heap_desencolar(heap));
	print_test("Ver maximo devuelve NULL", !heap_ver_max(heap));
	heap_destruir(heap, NULL);
	print_test("El heap se destruyo correctamente", true);

	void* arr_vacio[] = {NULL}; //Arreglo vacio
	heap_t* heap_arr = heap_crear_arr(arr_vacio, 0, heap_numcmp);
	print_test("El heap_arr cumple las condiciones iniciales", cond_inic_heap_arr(heap_arr, 0));
	print_test("Ver max de arreglo vacio es NULL", !heap_ver_max(heap_arr));
	print_test("Desencolar devuelve NULL", !heap_desencolar(heap_arr));
	heap_destruir(heap_arr, NULL);
	print_test("El heap_arr se destruyo correctamente", true);
}

static void pruebas_heap_encolar(){
	printf("\nINICIO PRUEBAS HEAP ENCOLAR\n");

	heap_t* heap = heap_crear(heap_strcmp);
	print_test("El heap cumple las condiciones iniciales", cond_inic_heap(heap));

	char *cad1 = "perro", *cad2 = "gato", *cad3 = "zorro";

	print_test("Heap encolar 'perro'", heap_encolar(heap, cad1));
	print_test("La cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Heap ver maximo es 'perro'", heap_ver_max(heap) == cad1);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));

	print_test("Heap encolar 'gato'", heap_encolar(heap, cad2));
	print_test("La cantidad de elementos es 2", heap_cantidad(heap) == 2);
	print_test("Heap ver maximo sigue siendo 'perro'", heap_ver_max(heap) == cad1);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));

	print_test("Heap encolar 'zorro'", heap_encolar(heap, cad3));
	print_test("La cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Heap ver maximo es 'zorro'", heap_ver_max(heap) == cad3);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap)); //hasta aca anda

	print_test("Heap desencolar devuelve 'zorro'", heap_desencolar(heap) == cad3);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Heap desencolar devuelve 'perro'", heap_desencolar(heap) == cad1);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Heap desencolar devuelve 'gato'", heap_desencolar(heap) == cad2);
	print_test("El heap esta vacio", heap_esta_vacio(heap));
	print_test("Heap desencolar devuelve NULL", !heap_desencolar(heap));
	heap_destruir(heap, NULL);
	print_test("El heap se destruyo correctamente", true);
}

/*static void prueba_heap_arr(){
	printf("INICIO DE PRUEBAS DE HEAP ARR\n");

	void* arr_vacio[] = {};
	heap_t* heap_arr = heap_crear_arr(arr_vacio, 0, heap_numcmp);
	print_test("El heap_arr cumple las condiciones iniciales", cond_inic_heap_arr(heap_arr, 0));

}*/

static void prueba_heap_destruir(){
	printf("\nINICIO PRUEBAS DE DESTRUCCION DEL HEAP\n");
	printf("\nPrueba 1: destruir pasandole parametro NULL\n");

	heap_t* heap = heap_crear(heap_strcmp);
	print_test("El heap cumple las condiciones iniciales", cond_inic_heap(heap));

	char *cad1 = "toro", *cad2 = "zorro", *cad3 = "rinoceronte";

	print_test("Heap encolar 'toro'", heap_encolar(heap, cad1));
	print_test("Heap encolar 'zorro'", heap_encolar(heap, cad2));
	print_test("Heap encolar 'rinoceronte'", heap_encolar(heap, cad3));
	print_test("La cantidad de elementos es 3 y el heap no esta vacio", !heap_esta_vacio(heap) && heap_cantidad(heap) == 3);
	print_test("El maximo es 'zorro'", heap_ver_max(heap) == cad2);
	heap_destruir(heap, NULL);
	print_test("Se destruyo correctamente el heap con el parametro NULL", true);

	printf("\nPrueba 2: destruir pasandole un parametro distinto de NULL\n");

	heap_t* heap_2 = heap_crear(heap_numcmp); //no se si esta funciona para este caso DEBUG
	print_test("El heap cumple las condiciones iniciales", cond_inic_heap(heap_2));

	int *num_1 = malloc(sizeof(int));
	int *num_2 = malloc(sizeof(int));
	int *num_3 = malloc(sizeof(int));
	int *num_4 = malloc(sizeof(int));

	*num_1 = 314;
	*num_2 = 73;
	*num_3 = 69;
	*num_4 = 32;

	print_test("Heap encolar num_1", heap_encolar(heap_2, num_1));
	print_test("Heap encolar num_2", heap_encolar(heap_2, num_2));
	print_test("Heap encolar num_3", heap_encolar(heap_2, num_3));
	print_test("Heap encolar num_4", heap_encolar(heap_2, num_4));
	print_test("La cantidad de elementos es 4 y el heap no esta vacio", !heap_esta_vacio(heap_2) && heap_cantidad(heap_2) == 4);
	print_test("El maximo es num_1", *(int*)heap_ver_max(heap_2) == *num_1);
	heap_destruir(heap_2, free);
	print_test("Se destruyo correctamente el heap con el parametro free", true);
}

/*static void prueba_heapsort(){
	printf("INICIO PRUEBA HEAPSORT\n");
}*/



static void prueba_heap_volumen(size_t largo)
{
    printf("INICIO PRUEBAS HEAP VOLÚMEN\n");
    heap_t* heap = heap_crear(heap_strcmp);

    const size_t largo_clave = 10;
    char** claves = malloc(sizeof(char*) * largo);

    // Inserta 'largo' parejas en el abb
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        claves[i] = malloc(sizeof(char) * largo_clave);
        sprintf(claves[i], "%08d", i);
    }
    //Encolo las claves
    for (unsigned i = 0; i < largo; i++){
        ok = heap_encolar(heap, claves[i]);
        if (!ok){
        	print_test("Falla encolando muchos elementos", ok);
        	break;
        }
    }

    print_test("Prueba heap almacenar muchos elementos", ok);
    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);
    print_test("Prueba heap ver maximo es corecta", heap_ver_max(heap) == claves[largo - 1]);

    // Verifica que desencole en el orden correcto
    for (size_t i = 0, j = largo - 1; i < largo; i++, j--) {
        char* desencolado = heap_desencolar(heap);
        ok = desencolado == claves[j]; //quizas falla
        if (!ok){
          printf("Valor esperado : %s\n", claves[j]);
          printf("Valor obtenido : %s\n", desencolado);
          break;
        };
    }

    print_test("Prueba heap desecolar muchos elementos", ok);
    printf("La cantidad de elementos es supuestamente %ld\n", heap_cantidad(heap));
    print_test("Prueba heap la cantidad de elementos es 0", !heap_cantidad(heap));
    print_test("Ver max devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("Heap desencolar devuelve NULL", heap_desencolar(heap) == NULL);


    // Destruye el abb y crea uno nuevo que sí libera
    heap_destruir(heap, NULL);
    //Libero la memoria pedida para las claves y valores
    for (size_t i = 0; i < largo; i ++) free(claves[i]);
    free(claves);
}

void pruebas_heap_alumno()
{
    /* Ejecuta todas las pruebas unitarias. */
    pruebas_heap_vacio();
    pruebas_heap_encolar();
    prueba_heap_destruir();
    prueba_heap_volumen(1000);
    //prueba_heapsort();
}
