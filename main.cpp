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

// Funzione per validare il formato della data (controllo completo)
bool validaData(const string& data) {
    if (data.length() != 10) return false;
    if (data[4] != '-' || data[7] != '-') return false;
    
    // Controllo che siano numeri nelle posizioni giuste
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(data[i])) return false;
    }
    
    // Estrai anno, mese e giorno
    string annoStr = data.substr(0, 4);
    string meseStr = data.substr(5, 2);
    string giornoStr = data.substr(8, 2);
    
    int anno = stoi(annoStr);
    int mese = stoi(meseStr);
    int giorno = stoi(giornoStr);
    
    // Validazione mese (1-12)
    if (mese < 1 || mese > 12) {
        cout << "Errore: Il mese deve essere compreso tra 1 e 12!" << endl;
        return false;
    }
    
    // Validazione giorno (1-31)
    if (giorno < 1 || giorno > 31) {
        cout << "Errore: Il giorno deve essere compreso tra 1 e 31!" << endl;
        return false;
    }
    
    // Validazione giorni per mesi specifici
    if (mese == 2) {  // Febbraio
        // Controllo anno bisestile semplificato
        bool bisestile = (anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0);
        if (giorno > (bisestile ? 29 : 28)) {
            cout << "Errore: Febbraio " << (bisestile ? "ha massimo 29 giorni!" : "ha massimo 28 giorni!") << endl;
            return false;
        }
    } else if (mese == 4 || mese == 6 || mese == 9 || mese == 11) {  // Aprile, Giugno, Settembre, Novembre
        if (giorno > 30) {
            cout << "Errore: Questo mese ha massimo 30 giorni!" << endl;
            return false;
        }
    }
    
    return true;
}

// Funzione per aggiungere una nuova transazione
void aggiungiTransazione(ContoCorrente& conto) {
    string descrizione, data;
    double importo;
    
    cout << "\n=== AGGIUNGI TRANSAZIONE ===" << endl;
    
    // Input descrizione con validazione
    do {
        cout << "Descrizione: ";
        cin.ignore();  // Pulisce il buffer
        getline(cin, descrizione);
        
        if (descrizione.empty()) {
            cout << "Errore: La descrizione non può essere vuota!" << endl;
        }
    } while (descrizione.empty());
    
    // Input importo con validazione
    bool importoValido = false;
    do {
        cout << "Importo (positivo per entrate, negativo per uscite): ";
        if (cin >> importo) {
            importoValido = true;
        } else {
            cout << "Errore: Inserisci un numero valido!" << endl;
            cin.clear();  // Pulisce lo stato di errore
            cin.ignore(1000, '\n');  // Ignora i caratteri rimasti nel buffer
        }
    } while (!importoValido);
    
    // Input data con validazione
    bool dataValida = false;
    do {
        cout << "Data (YYYY-MM-DD): ";
        cin >> data;
        
        if (validaData(data)) {
            dataValida = true;
        } else {
            cout << "Formato data non valido! Usa YYYY-MM-DD (es. 2024-12-31)" << endl;
        }
    } while (!dataValida);
    
    conto.aggiungiTransazione(descrizione, importo, data);
    cout << "Transazione aggiunta con successo!" << endl;
}

// Funzione per cercare transazioni per data
void cercaPerData(const ContoCorrente& conto) {
    string data;
    bool dataValida = false;
    
    do {
        cout << "\nInserisci la data da cercare (YYYY-MM-DD): ";
        cin >> data;
        
        if (validaData(data)) {
            dataValida = true;
        } else {
            cout << "Formato data non valido! Usa YYYY-MM-DD (es. 2024-12-31)" << endl;
        }
    } while (!dataValida);
    
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
    
    do {
        cout << "\nInserisci la parola chiave da cercare: ";
        cin.ignore();
        getline(cin, parola);
        
        if (parola.empty()) {
            cout << "Errore: La parola chiave non può essere vuota!" << endl;
        }
    } while (parola.empty());
    
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
        
        // Validazione input del menu
        bool sceltaValida = false;
        do {
            if (cin >> scelta) {
                if (scelta >= 0 && scelta <= 7) {
                    sceltaValida = true;
                } else {
                    cout << "Opzione non valida! Inserisci un numero tra 0 e 7: ";
                }
            } else {
                cout << "Errore: Inserisci un numero valido (0-7): ";
                cin.clear();  // Pulisce lo stato di errore
                cin.ignore(1000, '\n');  // Ignora i caratteri rimasti nel buffer
            }
        } while (!sceltaValida);
        
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