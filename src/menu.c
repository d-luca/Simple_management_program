/**
 * @file
 * Il file contiene metodi per i menu' come visualizzazione e inserimento scelta.
 */
#include "head_menu.h"


#define CHIUDI -1 ///<Definisce il valore che verra' considerato come quello per uscire.

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' nolog.
 */
void show_nolog_menu(){
	system("clear");
	printf("1) Leggi sezione Novita'\n2) Controlla prodotti in magazzino\n3) Accedi (solo dipendenti)\n4) Chiudi programma\n");
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' log.
 */
void show_log_menu(){
	system("clear");
	printf("1) Novita'\n2) Prodotti in magazzino\n3) Clienti\n4) Dipendenti\n5) LogOut\n6) Chiudi programma\n");;
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' principale novita'.
 */
void show_novita_menu(){
	system("clear");
	printf("1) Visualizza messaggio Novita' attuale\n2) Cambia messaggio Novita'\n3) Torna al menu' principale\n");
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' principale prodotti.
 */
void show_prodotti_menu(){
	system("clear");
	printf("1) Visualizza record prodotti\n2) Aggiungi record prodotto\n3) Modifica record prodotto\n4) Elimina record prodotto\n5) Aggiunta incrementale (vendita o aggiunta)\n6) Torna al menu' principale\n");
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' principale clienti.
 */
void show_clienti_menu(){
	system("clear");
	printf("1) Visualizza record clienti\n2) Aggiungi record clienti\n3) Modifica record clienti\n4) Elimina record clienti\n5) Torna al menu' principale\n");
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' principale dipendenti.
 */
void show_dipendenti_menu(){
	system("clear");
	printf("1) Visualizza record dipendenti\n2) Aggiungi record dipendenti\n3) Modifica record dipendenti\n4) Elimina record dipendenti\n5) Torna al menu' principale\n");
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' per la scelta del tipo di ordinamento per i prodotti.
 */
void show_ordinamento_prodotti_menu(){
	system("clear");
	printf("In che ordine vuoi visualizzare i prodotti?\n");
	printf("1) ID\n2) Alfabetico (Nome)\n3) Prezzo\n4) Torna al menu' principale\n");
}

/**
 * @brief
 * Mostra l'interfaccia grafica del menu' per la scelta del tipo di ordinamento per dipendenti e clienti.
 */
void show_ordinamento_persone_menu(){
	system("clear");
	printf("In che ordine vuoi visualizzare i record?\n");
	printf("1) Nome\n2) Cognome\n3) ID\n4) Torna al menu' principale\n");
}

/**
 * @brief
 * Gestisce l'accesso di un dipendente.
 * @param session_t *sessione
 */
void login(sessione_t *se){
	unsigned int id;
	char *psw = (char *)malloc(PERSONE_LENGTH+1);

	system("clear");
	printf("Inserire Id e Password:\nId: ");
	get_string(psw,ID_PERSONE_LENGTH);
	if(!is_unum(psw)){
		sscanf(psw, "%d", &id);
		printf("Password: ");
		get_string(psw,PERSONE_LENGTH);

		if(!match_access(id,psw)){
			se->id=id;
			strcpy(se->psw, psw);
		}
	}

	free(psw); psw=NULL;
}

/**
 * @brief
 * In base a quale menu' e' stato appena visualizzato, gestisce l'immissione dei possibili input controllandone la consistenza col tipo di menu.\n
 * Ritorna la scelta consistente dell'utente.
 * @param menu_t menu
 * @return int
 */
int choise(menu_t m){
	int scelta=0;
	char *stringa = (char *)malloc(2+1);

	get_string(stringa,2);

	if(!is_unum(stringa)){
		sscanf(stringa, "%d", &scelta);

		switch(m){// vado a definire per ogni tipo di menu' il range di scelta
			case nolog:
			case ordinamentoP:
			case ordinamentoC:
			case ordinamentoD:
				while(scelta<1 || scelta>4){
					printf("Valore immesso errato, inserire un valore corretto: ");
					get_string(stringa,2);
					if(!is_unum(stringa))
						sscanf(stringa, "%d", &scelta);
				}
				break;
			case prodotti:
			case log:
				while(scelta<1 || scelta>6){
					printf("Valore immesso errato, inserire un valore corretto: ");
					get_string(stringa,2);
					if(!is_unum(stringa))
						sscanf(stringa, "%d", &scelta);
				}
				break;
			case novita:
				while(scelta<1 || scelta>3){
					printf("Valore immesso errato, inserire un valore corretto: ");
					get_string(stringa,2);
					if(!is_unum(stringa))
						sscanf(stringa, "%d", &scelta);
				}
				break;
			case clienti:
			case dipendenti:
				while(scelta<1 || scelta>5){
					printf("Valore immesso errato, inserire un valore corretto: ");
					get_string(stringa,2);
					if(!is_unum(stringa))
						sscanf(stringa, "%d", &scelta);
				}
				break;
		}
	}//if(!is_unum(stringa))
	else
		printf("Valore immesso errato, inserire un valore corretto: ");

	free(stringa);stringa=NULL;
	return scelta;
}

/**
 * @brief
 * Imposta la prossima azione del programma.
 * @details
 * In base alla scelta presente all'interno della sessione richiama i metodi necessari per la visualizzazione e il comportamento del menu' scelto, impostando i parametri della sessione se necessario.\n
 * Ritorna CHIUDI se e' la scelta in sessione era un chiudi, altrimenti ritorna 0.
 * @param sessione_t *se
 * @return int
 */
int menu_nextmove(sessione_t *se){//m indica il tipo di menu, s la scelta effettuata dall'utente al menu m
	int chiudi=0;

	switch(se->menu){
		case nolog://1)Leggi sezione Novita' 2)Controlla prodotti in magazzino 3)Accedi (solo dipendenti) 4)Chiudi programma
			switch(se->scelta){
				case 1://Metodo visualizzazione Novita
					print_novita();
					printf("\n\nPremi invio per tornare al menu' precedente");
					while(getchar()!='\n');
					show_nolog_menu();
					break;
				case 2://Muoviti al Visualizza record prodotti
					se->menu=ordinamentoP;
					show_ordinamento_prodotti_menu();
					break;
				case 3: //Metodo Accesso
					login(se);
					if(se->id!=DEFAULT_ID_SESSION){
						se->menu=log;
						show_log_menu();
					}
					else{
						show_nolog_menu();
						printf("\nATTENZIONE: Id o Password non validi!\n");
					}
					break;
				case 4://Chiudi
					chiudi=CHIUDI;
					break;
			}
			break;

		case log://1)Novita' 2)Prodotti in magazzino 3)Clienti 4)Dipendenti 5)LogOut 6)Chiudi programma
			switch(se->scelta){
				case 1://Muoviti al menu Novita
					se->menu=novita;
					show_novita_menu();
					break;
				case 2://Muopviti al menu Prodotti
					se->menu=prodotti;
					show_prodotti_menu();
					break;
				case 3: //Muoviti al menu CLienti
					se->menu=clienti;
					show_clienti_menu();
					break;
				case 4://Muoviti al menu Dipendenti
					se->menu=dipendenti;
					show_dipendenti_menu();
					break;
				case 5://Metodo LogOut
					se->menu=nolog;
					initialize_session(se);
					show_nolog_menu();
					break;
				case 6://Chiudi
					chiudi=CHIUDI;
					break;
				}
			break;

		case novita://1)Visualizza messaggio Novita' attuale 2)Cambia messaggio Novita'3)Torna al menu' principale(log)
			switch(se->scelta){
				case 1://Metodo visualizza Novita
					print_novita();
					printf("\n\nPremi invio per tornare al menu' precedente");
					while(getchar()!='\n');
					show_novita_menu();
					break;
				case 2://Metodo modifica Novita
					set_novita();
					printf("\n\nPremi invio per tornare al menu' precedente");
					while(getchar()!='\n');
					show_novita_menu();
					break;
				case 3: //Torna al menu principale
					se->menu=log;
					show_log_menu();
					break;
				}
			break;

		case prodotti:
			switch(se->scelta){//1)Visualizza record prodotti 2)Aggiungi record prodotto 3) Modifica record prodotto 4) Elimina record prodotto 5) Torna al menu' principale
				case 1://Metodo visualizza record prodotti
					se->menu=ordinamentoP;
					show_ordinamento_prodotti_menu();
					break;
				case 2://Metodo aggiungi record prodotto
					if(insert_record(prod)){
						show_prodotti_menu();
						printf("\nATTENZIONE Non e' stato possibile inserire il prodotto per uno dei seguenti motivi:\n");
						printf("*) E' stato inserito un dato inconsistente (prezzo/quantita negativo)\n");
						printf("*) La memoria e' piena (controllare di avere meno di 4294967 record prodotto) o errore inaspettato\n");
					}
					else{
						show_prodotti_menu();
						printf("\nRecord inserito correttamente\n");
					}
					break;
				case 3: //Metodo modifica record prodotto
					if(edit_record(prod)){
						show_prodotti_menu();
						printf("\nATTENZIONE Non e' stato possibile inserire il prodotto per uno dei seguenti motivi:\n");
						printf("*) E' stato inserito un dato inconsistente (id/prezzo/quantita negativo)\n");
						printf("*) Il record con il seguente ID non esiste\n");
					}
					else{
						show_prodotti_menu();
						printf("\nRecord modificato correttamente\n");
					}
					break;
				case 4://Metodo elimina record prodotto
					if(delete_record(prod)){
						show_prodotti_menu();
						printf("\nATTENZIONE record non trovato\n");
					}
					else{
						show_prodotti_menu();
						printf("\nRecord cancellato\n");
					}
					break;
				case 5:
					if(incremental_add_prodotti()){
					show_prodotti_menu();
					printf("\nATTENZIONE Non e' stato possibile inserire il prodotto per uno dei seguenti motivi:\n");
					printf("*) E' stato inserito un dato inconsistente (id negativo)\n");
					printf("*) Il record con il seguente ID non esiste\n");
					printf("*) Si e' tolto una quantita' maggiore di quella disponibile\n");
				}
				else{
					show_prodotti_menu();
					printf("\nModifica avvenuta correttamente\n");
				}

					break;
				case 6://Muoviti al menu log
					se->menu=log;
					show_log_menu();
					break;
				}
			break;

		case clienti:
			switch(se->scelta){//1)Visualizza record clienti 2)Aggiungi record clienti 3) Modifica record clienti 4) Elimina record clienti 5) Torna al menu' principale
				case 1://Metodo visualizza record clienti
					se->menu=ordinamentoC;
					show_ordinamento_persone_menu();
					break;
				case 2://Metodo aggiungi record clienti
					if(insert_record(clie)){
						show_clienti_menu();
						printf("\nATTENZIONE Non e' stato possibile inserire il cliente perche' la memoria e' piena (controllare di avere meno di 65535 dipendenti) o errore inaspettato\n");
					}
					else{
						show_clienti_menu();
						printf("\nRecord inserito correttamente\n");
					}
					break;
				case 3: //Metodo modifica record clienti
					if(edit_record(clie)){
						show_clienti_menu();
						printf("\nATTENZIONE Il record con il seguente ID non esiste\n");
					}
					else{
						show_clienti_menu();
						printf("\nRecord modificato correttamente\n");
					}
					break;
				case 4://Metodo elimina record clienti
					if(delete_record(clie)){
						show_clienti_menu();
						printf("\nATTENZIONE record non trovato\n");
					}
					else{
						show_clienti_menu();
						printf("\nRecord cancellato\n");
					}
					break;
				case 5://Muoviti al menu log
					se->menu=log;
					show_log_menu();
					break;
			}
			break;

		case dipendenti:
			switch(se->scelta){//1)Visualizza record dipendenti 2)Aggiungi record dipendenti 3) Modifica record dipendenti 4) Elimina record dipendenti 5) Torna al menu' dipendenti
				case 1://Metodo visualizza record dipendenti
					se->menu=ordinamentoD;
					show_ordinamento_persone_menu();
					break;
				case 2://Metodo aggiungi record dipendenti
					if(insert_record(dipe)){
						show_dipendenti_menu();
						printf("\nATTENZIONE Non e' stato possibile iserire il dipendente perche' la memoria e' piena (controllare di avere meno di 65535 dipendenti) o errore inaspettato\n");
					}
					else{
						show_dipendenti_menu();
						printf("\nRecord inserito correttamente\n");
					}
					break;
				case 3: //Metodo modifica record dipendenti
					if(edit_record(dipe)){
						show_dipendenti_menu();
						printf("\nATTENZIONE Il record con il seguente ID non esiste\n");
					}
					else{
						show_dipendenti_menu();
						printf("\nRecord modificato correttamente\n");
					}
					break;
				case 4://Metodo elimina record dipendenti
					if(delete_record(dipe)){
						show_dipendenti_menu();
						printf("\nATTENZIONE record non trovato\n");
					}
					else{
						show_dipendenti_menu();
						printf("\nRecord cancellato\n");
					}
					break;
				case 5://Muoviti al menu log
					se->menu=log;
					show_log_menu();
					break;
			}
			break;

		case ordinamentoP:
			switch(se->scelta){//1)ID 2)nome 3)Prezzo 4)Torna al menu' principale");
				case 1://Metodo ordinamento ID
				case 2://Metodo ordinamento nome
				case 3: //Metodo ordinamnento prezzo
					print_file(prod,se->scelta+1);
					printf("\nPremi invio per tornare al menu' precedente");
					while(getchar()!='\n');
					show_ordinamento_prodotti_menu();
					break;
				case 4://Torna al menu principale
					if(se->id==DEFAULT_ID_SESSION){
						se->menu=nolog;
						show_nolog_menu();
					}
					else{
						se->menu=log;
						show_log_menu();
					}
					break;
			}
			break;

		case ordinamentoC://basta aggiungere menu==ordinamentoD prima del print cosi' da risparmiare spazio
		case ordinamentoD:
			switch(se->scelta){//1)Nome 2)Cognome 3)ID 4)Torna al menu' principale
				case 1://Metodo ordinamento nome
				case 2://Metodo ordinamento congome
				case 3: //Metodo ordinamnento ID
					if(se->menu==ordinamentoD)
						print_file(dipe,se->scelta-1);
					else
						print_file(clie,se->scelta-1);
					printf("\nPremi invio per tornare al menu' precedente");
					while(getchar()!='\n');
					show_ordinamento_persone_menu();
					break;
				case 4://Torna al menu principale
					se->menu=log;
					show_log_menu();
					break;
			}
			break;
	}// fine switch(*m)

	return chiudi;
}
