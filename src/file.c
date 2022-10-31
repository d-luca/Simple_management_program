/**
 * @file
 * Contiene i metodi inerenti alla gestione dei file e alcuni metodi utili per l'input di stringhe.
 */

#include "head_file.h"

/**
 * @brief
 * Stampa il contenuto del file novita.txt
 */
void print_novita(){
	FILE *fp=fopen("novita.txt", "r");
	char c;

	system("clear");
	if(fp==NULL){
		printf("\nNessun messaggio nella sezione novita'");
	}
	else{
		if(getc(fp)==EOF){//il file e' vuoto
			printf("\nNessun messaggio nella sezione novita'");
		}
		else{
			fseek(fp,0L,SEEK_SET);
			printf("Messaggio della sezione novita':\n\n");
			c=getc(fp);
			while(c!=EOF){
				putchar(c);
				c=getc(fp);
			}
		}
		fclose(fp); fp=NULL;
	}
}

/**
 * @brief
 * Salvascrive il messaggio inserito al posto di quello presente nel file novita.txt
 * @details
 * Il messaggio deve essere lungo al massimo 400 caratteri (e' escluso il carattere ~).\n
 * Il messaggio viene considerato fino al carattere ~ o fino a quando non si e' raggiunto i 400 caratteri.\n
 * Il messaggio viene salvascritto solo se si inserisce qualcosa oltre al simbolo ~ .
 */
void set_novita(){
	FILE *fp=fopen("novita.txt", "r+");
	int cont=0;
	char c;

	system("clear");
	if(fp==NULL){
		fp=fopen("novita.txt", "w");
		fclose(fp);
		fopen("novita.txt", "r+");
	}

	if(getc(fp)==EOF){//il file e' vuoto
		printf("Nessun messaggio nella sezione novita'\n");
	}
	else{
		fseek(fp,0L,SEEK_SET);
		printf("Messaggio della sezione novita':\n\n");
		c=getc(fp);
		while(c!=EOF){
			putchar(c);
			c=getc(fp);
		}
	}

	printf("\n\nIl messaggio deve essere lungo max 400 caratteri, per terminare il messaggio inserire come ultimo carattere ~ e premere invio\n");
	printf("Inserire il messaggio che andra' a sostituire quello attuale:\n\n");
	c=getchar();
	if(c!='~'){
		fclose(fp);
		fp=fopen("novita.txt", "w");
		while(c!='~' && cont!=NOVITA_LENGTH){
			fputc(c,fp);
			c=getchar();
			cont++;
		}
	}

	while(getchar()!='\n');
	fclose(fp); fp=NULL;
}

/**
 * @brief
 * Inserisce il record di default nel file dipendenti.txt.
 */
void insert_record_default(){//crea file e mette record di default
	persona_t *persona=(persona_t *)malloc(sizeof(persona_t));
	initialize_persona(persona);
	persona->id=ID_DEFAULT;
	persona->nome=NOME_DEFAULT;
	persona->cognome=COGNOME_DEFAULT;
	persona->psw=PSW_DEFAULT;
	//inserisce un record nel file con le info di default
	FILE *fp=fopen("dipendenti.txt", "w");
	fprintf(fp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome,persona->psw);
	fputc('\n',fp);

	fclose(fp);fp=NULL;
	free(persona);persona=NULL;
}

/**
 * @brief
 * Ritorna il numero di riga del record con quell'id all'interno del file prodotti.txt, inoltre imposta out secondo la logica dei metodi creati.
 * @param FILE *file
 * @param unsigned long id
 * @param int *out
 * @return
 * int
 */
int find_row_prodotti(FILE *fp, unsigned long id,int *out){
	int riga=0;
	char *stringa=(char*)malloc(ID_PRODOTTI_LENGTH+1);
	prodotto_t *prodotto=(prodotto_t *)malloc(sizeof(prodotto_t));
	initialize_prodotto(prodotto);


	fseek(fp,(ID_PRODOTTI_LENGTH*3+PRODOTTI_LENGTH+4)*(id-1),SEEK_SET);//mi posiziono prima del record che potrebbe essere quello giusto se non manca alcun id
	fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
	sscanf(stringa,"%lu",&(prodotto->id));

	if(prodotto->id!=id){//il record non si trova nella riga id
		fseek(fp,0L,SEEK_SET);
		while(prodotto->id!=id && !feof(fp)){
			fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
			sscanf(stringa,"%lu",&(prodotto->id));
			fseek(fp,2*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+3,SEEK_CUR);
			getc(fp);
			riga++;
		}
		if(prodotto->id==id)
			*out=0;
	}
	else{
		riga=id;
		*out=0;
	}
	free(prodotto); prodotto=NULL;
	free(stringa); stringa=NULL;
	return riga;
}

/**
 * @brief
 * Ritorna il numero di riga del record con quell'id all'interno del file clienti.txt o dipendenti.txt in base al file passato,
 * inoltre imposta out secondo la logica dei metodi creati.
 * @param FILE *file
 * @param unsigned int id
 * @param int *out
 * @return
 * int
 */
int find_row_persone(FILE *fp, unsigned int id,int *out){
	int riga=0;
	char *stringa=(char*)malloc(PERSONE_LENGTH+1);
	persona_t *persona=(persona_t *)malloc(sizeof(persona_t));
	initialize_persona(persona);


	fseek(fp,(ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4)*(id-1),SEEK_SET);//mi posiziono prima del record che potrebbe essere quello giusto se non manca alcun id
	fgets(stringa, ID_PERSONE_LENGTH+1, fp);
	sscanf(stringa,"%u",&(persona->id));

	if(persona->id!=id){//il record non si trova nella riga id
		fseek(fp,0L,SEEK_SET);
		while(persona->id!=id && !feof(fp)){
			fgets(stringa, ID_PERSONE_LENGTH+1, fp);
			sscanf(stringa,"%u",&(persona->id));
			fseek(fp,3*PERSONE_LENGTH+3,SEEK_CUR);
			getc(fp);
			riga++;
		}
		if(persona->id==id)
			*out=0;
	}
	else{
		riga=id;
		*out=0;
	}
	free(persona); persona=NULL;
	free(stringa); stringa=NULL;
	return riga;
}

/**
 * @brief
 * Ritorna il primo id disponibile in ordine numerico all'interno del file prodotti.txt.
 * @param FILE *file
 * @return
 * unsigned long
 */
unsigned long find_next_id_prodotti(FILE *fp){//fp gia aperto
	unsigned long out;
	unsigned long temp=0;
	char *stringa;

	if(fp==NULL){
		out=0;
	}
	else{
		if(getc(fp)==EOF){
			out=0;
		}
		else{
			//solo in dipendenti so che c'e' sempre il record di default quindi devo per forza fare la prima lettura
			stringa=(char*)malloc(ID_PRODOTTI_LENGTH+1);
			fseek(fp,0L,SEEK_SET);

			fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
			sscanf(stringa, "%lu",&out);

			if(out==1){
				fseek(fp,2*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+3,SEEK_CUR);
				getc(fp);
				if(!feof(fp)){
					fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
					sscanf(stringa, "%lu",&temp);
					fseek(fp,2*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+3,SEEK_CUR);
					getc(fp);
				}

				while(temp!=0 && temp==out+1 && !feof(fp)){
					out=temp;
					fgets(stringa, ID_PRODOTTI_LENGTH+1, fp);
					sscanf(stringa, "%lu",&temp);
					fseek(fp,2*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+3,SEEK_CUR);
					getc(fp);
				}
				free(stringa); stringa=NULL;
			}
			else
				out=0;
		}
		out++;
	}
	return out;
}

/**
 * @brief
 * Ritorna il primo id disponibile in ordine numerico all'interno del file clienti.txt o dipendenti.txt in base al file passato come argomento.
 * @param FILE *file
 * @return
 * unsigned int
 */
unsigned int find_next_id_persone(FILE *fp){//fp gia aperto
	unsigned int out;
	unsigned int temp=0;
	char *stringa;

	if(fp==NULL){
		out=0;
	}
	else{
		if(getc(fp)==EOF){
			out=0;
		}
		else{
			//solo in dipendenti so che c'e' sempre il record di default quindi devo per forza fare la prima lettura
			stringa=(char*)malloc(ID_PERSONE_LENGTH+1);
			fseek(fp,0L,SEEK_SET);

			fgets(stringa, ID_PERSONE_LENGTH+1, fp);
			sscanf(stringa, "%u",&out);
			if(out==1){
				fseek(fp,3*PERSONE_LENGTH+3,SEEK_CUR);
				getc(fp);
				if(!feof(fp)){
					fgets(stringa, ID_PERSONE_LENGTH+1, fp);
					sscanf(stringa, "%u",&temp);
					fseek(fp,3*PERSONE_LENGTH+3,SEEK_CUR);
					getc(fp);
				}

				while(temp!=0 && temp==out+1 && !feof(fp)){
					out=temp;
					fgets(stringa, ID_PERSONE_LENGTH+1, fp);
					sscanf(stringa, "%u",&temp);
					fseek(fp,3*PERSONE_LENGTH+3,SEEK_CUR);
					getc(fp);
				}
				free(stringa); stringa=NULL;
			}
			else
				out=0;
		}
		out++;
	}
	return out;
}

/**
 * @brief
 * Inserisce il record nel file specificato.
 * @details
 * In base al file specificato dal parametro d'ingresso, visualizza l'interfaccia grafica per inserire i dati dei record e se i dati inseriti sono consistenti inserisce
 * nel file giusto il record in ordine numerico col primo ID disponibile.\n
 * Ritorna 0 se l'inserimento e' avvenuto con successo, -1 se i dati inseriti sono inconsistenti o non e' stato possibile inserire il record nel file.\n
 * Se i file non esistono, questi verranno automaticamente creati./n
 * ATTENZIONE: Questo metodo non aggiunge il record di default al file dipendenti.txt, in questo modo e' possibile aggiungere il record 1 se prima si ha cancellato il record di default.
 * @param tipofile_t tipo
 * @return
 * int
 */
int insert_record(tipofile_t tipo){
	int out=-1;
	FILE *fp;
	persona_t *persona;
	persona_t *persona_temp;
	prodotto_t *prodotto;
	prodotto_t *prodotto_temp;
	char *stringa;

	switch(tipo){
		case prod:
			stringa=(char*)malloc(PRODOTTI_LENGTH+1);
			prodotto=(prodotto_t *)malloc(sizeof(prodotto_t));
			prodotto_temp=(prodotto_t *)malloc(sizeof(prodotto_t));
			initialize_prodotto(prodotto);
			initialize_prodotto(prodotto_temp);

			fp=fopen("prodotti.txt", "r+");
			if(fp==NULL){
				fp=fopen("prodotti.txt", "w");
				fclose(fp);
				fp=fopen("prodotti.txt", "r+");
			}
			printf("Inserire Nome Prezzo e Quantita' del prodotto d'aggiungere\nNome: ");
			get_string(prodotto->nome, PRODOTTI_LENGTH);
			printf("Prezzo con al massimo 7 cifre a sinistra del punto e due dopo, altrimenti verra' troncato\n");
			printf("Prezzo: ");
			get_string(stringa, ID_PRODOTTI_LENGTH-3);

				if(!is_ufnum(stringa)){
					sscanf(stringa, "%lf",&prodotto->prezzo);
					printf("Quantita': ");
					get_string(stringa, ID_PRODOTTI_LENGTH);

					if(!is_unum(stringa)){
						sscanf(stringa, "%d",&prodotto->quantita);

						prodotto->id=find_next_id_prodotti(fp);

						if(prodotto->id!=0){
						//se si vuole si puo' aggiungere controllo di record uguale (un po' inutile)
						//vado a stampare su file
						fseek(fp,(3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4)*(prodotto->id-1),SEEK_SET);//mi posiziono alla fine della riga prima di quella dove devo inserire (appena prima della riga sucessiva)
						//devo prendere i dati del record prima di scriverci sopra
						get_prodotto(prodotto_temp,fp);

						if(prodotto_temp->id==0){//inserisco direttamente alla fine
							fseek(fp,0L,SEEK_END);
							fprintf(fp,"%010lu~%40s~%#10.2lf~%10d",prodotto->id,prodotto->nome,prodotto->prezzo, prodotto->quantita);
							fputc('\n',fp);
						}
						else{
							fseek(fp,-(3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4),SEEK_CUR);//mi riposisziono prima del record
							//scrivo il nuovo record sopra a quello gia' esistente
							fprintf(fp,"%010lu~%40s~%#10.2lf~%10d",prodotto->id,prodotto->nome,prodotto->prezzo, prodotto->quantita);
							fputc('\n',fp);

							while(getc(fp)!=EOF){
								fseek(fp,-1,SEEK_CUR);
								//copio il record cancellato in quello da scrivere dopo
								prodotto->id=prodotto_temp->id;
								strcpy(prodotto->nome,prodotto_temp->nome);
								prodotto->prezzo=prodotto_temp->prezzo;
								prodotto->quantita=prodotto_temp->quantita;
								//prendo i dati del record che verra' rimpiazzato
								get_prodotto(prodotto_temp,fp);
								fseek(fp,-(3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4),SEEK_CUR);//mi riposisziono prima del record
								//scrivo il nuovo record sopra a quello gia' esistente
								fprintf(fp,"%010lu~%40s~%#10.2lf~%10d",prodotto->id,prodotto->nome,prodotto->prezzo, prodotto->quantita);
								fputc('\n',fp);
							}//while(getc(fp)!=EOF)
							//scrivo lultimo record sapendo che sara' l'ultimo
							fprintf(fp,"%010lu~%40s~%#10.2lf~%10d",prodotto_temp->id,prodotto_temp->nome,prodotto_temp->prezzo, prodotto_temp->quantita);
							fputc('\n',fp);
						}//else
						out=0;
						}//if(persona->id!=0){
						else{
							//errore
							out=-1;
						}
					}//if(!is_uint(stringa)) quantita
					else{
						out=-1;
					}
				}//if(!is_uint(stringa)) prezzo
				else{
					out=-1;
				}

			free(prodotto_temp); prodotto_temp=NULL;
			free(prodotto); prodotto=NULL;
			fclose(fp); fp=NULL;
			break;

		case clie:
		case dipe:
			stringa=(char*)malloc(PERSONE_LENGTH+1);
			persona=(persona_t *)malloc(sizeof(persona_t));
			persona_temp=(persona_t *)malloc(sizeof(persona_t));
			initialize_persona(persona);
			initialize_persona(persona_temp);

			if (tipo==dipe){
				fp=fopen("dipendenti.txt", "r+");//non controllo se e' apribile perche' arrivo qua solo se sono sicuro che lo sia
				printf("Inserire Nome, Cognome e Pssword del dipendente d'aggiungere\nNome: ");
			}
			else{
				fp=fopen("clienti.txt", "r+");
				if(fp==NULL){
					fp=fopen("clienti.txt", "w");
					fclose(fp);
					fp=fopen("clienti.txt", "r+");
				}
				printf("Inserire Nome e Cognome del cliente d'aggiungere\nNome: ");
			}

			get_string(persona->nome, PERSONE_LENGTH);
			printf("Cognome: ");
			get_string(persona->cognome, PERSONE_LENGTH);
			if (tipo==dipe){
				printf("Password: ");
				get_string(persona->psw, PERSONE_LENGTH);
			}

			persona->id=find_next_id_persone(fp);

			if(persona->id!=0){
			//se si vuole si puo' aggiungere controllo di record uguale (un po' inutile)
			//vado a stampare su file
			fseek(fp,(ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4)*(persona->id-1),SEEK_SET);//mi posiziono alla fine della riga prima di quella dove devo inserire (appena prima della riga sucessiva)
			//devo prendere i dati del record prima di scriverci sopra
			get_persona(persona_temp,fp);

			if(persona_temp->id==0){//inserisco direttamente alla fine
				fseek(fp,0L,SEEK_END);
				fprintf(fp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome, persona->psw);
				fputc('\n',fp);
			}
			else{
				fseek(fp,-(ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4),SEEK_CUR);//mi riposisziono prima del record
				//scrivo il nuovo record sopra a quello gia' esistente
				fprintf(fp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome, persona->psw);
				fputc('\n',fp);

				while(getc(fp)!=EOF){
					fseek(fp,-1,SEEK_CUR);
					//copio il record cancellato in quello da scrivere dopo
					persona->id=persona_temp->id;
					strcpy(persona->nome,persona_temp->nome);
					strcpy(persona->cognome,persona_temp->cognome);
					strcpy(persona->psw,persona_temp->psw);
					//prendo i dati del record che verra' rimpiazzato
					get_persona(persona_temp,fp);
					fseek(fp,-(ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4),SEEK_CUR);//mi riposisziono prima del record
					//scrivo il nuovo record sopra a quello gia' esistente
					fprintf(fp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome, persona->psw);
					fputc('\n',fp);
				}//while(getc(fp)!=EOF)
				//scrivo lultimo record sapendo che sara' l'ultimo
				fprintf(fp,"%05u~%20s~%20s~%20s",persona_temp->id,persona_temp->nome,persona_temp->cognome, persona_temp->psw);
				fputc('\n',fp);
			}//else
			out=0;
			}//if(persona->id!=0){
			else{
				//errore
				out=-1;
			}
			free(persona_temp); persona_temp=NULL;
			free(persona); persona=NULL;
			fclose(fp); fp=NULL;
			break;//case dipe
	}//switch(tipo){

	free(stringa); stringa=NULL;
	return out;
}

/**
 * @brief
 * Elimina il record dal file specificato se esiste.
 * @details
 * In base al file specificato dal parametro d'ingresso, visualizza l'interfaccia grafica per inserire i dati dei record e se i dati inseriti sono consistenti elimina
 * dal file giusto il record con l'ID inserito.\n
 * Ritorna 0 se la cancellazione e' avvenuta con successo, -1 se i dati inseriti sono inconsistenti o non e' stato possibile trovare il record con l'ID inserito.\n
 * Se i file non esistono, questi verranno automaticamente creati.\n
 * Inserisce il record di default se necessario nel file dipendenti.txt.
 * @param tipofile_t tipo
 * @return
 * int
 */
int delete_record(tipofile_t tipo){//id>0
	int out=-1;
	int riga=0;
	unsigned int id_persona;
	unsigned long id_prodotto;
	char *stringa;
	FILE *fp;
	FILE *temp;
	persona_t *persona;
	prodotto_t *prodotto;

	printf("Inserire l'ID del record da cancellare: ");
	if(tipo!=prod){
		stringa=(char*)malloc(ID_PERSONE_LENGTH+1);
		get_string(stringa,ID_PERSONE_LENGTH);
		if(!is_unum(stringa))
			sscanf(stringa,"%u",&id_persona);
		else
			id_persona=0;
		strcpy(stringa,"");
	}
	else{
		stringa=(char*)malloc(ID_PRODOTTI_LENGTH+1);
		get_string(stringa,ID_PRODOTTI_LENGTH);
		if(!is_unum(stringa))
			sscanf(stringa,"%lu",&id_prodotto);
		else
			id_prodotto=0;
		strcpy(stringa,"");
	}

	if(id_persona!=0 || id_prodotto!=0){
		switch(tipo){
		case prod:
			prodotto=(prodotto_t *)malloc(sizeof(prodotto_t));
			initialize_prodotto(prodotto);

			fp=fopen("prodotti.txt", "r+");
			if(fp==NULL){
				fp=fopen("prodotti.txt", "w");
				fclose(fp);
				fp=fopen("prodotti.txt", "r+");
			}

			riga=find_row_prodotti(fp,id_prodotto,&out);

			if(out!=-1){
				//sono sicuro che esista il record da eliminare e so il suo numero di riga
				temp=fopen("temp.txt","w");
				fseek(fp,0L,SEEK_SET);
				//ora devo copiare il file dall'inizio fino a prima della riga e da dopo la riga fino alla fine
				//dall'inizio fino a prima della riga
				for(int i=0;i<riga-1;i++){
					get_prodotto(prodotto, fp);
					fprintf(temp,"%010lu~%40s~%#10.2lf~%10d",prodotto->id,prodotto->nome,prodotto->prezzo, prodotto->quantita);
					fputc('\n',temp);
				}
				//salto una riga
				fseek(fp,3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4,SEEK_CUR);
				//dopo la riga fino alla fine
				while(getc(fp)!=EOF){
					fseek(fp,-1,SEEK_CUR);
					get_prodotto(prodotto,fp);
					fprintf(temp,"%010lu~%40s~%#10.2lf~%10d",prodotto->id,prodotto->nome,prodotto->prezzo, prodotto->quantita);
					fputc('\n',temp);
				}
				//assegno i puntatori dei file in modo che fp punti al nuovo file
				fclose(fp); fp=NULL;
				fclose(temp); temp=NULL;
				remove("prodotti.txt");
				rename("temp.txt","prodotti.txt");
			}
			free(prodotto); prodotto=NULL;
			break;

		case clie:
		case dipe:
			if (tipo==dipe){
				fp=fopen("dipendenti.txt", "r+");
				if(fp==NULL){//non c'e' il file
					insert_record_default();
					fp=fopen("dipendenti.txt", "r+");
				}
				else{
					fseek(fp,0L,SEEK_SET);
					if(getc(fp)==EOF){//il file e' vuoto
						fclose(fp);
						insert_record_default();
						fp=fopen("dipendenti.txt", "r+");
					}
				}//sono sicuro di essere all'inizio, il file e' aperto e contiene almeno il record di default
			}
			else{
				fp=fopen("clienti.txt", "r+");
				if(fp==NULL){
					fp=fopen("clienti.txt", "w");
					fclose(fp);
					fp=fopen("clienti.txt", "r+");
				}
			}
			persona=(persona_t *)malloc(sizeof(persona_t));
			initialize_persona(persona);

			riga=find_row_persone(fp,id_persona,&out);

			if(out!=-1){
				//sono sicuro che esista il record da eliminare e so il suo numero di riga
				temp=fopen("temp.txt","w");
				fseek(fp,0L,SEEK_SET);
				//ora devo copiare il file dall'inizio fino a prima della riga e da dopo la riga fino alla fine
				//dall'inizio fino a prima della riga
				for(int i=0;i<riga-1;i++){
					get_persona(persona, fp);
					fprintf(temp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome, persona->psw);
					fputc('\n',temp);
				}
				//salto una riga
				fseek(fp,ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4,SEEK_CUR);
				//dopo la riga fino alla fine
				while(getc(fp)!=EOF){
					fseek(fp,-1,SEEK_CUR);
					get_persona(persona,fp);
					fprintf(temp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome, persona->psw);
					fputc('\n',temp);
				}
				//assegno i puntatori dei file in modo che fp punti al nuovo file
				fclose(fp); fp=NULL;
				fclose(temp); temp=NULL;
				if(tipo==dipe){
					remove("dipendenti.txt");
					rename("temp.txt","dipendenti.txt");
				}
				else{
					remove("clienti.txt");
					rename("temp.txt","clienti.txt");
				}
			}
			free(persona); persona=NULL;
			break;
		}//switch(tipo)
	}

	free(stringa); stringa=NULL;
	return out;
}

/**
 * @brief
 * Modifica il record dal file specificato se esiste.
 * @details
 * In base al file specificato dal parametro d'ingresso, visualizza l'interfaccia grafica per inserire i dati dei record e se i dati inseriti sono consistenti modifica
 * nel file giusto il record con l'ID inserito.\n
 * Ritorna 0 se la modifica e' avvenuta con successo, -1 se i dati inseriti sono inconsistenti o non e' stato possibile trovare il record con l'ID inserito.\n
 * Se i file non esistono, questi verranno automaticamente creati.\n
 * Inserisce il record di default se necessario nel file dipendenti.txt.
 * @param tipofile_t tipo
 * @return
 * int
 */
int edit_record(tipofile_t tipo){
	int out=-1;
	int riga=0;
	unsigned int id_persona;
	unsigned long id_prodotto;
	char *stringa;
	FILE *fp;
	persona_t *persona;
	prodotto_t *prodotto;

	system("clear");
	printf("Inserire l'ID del record da modificare: ");
	if(tipo!=prod){
		stringa=(char*)malloc(PERSONE_LENGTH+1);
		get_string(stringa,ID_PERSONE_LENGTH);
		if(!is_unum(stringa))
			sscanf(stringa,"%u",&id_persona);
		else
			id_persona=0;
		strcpy(stringa,"");
	}
	else{
		stringa=(char*)malloc(PRODOTTI_LENGTH+1);
		get_string(stringa,ID_PRODOTTI_LENGTH);
		if(!is_unum(stringa))
			sscanf(stringa,"%lu",&id_prodotto);
		else
			id_prodotto=0;
		strcpy(stringa,"");
	}

	if (id_persona!=0 || id_prodotto!=0){
		switch(tipo){
		case prod:
			fp=fopen("prodotti.txt", "r+");
			if(fp==NULL){
				fp=fopen("prodotti.txt", "w");
				fclose(fp);
				fp=fopen("prodotti.txt", "r+");
			}

			prodotto=(prodotto_t *)malloc(sizeof(prodotto_t));
			initialize_prodotto(prodotto);

			riga=find_row_prodotti(fp,id_prodotto,&out);

			if(out!=-1){
				//sono sicuro che esista il record da editare e so il suo numero di riga
				//mi posiziono prima del record da editare
				fseek(fp,(3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4)*(riga-1),SEEK_SET);
				get_prodotto(prodotto,fp);
				fseek(fp,-(3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4),SEEK_CUR);

				printf("Il record con ID: %010lu contiene:\n", id_prodotto);
				printf("Nome: %s | Prezzo: %10.2lf€, | Quantita': %d\n",prodotto->nome, prodotto->prezzo, prodotto->quantita);

				printf("Di seguito verranno proposti i campi da modificare, premere invio senza digitare nulla per mantenere il campo invariato\n");

				printf("Nome: ");
				get_string(stringa, PRODOTTI_LENGTH);
				if(strcmp(stringa,""))
					strcpy(prodotto->nome,stringa);
				printf("Prezzo: ");
				get_string(stringa, ID_PRODOTTI_LENGTH);
				if(strcmp(stringa,"")){
					if(!is_ufnum(stringa))
						sscanf(stringa,"%lf",&prodotto->prezzo);
					//if(!is_ufnum(stringa))
					else
						out=-1;
				}//if(strcmp(stringa,""))
				printf("Quantita': ");
				get_string(stringa, ID_PRODOTTI_LENGTH);
				if(strcmp(stringa,"")){
					if(!is_unum(stringa))
						sscanf(stringa,"%d",&prodotto->quantita);
					else
						out=-1;
				}
				if(out!=-1)
					fprintf(fp,"%010lu~%40s~%#10.2lf~%10d",prodotto->id,prodotto->nome,prodotto->prezzo, prodotto->quantita);
			}
			free(prodotto); prodotto=NULL;
			fclose(fp); fp=NULL;
			break;

		case clie:
		case dipe:
			if (tipo==dipe){
				fp=fopen("dipendenti.txt", "r+");
				if(fp==NULL){//non c'e' il file
					insert_record_default();
					fp=fopen("dipendenti.txt", "r+");
				}
				else{
					fseek(fp,0L,SEEK_SET);
					if(getc(fp)==EOF){//il file e' vuoto
						fclose(fp);
						insert_record_default();
						fp=fopen("dipendenti.txt", "r+");
					}
				}//sono sicuro di essere all'inizio, il file e' aperto e contiene almeno il record di default
			}
			else{
				fp=fopen("clienti.txt", "r+");
				if(fp==NULL){
					fp=fopen("clienti.txt", "w");
					fclose(fp);
					fp=fopen("clienti.txt", "r+");
				}
			}

			persona=(persona_t *)malloc(sizeof(persona_t));
			initialize_persona(persona);

			riga=find_row_persone(fp,id_persona,&out);

			if(out!=-1){
				//sono sicuro che esista il record da editare e so il suo numero di riga
				//mi posiziono prima del record da editare
				fseek(fp,(ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4)*(riga-1),SEEK_SET);
				get_persona(persona,fp);
				fseek(fp,-(ID_PERSONE_LENGTH+3*PERSONE_LENGTH+4),SEEK_CUR);

				printf("Il record con ID: %05u contiene:\n", id_persona);
				if(tipo==dipe)
					printf("Nome: %s | Cognome: %s, | Password: %s\n",persona->nome, persona->cognome, persona->psw);
				else
					printf("Nome: %s | Cognome: %s\n",persona->nome, persona->cognome);

				printf("Di seguito verranno proposti i campi da modificare, premere invio senza digitare nulla per mantenere il campo invariato\n");

				printf("Nome: ");
				get_string(stringa, PERSONE_LENGTH);
				if(strcmp(stringa,""))
					strcpy(persona->nome,stringa);

				printf("Cognome: ");
				get_string(stringa, PERSONE_LENGTH);
				if(strcmp(stringa,""))
					strcpy(persona->cognome,stringa);

				if(tipo==dipe){
					printf("Password: ");
					get_string(stringa, PERSONE_LENGTH);
					if(strcmp(stringa,""))
						strcpy(persona->psw,stringa);
				}

				fprintf(fp,"%05u~%20s~%20s~%20s",persona->id,persona->nome,persona->cognome, persona->psw);
			}
			free(persona); persona=NULL;
			fclose(fp); fp=NULL;
			break;
		}
	}

	free(stringa); stringa=NULL;
	return out;
}

/**
 * @brief
 * Controlla se i dati passati come parametri corrispondono a un record nel file dipendenti.txt.
 * @details
 * Se il file dipendenti.txt non esiste, questo verra' automaticamente creato, inoltre verra' inserito il record di default.
 * Ritorna 0 se c'e' stato un match, altrimenti ritorna -1.\n
 * Inserisce il record di default se necessario nel file dipendenti.txt.
 * @param unsigned int id
 * @param char *psw
 * @return
 * int
 */
int match_access(unsigned int id, char *psw){//si ipotizza che i record siano in ordine id
	unsigned int idf;
	char *pswf;
	char *psw_temp;
	FILE *fp;
	int out=-1;

	fp=fopen("dipendenti.txt", "r");
	if(fp==NULL){//non c'e' il file
		insert_record_default();
		fp=fopen("dipendenti.txt", "r");
	}
	else{
		fseek(fp,0L,SEEK_SET);
		if(getc(fp)==EOF){//il file e' vuoto
			fclose(fp);
			insert_record_default();
			fp=fopen("dipendenti.txt", "r");
		}
	}

	pswf=(char *)malloc(PERSONE_LENGTH+1);
	psw_temp=(char *)malloc(PERSONE_LENGTH+1);
	sprintf(psw_temp,"%20s",psw);

	fseek(fp,0L,SEEK_SET);
	fgets(pswf,ID_PERSONE_LENGTH+1,fp);
	sscanf(pswf,"%u", &idf);
	fseek(fp,3*PERSONE_LENGTH+3,SEEK_CUR);
	getc(fp);

	while(idf!=id && !feof(fp)){//ho dovuto mettere feof e getc perche' fseek non mi ritornava mai un valore !=0 anche se arrivava alla fine del file
		fgets(pswf,ID_PERSONE_LENGTH+1,fp);
		sscanf(pswf,"%u", &idf);
		fseek(fp,3*PERSONE_LENGTH+3,SEEK_CUR);
		getc(fp);
	}

	if(idf==id){
		fseek(fp,-(PERSONE_LENGTH+1),SEEK_CUR);
		fgets(pswf,PERSONE_LENGTH+1,fp);

		if(!strcmp(pswf,psw_temp)){
			out=0;
		}
	}

	free(pswf); pswf=NULL;
	free(psw_temp); psw_temp=NULL;
	fclose(fp); fp=NULL;
	return out;
}

/**
 * @brief
 * Incrementa o diminuisce il campo quantita' del record con l'ID specificato, se esiste.
 * @details
 * Non si puo' decrementare piu' di quello che c'e' in magazzino, se si fa', il record non viene modificato e compare un messaggio di errore.\n
 * Se il file prodotti.txt non esiste viene creato automaticamente.\n
 * Ritorna 0 se la modifica e' avvenuta correttamente, -1 se si e' tentato di decrementare piu' di quanto consentito, o i dati inseriti sono inconsistenti.
 * @return
 * int
 */
int incremental_add_prodotti(){
	FILE *fp;
	char *stringa;
	unsigned long id=0;
	int quantita;
	int temp;
	int out=-1;

	system("clear");
	stringa=(char*)malloc(ID_PRODOTTI_LENGTH+1);
	printf("Inserire l'ID del record da modificare e la quantita d'aggiungere (per toglire usare -quantita'):\n");
	printf("ID: ");
	get_string(stringa,ID_PRODOTTI_LENGTH);
	if(!is_unum(stringa)){
		sscanf(stringa,"%lu",&id);
		printf("Quantita': ");
		get_string(stringa,ID_PRODOTTI_LENGTH);
		if(!is_num(stringa))
			sscanf(stringa,"%d",&quantita);
		else
			id=0;
	}

	if(id!=0){
		int riga=0;

		fp=fopen("prodotti.txt", "r+");
		if(fp==NULL){
			fp=fopen("prodotti.txt", "w");
			fclose(fp);
			fp=fopen("prodotti.txt", "r+");
		}

		riga=find_row_prodotti(fp,id,&out);
		fseek(fp,((3*ID_PRODOTTI_LENGTH+PRODOTTI_LENGTH+4)*(riga))-(ID_PRODOTTI_LENGTH+1),SEEK_SET);
		fgets(stringa,ID_PRODOTTI_LENGTH+1,fp);
		sscanf(stringa,"%d",&temp);

		if(out!=-1){
			//faccio cosi' anche perche' se volessi mettere in futuro la uestione delle prenotazioni, questo campo potrebbe anchge andare <0
			if(temp+quantita>=0){
				temp+=quantita;
				fseek(fp,-(ID_PRODOTTI_LENGTH),SEEK_CUR);
				fprintf(fp,"%10d",temp);
				out=0;
			}
			else
				out=-1;
		}
		fclose(fp); fp=NULL;
	}

	free(stringa); stringa=NULL;
	return out;
}
