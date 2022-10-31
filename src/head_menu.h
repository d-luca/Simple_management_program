/**
 * @file
 * Contiene le definizioni dei metodi che potrebbero essere usati da altri file\n.
 * In particolare sono presenti i metodi inerenti alla parte di visualizzazione e gestione dei menu.
 */

#include "head_file.h"
#include "head_ordinamento.h"

//visualizza il menu' per l'utente che non ha effettuato l'accesso
void show_nolog_menu();

//visualizza il menu' per l'utente che ha effettuato l'acceso
void show_log_menu();

//visualizza il menu' della gestione dei record dei prodotti, e' accessibile solo se loggato perche' da non loggato non si passa per un menu' del genere
void show_prodotti_menu();

//visualizza il menu' della sezione novita'
void show_novita_menu();

//visualizza il menu' della gestione dei record dei clienti
void show_clienti_menu();

//visualizza il menu' della gestione dei record dei dipendenti
void show_dipendenti_menu();

//visualizza il menu' per la scelta dell'ordinamento dei prodotti
void show_ordinamento_prodotti_menu();

//visualizza il menu' per la scelta dell'ordinamento dei record clienti o dipendenti
void show_ordinamento_persone_menu();

//prende in input la scelta dell'utente assicurandosi che sia un valore corretto
int choise(menu_t m);

//gestisce che metodi chiamare o che menu visualizzare in base a al menu passato e alla scelta dell'utente all'interno di quel menu
int menu_nextmove(sessione_t *se);
