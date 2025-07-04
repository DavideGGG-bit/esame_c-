#ifndef TRANSAZIONE_H
#define TRANSAZIONE_H

#include <string>

using namespace std;

/**
 * @brief Classe che rappresenta una singola transazione finanziaria
 * 
 * La classe Transazione gestisce una singola operazione finanziaria 
 * con descrizione, importo e data. Fornisce metodi per la serializzazione
 * e la ricerca per parole chiave.
 */
class Transazione {
private:
    string descrizione;  /**< Descrizione della transazione */
    double importo;      /**< Importo (positivo per entrate, negativo per uscite) */
    string data;         /**< Data in formato YYYY-MM-DD */

public:
    /**
     * @brief Costruttore parametrico
     * @param desc Descrizione della transazione
     * @param imp Importo della transazione (positivo per entrate, negativo per uscite)
     * @param dt Data della transazione in formato YYYY-MM-DD
     */
    Transazione(const string& desc, double imp, const string& dt);
    
    /**
     * @brief Costruttore di default
     * 
     * Inizializza una transazione vuota con descrizione e data vuote
     * e importo pari a 0.0
     */
    Transazione();
    
    /**
     * @brief Restituisce la descrizione della transazione
     * @return string Descrizione della transazione
     */
    string getDescrizione() const;
    
    /**
     * @brief Restituisce l'importo della transazione
     * @return double Importo della transazione
     */
    double getImporto() const;
    
    /**
     * @brief Restituisce la data della transazione
     * @return string Data in formato YYYY-MM-DD
     */
    string getData() const;
    
    /**
     * @brief Imposta la descrizione della transazione
     * @param desc Nuova descrizione della transazione
     */
    void setDescrizione(const string& desc);
    
    /**
     * @brief Imposta l'importo della transazione
     * @param imp Nuovo importo della transazione
     */
    void setImporto(double imp);
    
    /**
     * @brief Imposta la data della transazione
     * @param dt Nuova data in formato YYYY-MM-DD
     */
    void setData(const string& dt);
    
    /**
     * @brief Converte la transazione in stringa per il salvataggio
     * @return string Stringa formattata con descrizione;importo;data
     * 
     * Formato: "descrizione;importo.xx;YYYY-MM-DD"
     * L'importo viene formattato con 2 cifre decimali
     */
    string toString() const;
    
    /**
     * @brief Crea una transazione da una stringa
     * @param str Stringa nel formato "descrizione;importo;data"
     * @return Transazione Nuova transazione creata dalla stringa
     * @throws std::invalid_argument Se la stringa non è nel formato corretto
     */
    static Transazione fromString(const string& str);
    
    /**
     * @brief Verifica se la transazione contiene una parola chiave
     * @param parola Parola chiave da cercare nella descrizione
     * @return bool true se la parola è trovata, false altrimenti
     * 
     * La ricerca è case-insensitive
     */
    bool contieneParolaChiave(const string& parola) const;
};

#endif // TRANSAZIONE_H