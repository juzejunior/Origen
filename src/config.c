

void converte_maisculo(char *palavra)
{
	while(*palavra)	
	{
		*palavra = toupper(*palavra);
		palavra++;
	}		
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
/*Retorna data e hora atual*/
char* now()
{
	time_t t;
	time(&t);
	return asctime(localtime(&t));
}


