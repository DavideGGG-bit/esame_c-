#ifndef TRANSAZIONE_H
#define TRANSAZIONE_H

#include <string>

using namespace std;

class Transazione {
private:
    string descrizione;  // Descrizione della transazione
    double importo;      // Importo (positivo per entrate, negativo per uscite)
    string data;         // Data in formato YYYY-MM-DD

public:
    // Costruttore
    Transazione(const string& desc, double imp, const string& dt);
    
    // Costruttore di default
    Transazione();
    
    // Getter
    string getDescrizione() const;
    double getImporto() const;
    string getData() const;
    
    // Setter
    void setDescrizione(const string& desc);
    void setImporto(double imp);
    void setData(const string& dt);
    
    // Metodo per convertire la transazione in stringa per il salvataggio
    string toString() const;
    
    // Metodo per creare una transazione da una stringa
    static Transazione fromString(const string& str);
    
    // Metodo per verificare se la transazione contiene una parola chiave
    bool contieneParolaChiave(const string& parola) const;
};

#endif // TRANSAZIONE_H