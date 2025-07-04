#include "contocorrente.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

using namespace std;

/**
 * @brief Costruttore del conto corrente
 * @param file Nome del file per la persistenza dei dati
 * 
 * Imposta il nome del file e carica automaticamente le transazioni esistenti
 */
ContoCorrente::ContoCorrente(const string& file) : nomeFile(file) {
    caricaDaFile();  // Carica le transazioni all'avvio
}

/**
 * @brief Aggiunge una transazione esistente al conto
 * @param t Transazione da aggiungere
 * 
 * Aggiunge la transazione al vettore delle transazioni
 */
void ContoCorrente::aggiungiTransazione(const Transazione& t) {
    transazioni.push_back(t);
}

/**
 * @brief Aggiunge una nuova transazione con i parametri specificati
 * @param desc Descrizione della transazione
 * @param importo Importo della transazione
 * @param data Data della transazione
 * 
 * Crea una nuova transazione e la aggiunge al vettore
 */
void ContoCorrente::aggiungiTransazione(const string& desc, double importo, const string& data) {
    Transazione t(desc, importo, data);
    transazioni.push_back(t);
}

/**
 * @brief Calcola il saldo totale sommando tutti gli importi
 * @return double Saldo totale del conto
 * 
 * Somma algebrica di tutti gli importi delle transazioni
 */
double ContoCorrente::calcolaSaldo() const {
    double saldo = 0.0;
    for (const Transazione& t : transazioni) {
        saldo += t.getImporto();
    }
    return saldo;
}

/**
 * @brief Cerca transazioni per data specifica
 * @param data Data da cercare in formato YYYY-MM-DD
 * @return vector<Transazione> Vettore delle transazioni trovate
 * 
 * Confronta la data esatta di ogni transazione con quella specificata
 */
vector<Transazione> ContoCorrente::cercaPerData(const string& data) const {
    vector<Transazione> risultati;
    for (const Transazione& t : transazioni) {
        if (t.getData() == data) {
            risultati.push_back(t);
        }
    }
    return risultati;
}

/**
 * @brief Cerca transazioni per parola chiave nella descrizione
 * @param parola Parola chiave da cercare
 * @return vector<Transazione> Vettore delle transazioni che contengono la parola
 * 
 * Utilizza il metodo contieneParolaChiave di ogni transazione
 */
vector<Transazione> ContoCorrente::cercaPerParolaChiave(const string& parola) const {
    vector<Transazione> risultati;
    for (const Transazione& t : transazioni) {
        if (t.contieneParolaChiave(parola)) {
            risultati.push_back(t);
        }
    }
    return risultati;
}

/**
 * @brief Carica le transazioni dal file specificato
 * 
 * Legge il file riga per riga e converte ogni riga in una transazione
 * usando il metodo fromString. Gestisce errori di lettura e formato.
 */
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

/**
 * @brief Salva le transazioni su file
 * 
 * Crea la directory se non esiste, poi salva ogni transazione
 * usando il metodo toString. Gestisce errori di creazione directory
 * e di scrittura file.
 */
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

/**
 * @brief Getter per tutte le transazioni
 * @return vector<Transazione> Copia del vettore delle transazioni
 */
vector<Transazione> ContoCorrente::getTransazioni() const {
    return transazioni;
}

/**
 * @brief Getter per il numero di transazioni
 * @return int Numero totale di transazioni
 */
int ContoCorrente::getNumeroTransazioni() const {
    return transazioni.size();
}

/**
 * @brief Stampa tutte le transazioni in formato tabellare
 * 
 * Mostra le transazioni in una tabella formattata con colonne:
 * Data, Importo, Descrizione
 */
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

/**
 * @brief Stampa un riepilogo completo del conto
 * 
 * Mostra: numero transazioni, saldo attuale, totale entrate, totale uscite
 * Calcola separatamente entrate (importi positivi) e uscite (importi negativi)
 */
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