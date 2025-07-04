#include <gtest/gtest.h>
#include "../lib/transazione.h"
#include "../lib/contocorrente.h"
#include <chrono>

using namespace std;

// Test per la classe Transazione
class TransazioneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup che viene eseguito prima di ogni test
    }
};

// Test costruttore e getter della classe Transazione
TEST_F(TransazioneTest, CostruttoreEGetter) {
    Transazione t("Stipendio", 1500.0, "2024-01-15");
    
    EXPECT_EQ(t.getDescrizione(), "Stipendio");
    EXPECT_DOUBLE_EQ(t.getImporto(), 1500.0);
    EXPECT_EQ(t.getData(), "2024-01-15");
}

// Test costruttore di default
TEST_F(TransazioneTest, CostruttoreDefault) {
    Transazione t;
    
    EXPECT_EQ(t.getDescrizione(), "");
    EXPECT_DOUBLE_EQ(t.getImporto(), 0.0);
    EXPECT_EQ(t.getData(), "");
}

// Test setter
TEST_F(TransazioneTest, Setter) {
    Transazione t;
    
    t.setDescrizione("Spesa");
    t.setImporto(-50.0);
    t.setData("2024-01-16");
    
    EXPECT_EQ(t.getDescrizione(), "Spesa");
    EXPECT_DOUBLE_EQ(t.getImporto(), -50.0);
    EXPECT_EQ(t.getData(), "2024-01-16");
}

// Test toString e fromString
TEST_F(TransazioneTest, ToStringFromString) {
    Transazione originale("Benzina", -45.50, "2024-01-17");
    string str = originale.toString();
    
    Transazione copia = Transazione::fromString(str);
    
    EXPECT_EQ(copia.getDescrizione(), "Benzina");
    EXPECT_DOUBLE_EQ(copia.getImporto(), -45.50);
    EXPECT_EQ(copia.getData(), "2024-01-17");
}

// Test ricerca parola chiave
TEST_F(TransazioneTest, ContieneParolaChiave) {
    Transazione t("Spesa al supermercato", -35.0, "2024-01-18");
    
    EXPECT_TRUE(t.contieneParolaChiave("spesa"));
    EXPECT_TRUE(t.contieneParolaChiave("SUPERMERCATO"));
    EXPECT_TRUE(t.contieneParolaChiave("mercato"));
    EXPECT_FALSE(t.contieneParolaChiave("benzina"));
}

// Test ricerca parola chiave con stringhe vuote e casi limite
TEST_F(TransazioneTest, ContieneParolaChiaveCasiLimite) {
    Transazione t("Test", 100.0, "2024-01-01");
    
    EXPECT_TRUE(t.contieneParolaChiave("Test"));
    EXPECT_TRUE(t.contieneParolaChiave("test"));
    EXPECT_TRUE(t.contieneParolaChiave("TEST"));
    EXPECT_TRUE(t.contieneParolaChiave("es"));
    EXPECT_TRUE(t.contieneParolaChiave("")); // Stringa vuota dovrebbe essere trovata
    EXPECT_FALSE(t.contieneParolaChiave("xyz"));
}

// Test serializzazione con valori decimali precisi
TEST_F(TransazioneTest, SerializzazioneDecimali) {
    Transazione t("Test decimali", 123.456, "2024-01-01");
    string str = t.toString();
    
    // Verifica che l'importo sia formattato con 2 cifre decimali
    EXPECT_TRUE(str.find("123.46") != string::npos);
    
    Transazione copia = Transazione::fromString(str);
    // Usa EXPECT_NEAR per confrontare valori float con tolleranza
    EXPECT_NEAR(copia.getImporto(), 123.456, 0.01);
}

// Test serializzazione con importi negativi
TEST_F(TransazioneTest, SerializzazioneImportiNegativi) {
    Transazione t("Spesa", -99.99, "2024-12-31");
    string str = t.toString();
    
    EXPECT_TRUE(str.find("-99.99") != string::npos);
    
    Transazione copia = Transazione::fromString(str);
    EXPECT_DOUBLE_EQ(copia.getImporto(), -99.99);
    EXPECT_EQ(copia.getDescrizione(), "Spesa");
    EXPECT_EQ(copia.getData(), "2024-12-31");
}

// Test gestione stringhe con caratteri speciali
TEST_F(TransazioneTest, CaratteriSpeciali) {
    Transazione t("Spesa supermercato con sconti", -50.0, "2024-01-01");
    string str = t.toString();
    
    Transazione copia = Transazione::fromString(str);
    EXPECT_EQ(copia.getDescrizione(), "Spesa supermercato con sconti");
}

// Test per la classe ContoCorrente
class ContoCorrenteTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Usa un file di test temporaneo
        conto = new ContoCorrente("test_data.txt");
    }
    
    void TearDown() override {
        delete conto;
        // Rimuovi il file di test (opzionale)
        remove("test_data.txt");
    }
    
    ContoCorrente* conto;
};

// Test aggiunta transazioni
TEST_F(ContoCorrenteTest, AggiungiTransazione) {
    EXPECT_EQ(conto->getNumeroTransazioni(), 0);
    
    conto->aggiungiTransazione("Test", 100.0, "2024-01-19");
    EXPECT_EQ(conto->getNumeroTransazioni(), 1);
    
    Transazione t("Test2", -50.0, "2024-01-20");
    conto->aggiungiTransazione(t);
    EXPECT_EQ(conto->getNumeroTransazioni(), 2);
}

// Test calcolo saldo
TEST_F(ContoCorrenteTest, CalcoloSaldo) {
    EXPECT_DOUBLE_EQ(conto->calcolaSaldo(), 0.0);
    
    conto->aggiungiTransazione("Entrata", 100.0, "2024-01-19");
    conto->aggiungiTransazione("Uscita", -30.0, "2024-01-20");
    conto->aggiungiTransazione("Entrata2", 50.0, "2024-01-21");
    
    EXPECT_DOUBLE_EQ(conto->calcolaSaldo(), 120.0);
}

// Test ricerca per data
TEST_F(ContoCorrenteTest, RicercaPerData) {
    conto->aggiungiTransazione("Trans1", 100.0, "2024-01-19");
    conto->aggiungiTransazione("Trans2", -50.0, "2024-01-20");
    conto->aggiungiTransazione("Trans3", 25.0, "2024-01-19");
    
    vector<Transazione> risultati = conto->cercaPerData("2024-01-19");
    EXPECT_EQ(risultati.size(), 2);
    
    risultati = conto->cercaPerData("2024-01-20");
    EXPECT_EQ(risultati.size(), 1);
    
    risultati = conto->cercaPerData("2024-01-21");
    EXPECT_EQ(risultati.size(), 0);
}

// Test ricerca per parola chiave
TEST_F(ContoCorrenteTest, RicercaPerParolaChiave) {
    conto->aggiungiTransazione("Spesa supermercato", -50.0, "2024-01-19");
    conto->aggiungiTransazione("Benzina auto", -45.0, "2024-01-20");
    conto->aggiungiTransazione("Stipendio lavoro", 1500.0, "2024-01-21");
    
    vector<Transazione> risultati = conto->cercaPerParolaChiave("spesa");
    EXPECT_EQ(risultati.size(), 1);
    
    risultati = conto->cercaPerParolaChiave("auto");
    EXPECT_EQ(risultati.size(), 1);
    
    risultati = conto->cercaPerParolaChiave("casa");
    EXPECT_EQ(risultati.size(), 0);
}

// Test salvataggio e caricamento (test di integrazione semplice)
TEST_F(ContoCorrenteTest, SalvataggioCaricamento) {
    // Aggiungi alcune transazioni
    conto->aggiungiTransazione("Test1", 100.0, "2024-01-19");
    conto->aggiungiTransazione("Test2", -50.0, "2024-01-20");
    
    // Salva
    conto->salvaSuFile();
    
    // Crea un nuovo conto che dovrebbe caricare le transazioni
    ContoCorrente nuovoConto("test_data.txt");
    
    EXPECT_EQ(nuovoConto.getNumeroTransazioni(), 2);
    EXPECT_DOUBLE_EQ(nuovoConto.calcolaSaldo(), 50.0);
}

// Test con transazioni con importi zero
TEST_F(ContoCorrenteTest, ImportiZero) {
    conto->aggiungiTransazione("Nessun movimento", 0.0, "2024-01-01");
    
    EXPECT_EQ(conto->getNumeroTransazioni(), 1);
    EXPECT_DOUBLE_EQ(conto->calcolaSaldo(), 0.0);
}

// Test con molte transazioni
TEST_F(ContoCorrenteTest, MolteTransazioni) {
    // Aggiungi 100 transazioni
    for (int i = 1; i <= 100; i++) {
        conto->aggiungiTransazione("Transazione " + to_string(i), i * 10.0, "2024-01-01");
    }
    
    EXPECT_EQ(conto->getNumeroTransazioni(), 100);
    EXPECT_DOUBLE_EQ(conto->calcolaSaldo(), 50500.0); // Somma di 10+20+...+1000
}

// Test ricerca per data con risultati multipli
TEST_F(ContoCorrenteTest, RicercaPerDataMultipla) {
    string data = "2024-01-15";
    
    conto->aggiungiTransazione("Prima", 100.0, data);
    conto->aggiungiTransazione("Seconda", -50.0, data);
    conto->aggiungiTransazione("Terza", 25.0, data);
    conto->aggiungiTransazione("Altra data", 200.0, "2024-01-16");
    
    vector<Transazione> risultati = conto->cercaPerData(data);
    EXPECT_EQ(risultati.size(), 3);
    
    // Verifica che tutte le transazioni trovate abbiano la data corretta
    for (const auto& t : risultati) {
        EXPECT_EQ(t.getData(), data);
    }
}

// Test ricerca per parola chiave con case sensitivity
TEST_F(ContoCorrenteTest, RicercaParolaChiaveCaseSensitive) {
    conto->aggiungiTransazione("SPESA SUPERMERCATO", -50.0, "2024-01-01");
    conto->aggiungiTransazione("spesa benzina", -45.0, "2024-01-02");
    conto->aggiungiTransazione("Spesa Ristorante", -30.0, "2024-01-03");
    conto->aggiungiTransazione("Stipendio", 1500.0, "2024-01-04");
    
    vector<Transazione> risultati = conto->cercaPerParolaChiave("spesa");
    EXPECT_EQ(risultati.size(), 3); // Dovrebbe trovare tutte e 3 indipendentemente dal case
    
    risultati = conto->cercaPerParolaChiave("SPESA");
    EXPECT_EQ(risultati.size(), 3);
    
    risultati = conto->cercaPerParolaChiave("Spesa");
    EXPECT_EQ(risultati.size(), 3);
}

// Test calcolo saldo con mix di entrate e uscite
TEST_F(ContoCorrenteTest, CalcoloSaldoMisto) {
    conto->aggiungiTransazione("Stipendio", 2000.0, "2024-01-01");
    conto->aggiungiTransazione("Affitto", -800.0, "2024-01-02");
    conto->aggiungiTransazione("Spesa", -150.0, "2024-01-03");
    conto->aggiungiTransazione("Bonus", 300.0, "2024-01-04");
    conto->aggiungiTransazione("Bolletta", -100.0, "2024-01-05");
    
    EXPECT_DOUBLE_EQ(conto->calcolaSaldo(), 1250.0);
}

// Test con descrizioni vuote (caso limite)
TEST_F(ContoCorrenteTest, DescrizioniVuote) {
    conto->aggiungiTransazione("", 100.0, "2024-01-01");
    conto->aggiungiTransazione("Normale", -50.0, "2024-01-02");
    
    EXPECT_EQ(conto->getNumeroTransazioni(), 2);
    EXPECT_DOUBLE_EQ(conto->calcolaSaldo(), 50.0);
    
    // Test ricerca con descrizione vuota - dovrebbe trovare tutte le transazioni
    vector<Transazione> risultati = conto->cercaPerParolaChiave("");
    EXPECT_EQ(risultati.size(), 2); // Stringa vuota è contenuta in tutte le stringhe
}

// Test salvataggio e caricamento con formattazione corretta
TEST_F(ContoCorrenteTest, SalvataggioFormattazione) {
    conto->aggiungiTransazione("Test formato", 123.456, "2024-01-01");
    conto->aggiungiTransazione("Test negativo", -99.999, "2024-01-02");
    
    conto->salvaSuFile();
    
    ContoCorrente nuovoConto("test_data.txt");
    
    EXPECT_EQ(nuovoConto.getNumeroTransazioni(), 2);
    
    vector<Transazione> transazioni = nuovoConto.getTransazioni();
    // Usa EXPECT_NEAR per confrontare valori float con tolleranza
    EXPECT_NEAR(transazioni[0].getImporto(), 123.456, 0.01);
    EXPECT_NEAR(transazioni[1].getImporto(), -99.999, 0.01);
}

// Test performance con grandi quantità di dati
TEST_F(ContoCorrenteTest, PerformanceGrandiDati) {
    auto start = chrono::high_resolution_clock::now();
    
    // Aggiungi 1000 transazioni
    for (int i = 1; i <= 1000; i++) {
        conto->aggiungiTransazione("Transazione " + to_string(i), 
                                 (i % 2 == 0) ? i * 10.0 : -i * 5.0, 
                                 "2024-01-01");
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    EXPECT_EQ(conto->getNumeroTransazioni(), 1000);
    EXPECT_LT(duration.count(), 100); // Dovrebbe completare in meno di 100ms
}

// Test ricerca con date non esistenti
TEST_F(ContoCorrenteTest, RicercaDataInesistente) {
    conto->aggiungiTransazione("Test", 100.0, "2024-01-01");
    
    vector<Transazione> risultati = conto->cercaPerData("2024-12-31");
    EXPECT_EQ(risultati.size(), 0);
    
    risultati = conto->cercaPerData("2023-01-01");
    EXPECT_EQ(risultati.size(), 0);
}

// Test per le funzioni di validazione
class ValidazioneTest : public ::testing::Test {
protected:
    // Replica della funzione validaData dal main per testare
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
        if (mese < 1 || mese > 12) return false;
        
        // Validazione giorno (1-31)
        if (giorno < 1 || giorno > 31) return false;
        
        // Validazione giorni per mesi specifici
        if (mese == 2) {  // Febbraio
            // Controllo anno bisestile semplificato
            bool bisestile = (anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0);
            if (giorno > (bisestile ? 29 : 28)) return false;
        } else if (mese == 4 || mese == 6 || mese == 9 || mese == 11) {  // Aprile, Giugno, Settembre, Novembre
            if (giorno > 30) return false;
        }
        
        return true;
    }
};

// Test validazione date corrette
TEST_F(ValidazioneTest, DateValide) {
    EXPECT_TRUE(validaData("2024-01-01"));
    EXPECT_TRUE(validaData("2024-12-31"));
    EXPECT_TRUE(validaData("2000-02-29")); // Anno bisestile
    EXPECT_TRUE(validaData("2024-02-29")); // Anno bisestile
    EXPECT_TRUE(validaData("2023-02-28")); // Anno non bisestile
    EXPECT_TRUE(validaData("2024-04-30")); // Aprile con 30 giorni
    EXPECT_TRUE(validaData("2024-06-30")); // Giugno con 30 giorni
    EXPECT_TRUE(validaData("2024-09-30")); // Settembre con 30 giorni
    EXPECT_TRUE(validaData("2024-11-30")); // Novembre con 30 giorni
}

// Test validazione date non valide
TEST_F(ValidazioneTest, DateNonValide) {
    // Formato sbagliato
    EXPECT_FALSE(validaData("2024/01/01"));
    EXPECT_FALSE(validaData("01-01-2024"));
    EXPECT_FALSE(validaData("2024-1-1"));
    EXPECT_FALSE(validaData("24-01-01"));
    EXPECT_FALSE(validaData("2024-01-1"));
    EXPECT_FALSE(validaData(""));
    EXPECT_FALSE(validaData("2024-01"));
    EXPECT_FALSE(validaData("2024-01-01-01"));
    
    // Caratteri non numerici
    EXPECT_FALSE(validaData("abcd-01-01"));
    EXPECT_FALSE(validaData("2024-ab-01"));
    EXPECT_FALSE(validaData("2024-01-ab"));
    
    // Mesi non validi
    EXPECT_FALSE(validaData("2024-00-01"));
    EXPECT_FALSE(validaData("2024-13-01"));
    EXPECT_FALSE(validaData("2024-99-01"));
    
    // Giorni non validi
    EXPECT_FALSE(validaData("2024-01-00"));
    EXPECT_FALSE(validaData("2024-01-32"));
    EXPECT_FALSE(validaData("2024-01-99"));
    
    // Date specifiche non valide
    EXPECT_FALSE(validaData("2023-02-29")); // Non bisestile
    EXPECT_FALSE(validaData("2024-02-30")); // Febbraio non ha 30 giorni
    EXPECT_FALSE(validaData("2024-04-31")); // Aprile non ha 31 giorni
    EXPECT_FALSE(validaData("2024-06-31")); // Giugno non ha 31 giorni
    EXPECT_FALSE(validaData("2024-09-31")); // Settembre non ha 31 giorni
    EXPECT_FALSE(validaData("2024-11-31")); // Novembre non ha 31 giorni
}

// Test anni bisestili
TEST_F(ValidazioneTest, AnniBisestili) {
    // Anni bisestili
    EXPECT_TRUE(validaData("2000-02-29")); // Divisibile per 400
    EXPECT_TRUE(validaData("2004-02-29")); // Divisibile per 4
    EXPECT_TRUE(validaData("2024-02-29")); // Divisibile per 4
    
    // Anni non bisestili
    EXPECT_FALSE(validaData("1900-02-29")); // Divisibile per 100 ma non per 400
    EXPECT_FALSE(validaData("2023-02-29")); // Non divisibile per 4
    EXPECT_FALSE(validaData("2021-02-29")); // Non divisibile per 4
}

// Test edge cases per gli anni
TEST_F(ValidazioneTest, EdgeCasesAnni) {
    EXPECT_TRUE(validaData("0001-01-01")); // Anno minimo
    EXPECT_TRUE(validaData("9999-12-31")); // Anno massimo
    EXPECT_TRUE(validaData("1000-06-15")); // Anno a 4 cifre
}