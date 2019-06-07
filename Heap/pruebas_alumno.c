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

typedef int (*cmp_func_t) (const void *a, const void *b);
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

	void* arr_vacio[] = {}; //Arreglo vacio
	heap_t* heap_arr = heap_crear_arr(arr_vacio, 0, heap_numcmp);
	print_test("El heap_arr cumple las condiciones iniciales", cond_inic_heap_arr(heap_arr, 0));
	print_test("Ver max de arreglo vacio es NULL", !heap_ver_max(heap_arr));
	print_test("Desencolar devuelve NULL", !heap_desencolar(heap_arr));
	heap_destruir(heap_arr, NULL);
	print_test("El heap_arr se destruyo correctamente", true);
}

static void pruebas_heap_encolar(){
	printf("INICIO PRUEBAS HEAP ENCOLAR\n");

	heap_t* heap = heap_crear(heap_strcmp);
	print_test("El heap cumple las condiciones iniciales", cond_inic_heap(heap));

	char* cad1 = "perro", cad2 = "gato", cad3 = "zorro";
	print_test("Heap encolar 'perro'", heap_encolar(heap, cad1));
	print_test("La cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Heap ver maximo es 'perro'", heap_ver_max(heap) == cad1);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));

	print_test("Heap encolar 'gato'", heap_encolar(heap, cad2));
	print_test("La cantidad de elementos es 1", heap_cantidad(heap) == 2);
	print_test("Heap ver maximo sigue siendo 'perro'", heap_ver_max(heap) == cad1);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));

	print_test("Heap encolar 'zorro'", heap_encolar(heap, cad3));
	print_test("La cantidad de elementos es 1", heap_cantidad(heap) == 3);
	print_test("Heap ver maximo es 'zorro'", heap_ver_max(heap) == cad3);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));

	print_test("Heap desencolar devuelve 'zorro'", heap_desencolar(heap) == cad3);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Heap desencolar devuelve 'perro'", heap_desencolar(heap) == cad1);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Heap desencolar devuelve 'gato'", heap_desencolar(heap) == cad2);
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Heap desencolar devuelve NULL", !heap_desencolar(heap));
	print_test("El heap esta vacio", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
	print_test("El heap se destruyo correctamente", true);
}

static void prueba_heap_arr(){
	printf("INICIO DE PRUEBAS DE HEAP ARR\n");

	void* arr_vacio[] = {};
	heap_t* heap_arr = heap_crear_arr(arr_vacio, 0, heap_numcmp);
	print_test("El heap_arr cumple las condiciones iniciales", cond_inic_heap_arr(heap_arr, 0));

}

static void prueba_heap_destruir(){
	printf("INICIO PRUEBAS DE DESTRUCCION DEL HEAP\n");
	printf("\nPrueba 1: destruir pasandole parametro NULL\n");

	heap_t* heap = heap_crear(heap_strcmp);
	print_test("El heap cumple las condiciones iniciales", cond_inic_heap(heap));

	char* cad1 = "toro", cad2 = "zorro", cad3 = "rinoceronte";

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

static void prueba_heapsort(){
	printf("INICIO PRUEBA HEAPSORT\n");

}

static void prueba_volumen(size_t volumen){
	
}
