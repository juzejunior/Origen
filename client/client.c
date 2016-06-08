#include "../include/client.h"
#include "../src/config.c"

void menuClient()
{
	int opcao;
	
	do{
	  limparTela();
	  printf("\n             Origen - Empresas         %s\n\n", now());
	  printf(" 1-Nova solicitação | 2-Solicitações em andamento | 3-Finalizadas | 4-Sair\n\n");
	  printf(" Opção: ");
	  scanf("%d", &opcao);
	  switch(opcao)
	  {
		case 1:
			formulario();
		break;
		case 2:
			solicitacoesAndamento();
		break;
		case 3:
			solicitacoesFinalizadas();
		break;
		case 4:
			limparTela();
			printf(" Até logo ;)\n");
		break;
	  }	
    } while(opcao < 1 || opcao > 4);
}

void formulario()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	Solicitacao s;
	char opcao;
	FILE *file = fopen("../database/solicitacao.bin","ab");
	limparTela();

	printf("\n             Origen - Empresas         %s\n\n", now());

	if(file != NULL)
	{
		if(criarBloqueio()){
			do{
				file = fopen("../database/solicitacao.bin","ab");	
				printf("\n             Origen - Empresas         %s\n\n", now());
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
				fclose(file);	
			}while(opcao != 'n');

			removerBloqueio();
		}else{
			printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
			fclose(file);
		}
		pause();	
		menuClient();
	}else{
		printf(" Arquivo não encontrado :(\n\n");
	}
}

int pedido_id()
{
	FILE *file = fopen("../database/solicitacao.bin","rb");
	Solicitacao s;
	int contador = 0;
	
	if(fread(&s, sizeof(Solicitacao),1, file) == 1)
	{
		 //se houver arquivos
		 fseek(file,sizeof(Solicitacao), SEEK_END);
		 fread(&s,sizeof(Solicitacao),1,file);
		 return contador = s.id;
	}
	
	fclose(file);
	return contador;
}

void solicitacoesAndamento()
{
	char nomeEmpresa[30];
	char escolha;
	FILE *file = fopen("../database/solicitacao.bin","rb");
	Solicitacao s;
	int i = 0;
	
	limparTela();
	printf("\n             Origen - Empresas         %s\n\n", now());
	
	if(file != NULL)
	{
		if(criarBloqueio()){
			printf(" Empresa: ");
			scanf(" %[^\n]s", nomeEmpresa);	
			converte_maisculo(nomeEmpresa);
			printf("\n");
			while(fread(&s,sizeof(Solicitacao),1, file) == 1)
			{
				if(strcmp(s.nomeEmpresa, nomeEmpresa) == 0 && (s.situacao == 'E' || s.situacao == 'A'))
				{
					printf(" Empresa: %s              Em: %d/%d/%d\n",  s.nomeEmpresa,s.data.dia, s.data.mes, s.data.ano);	
					printf(" Setor: %s\n", s.setorEmpresa);
					printf(" Solicitante: %s\n", s.nomeSolicitante);
					switch(s.situacao)
					{
						case 'A': 	printf(" Cod: %d       Situação: Aguardando atendimento\n", s.id);
						break;
						case 'E': printf(" Cod: %d       Situação: Em atendimento\n", s.id);
						break;
					}
			
					printf(" Problema: %s\n\n", s.problema);
					printf(" ------------------------------------------------\n\n");
					i++;
				}
			}
			if(i == 0) printf(" Não há solicitações em andamento!\n\n");
			removerBloqueio();
		}else{
			printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
		}
		fclose(file);	
	}else{
		printf(" Arquivo indisponível :(\n\n");
	}
	
	printf(" Deseja retornar(s/n): ");
	scanf(" %c", &escolha);
	if(escolha == 's') menuClient();
}

void solicitacoesFinalizadas()
{
	char nomeEmpresa[30];
	char escolha;
	FILE *file = fopen("../database/solicitacao.bin","rb");
	Solicitacao s;
	int i = 0;
	
	limparTela();
	printf("\n             Origen - Empresas         %s\n\n", now());
	
	if(file != NULL)
	{
		if(criarBloqueio()){
			printf(" Empresa: ");
			scanf(" %[^\n]s", nomeEmpresa);	
			converte_maisculo(nomeEmpresa);
			printf("\n");
			while(fread(&s,sizeof(Solicitacao),1, file) == 1)
			{
				if(strcmp(s.nomeEmpresa, nomeEmpresa) == 0 && s.situacao == 'F')
				{
					printf(" Empresa: %s              Em: %d/%d/%d\n",  s.nomeEmpresa,s.data.dia, s.data.mes, s.data.ano);	
					printf(" Setor: %s\n", s.setorEmpresa);
					printf(" Solicitante: %s\n", s.nomeSolicitante);
					printf(" Contato: %s\n", s.contato);
					printf(" Email: %s\n", s.email);
					printf(" Cod: %d       Situação: Problema resolvido\n", s.id);
					printf(" Problema: %s\n", s.problema);
					printf(" Solução aplicada: %s\n\n", s.solucao);
					printf(" ------------------------------------------------\n\n");
					i++;
				}
			}
	
			if(i == 0) printf(" Não há solicitações finalizadas!\n\n");
			removerBloqueio();
		}else{
			printf(" Arquivo em uso no momento. Aguarde 1 minuto :)\n\n");
		}
		fclose(file);
	}else{
		printf(" Arquivo indisponível :(\n\n");
	}
	
	printf(" Deseja retornar(s/n): ");
	scanf(" %c", &escolha);
	if(escolha == 's') menuClient();
}



