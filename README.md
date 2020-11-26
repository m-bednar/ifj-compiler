
# IFJ Project compiler

**[IFJ Wiki](https://wis.fit.vutbr.cz/FIT/st/cwk.php?id=13981&csid=735505), [Zadání](https://wis.fit.vutbr.cz/FIT/st/cfs.php?file=/course/IFJ-IT/projects/ifj2020.pdf) (Varianta I)**

### Struktura projektu
```
├── src
|   ├── main.c
|   └── # zdrojové soubory (.c, .h)
├── dokumentace.pdf (TODO)
├── Makefile
├── README.md
├── .editorconfig
└── .gitignore
```

### TODO list
- [x] Tabulka symbolů
- [x] Lexikální analyzátor (scanner)
- [ ] Syntaktický analyzátor (parser)
- [ ] Sémantický analyzátor (Generátor vnitřního kódu)
- [ ] Generátor konečného kódu

### Rozdělení práce
- **Martin**
   - Tabulka symbolů
   - Generátor konečného kódu
- **Daniel**
   - Syntaktický analyzátor
- **Franta**
   - Lexikální analyzátor
- **Michal**
   - Sémantický analyzátor

### Deadlines
- Tabulka symbolů (31.10.)
- Lexikální analyzátor (31.10.)  
- Syntaktický analyzátor (18.11.)
- Sémantický analyzátor (22.11.)
- Generátor konečného kódu (27.11.)

### Makefile příkazy

##### make run
- Zkompiluje projekt
- Spustí */main* bez vstupu

##### make try FILE=*vstupni_soubor*
- Zkompiluje projekt
- Spustí */main* se standartním vstupem ze souboru *vstupni_soubor*
- Po skončení vypíše návratový kód kompilátoru

##### make gen IN=*vstupni_soubor* OUT=*vystupni_soubor*
- Zkompiluje projekt
- Spustí */main* se standartním vstupem ze souboru *vstupni_soubor*
- Výstup souboru je přesměrován do souboru *vystupni_soubor*
- Po skončení vypíše návratový kód kompilátoru

### Registrovaná rozšíření

##### BASE (0.5b)
Možnost zápisu celých čísel v binární, hexadecimální a octalové soustavě.

##### BOOLTHEN (1.5b)
Podpora boolenovských typů a literálů true, false. 
Podpora zjednodušeného podmíněného příkaz if bez části else.

##### ~~UNARY (0.5b)~~
Operátory +=, -=, *=, /=, unární - a +.
