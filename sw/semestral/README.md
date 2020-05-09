
### **Semestrální práce z BI-PA2, LS 2020** 
# **Dotazování v relační algebře** 

## 1. Úvodní zadání práce
Dotazování pomocí SQL dotazů je dnes běžnou praxí, ale relační algebra příliš podporovaná není. A přitom určité typy dotazů jsou snáze realizovatelné v RA než v SQL. A proto je vhodné mít k dispozici program, který tyto dotazy podporuje.

### **Implementujte následující funkcionalitu**
1. **Relační operátory**: selekce, projekce, přejmenování, přirozené spojení, obecné spojení
2. **Množinové operace**: sjednocení, průnik, rozdíl, kartézský součin
3. Položení dotazu v relační algebře a zobrazení výsledku dotazu
4. Implementujte možnost uložit dotaz do proměnné a využít jej jako poddotaz v jiném dotazu.
5. Převod dotazu v **relační algebře** na validní **SQL**

### **Nástroj musí dále splňovat alespoň 1 následující funkcionalitu:**
1. Podpora **CSV** formátu pro import a export dat do tabulek
2. Přímé napojení na databázi

### **Kde lze využít polymorfismus? (doporučené)**
- Operátory RA a množinové operace
- Parsování RA výrazu
- Pokud se vám zdá využití polymorfismu málo, přidejte víc podporovaných formátů (xml, json, sql)

### **Další informace**
Vzhledem k nutnosti vytvoření překladu z RA do SQL je vhodné vytvořit napojení přímo na databázi.
https://en.wikipedia.org/wiki/Relational_algebra

## 2. Vlastní návrh implementace
### **Import a export**
###### Moduly: CFileManager, CDataParser, CDatabase, CLog
Program bude implementován ve formě konzolové aplikace. Konfigurace databázových souborů bude fungovat následovně:
- V kořenové složce je soubor **sources.cfg** obsahující odkaz na existující soubory (musí podporovat CSV formátování).  Tyto soubory program naimportuje, pokud existují. Pokud existovat nebudou, program je zaregistruje jako vadné, ale neukončí se.
- Po dokončení importu program informuje o tom, které tabulky byly úspěšně naimportovány. Pouze se správně naimportovanými tabulkami lze nadále pracovat.

Pro správné fungování např. projekce bude nutné specifikovat i datové typy jednotlivých sloupců. K tomu slouží první řádek každého importované tabulky. Podporované datové typy budou  **int**, **string**, **double**. Právě podle těchto sloupců se program rozhoduje, jestli je zbytek záznamů ve správném formátu (jak datovým typem sloupců, tak i jejich součtem).

Druhý řádek tabulky je vždy považován jako hlavička a definice sloupců. Tabulka musí mít alespoň jeden záznam.

**Ukázka**

**`sources.cfg`** obsahuje **`data/uzivatele.csv`**

**`data/uzivatele.csv`**

| string          | string                | int       |
| :-------------: |:---------------------:|:---------:|
| **jmeno**       | **email**             | **vek**   |
| Jan             | novajan@fit.cvut.cz   | 20        |
| Petr            | novpet@fit.cvut.cz    | 24        |

### **Relační operátory a operace s množinami**
###### Moduly: CConsole, CDatabase, CTable, CCell
Po úspěšném načtení alespoň jedné tabulky program přejde do stavu dotazování uživatele. Každá operace definovaná RA má svojí syntaxi, která musí být dodržena. Následující shrnutí definuje tyto dotazy.

- ##### selekce
sfapofsapofas

- ##### projekce
sdpaodpsa

- ##### přirozené spojení
dadasdsad

- ##### obecné spojení
dsadsad

Každý z těchto dotazů musí být ukončen středníkem.
K této základní sadě příkazů této aplikace se bude uživatel kdykoli schopen dostat příkazem **help**.

### **Využití polymorfismu**
###### Moduly: CCell, CString, CDouble, CInt
Polymorfismus bude využíván ve třídě **CCell**. Obecně nevíme, jaké datové typy budou dané sloupce mít. Tabulka pouze drží odkazy na tyto buňky, dovnitř vkládáme konkrétní dynamicky alokované potomky zapouzdrující string, int a double, které z CCell dědí.   

### **Testy**
Ve složce **test** budou uloženy tabulky/asserty sloužící k testování různých funkcionalit.
Pro spuštění testu stačí konkrétní test přidat do **sources.cfg** a spustit přes zkompilovanou verzi programu.