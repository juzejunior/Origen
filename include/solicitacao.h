#ifndef SOLICITACAO_H
#define SOLICITACAO_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "config.h"

typedef struct Solicitacao
{
	int id;
 	char nomeEmpresa[30];
 	char setorEmpresa[30];
 	char nomeSolicitante[30];
 	char contato[14];
 	char email[30];
 	char problema[500];
 	Data data; 
	char situacao;	
	int codigo_funcionario;
	char solucao[500];	
} Solicitacao;

/*Realiza uma nova solicitacao de atendimento*/
void formulario();
/*Retorna o total de solicitacoes*/
int pedido_id();
/*exibe ordenado por data as solicitações não atendidas*/
void exibirNaoAtendidas();

#endif
