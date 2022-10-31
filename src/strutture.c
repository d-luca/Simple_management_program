/**
 * @file
 * Contiene i metodi inerenti alle strutture e altri metodi utili come:\n
 * -)Inizializzazione delle variabili che compongono una struttura.\n
 * -)Controlli di consistenza di un tipo di dato.\n
 * -)Gestione d'ellimput di stringhe da stdin e non.
 */
#include "head_strutture.h"

/**
 * @brief
 * Inizializza le componenti di una struttura sessione
 * @param sessione_t *sessione
 */
void initialize_session(sessione_t *se){
	se->id=DEFAULT_ID_SESSION;
	se->psw=(char *) malloc(PERSONE_LENGTH+1);
	strcpy(se->psw,DEFAULT_PSW_SESSION);

	se->scelta=DEFAULT_SCELTA;
	se->menu=DEFAULT_MENU;
}

/**
 * @brief
 * Inizializza le componenti di una struttura prodotto
 * @param prodotto_t *prodotto
 */
void initialize_prodotto(prodotto_t *p){//dato che dopo vado a lavorare con la concatenazione, mi assicuro che siano vuote (esperienza brutta con residui in memoria dinamica)
	p->nome=(char*)malloc(PRODOTTI_LENGTH+1);
	strcpy(p->nome, "");
	p->id=0;
	p->prezzo=0;
	p->quantita=0;
}

/**
 * @brief
 * Inizializza le componenti di una struttura persona
 * @param persona_t *persona
 */
void initialize_persona(persona_t *p){//dato che dopo vado a lavorare con la concatenazione, mi assicuro che siano vuote (esperienza brutta con residui in memoria dinamica)
	p->nome=(char*)malloc(PERSONE_LENGTH+1);
	strcpy(p->nome, "");
	p->cognome=(char*)malloc(PERSONE_LENGTH+1);
	strcpy(p->cognome, "");
	p->psw=(char*)malloc(PERSONE_LENGTH+1);
	strcpy(p->psw, "");
	p->id=0;
}

/**
 * @brief
 * Carica in prodotto i dati di una riga del file prodotti.txt.
 * @details
 * Si ipotizza di essere nella posizione giusta e che il puntatore al file sia open.\n
 * Alla fine lascia il puntatore sulla nuova riga se esiste.
 * @param prodotto_t *prodotto
 * @param FILE *file
 */
void get_prodotto(prodotto_t *prodotto, FILE *fp){//si ipotizza che fp sia aperto e che il puntatore sia nella giusta posizione, alla fine ti lascia sulla nuova riga se esiste
	char *stringa=(char*)malloc(PRODOTTI_LENGTH+1);

	fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
	sscanf(stringa,"%lu",&(prodotto->id));//id
	getc(fp);
	fgets(prodotto->nome, PRODOTTI_LENGTH+1, fp);//nome
	getc(fp);
	fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
	sscanf(stringa,"%lf",&(prodotto->prezzo));//prezzo
	getc(fp);
	fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
	sscanf(stringa,"%d",&(prodotto->quantita));//quantita
	getc(fp);

	free(stringa); stringa=NULL;
}

/**
 * @brief
 * Carica in persona i dati di una riga del file clienti.txt o dipendenti.txt.
 * @details
 * Si ipotizza di essere nella posizione giusta e che il puntatore al file sia open.\n
 * Alla fine lascia il puntatore sulla nuova riga se esiste.
 * @param persona_t *persona
 * @param FILE *file
 */
void get_persona(persona_t *persona, FILE * fp){//si ipotizza che fp sia aperto e che il puntatore sia nella giusta posizione, alla fine ti lascia sulla nuova riga se esiste
	char *stringa=(char*)malloc(ID_PERSONE_LENGTH+1);

	fgets(stringa, ID_PERSONE_LENGTH+1, fp);
	sscanf(stringa,"%u",&(persona->id));//id
	getc(fp);
	fgets(persona->nome, PERSONE_LENGTH+1, fp);//nome
	getc(fp);
	fgets(persona->cognome, PERSONE_LENGTH+1, fp);//cognome
	getc(fp);
	fgets(persona->psw, PERSONE_LENGTH+1, fp);//password
	getc(fp);

	free(stringa); stringa=NULL;
}

/**
 * @brief
 * Estrae da stdin una stringa di lunghezza massima data e la mette nella stringa passata come parametro, pone sempre \0 alla fine.
 * @details
 * Se ci sono piu' caratteri di quelli ammissibili svuota anche il buffer.
 * @param char *stringa
 * @param int lunghezza
 */
void get_string(char *stringa,int lunghezza){
	char c;
	strcpy(stringa,"");//dato che uso la concatenazione, usando la memoria dinamica potrei trovarmi celle gia' scritte, allora mi assicuro che la stringa sia vuota

	c=getchar();
	while(c!='\n' && c!=EOF){
		strcat(stringa, &c);
		if(strlen(stringa)==lunghezza){
			while(getchar()!='\n');// consumo tutto il buffer
			break;
		}
		c=getchar();
	}
	c='\0';
	strcat(stringa, &c);
}

/**
 * @brief
 * Estrae dal file passato come parametro una stringa di lunghezza massima data e la mette nella stringa passata come parametro, pone sempre \0 alla fine.
 * @details
 * Se ci sono piu' caratteri di quelli ammissibili svuota anche il buffer.
 * ATTENZIONE: Questo metodo non e' mai stato utilizzato nel programma quindi potrebbe non funzionare correttamente, potrebbe tornare utile.
 * @param char *stringa
 * @param int lunghezza
 * @param FILE *fp
 */
void get_string_file(char *stringa,int lunghezza,FILE *fp){// si considera fp gia open, la lunghezza deve essere = numero di caratteri da prendere verra' messo\0 alla fine
	char c;

	c=fgetc(fp);
	while(c!='\n' && c!=EOF){
		strcat(stringa, &c);
		if(strlen(stringa)==lunghezza){
			break;
		}
		c=fgetc(fp);
	}
	c='\0';
	strcat(stringa, &c);
}

/**
 * @brief
 * Controlla se la stringa passata come parametro contiene solo caratteri numerici interi senza segno.
 * @details
 * Ritorna 0 se la stringa corrisponde al tipo definito, -1 altrimenti.
 * @param char *stringa
 * @return int
 */
int is_unum(char *stringa){
	int out=0;
	char c;
	int temp=1;

	c=*stringa++;
	if(c=='\0')
		out=-1;

	while(out==0 && c!='\0'){
		if(c<'0' || c>'9')
			out=-1;
		c=*stringa++;
		temp++;
	}

	stringa-=temp;
	return out;
}

/**
 * @brief
 * Controlla se la stringa passata come parametro contiene solo caratteri numerici interi con segno.
 * @details
 * Ritorna 0 se la stringa corrisponde al tipo definito, -1 altrimenti.
 * @param char *stringa
 * @return int
 */
int is_num(char *stringa){
	int out=0;
	char c;
	int temp=1;

	c=*stringa++;
	if(c=='\0'){
		out=-1;
	}
	else{
		if(c=='-'){
			c=*stringa++;
			temp++;
		}
	}

	while(out==0 && c!='\0'){
		if(c<'0' || c>'9')
			out=-1;
		c=*stringa++;
		temp++;
	}

	stringa-=temp;
	return out;
}

/**
 * @brief
 * Controlla se la stringa passata come parametro contiene solo caratteri numerici interi o non senza segno.
 * @details
 * Ritorna 0 se la stringa corrisponde al tipo definito, -1 altrimenti.
 * @param char *stringa
 * @return int
 * @bug
 * E' stato necessario includere sia , che . quindi potrebbe avere un comportamento
 * leggermente diverso se usato un delimitatore di cifre diverso da quello definito dal proprio sistema (non causa comunque errori gravi, si perde al massimo la parte decimale).
 */
int is_ufnum(char *stringa){
	int out=0;
	char c;
	int temp=1;

	c=*stringa++;
	if(c=='\0')
		out=-1;

	while(out==0 && c!='\0'){
		if(c!=',' && c!='.' && (c<'0' || c>'9'))
			out=-1;
		c=*stringa++;
		temp++;
	}

	stringa-=temp;
	return out;
}
