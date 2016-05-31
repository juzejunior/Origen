#include "../include/helpdesk.h"
#include "../src/config.c"


void menuHelpDesk()
{
	int opcao;
	
	do{
	  limparTela();
		printf("\n             Origen - HelpDesk          %s\n\n", now());
		printf(" 1 - Solicitações não atendidas | 2 - Em andamento | 3 - Sair\n\n");
		printf(" Opção: ");
		scanf("%d", &opcao);
		switch(opcao)
		{
			case 1:
				exibirNaoAtendidas();
			break;
			case 2:
				atendimentosPendentes();
			break;
			case 3:
				limparTela();
				printf(" Volte sempre!\n");
			break;
		}
	}while(opcao != 1 && opcao != 2 && opcao != 3);
}

void exibirNaoAtendidas()
{
	int opcao;
	FILE *file = fopen("../database/solicitacao.bin","rb");
	Solicitacao s;
	int i = 0;
	
	limparTela();
	printf("\n             Origen - HelpDesk          %s\n\n", now());
	printf("         Solicitações não atendidas\n\n");

	if(file != NULL)
	{
		while(fread(&s,sizeof(Solicitacao),1, file) == 1) {
			if(s.situacao == 'A' && i < 10)
			{
				printf(" Empresa: %s              Em: %d/%d/%d\n",  s.nomeEmpresa, s.data.dia, s.data.mes, s.data.ano);	
				printf(" Setor: %s\n", s.setorEmpresa);
				printf(" Solicitante: %s\n", s.nomeSolicitante);
				printf(" Contato: %s\n", s.contato);
				printf(" Email: %s\n", s.email);
				printf(" Cod: %d       Situação: %c\n", s.id, s.situacao);
				printf(" Problema: %s\n\n", s.problema);
				printf(" ------------------------------------------------\n\n");	
				i++;
			}
		}
		//se nao houver novas solicitacoes
		if(i == 0)
		{
			printf(" Não há novas solicitações.\n");
			printf(" 1 - Voltar\n\n Opção: ");
			scanf("%d", &opcao);
		}else{
			printf(" 1 - Resolver problema | 2 - Voltar\n\n Opção: ");
			scanf("%d", &opcao);
		}
		limparTela();
		
		if(opcao == 1 && i != 0) resolverUmProblema();
		else menuHelpDesk();
		fclose(file);
	}else{
		printf(" Arquivo não encontrado. Entre em contato com os administradores do sistema :)\n");
	}
}

void resolverUmProblema()
{
	 char escolha;
	 int opcao;
	 int codigo_funcionario;
	 int codigo_solicitacao;
	 Solicitacao s;
	 
	 FILE *file = fopen("../database/solicitacao.bin","rb+");
	 
	 if(file != NULL)
	 {
		limparTela(); 
		printf("\n             Origen - HelpDesk          %s\n\n", now());
		printf(" Informe o código da solicitação: ");
		scanf("%d", &codigo_solicitacao);
		printf(" Informe o seu código de funcionário: ");
		scanf("%d", &codigo_funcionario);
		
		if(fseek(file, sizeof(Solicitacao)*(codigo_solicitacao-1), SEEK_SET) == 0)
		{
			fread(&s,sizeof(Solicitacao),1, file);
			if(s.situacao == 'A' && s.id == codigo_solicitacao)
			{
				fseek(file, sizeof(Solicitacao)*(codigo_solicitacao-1), SEEK_SET);
				s.situacao = 'E';
				s.codigo_funcionario = codigo_funcionario;
				printf(" Empresa: %s\n", s.nomeEmpresa);
				printf(" Solicitante: %s\n", s.nomeSolicitante);
				printf(" Setor: %s\n", s.setorEmpresa);
				printf(" E-mail: %s\n", s.email);
				printf(" Contato: %s\n", s.contato);
				printf(" Problema: %s\n\n", s.problema);
				strcpy(s.solucao," Em andamento");
				printf(" Problema resolvido?(s/n): ");
				scanf(" %c", &escolha);
				if(escolha == 's')
				{
				  printf(" Qual a solução aplicada para o problema: ");
				  scanf(" %[^\n]s", s.solucao);
				  s.situacao = 'F';	
				  printf(" Solucionado com sucesso :)\n\n");
				}
				fwrite(&s,sizeof(Solicitacao),1,file);
			}else{
				printf(" Esta solicitação já foi solucionada ou está em andamento!\n\n");
			}
		}else{
			printf(" Código de solicitação inválido :(\n\n");
		}
		fclose(file);
	 }else{
		printf(" Arquivo não encontrado. Entre em contato com os administradores do sistema :)\n");
	}
	printf(" 1-Voltar\n Opção: ");
	scanf("%d", &opcao);
	menuHelpDesk();
}

void atendimentosPendentes()
{
	FILE *file = fopen("../database/solicitacao.bin","rb");
	int codigo_funcionario;
	int opcao;
	Solicitacao s;
	int i = 0;

	limparTela();
	printf("\n             Origen - HelpDesk          %s\n\n", now());
	printf("         Atendimentos Pendentes\n\n");
	printf(" Informe o seu código: ");
	scanf("%d", &codigo_funcionario);	
	printf("\n");
	if(file != NULL)
	{
		while(fread(&s,sizeof(Solicitacao),1, file) == 1)
		{
			if(s.codigo_funcionario == codigo_funcionario && s.situacao == 'E')
			{
				printf(" Empresa: %s              Em: %d/%d/%d\n",  s.nomeEmpresa,s.data.dia, s.data.mes, s.data.ano);	
				printf(" Setor: %s\n", s.setorEmpresa);
				printf(" Solicitante: %s\n", s.nomeSolicitante);
				printf(" Contato: %s\n", s.contato);
				printf(" Email: %s\n", s.email);
				printf(" Cod: %d       Situação: %c\n", s.id, s.situacao);
				printf(" Problema: %s\n\n", s.problema);
				printf(" ------------------------------------------------\n\n");
				i++;
			}
		}
		//se nao houver atendimentos pendentes
		if(i == 0)
		{
			printf(" Não há atendimentos pendentes.\n");
			printf(" 1 - Voltar\n\n Opção: ");	
		}else{
			printf(" 1 - Resolver problema | 2 - Voltar\n\n Opção: ");
		}
	    scanf("%d", &opcao);
	    if(opcao == 1 && i != 0) resolverPendente(codigo_funcionario); 
		else menuHelpDesk();
		fclose(file);
	}else{
		printf(" Arquivo não encontrado. Entre em contato com os administradores do sistema :)\n");
	}		
}

/*resolve os problemas pendentes*/
void resolverPendente(int codigo_funcionario)
{
	FILE *file = fopen("../database/solicitacao.bin","rb+");
	Solicitacao s;
	int codigo_solicitacao;
	int opcao;
	
	if(file != NULL)
	{
		limparTela(); 
		printf("\n             Origen - HelpDesk          %s\n\n", now());
		printf(" Informe o código do problema: ");
		scanf("%d", &codigo_solicitacao);
		
		if(fseek(file, sizeof(Solicitacao)*(codigo_solicitacao-1), SEEK_SET) == 0)
		{
			fread(&s,sizeof(Solicitacao),1, file);
			if(s.id == codigo_solicitacao && s.codigo_funcionario == codigo_funcionario && s.situacao != 'F')
			{
				fseek(file, sizeof(Solicitacao)*(codigo_solicitacao-1), SEEK_SET);
				s.situacao = 'F';
				printf(" Qual a solução aplicada para o problema: ");
				scanf(" %[^\n]s", s.solucao);	
				printf(" Solucionado com sucesso :)\n\n");
				fwrite(&s,sizeof(Solicitacao),1,file);
			}else{
				printf(" Esta solicitação está em nome de outro helpdesk ou já foi finalizada.\n\n");
			}
		}else{
			printf(" Código da solicitação não encontrado :(\n\n");
		}
		fclose(file);
	}else{
		printf(" Arquivo não encontrado. Entre em contato com os administradores do sistema :)\n");
	}
	printf(" 1-Voltar\n\n Opção: ");
	scanf("%d", &opcao);
	menuHelpDesk();
}

void exibirSolucionadas()
{
	FILE *file = fopen("../database/solicitacao.bin","rb");
	Solicitacao s;
	
	while(fread(&s,sizeof(Solicitacao),1, file) == 1)
	{
		if(s.situacao == 'F')
		{
			printf(" Empresa: %s              Em: %d/%d/%d\n",  s.nomeEmpresa,s.data.dia, s.data.mes, s.data.ano);	
			printf(" Setor: %s\n", s.setorEmpresa);
			printf(" Solicitante: %s\n", s.nomeSolicitante);
			printf(" Contato: %s\n", s.contato);
			printf(" Email: %s\n", s.email);
			printf(" Cod: %d       Situação: %c\n", s.id, s.situacao);
			printf(" Problema: %s\n", s.problema);
			printf(" Solução: %s\n", s.solucao);
			printf(" Funcionario: %d\n\n", s.codigo_funcionario);
			printf(" ------------------------------------------------\n\n");		
		}
	}
}

