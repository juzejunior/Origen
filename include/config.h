#include<time.h>
#include<ctype.h>
#include<stdlib.h>

typedef struct Data
{
  int dia;
  int mes;
  int ano;
  int hora;
  int minuto;
  int segundos;
  	
} Data;

void converte_maisculo(char *palavra);
void limparTela();
int checkSO();
void limparTela();
char* now();