/**
 * @file
 * Contiene le definizioni dei metodi che potrebbero essere usati da altri file.
 */
#include "head_strutture.h"

//crea il file dipendenti e ci aggiunge il record di default
void insert_record_default();

//cerca nel file dipendenti.txt una corrispondenza, se il file non esiste o e' vuoto viene chiamato il metodo per creare il file e aggiungere il record di default
int match_access(unsigned int id, char *psw);

//inserisce un record nel file scelto come parametro enumeratore, e' compresa anche l'interfaccia per l'utente
/*Il metodo e' stato scritto utilizzando le funzioni w e r+ di fopen non sapendo che l'append (a+) avrebbe semplificato
 * molto le cose, si e' andati infatti a simulare il funzionamento (ormai avevo gia' finito quando l'ho scoperto)*/
int insert_record(tipofile_t tipo);

//elimina il record col determinato id, restituisce -1 se non esiste, 0 se esiste
int delete_record(tipofile_t tipo);

//edita il record col determinato id, restituisce -1 se non esiste, 0 se esiste
int edit_record(tipofile_t tipo);

//stampa su stdin il contenuto del fine novita.txt se ne ha, altrimenti stampa un messaggio standard
void print_novita();

//permette di sostituire il messaggio novita
void set_novita();

//permette la modifica del campo quantita', in maniera incrementale, velocemente
int incremental_add_prodotti();
