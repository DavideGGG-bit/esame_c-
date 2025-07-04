#include "contocorrente.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

using namespace std;

// Costruttore
ContoCorrente::ContoCorrente(const string& file) : nomeFile(file) {
    caricaDaFile();  // Carica le transazioni all'avvio
}

// Aggiunge una transazione esistente
void ContoCorrente::aggiungiTransazione(const Transazione& t) {
    transazioni.push_back(t);
}

// Aggiunge una nuova transazione con i parametri
void ContoCorrente::aggiungiTransazione(const string& desc, double importo, const string& data) {
    Transazione t(desc, importo, data);
    transazioni.push_back(t);
}

// Calcola il saldo totale sommando tutti gli importi
double ContoCorrente::calcolaSaldo() const {
    double saldo = 0.0;
    for (const Transazione& t : transazioni) {
        saldo += t.getImporto();
    }
    return saldo;
}

// Cerca transazioni per data
vector<Transazione> ContoCorrente::cercaPerData(const string& data) const {
    vector<Transazione> risultati;
    for (const Transazione& t : transazioni) {
        if (t.getData() == data) {
            risultati.push_back(t);
        }
    }
    return risultati;
}

// Cerca transazioni per parola chiave nella descrizione
vector<Transazione> ContoCorrente::cercaPerParolaChiave(const string& parola) const {
    vector<Transazione> risultati;
    for (const Transazione& t : transazioni) {
        if (t.contieneParolaChiave(parola)) {
            risultati.push_back(t);
        }
    }
    return risultati;
}

// Carica le transazioni dal file
void ContoCorrente::caricaDaFile() {
    ifstream file(nomeFile);
    if (!file.is_open()) {
        cout << "File " << nomeFile << " non trovato. Sarà creato al primo salvataggio." << endl;
        return;
    }
    
    string linea;
    int count = 0;
    while (getline(file, linea)) {
        if (!linea.empty()) {
            try {
                Transazione t = Transazione::fromString(linea);
                transazioni.push_back(t);
                count++;
            } catch (const exception& e) {
                cout << "Errore nel caricamento della linea: " << linea << endl;
            }
        }
    }
    file.close();
    cout << "Caricate " << count << " transazioni dal file." << endl;
}

// Salva le transazioni su file
void ContoCorrente::salvaSuFile() const {
    // Crea la directory se non esiste
    filesystem::path filePath(nomeFile);
    filesystem::path directory = filePath.parent_path();
    
    if (!directory.empty() && !filesystem::exists(directory)) {
        try {
            filesystem::create_directories(directory);
            cout << "Directory creata: " << directory << endl;
        } catch (const filesystem::filesystem_error& e) {
            cout << "Errore nella creazione della directory: " << e.what() << endl;
            return;
        }
    }
    
    ofstream file(nomeFile);
    if (!file.is_open()) {
        cout << "Errore nell'apertura del file per la scrittura!" << endl;
        return;
    }
    
    for (const Transazione& t : transazioni) {
        file << t.toString() << endl;
    }
    file.close();
    cout << "Transazioni salvate nel file " << nomeFile << endl;
}

// Getter
vector<Transazione> ContoCorrente::getTransazioni() const {
    return transazioni;
}

int ContoCorrente::getNumeroTransazioni() const {
    return transazioni.size();
}

// Stampa tutte le transazioni
void ContoCorrente::stampaTransazioni() const {
    if (transazioni.empty()) {
        cout << "Nessuna transazione presente." << endl;
        return;
    }
    
    cout << "\n=== ELENCO TRANSAZIONI ===" << endl;
    cout << setw(12) << "Data" << setw(15) << "Importo" << "  Descrizione" << endl;
    cout << string(50, '-') << endl;
    
    for (const Transazione& t : transazioni) {
        cout << setw(12) << t.getData() 
             << setw(15) << fixed << setprecision(2) << t.getImporto() 
             << "  " << t.getDescrizione() << endl;
    }
}

// Stampa un riepilogo del conto
void ContoCorrente::stampaRiepilogo() const {
    cout << "\n=== RIEPILOGO CONTO ===" << endl;
    cout << "Numero transazioni: " << transazioni.size() << endl;
    cout << "Saldo attuale: " << fixed << setprecision(2) << calcolaSaldo() << " €" << endl;
    
    // Calcola entrate e uscite separate
    double entrate = 0.0, uscite = 0.0;
    for (const Transazione& t : transazioni) {
        if (t.getImporto() > 0) {
            entrate += t.getImporto();
        } else {
            uscite += t.getImporto();
        }
    }
    cout << "Totale entrate: " << entrate << " €" << endl;
    cout << "Totale uscite: " << uscite << " €" << endl;
}