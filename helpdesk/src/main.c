#include "../../src/solicitacao.c"
#include "../../src/config.c"



int main()
{
	int escolha;
	
	printf("1 - Não atendidas | 2 - Atendimentos pendentes | 3-Sair\n\n");
	printf(" Opção: ");
	scanf("%d",&escolha);
	
	switch(escolha)
	{
		case 1:
			exibirNaoAtendidas();
		break;
		case 2:
			atendimentosPendentes();
		break;	
		case 3:
			printf(" xau xau!\n");
		break;
	}
	
	return 0;
}
