### Semestrální práce z BI-PA2, LS 2020

# Dotazování v relační algebře

## Úvodní zadání práce
Dotazování pomocí SQL dotazů je dnes běžnou praxí, ale relační algebra příliš podporovaná není. A přitom určité typy dotazů jsou snáze realizovatelné v RA než v SQL. A proto je vhodné mít k dispozici program, který tyto dotazy podporuje.

### Implementujte následující funkcionalitu
1. **Relační operátory**: selekce, projekce, přejmenování, přirozené spojení, obecné spojení
2. **Množinové operace**: sjednocení, průnik, rozdíl, kartézský součin
3. Položení dotazu v relační algebře a zobrazení výsledku dotazu
4. Implementujte možnost uložit dotaz do proměnné a využít jej jako poddotaz v jiném dotazu.
5. Převod dotazu v **relační algebře** na validní **SQL**

### Nástroj musí dále splňovat alespoň 1 následující funkcionalitu:
1. Podpora **CSV** formátu pro import a export dat do tabulek
2. Přímé napojení na databázi

### Kde lze využít polymorfismus? (doporučené)
- Operátory RA a množinové operace
- Parsování RA výrazu
- Pokud se vám zdá využití polymorfismu málo, přidejte víc podporovaných formátů (xml, json, sql)

### Další informace
Vzhledem k nutnosti vytvoření překladu z RA do SQL je vhodné vytvořit napojení přímo na databázi.
https://en.wikipedia.org/wiki/Relational_algebra

## Vlastní návrh implementace
### Import a export
Program bude implementován ve formě konzolové aplikace. Konfigurace databázových souborů bude fungovat následovně:
- V kořenové složce je soubor **sources.cfg** obsahující odkaz na existující soubory (musí podporovat CSV formátování).  Tyto soubory program naimportuje, pokud existují. Pokud existovat nebudou, program je zaregistruje jako vadné, ale neukončí se.
- Po dokončení importu program informuje o tom, které tabulky byly úspěšně naimportovány. Pouze se správně naimportovanými tabulkami lze nadále pracovat.

Pro správné fungování např. projekce bude nutné specifikovat i datové typy jednotlivých sloupců. K tomu bude sloužit první řádek každé importované tabulky. Podporované datové typy budou  **int**, **string**, **double**. Právě podle těchto sloupců se program rozhoduje, jestli je zbytek záznamů ve správném formátu (jak datovým typem sloupců, tak i jejich součtem).

Druhý řádek tabulky bude považován jako hlavička a definice sloupců. Tabulka musí mít alespoň jeden záznam.

#### Ukázka

**`sources.cfg`** obsahuje **`data/uzivatele.csv`**

**`data/uzivatele.csv`** obsahuje:


```
int, string, string, string, string, double, int
id, first_name, last_name, email, gender, ratio, overall_score
1, Venita,Gower, vgower0@360.cn, Female, 40.46981,47
2, Roanne,Howsam, rhowsam1@baidu.com, Female, 14.49089, 94

# vygenerovaná data - https://mockaroo.com/
```

### Relační operátory a operace s množinami
Po úspěšném načtení alespoň jedné tabulky program přejde do stavu dotazování uživatele. Každá operace definovaná RA má svojí syntaxi, která musí být dodržena. Následující shrnutí definuje tyto dotazy. 

Na mezerách v dotazech nezáleží, pokud to není v operaci přesněji definováno. Správné závorkování musí být dodrženo vždy.

---

#### Rozhraní (není součástí původního zadání):

``TABLES;`` vypíše tabulky, které jsou momentálně v databázi.

``QUERIES;`` vypíše dotazy, které jsou momentálně uloženy v databázi.

``IMPORTED;`` vypíše záznamy tabulek, které byly importovány.

``SAVED;`` vypíše záznamy tabulek, které byly uloženy jako poddotaz.

``EXPORT;`` vyexportuje výsledky všech uložených dotazů do CSV.

``QUIT;`` ukončí aplikaci.

---

#### Selekce
Popis operace: Vybere konkrétní sloupce z tabulky.

Použití: ``SEL[ jmeno_sloupce_1, jmeno_sloupce_2, .. ]( jmeno_tabulky );`` 

---

#### Přejmenování
Popis operace: Vybere sloupec z tabulky a přejmenuje ho.

Použití: ``ALIAS[ jmeno_sloupce, novy_nazev ]( jmeno_tabulky );`` 

---

#### Projekce
Popis operace: Vybere záznamy tabulky na základě kritérií.

Použití: ``PRO[ podminka ]( jmeno_tabulky );``

Podmínka musí být ve tvaru: ``sloupec λ konstanta``, kde `λ` je vztah ``<,>,>=,=>,==,!=``. Operandy nelze zaměnit.

Konstanty jsou buď ve formátu číselném (např. **666**, **3.14**), nebo textovém (**"Maxipes"** - lze i bez uvozovek, pokud hledaný výraz má pouze jedno slovo). Program se pokusí o konverzi na typ, který mají buňky vybraného sloupce.

---

#### Přirozené spojení
Popis operace: Hledá společný sloupec dvou tabulek, pokud jej najde, výsledkem je tabulka se sloupci z obou těchto tabulek se záznamy, které mají stejnou hodnotu ve společném sloupci.

Použití: ``NJOIN( jmeno_tabulky, jmeno_tabulky );``

---

#### Obecné spojení
Popis operace: Spojení dvou tabulek na základě sloupce, který uživatel definuje.

Použití: ``JOIN[ jmeno_sloupce ]( jmeno_tabulky, jmeno_tabulky );``

---

#### Sjednocení
Popis operace: Množinové sloučení všech záznamů ze dvou tabulek. Obě tabulky musí mít stejné sloupce.

Použití: ``UNION( jmeno_tabulky, jmeno_tabulky );``

---

#### Průnik
Popis operace: Množinový průnik záznamů ze dvou tabulek. Obě tabulky musí mít stejné sloupce.

Použití: ``INTERSECT( jmeno_tabulky, jmeno_tabulky );``

---

#### Rozdíl
Popis operace: Množinový rozdíl záznamů tabulky A s tabulkou B. Obě tabulky musí mít stejné sloupce.

Použití: ``MINUS( jmeno_tabulky, jmeno_tabulky );``

---

#### Kartézský součin (Cartesian product)
Popis operace: Spojení dvou tabulek se všemi sloupci, záznamy jsou množinou všech možných n-tic záznamů tabulky A se záznamy z tabulky B.

Použití: ``CP( jmeno_tabulky, jmeno_tabulky );``

---

#### Ukládání dotazu do proměnné
Popis operace: Vytvoří dočasnou kopii dotazu a uloží jej do paměti pod názvem proměnné. Proměnná lze poté použít na místě pro "jmeno_tabulky" v definicích dotazů výše. 

Příkladné použití: 

``PRO[ vek >= 18 ]( uzivatele ) ~ dospeli``

``PRO[ auto == "BMW" ]( uzivatele ) ~ dospeliBMW``

``// Výsledkem budou dospělí lidé s BMW.``

``INTERSECT( dospeli, dospeliBMW )``

---

Každý z těchto dotazů musí být ukončen středníkem.

## Využití polymorfismu

#### Polymorfismus při zapouzdření objektů v buňkách tabulek
- První využití polymorfismu bude ve třídě **CCell**. Obecně nevíme, jaké datové typy budou dané sloupce mít. Tabulka pouze drží odkazy na tyto buňky, dovnitř vkládáme konkrétní dynamicky alokované potomky (využíváme dynamickou vazbu) zapouzdrující podporované datové typy (string, int, double), kteří CCell implementují. Tyto jednotlivé typy buněk se budou lišit například v porovnávání (lexikografické, přesnost na desetinná čísla). Tyto třídy jsou tedy připraveny na jakoukoliv další nástavbu (například úplně vlastní objekt).

#### Polymorfismus při dotazování - různé chování, různé výsledky
- Každý dotaz má jiné chování a výsledek. Může být například unární/binární, má jiné argumenty a strukturu - je přesto nutné, aby byl program schopen dotazy jakéhokoliv typu ukládat. Je tedy na místě, aby každý z dotazů vycházel z nějaké abstraktní třídy a svou zbylou charakteristiku doimplementoval (třída **CTableQuery**). Při volání poddotazu v aktuálním dotazu už se program nebude zajímat o to, s jakým konkrétním dotazem zrovna pracuje.
       
## Testování
Ve složce **examples** budou uloženy tabulky/asserty sloužící k testování různých částí programu.
Pro spuštění testu stačí konkrétní test přidat do **sources.cfg**.

## Nástroje a instalace
#### nástroje definované podle stránky předmětu
``make`` vytvoří veškeré výstupy ze zdrojového kódu (dokumentace, závislosti, binární podoba semestrální práce).

``make compile`` zkompiluje a nalinkuje zdrojové soubory - spustitelná binární podoba práce se uloží pod názvem kroupkev.

``make run`` spustí binární podobu semestrální práce.

``make clean`` smaže veškeré dříve vygenerované instance.

``make doc`` vygeneruje dokumentaci.

#### vlastní dodatečné nástroje
``make memcheck`` kontrola správy dynamické paměti ve spuštěné aplikaci (valgrind).

``make dev`` spustí skript, který sleduje změnu souborů (live filewatch).

``make dep`` vygeneruje veškeré závislosti na základě výstupu z g++ -MM.

