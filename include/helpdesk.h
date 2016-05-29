#ifndef HELPDESK_H
#define HELPDESK_H

#include "solicitacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void menuHelpDesk();
void exibirNaoAtendidas();
void atendimentosPendentes();
void resolverUmProblema();
void resolverPendente(int codigo_funcionario);
void finalizacaoDoProblema(int id);
void insertSortData(Solicitacao *s, int n);

#endif
