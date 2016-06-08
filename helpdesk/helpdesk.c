#include "../include/helpdesk.h"
#include "../src/config.c"


void menuHelpDesk()
{
	int opcao;
	
	do{
	  limparTela();
		printf("\n             Origen - HelpDesk          %s\n\n", now());
		printf(" 1 - Não atendidas | 2 - Em andamento | 3 - Transferir | 4 - Finalizadas | 5 - Sair\n\n");
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
				trocarSolicitacao();
			break;
			case 4:
				exibirSolucionadas();
			break;
			case 5:
				limparTela();
				printf(" Até mais ;)\n");
			break;
		}
	}while(opcao < 1 || opcao > 5);
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
		if(criarBloqueio())
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
			fclose(file);
			removerBloqueio();
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
			
		}else{
			printf(" Arquivo indisponível no momento. Aguarde 1 minuto :)\n\n");
			fclose(file);
			pause();
			menuHelpDesk();
		}	
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
	 
	 limparTela(); 
	 printf("\n             Origen - HelpDesk          %s\n\n", now());
	 
	 if(file != NULL)
	 { 
		if(criarBloqueio()) 
		{ 
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
			removerBloqueio();
		}else{
			printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
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
	
	if(criarBloqueio()){
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
			removerBloqueio();
			if(opcao == 1 && i != 0) resolverPendente(codigo_funcionario); 
			else menuHelpDesk();
			fclose(file);
		}else{
			printf(" Arquivo não encontrado. Entre em contato com os administradores do sistema :)\n");
		}
	}else{
		printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
		pause();
		menuHelpDesk();
	}			
}

/*resolve os problemas pendentes*/
void resolverPendente(int codigo_funcionario)
{
	FILE *file = fopen("../database/solicitacao.bin","rb+");
	Solicitacao s;
	int codigo_solicitacao;
	int opcao;
	
	if(criarBloqueio())
	{
		if(file != NULL)
		{
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
		removerBloqueio();
	}else{
		printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
	}	
	
	printf(" 1-Voltar\n\n Opção: ");
	scanf("%d", &opcao);
	menuHelpDesk();
}

void exibirSolucionadas()
{
	FILE *file = fopen("../database/solicitacao.bin","rb");
	Solicitacao s;
	int codigoFuncionario, total;
	total = 0;

	limparTela(); 
	printf("\n             Origen - HelpDesk          %s\n\n", now());
	
	if(criarBloqueio())
	{
		if(file != NULL)
		{
			printf(" Informe o seu código de funcionário: ");
			scanf("%d", &codigoFuncionario);
			
			while(fread(&s,sizeof(Solicitacao),1, file) == 1)
			{
				if(s.situacao == 'F' && s.codigo_funcionario == codigoFuncionario)
				{
					printf("\n Empresa: %s              Em: %d/%d/%d\n",  s.nomeEmpresa,s.data.dia, s.data.mes, s.data.ano);	
					printf(" Setor: %s\n", s.setorEmpresa);
					printf(" Solicitante: %s\n", s.nomeSolicitante);
					printf(" Contato: %s\n", s.contato);
					printf(" Email: %s\n", s.email);
					printf(" Cod: %d       Situação: %c\n", s.id, s.situacao);
					printf(" Problema: %s\n", s.problema);
					printf(" Solução: %s\n", s.solucao);
					printf(" Funcionario: %d\n\n", s.codigo_funcionario);
					printf(" ------------------------------------------------\n");	
					total++;	
				}
			}
			fclose(file);
			removerBloqueio();
			printf("\n %d resultado(s) encontrado(s).\n\n", total);
		}else{
			printf(" Arquivo indisponível. Entre em contato com os administradores do sistema :(\n\n");	
		}
	}else{
		printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
	}
	pause();
	menuHelpDesk();
}

/*trocar solicitacao entre helpdesks*/
void trocarSolicitacao()
{
	FILE *file = fopen("../database/solicitacao.bin","rb+");
	Solicitacao s;
	int opcao, id_atual, id_proximo, cod_solicitacao;
	
	limparTela(); 
	printf("\n             Origen - HelpDesk          %s\n\n", now());
	
	if(criarBloqueio())
	{
		if(file != NULL)
		{
			printf(" Informe o seu código de helpdesk: ");
			scanf("%d", &id_atual);
			printf(" Informe o código da solicitação: ");
			scanf("%d", &cod_solicitacao);
			
			if(fseek(file, sizeof(Solicitacao)*(cod_solicitacao-1), SEEK_SET) == 0)
			{
				fread(&s,sizeof(Solicitacao),1, file);
				if(s.id == cod_solicitacao && s.situacao == 'E' && s.codigo_funcionario == id_atual)
				{
					fseek(file, sizeof(Solicitacao)*(cod_solicitacao-1), SEEK_SET);
					printf(" Informe o código do novo encarregado: ");
					scanf("%d", &id_proximo);
					s.codigo_funcionario = id_proximo;
					printf(" Transferência realizada com sucesso :)\n\n");
					fwrite(&s,sizeof(Solicitacao),1,file);
				}else{
					printf(" Esta solicitação está em nome de outro helpdesk ou já foi finalizada.\n\n");
				}
			}else{
				printf(" Solicitação inexistente :(\n\n");
			}
			fclose(file);
			removerBloqueio();	
		}else{
			printf(" Arquivo não encontrado. Entre em contato com os administradores do sistema :)\n");
		}
	}else{
		printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
	} 	
	printf(" 1-Voltar\n\n Opção: ");
	scanf("%d", &opcao);
	menuHelpDesk();
}

