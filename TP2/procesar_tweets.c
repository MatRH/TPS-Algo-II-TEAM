/*
Alumnos: Ruiz Huidobro, Matias; Torresetti, Lisandro
Padrones: 102251, 99846
Numero de grupo: 23
Corrector: Secchi, Ana
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
      fprintf( stderr, "Error: Cantidad erronea de parametros");
      return 0;
    }
    FILE* input;
    input = stdin;
    if(!input){
      fprintf( stderr, "Error: archivo fuente inaccesible");
      return 0;
    }
    /*El programa deberá imprimir por salida estándar (stdout) el histórico
     de los k TTs aproximados cada n lineas*/
    int k = argv[1];
    int n = argv[2]; //hay que chequear que sean numeros validos

    return 0;
  }

