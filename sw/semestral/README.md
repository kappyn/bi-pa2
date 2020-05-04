
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
Program bude implementován ve formě konzolové aplikace. Konfigurace databázových souborů bude fungovat následovně:
- V kořenové složce je soubor **sources.cfg** obsahující odkaz na existující **.csv** soubory. Tyto soubory program naimportuje, pokud existují. Pokud existovat nebudou, program je zaregistruje jako vadné, ale program neukončí.
- Po dokončení importu program informuje o tom, které tabulky byly úspěšně naimportovány. Pouze se správně naimportovanými tabulkami lze nadále pracovat.

Program nebude tolerovat prázdné buňky - pokud se jedná o řetězce, musí být náležitě označeny uvozovkami bez obsahu. Při detekování takovéto buňky program automaticky vyhodí chybovou hlášku. Jméno tabulky je vždy název importovaného souboru, pokud nezvolí uživatel jinak.


### **Relační operátory a operace s množinami**
Po úspěšném importu tabulek může uživatel zadávat následující dotazy:
- selekce
- projekce
- přirozené spojení
- obecné spojení

Každý z těchto dotazů musí být ukončen středníkem.
K této základní sadě příkazů této aplikace se bude uživatel kdykoli schopen dostat příkazem **help**. Při volání těchto operací program nebude case sensitive.

### **Využití polymorfismu**

### **Testování**

Ve složce test budou uloženy základní testy.

