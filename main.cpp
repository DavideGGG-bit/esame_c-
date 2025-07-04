#include <iostream>
#include <string>
#include <vector>
#include "lib/contocorrente.h"
#include "lib/transazione.h"

using namespace std;

// Funzione per mostrare il menu principale
void mostraMenu() {
    cout << "\n=== GESTIONE CONTO CORRENTE ===" << endl;
    cout << "1. Aggiungi transazione" << endl;
    cout << "2. Visualizza tutte le transazioni" << endl;
    cout << "3. Visualizza saldo" << endl;
    cout << "4. Cerca per data" << endl;
    cout << "5. Cerca per parola chiave" << endl;
    cout << "6. Salva su file" << endl;
    cout << "7. Riepilogo conto" << endl;
    cout << "0. Esci" << endl;
    cout << "Scegli un'opzione: ";
}

// Funzione per validare il formato della data (semplice controllo)
bool validaData(const string& data) {
    if (data.length() != 10) return false;
    if (data[4] != '-' || data[7] != '-') return false;
    
    // Controllo basic che siano numeri nelle posizioni giuste
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(data[i])) return false;
    }
    return true;
}

// Funzione per aggiungere una nuova transazione
void aggiungiTransazione(ContoCorrente& conto) {
    string descrizione, data;
    double importo;
    
    cout << "\n=== AGGIUNGI TRANSAZIONE ===" << endl;
    
    cout << "Descrizione: ";
    cin.ignore();  // Pulisce il buffer
    getline(cin, descrizione);
    
    cout << "Importo (positivo per entrate, negativo per uscite): ";
    cin >> importo;
    
    cout << "Data (YYYY-MM-DD): ";
    cin >> data;
    
    // Validazione della data
    if (!validaData(data)) {
        cout << "Formato data non valido! Usa YYYY-MM-DD" << endl;
        return;
    }
    
    conto.aggiungiTransazione(descrizione, importo, data);
    cout << "Transazione aggiunta con successo!" << endl;
}

// Funzione per cercare transazioni per data
void cercaPerData(const ContoCorrente& conto) {
    string data;
    cout << "\nInserisci la data da cercare (YYYY-MM-DD): ";
    cin >> data;
    
    if (!validaData(data)) {
        cout << "Formato data non valido! Usa YYYY-MM-DD" << endl;
        return;
    }
    
    vector<Transazione> risultati = conto.cercaPerData(data);
    
    if (risultati.empty()) {
        cout << "Nessuna transazione trovata per la data " << data << endl;
    } else {
        cout << "\nTransazioni del " << data << ":" << endl;
        for (const Transazione& t : risultati) {
            cout << "- " << t.getDescrizione() << ": " << t.getImporto() << " €" << endl;
        }
    }
}

// Funzione per cercare transazioni per parola chiave
void cercaPerParolaChiave(const ContoCorrente& conto) {
    string parola;
    cout << "\nInserisci la parola chiave da cercare: ";
    cin.ignore();
    getline(cin, parola);
    
    vector<Transazione> risultati = conto.cercaPerParolaChiave(parola);
    
    if (risultati.empty()) {
        cout << "Nessuna transazione trovata con la parola \"" << parola << "\"" << endl;
    } else {
        cout << "\nTransazioni contenenti \"" << parola << "\":" << endl;
        for (const Transazione& t : risultati) {
            cout << "- " << t.getData() << ": " << t.getDescrizione() 
                 << " (" << t.getImporto() << " €)" << endl;
        }
    }
}

int main() {
    cout << "Benvenuto nel sistema di gestione conto corrente!" << endl;
    
    // Crea il conto corrente (carica automaticamente dal file)
    ContoCorrente conto("../data/dati.txt");
    
    int scelta;
    do {
        mostraMenu();
        cin >> scelta;
        
        switch (scelta) {
            case 1:
                aggiungiTransazione(conto);
                break;
            case 2:
                conto.stampaTransazioni();
                break;
            case 3:
                cout << "\nSaldo attuale: " << conto.calcolaSaldo() << " €" << endl;
                break;
            case 4:
                cercaPerData(conto);
                break;
            case 5:
                cercaPerParolaChiave(conto);
                break;
            case 6:
                conto.salvaSuFile();
                break;
            case 7:
                conto.stampaRiepilogo();
                break;
            case 0:
                cout << "\nSalvataggio automatico..." << endl;
                conto.salvaSuFile();
                cout << "Arrivederci!" << endl;
                break;
            default:
                cout << "Opzione non valida!" << endl;
        }
        
    } while (scelta != 0);
    
    return 0;
}