#include <gtest/gtest.h>
#include "../lib/transazione.h"
#include "../lib/contocorrente.h"

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