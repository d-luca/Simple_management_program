/**
 * @file
 * @author Davi' Luca
 * @details
 * Questo file contiene solamente il metodo main nel quale vengono richiamati i due metodi principali per la navigazione
 */
#include "head_menu.h"

int main (void){
	sessione_t *se=(sessione_t *) malloc(sizeof(sessione_t));
	initialize_session(se);

	show_nolog_menu();

	while(se->scelta!=-1){
		printf("\nDigita un numerro per scegliere e premi invio: ");
		se->scelta=choise(se->menu);
		se->scelta=menu_nextmove(se);
	}
	free(se); se=NULL;
	exit(0);
}

