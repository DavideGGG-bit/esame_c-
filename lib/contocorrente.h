#ifndef CONTOCORRENTE_H
#define CONTOCORRENTE_H

#include "transazione.h"
#include <vector>
#include <string>

using namespace std;

/**
 * @brief Classe che gestisce un conto corrente con transazioni
 * 
 * La classe ContoCorrente gestisce una lista di transazioni,
 * fornisce funzionalità di ricerca, calcolo del saldo e 
 * persistenza dei dati su file.
 */
class ContoCorrente {
private:
    vector<Transazione> transazioni;  /**< Lista delle transazioni */
    string nomeFile;                  /**< Nome del file per salvare/caricare i dati */

public:
    /**
     * @brief Costruttore del conto corrente
     * @param file Nome del file per salvare/caricare le transazioni (default: "../data/dati.txt")
     * 
     * Il costruttore carica automaticamente le transazioni dal file specificato
     */
    ContoCorrente(const string& file = "../data/dati.txt");
    
    /**
     * @brief Aggiunge una transazione esistente al conto
     * @param t Transazione da aggiungere
     */
    void aggiungiTransazione(const Transazione& t);
    
    /**
     * @brief Aggiunge una nuova transazione al conto
     * @param desc Descrizione della transazione
     * @param importo Importo della transazione (positivo per entrate, negativo per uscite)
     * @param data Data della transazione in formato YYYY-MM-DD
     */
    void aggiungiTransazione(const string& desc, double importo, const string& data);
    
    /**
     * @brief Calcola il saldo totale del conto
     * @return double Saldo totale (somma di tutti gli importi)
     */
    double calcolaSaldo() const;
    
    /**
     * @brief Cerca transazioni per data specifica
     * @param data Data da cercare in formato YYYY-MM-DD
     * @return vector<Transazione> Vettore delle transazioni trovate
     */
    vector<Transazione> cercaPerData(const string& data) const;
    
    /**
     * @brief Cerca transazioni per parola chiave nella descrizione
     * @param parola Parola chiave da cercare
     * @return vector<Transazione> Vettore delle transazioni che contengono la parola
     * 
     * La ricerca è case-insensitive
     */
    vector<Transazione> cercaPerParolaChiave(const string& parola) const;
    
    /**
     * @brief Carica le transazioni dal file
     * 
     * Legge le transazioni dal file specificato nel costruttore.
     * Se il file non esiste, non fa nulla e stampa un messaggio informativo.
     */
    void caricaDaFile();
    
    /**
     * @brief Salva le transazioni su file
     * 
     * Salva tutte le transazioni nel file specificato nel costruttore.
     * Crea la directory se non esiste.
     */
    void salvaSuFile() const;
    
    /**
     * @brief Restituisce tutte le transazioni
     * @return vector<Transazione> Copia del vettore delle transazioni
     */
    vector<Transazione> getTransazioni() const;
    
    /**
     * @brief Restituisce il numero di transazioni
     * @return int Numero totale di transazioni
     */
    int getNumeroTransazioni() const;
    
    /**
     * @brief Stampa tutte le transazioni in formato tabellare
     * 
     * Mostra data, importo e descrizione di ogni transazione
     * in una tabella formattata
     */
    void stampaTransazioni() const;
    
    /**
     * @brief Stampa un riepilogo completo del conto
     * 
     * Mostra: numero transazioni, saldo attuale, totale entrate, totale uscite
     */
    void stampaRiepilogo() const;
};

#endif // CONTOCORRENTE_H