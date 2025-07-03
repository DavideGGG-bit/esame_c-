#include "transazione.h"
#include <sstream>
#include <cctype>

using namespace std;

// Costruttore
Transazione::Transazione(const string& desc, double imp, const string& dt) 
    : descrizione(desc), importo(imp), data(dt) {
}

// Costruttore di default
Transazione::Transazione() : descrizione(""), importo(0.0), data("") {
}

// Getter
string Transazione::getDescrizione() const {
    return descrizione;
}

double Transazione::getImporto() const {
    return importo;
}

string Transazione::getData() const {
    return data;
}

// Setter
void Transazione::setDescrizione(const string& desc) {
    descrizione = desc;
}

void Transazione::setImporto(double imp) {
    importo = imp;
}

void Transazione::setData(const string& dt) {
    data = dt;
}

// Converte la transazione in stringa per il salvataggio su file
string Transazione::toString() const {
    return descrizione + ";" + to_string(importo) + ";" + data;
}

// Crea una transazione da una stringa letta dal file
Transazione Transazione::fromString(const string& str) {
    stringstream ss(str);
    string desc, importoStr, dt;
    
    // Separa i campi usando il punto e virgola come delimitatore
    getline(ss, desc, ';');
    getline(ss, importoStr, ';');
    getline(ss, dt, ';');
    
    double importo = stod(importoStr);
    
    return Transazione(desc, importo, dt);
}

// Verifica se la transazione contiene una parola chiave nella descrizione
bool Transazione::contieneParolaChiave(const string& parola) const {
    // Converte tutto in minuscolo per confronto case-insensitive
    string descLower = descrizione;
    string parolaLower = parola;
    
    // Converte in minuscolo
    for (char& c : descLower) {
        c = tolower(c);
    }
    for (char& c : parolaLower) {
        c = tolower(c);
    }
    
    // Cerca la parola nella descrizione
    return descLower.find(parolaLower) != string::npos;
}