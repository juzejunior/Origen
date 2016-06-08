#include "../include/config.h"


void converte_maisculo(char *palavra)
{
	while(*palavra)	
	{
		*palavra = toupper(*palavra);
		palavra++;
	}		
}

/*utilizada para encriptar a senha do usuario*/
char *encrypt(char *senha)
{
	while(*senha)
	{
		*senha = *senha ^ 31;
		senha++;
	}
	
	return senha;
}

int checkSO()
{
	#ifdef __unix__
		return 0;
	#elif defined(_WIN32) || defined(WIN32)
		return 1;
	#endif
}

void limparTela()
{
	int SO;
	SO = checkSO();
	if (SO==0) system ("clear");
	else system ("cls");
	return;
}
///Retorna data e hora atual
char* now()
{
	time_t t;
	time(&t);
	return asctime(localtime(&t));
}

int criarBloqueio()
{
	FILE *file_block = fopen("../block.txt","r");
	
	/*se o arquivo de solicitação estiver liberado para uso*/
	if(file_block == NULL) {
		file_block = fopen("../block.txt","w");
		return 1;
	}
		
	return 0;
}

int removerBloqueio()
{
	FILE *file_block = fopen("../block.txt","r");
	
	if(file_block != NULL){
		remove("../block.txt");
		return 1;
	}

	return 0;
}

void pause()
{
	if(checkSO())
		system("pause");
	else
		system("read -p \"	Pressione <ENTER> para continuar\" saindo");
}



