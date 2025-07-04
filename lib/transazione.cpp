#include "transazione.h"
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

/**
 * @brief Costruttore parametrico
 * @param desc Descrizione della transazione
 * @param imp Importo della transazione
 * @param dt Data della transazione
 */
Transazione::Transazione(const string& desc, double imp, const string& dt) 
    : descrizione(desc), importo(imp), data(dt) {
}

/**
 * @brief Costruttore di default
 * 
 * Inizializza tutti i campi con valori di default
 */
Transazione::Transazione() : descrizione(""), importo(0.0), data("") {
}

/**
 * @brief Getter per la descrizione
 * @return string Descrizione della transazione
 */
string Transazione::getDescrizione() const {
    return descrizione;
}

/**
 * @brief Getter per l'importo
 * @return double Importo della transazione
 */
double Transazione::getImporto() const {
    return importo;
}

/**
 * @brief Getter per la data
 * @return string Data della transazione
 */
string Transazione::getData() const {
    return data;
}

/**
 * @brief Setter per la descrizione
 * @param desc Nuova descrizione
 */
void Transazione::setDescrizione(const string& desc) {
    descrizione = desc;
}

/**
 * @brief Setter per l'importo
 * @param imp Nuovo importo
 */
void Transazione::setImporto(double imp) {
    importo = imp;
}

/**
 * @brief Setter per la data
 * @param dt Nuova data
 */
void Transazione::setData(const string& dt) {
    data = dt;
}

/**
 * @brief Converte la transazione in stringa per il salvataggio su file
 * @return string Stringa formattata con separatori punto e virgola
 * 
 * Formato: "descrizione;importo.xx;YYYY-MM-DD"
 * L'importo viene formattato con precisione a 2 cifre decimali
 */
string Transazione::toString() const {
    // Formatta l'importo con 2 cifre decimali
    stringstream ss;
    ss << fixed << setprecision(2) << importo;
    return descrizione + ";" + ss.str() + ";" + data;
}

/**
 * @brief Crea una transazione da una stringa letta dal file
 * @param str Stringa nel formato "descrizione;importo;data"
 * @return Transazione Nuova transazione creata
 * @throws std::invalid_argument Se la conversione dell'importo fallisce
 * 
 * Utilizza il punto e virgola come separatore dei campi
 */
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

/**
 * @brief Verifica se la transazione contiene una parola chiave nella descrizione
 * @param parola Parola chiave da cercare
 * @return bool true se la parola è contenuta nella descrizione, false altrimenti
 * 
 * La ricerca è case-insensitive: converte sia la descrizione che la parola
 * in minuscolo prima del confronto
 */
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