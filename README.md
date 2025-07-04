# Sistema di Gestione Conto Corrente

## Descrizione
Questo progetto implementa un sistema di gestione per un conto corrente bancario sviluppato in C++. 
Il programma permette di gestire transazioni finanziarie, calcolare saldi, effettuare ricerche e salvare i dati su file.

## Funzionalità principali
- Aggiunta di transazioni (entrate e uscite)
- Visualizzazione di tutte le transazioni
- Calcolo del saldo attuale
- Ricerca transazioni per data
- Ricerca transazioni per parola chiave nella descrizione
- Salvataggio automatico su file
- Riepilogo completo del conto

## Struttura del progetto
```
esame c++/
├── CMakeLists.txt              # File di configurazione CMake principale
├── main.cpp                    # Programma principale con menu interattivo
├── README.md                   # Documentazione del progetto
├── .gitignore                  # File per escludere cartelle dal versioning
├── lib/                        # Libreria con le classi principali
│   ├── CMakeLists.txt          # Configurazione CMake per la libreria
│   ├── transazione.h           # Header della classe Transazione
│   ├── transazione.cpp         # Implementazione della classe Transazione
│   ├── contocorrente.h         # Header della classe ContoCorrente
│   └── contocorrente.cpp       # Implementazione della classe ContoCorrente
├── test/                       # Test unitari
│   ├── CMakeLists.txt          # Configurazione CMake per i test
│   ├── main.cpp                # Entry point per i test
│   └── test_contocorrente.cpp  # Test unitari per le classi
├── data/                       # Directory per i file di dati
│   └── dati.txt                # File di salvataggio delle transazioni
└── build/                      # Directory per i file compilati, verra creata automaticamente 
```

## Descrizione delle classi

### Classe Transazione
La classe `Transazione` rappresenta una singola operazione finanziaria.

**Attributi privati:**
- `string descrizione`: Descrizione della transazione
- `double importo`: Importo della transazione (positivo per entrate, negativo per uscite)
- `string data`: Data della transazione in formato YYYY-MM-DD

**Metodi pubblici:**
- `Transazione(const string& desc, double imp, const string& dt)`: Costruttore parametrico
- `Transazione()`: Costruttore di default
- `string getDescrizione() const`: Restituisce la descrizione
- `double getImporto() const`: Restituisce l'importo
- `string getData() const`: Restituisce la data
- `void setDescrizione(const string& desc)`: Imposta la descrizione
- `void setImporto(double imp)`: Imposta l'importo
- `void setData(const string& dt)`: Imposta la data
- `string toString() const`: Converte la transazione in stringa per il salvataggio
- `static Transazione fromString(const string& str)`: Crea una transazione da stringa
- `bool contieneParolaChiave(const string& parola) const`: Verifica se contiene una parola chiave

### Classe ContoCorrente
La classe `ContoCorrente` gestisce un insieme di transazioni e le operazioni associate.

**Attributi privati:**
- `vector<Transazione> transazioni`: Lista delle transazioni del conto
- `string nomeFile`: Nome del file per il salvataggio persistente

**Metodi pubblici:**
- `ContoCorrente(const string& file)`: Costruttore che carica i dati dal file specificato
- `void aggiungiTransazione(const Transazione& t)`: Aggiunge una transazione esistente
- `void aggiungiTransazione(const string& desc, double importo, const string& data)`: Crea e aggiunge una nuova transazione
- `double calcolaSaldo() const`: Calcola il saldo totale del conto
- `vector<Transazione> cercaPerData(const string& data) const`: Cerca transazioni per data
- `vector<Transazione> cercaPerParolaChiave(const string& parola) const`: Cerca transazioni per parola chiave
- `void caricaDaFile()`: Carica le transazioni dal file
- `void salvaSuFile() const`: Salva le transazioni su file
- `vector<Transazione> getTransazioni() const`: Restituisce tutte le transazioni
- `int getNumeroTransazioni() const`: Restituisce il numero di transazioni
- `void stampaTransazioni() const`: Stampa tutte le transazioni formattate
- `void stampaRiepilogo() const`: Stampa un riepilogo del conto

## Dipendenze
- **C++ Standard Library**: iostream, string, vector, fstream, iomanip, filesystem, sstream, cctype
- **Google Test Framework**: Per i test unitari (libgtest-dev)
- **CMake**: Sistema di build (versione 3.10 o superiore)

## Compilazione ed esecuzione

### Prerequisiti
```bash
sudo apt-get install cmake build-essential libgtest-dev
```

### Compilazione
```bash
mkdir build
cd build
cmake ..
make
```

### Esecuzione
```bash
# Eseguire il programma principale
./main

# Eseguire i test unitari
./test/runAllTests
```

## Formato dei dati
Le transazioni vengono salvate nel file `data/dati.txt` nel seguente formato:
```
descrizione;importo;data
```

Esempio:
```
Stipendio;1500.00;2024-01-15
Spesa supermercato;-45.30;2024-01-16
```

## Validazione degli input
Il programma include validazione completa per:
- **Date**: Formato YYYY-MM-DD con controllo di mesi validi (1-12), giorni validi (1-31), e gestione anni bisestili
- **Importi**: Controllo che siano numeri validi
- **Descrizioni**: Controllo che non siano vuote
- **Menu**: Validazione delle opzioni selezionate
