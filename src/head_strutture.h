/**
 * @file
 * Contiene le definizioni dei metodi, le strutture, gli enumeratori che potrebbero essere usati da altri file.\n
 * In particolare sono presenti i metodi per inizzializzare tipi struttura, metodi di controllo di consistenza, metodi per recuperare record dai vari file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef DEFINIZIONI
#define DEFINIZIONI /**< Blocco contenente le definizioni di tutte le macro.*/
	#define NOVITA_LENGTH 401
	#define PERSONE_LENGTH 20
	#define PRODOTTI_LENGTH 40
	#define ID_PERSONE_LENGTH 5
	#define ID_PRODOTTI_LENGTH 10
	#define NOME_DEFAULT "default"
	#define COGNOME_DEFAULT "default"
	#define ID_DEFAULT 1
	#define PSW_DEFAULT "default"
	#define DEFAULT_ID_SESSION 0
	#define DEFAULT_PSW_SESSION "password"
	#define DEFAULT_MENU nolog
	#define DEFAULT_SCELTA 0
#endif//DEFINIZIONI

#ifndef ENUMERATORI
#define ENUMERATORI/**< Blocco contenente le definizioni di tutti gli enumeratori.*/
/**
 * @brief
 * Indica il campo scelto nella selezione di ordinamento.
 */
typedef enum campo_record {nome, cognome, id, alfabetico, prezzo} campo_record_t;

/**
 * @brief
 * Indica il tipo di menu' che deve essere visualizzato.
 */
typedef enum menu {nolog, log, novita, prodotti, clienti, dipendenti, ordinamentoP, ordinamentoC, ordinamentoD} menu_t;

/**
 * @brief
 * Indica il file da usare.
 */
typedef enum tipofile {prod, clie, dipe} tipofile_t;// prodotti, clienti, dipendenti
#endif//ENUMERATORI

#ifndef STRUTTURE
#define STRUTTURE/**< Blocco contenente le definizioni di tutte le strutture.*/
/**
 *@brief Struttura dati Prodotto.
 */
typedef struct prodotto{
	int quantita;
	unsigned long id;
	char *nome;
	double prezzo;
} prodotto_t;

/**
 * @brief Struttura dati Persona.
 */
typedef struct persona{
	unsigned int id; //range  [0, 65,535]
	char *nome;
	char *cognome;
	char *psw;/**< Password, nel caso di un cliente sara' sempre "".*/
}persona_t;

/**
 * @brief Struttura dati Sessione.
 */
typedef struct sessione{
	unsigned int id;/**< Password del dipendente con cui si e' fatto l'accesso.*/
	int scelta;/**< Ultima scelta effettutata dall'utente.*/
	menu_t menu;/**< Indica quale in quale menu' si e'.*/
	char *psw;/**< Password del dipendente con cui si e' fatto l'accesso.*/
}sessione_t;

/**
 * @brief
 * Struttura dati tree_noode.\n
 * Definisce un nodo di un albero binario.\n
 * Puo' essere usato sia per le strutture prodotti che per quelle persona, per questo e' presente un componente per ciascuno.
 */
typedef struct tree_node{
	prodotto_t *prodotto;
	persona_t *persona;
	struct tree_node *left;
	struct tree_node *right;
} tree_node_t;
#endif//STRUTTURE

//inizializza una variabile di tipo sessione ponendo tutti i suoi valori a quelli definiti di default
void initialize_session(sessione_t *se);

//inizializza solo le componenti di una variabile persona, ATTENZIONE: non pone valori impostabili ma solo quelli di default del tipo delle variabili
void initialize_prodotto(prodotto_t *p);

//inizializza solo le componenti di una variabile persona, ATTENZIONE: non pone valori impostabili ma solo quelli di default del tipo delle variabili
void initialize_persona(persona_t *p);

//estrae da stdin una stringa di lunghezza massima data, pone sempre \0 alla fine, se ci sono piu' caratteri di quelli ammissibili svuota anche il buffer
void get_string(char *stringa,int lunghezza);

//estrae da fp una stringa di lunghezza data
void get_string_file(char *stringa,int lunghezza,FILE *fp);// si considera fp gia open

//carica in persona i dati di una riga del file dipendenti.txt o clienti.txt, si ipotizza di essere nella posizione giusta
void get_persona(persona_t *persona, FILE * fp);

//carica in prodotto i dati di una riga del file prodotti.txt, si ipotizza di essere nella posizione giusta
void get_prodotto(prodotto_t *prodotto, FILE * fp);

//controlla se tutti i caratteri della stringa sono numeri interi senza segno ritornando 0 se e' vero o -1 se e' falso
int is_unum(char *stringa);

//controlla se tutti i caratteri della stringa sono numeri interi ritornando 0 se e' vero o -1 se e' falso
int is_num(char *stringa);

//controlla se la stringa contiene un numero eventualmente anche con decimali
int is_ufnum(char *stringa);
