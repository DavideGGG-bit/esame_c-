#ifndef CONTOCORRENTE_H
#define CONTOCORRENTE_H

#include "transazione.h"
#include <vector>
#include <string>

using namespace std;

class ContoCorrente {
private:
    vector<Transazione> transazioni;  // Lista delle transazioni
    string nomeFile;                  // Nome del file per salvare/caricare i dati

public:
    // Costruttore
    ContoCorrente(const string& file = "data/dati.txt");
    
    // Metodi per gestire le transazioni
    void aggiungiTransazione(const Transazione& t);
    void aggiungiTransazione(const string& desc, double importo, const string& data);
    
    // Calcola il saldo totale
    double calcolaSaldo() const;
    
    // Ricerca transazioni
    vector<Transazione> cercaPerData(const string& data) const;
    vector<Transazione> cercaPerParolaChiave(const string& parola) const;
    
    // Gestione file
    void caricaDaFile();
    void salvaSuFile() const;
    
    // Getter
    vector<Transazione> getTransazioni() const;
    int getNumeroTransazioni() const;
    
    // Metodi di utilità
    void stampaTransazioni() const;
    void stampaRiepilogo() const;
};

#endif // CONTOCORRENTE_H