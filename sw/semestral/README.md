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

Pro správné fungování např. projekce bude nutné specifikovat i datové typy jednotlivých sloupců. K tomu bude sloužit první řádek každé importované tabulky. Podporované datové typy budou  **int**, **string**, **double**. Právě podle těchto sloupců se program rozhoduje, jestli je zbytek záznamů ve správném formátu (jak datovým typem sloupců, tak i jejich součtem).

Druhý řádek tabulky bude považován jako hlavička a definice sloupců. Tabulka musí mít alespoň jeden záznam.

**Ukázka**

**`sources.cfg`** obsahuje **`data/uzivatele.csv`**

**`data/uzivatele.csv`** obsahuje:


```
int, string, string, string, string, double, int
id, first_name, last_name, email, gender, ratio, overall_score
1, Venita,Gower, vgower0@360.cn, Female, 40.46981,47
2, Roanne,Howsam, rhowsam1@baidu.com, Female, 14.49089, 94

# vygenerovaná data - https://mockaroo.com/
```

### **Relační operátory a operace s množinami**
###### Moduly: CConsole, CDatabase, CTable, CCell
Po úspěšném načtení alespoň jedné tabulky program přejde do stavu dotazování uživatele. Každá operace definovaná RA má svojí syntaxi, která musí být dodržena. Následující shrnutí definuje tyto dotazy.

---

##### Selekce
Popis operace: Vybere konkrétní sloupce z tabulky.\
Použití: ``SEL[ "jmeno_sloupce", "jmeno_sloupce", ... ]( "jmeno_tabulky" );`` 

---

##### Projekce
Popis operace: Vybere záznamy tabulky na základě kritérií.\
Použití: ``PRO[ podminka ]( "jmeno_tabulky" );``\
Podmínka musí být ve tvaru: ``atribut λ konstanta``, kde `λ` je vztah ``<,>,<=,=>,==``. Operandy nelze zaměnit.

---

##### Přirozené spojení
Popis operace: Hledá společný sloupec dvou tabulek, pokud jej najde, výsledkem je tabulka se sloupci z obou těchto tabulek se záznamy, které mají stejnou hodnotu ve společném sloupci.\
Použití: ``NJOIN( "jmeno_tabulky", "jmeno_tabulky" );``

---

##### Obecné spojení
Popis operace: Spojení dvou tabulek na základě sloupce, který uživatel definuje.\
Použití: ``JOIN[ "jmeno_sloupce" ]( "jmeno_tabulky", "jmeno_tabulky" );``

---

##### Sjednocení
Popis operace: Množinové sloučení všech záznamů ze dvou tabulek. Obě tabulky musí mít stejné sloupce.\
Použití: ``UNION( "jmeno_tabulky", "jmeno_tabulky" );``

---

##### Průnik
Popis operace: Množinový průnik záznamů ze dvou tabulek. Obě tabulky musí mít stejné sloupce.\
Použití: ``INTERSECT( "jmeno_tabulky", "jmeno_tabulky" );``

---

##### Rozdíl
Popis operace: Množinový rozdíl záznamů tabulky A s tabulkou B. Obě tabulky musí mít stejné sloupce.\
Použití: ``MINUS( "jmeno_tabulky", "jmeno_tabulky" );``

---

##### Kartézský součin (Cartesian product)
Popis operace: Spojení dvou tabulek se všemi sloupci, záznamy jsou množinou všech možných n-tic záznamů tabulky A se záznamy z tabulky B.\
Použití: ``CP( "jmeno_tabulky", "jmeno_tabulky" );``

---

##### Ukládání dotazu do proměnné
Popis operace: Vytvoří dočasnou kopii dotazu a uloží jej do paměti pod názvem proměnné.\
Použití: ``SEL[ vek >= 18 ]( uzivatele ) === dospeli``

---

Každý z těchto dotazů musí být ukončen středníkem.
K této základní sadě příkazů této aplikace se bude uživatel kdykoli schopen dostat příkazem **HELP**.

### **Využití polymorfismu**
###### Moduly: CCell, CString, CDouble, CInt, CTable, CTableQuery, CBinaryOperator, CUnaryOperator

- První využití polymorfismu bude ve třídě **CCell**. Obecně nevíme, jaké datové typy budou dané sloupce mít. Tabulka pouze drží odkazy na tyto buňky, dovnitř vkládáme konkrétní dynamicky alokované potomky (využíváme dynamické vazbu) zapouzdrující podporované datové typy (string, int, double), kteří CCell implementují. Tyto jednotlivé typy buněk se budou lišit například v porovnávání (lexikografické, přesnost na desetinná čísla). Tyto třídy jsou tedy připraveny na jakoukoliv další nástavbu (například úplně vlastní objekt).

---

- _Další využitím bude systém dotazování. Implementace této polymorfní části programu ještě není do podrobna jasná, protože se hodně odvíjí od ostatních částí programu, které ji předcházejí (načítání tabulek, rozhraní na dotazování). Proto tuto část považuji hlavně jako úvahu, jak by to mohlo v budoucnu vypadat:_\
 Každý dotaz má jiné chování a výsledek. Může být například unární/binární, a je nutné, aby byl program schopen všechny dotazy ukládat. Je tedy na místě, aby každý z dotazů vycházel z nějaké abstraktní třídy a svou zbylou charakteristiku doimplementoval (_doteď pouze dědičnost_). Při volání poddotazu v aktuálním dotazu už se program nebude zajímat o to, s jakým konkrétním dotazem zrovna pracuje. Hrubá představa hierarchie tříd je ve složce **src/query**.
   
### **Testy**
Ve složce **test** budou uloženy tabulky/asserty sloužící k testování různých částí programu.
Pro spuštění testu stačí konkrétní test přidat do **sources.cfg**.