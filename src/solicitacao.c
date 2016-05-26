#include "../include/solicitacao.h"

void formulario()
{
	time_t t = time(NULL);//uteis para datas e hora
	struct tm tm = *localtime(&t);
	Solicitacao s;
	FILE *file = fopen("../../database/solicitacao.bin","ab");
	char opcao;
	
	if(file != NULL)
	{
		do{
		  printf(" Origen Empresas - Atendimento\n\n");
		  printf(" Empresa: ");
		  scanf(" %[^\n]s", s.nomeEmpresa);
		  converte_maisculo(s.nomeEmpresa);
		  printf(" Setor: ");
		  scanf(" %[^\n]s", s.setorEmpresa);
		  printf(" Solicitante: ");
		  scanf(" %[^\n]s", s.nomeSolicitante);
		  printf(" Contato: ");
		  scanf(" %[^\n]s", s.contato);
		  printf(" E-mail: ");
		  scanf(" %[^\n]s", s.email);
		  printf(" Informe o problema: ");
		  scanf(" %[^\n]s", s.problema);
		  strcpy(s.solucao," Aguardando atendimento!");
		  s.situacao = 'A';
		  s.data.ano = tm.tm_year + 1900;
		  s.data.mes = tm.tm_mon + 1;
		  s.data.dia = tm.tm_mday;
		  s.data.hora = tm.tm_hour;
		  s.data.minuto = tm.tm_min;
		  s.data.segundos = tm.tm_sec;
		  s.id = pedido_id()+1;
		  //gravando solicitacao
		  if(fwrite(&s,sizeof(Solicitacao),1, file) != 0) printf(" Solicitação realizada com sucesso!\n\n");
		  else printf(" Não foi possível enviar a solicitação!\n\n");
		
		  printf(" Nova solicitação? (s/n): ");
		  scanf(" %c", &opcao);
			
		}while(opcao != 'n');	
	}	
}

int pedido_id()
{
	FILE *file = fopen("../../database/solicitacao.bin","rb");
	Solicitacao s;
	int contador = 0;
	
	while(fread(&s,sizeof(Solicitacao),1, file) == 1) contador++;
	
	return contador;
}

void exibirNaoAtendidas()
{
	char opcao;
	FILE *file = fopen("../../database/solicitacao.bin","rb");
	Solicitacao s;
	Solicitacao *v;
	int n = 5;
	int i = 0;
    int k = 0;
	//iniciando o vetor com 5 espaços
	v = (Solicitacao*) malloc(n*sizeof(Solicitacao));
	
	limparTela();
	printf("         Solicitações não atendidas\n\n");

	if(file != NULL)
	{
		while(fread(&s,sizeof(Solicitacao),1, file) == 1) {
			if(s.situacao == 'A')
			{
				if(i % n == 0 && i != 0) v = (Solicitacao*) realloc(v,i+n *(sizeof(Solicitacao)));
				v[i].id = s.id;
				strcpy(v[i].nomeEmpresa, s.nomeEmpresa);
				strcpy(v[i].nomeSolicitante, s.nomeSolicitante);
				strcpy(v[i].email, s.email);
				strcpy(v[i].contato, s.contato);
				strcpy(v[i].problema, s.problema);
				strcpy(v[i].setorEmpresa, s.setorEmpresa);
				strcpy(v[i].solucao, s.solucao);
				v[i].situacao = s.situacao;
				v[i].data.dia = s.data.dia;
				v[i].data.mes = s.data.mes;
				v[i].data.ano = s.data.ano;
				v[i].data.hora = s.data.hora;
				v[i].data.minuto = s.data.minuto;
				v[i].data.segundos = s.data.segundos;
				i++;
			}
		}	
		
		insertSortAno(v, i+1);	
		for(k = 1; k < i+1; k++) printf(" %s   %d\n", v[k].nomeEmpresa,v[k].data.ano);
	}
	else
	{	
		printf(" Arquivo não encontrado!\n\n");	
	}
}

void atendimentosPendentes()
{
	FILE *file = fopen("../../database/solicitacao.bin","rb");
	int codigo_funcionario;
	char escolha;
	Solicitacao s;

	do{
	  limparTela();
	  printf("         Atendimentos Pendentes\n\n");
	  printf(" Informe o seu código: ");
	  scanf("%d", &codigo_funcionario);	
	  
	  if(file != NULL)
	  {
		while(fread(&s,sizeof(Solicitacao),1, file) == 1)
		{
			if(s.codigo_funcionario == codigo_funcionario && s.situacao == 'E')
			{
				 printf("  Empresa: %s\n Problema: %s\n\n");
			}
		}
	  }
	}while(escolha != 'n');
}



void insertSortAno(Solicitacao *s, int n)
{
	int i, j;
	Solicitacao aux;
	for(i = 1; i < n; i++)
	{
		aux = s[i];
		for(j=i; (j>0) && (aux.data.segundos < s[j-1].data.segundos); j--)
			s[j] = s[j-1];
		s[j] = aux;
	}
}


