/**
 * @file
 * Contiene i metodi inerenti alla creazione di un albero binario ordinato, e alla visualizzazione di esso.
 */
#include "head_ordinamento.h"

/**
 * @brief
 * Libera la memoria allocata dell'albero a partire dal nodo passato come parametro.
 * @details
 * Se viene passata la radice, tutta la memoria allocata delll'albero verra' liberata.
 * @param tree_node_t * nodo
 */
void destroy_tree(tree_node_t * n) {
	if (n){
		destroy_tree(n->left);
		destroy_tree(n->right);
		free(n);
	}
}

/**
 * @brief
 * Inserisce il record nella posizione giusta dell'albero, la cui radice e' il nodo passato come parametro.
 * @details
 * E' possibile sceglire il campo da considerare per l'ordinamento.
 * @param prodotto_t *record
 * @param tree_node_t **radice
 * @param campo_record_t campo
 * @bug
 * L'ordinamento alfabetico funziona cosi' per ora: ordinamento alfabetico sulla prima lettera del nome e se uguale mette prima quello con id minore (si sa' il perche')
 */
void insert_key_prodotti(prodotto_t *record, tree_node_t **n, campo_record_t campo){
	int count1=0;
	int count2=0;

	if(*n == NULL){
		*n = (tree_node_t *)malloc(sizeof(tree_node_t));;
		(*n)->prodotto=(prodotto_t *)malloc(sizeof(prodotto_t));
		initialize_prodotto((*n)->prodotto);
		(*n)->prodotto->id=record->id;
		strcpy((*n)->prodotto->nome,record->nome);
		(*n)->prodotto->prezzo=record->prezzo;
		(*n)->prodotto->quantita=record->quantita;
		(*n)->left = NULL;
		(*n)->right = NULL;
	}
	else{
		switch(campo){
		case id:
			if(record->id <(*n)->prodotto->id){
				insert_key_prodotti(record, &(*n)->left,campo);
			}
			else{
				insert_key_prodotti(record, &(*n)->right,campo);
			}
			break;
		case alfabetico:
			//if(strcmp(record->nome,(*n)->prodotto->nome)<0){
			while(record->nome[count1]==' ' && ++count1<PRODOTTI_LENGTH);
			while((*n)->prodotto->nome[count2]==' ' && count2++<PRODOTTI_LENGTH);

			if(record->nome[count1]<(*n)->prodotto->nome[count2]){
				insert_key_prodotti(record, &(*n)->left,campo);
			}
			else{
				insert_key_prodotti(record, &(*n)->right,campo);
			}
			break;
		case prezzo:
			if(record->prezzo <(*n)->prodotto->prezzo){
				insert_key_prodotti(record, &(*n)->left,campo);
			}
			else{
				insert_key_prodotti(record, &(*n)->right,campo);
			}
			break;
		default:
			break;
		}//switch(campo)
	}
}

/**
 * @brief
 * Inserisce il record nella posizione giusta dell'albero, la cui radice e' il nodo passato come parametro.
 * @details
 * E' possibile sceglire il campo da considerare per l'ordinamento.
 * @param persona_t *record
 * @param tree_node_t **radice
 * @param campo_record_t campo
 */
void insert_key_persone(persona_t *record, tree_node_t **n, campo_record_t campo){
	int count1=0;
	int count2=0;

	if(*n == NULL){
		*n = (tree_node_t *)malloc(sizeof(tree_node_t));;
		(*n)->persona=(persona_t *)malloc(sizeof(persona_t));
		initialize_persona((*n)->persona);
		(*n)->persona->id=record->id;
		strcpy((*n)->persona->nome,record->nome);
		strcpy((*n)->persona->cognome,record->cognome);
		strcpy((*n)->persona->psw,record->psw);
		(*n)->left = NULL;
		(*n)->right = NULL;
	}
	else{
		switch(campo){
		case nome:
			while(record->nome[count1]==' ' && ++count1<PERSONE_LENGTH);
			while((*n)->persona->nome[count2]==' ' && count2++<PERSONE_LENGTH);

			if(record->nome[count1]<(*n)->persona->nome[count2]){
				insert_key_persone(record, &(*n)->left,campo);
			}
			else{
				insert_key_persone(record, &(*n)->right,campo);
			}
			break;
		case cognome:
			while(record->cognome[count1]==' ' && ++count1<PERSONE_LENGTH);
			while((*n)->persona->cognome[count2]==' ' && count2++<PERSONE_LENGTH);

			if(record->cognome[count1]<(*n)->persona->cognome[count2]){
				insert_key_persone(record, &(*n)->left,campo);
			}
			else{
				insert_key_persone(record, &(*n)->right,campo);
			}
			break;
		case id:
			if(record->id <(*n)->persona->id){
				insert_key_persone(record, &(*n)->left,campo);
			}
			else{
				insert_key_persone(record, &(*n)->right,campo);
			}
			break;
		default:
			break;
		}//switch(campo)
	}
}

/**
 * @brief
 * Stampa l'albero a partire dal nodo passato come parametro, e' necessario specificare il tipo file che si usera'.
 * @param tree_node_t **nodo
 * @param tipofile_t tipo
 */
void print_tree(tree_node_t **n, tipofile_t tipo){
	if(*n == NULL){
		printf("Non sono presenti record\n");
	}
	else{
		if((*n)->left!=NULL){
			print_tree(&(*n)->left,tipo);
		}
		if(tipo!=prod)
			printf("ID: %05u | Nome: %s | Cognome: %s | Password: %s\n",(*n)->persona->id,(*n)->persona->nome, (*n)->persona->cognome, (*n)->persona->psw);
		else
			printf("ID: %010lu | Nome: %s | Prezzo: %#10.2lf | Quantita': %d\n",(*n)->prodotto->id,(*n)->prodotto->nome, (*n)->prodotto->prezzo, (*n)->prodotto->quantita);

		if((*n)->right!=NULL)
			print_tree(&(*n)->right,tipo);
	}
}

/**
 * @brief
 * Stampa l'intero file selezionato datl tipo file passato come parametro, ordinato a seconda del campo passato come parametro.
 * @param tipofile_t tipo
 * @param campo_record_t campo
 */
void print_file(tipofile_t tipo, campo_record_t campo){
	tree_node_t *root=NULL;
	FILE* fp;
	persona_t *persona;
	prodotto_t *prodotto;

	system("clear");
	printf("Nel database sono presenti questi record:\n\n");

	switch(tipo){
	case prod:
		fp=fopen("prodotti.txt", "r");

		if(fp!=NULL){// il file esiste
			fseek(fp,0L,SEEK_SET);
			if(getc(fp)!=EOF){//il file non e' vuoto
				fseek(fp,0L,SEEK_SET);
				prodotto=(prodotto_t *)malloc(sizeof(prodotto_t));
				initialize_prodotto(prodotto);
				//carico l'albero ordinato secondo il campo
				while(getc(fp)!=EOF){
					fseek(fp,-1,SEEK_CUR);
					get_prodotto(prodotto, fp);
					insert_key_prodotti(prodotto, &root, campo);
				}
				free(prodotto); prodotto=NULL;
			}
			fclose(fp); fp=NULL;
		}
		break;
	case clie:
	case dipe:
		if(tipo==dipe)
			fp=fopen("dipendenti.txt", "r");
		else
			fp=fopen("clienti.txt", "r");

		if(fp!=NULL){// il file esiste
			fseek(fp,0L,SEEK_SET);
			if(getc(fp)!=EOF){//il file non e' vuoto
				fseek(fp,0L,SEEK_SET);
				persona=(persona_t *)malloc(sizeof(persona_t));
				initialize_persona(persona);
				//carico l'albero ordinato secondo il campo
				while(getc(fp)!=EOF){
					fseek(fp,-1,SEEK_CUR);
					get_persona(persona, fp);
					insert_key_persone(persona, &root, campo);
				}
				free(persona); persona=NULL;
			}
			fclose(fp); fp=NULL;
		}
		break;
	}

	print_tree(&root,tipo);
	destroy_tree(root);
}
