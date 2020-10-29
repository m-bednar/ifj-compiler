
# IFJ Project compiler

[Zadání](https://wis.fit.vutbr.cz/FIT/st/cfs.php?file=/course/IFJ-IT/projects/ifj2020.pdf) (Varianta I)

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
- [ ] Lexikální analyzátor (scanner)
- [ ] Syntaktický analyzátor (parser)
- [ ] Sémantický analyzátor
- [ ] Generátor vnitřního kódu (?)
- [ ] Generátor konečného kódu

### Rozdělení práce
- **Martin**
   - Tabulka symbolů
   - Generátor konečného kódu
- **Daniel**
   - Syntaktický analyzátor
- **Franta**
   - Lexikální analyzátor
   - Generátor vnitřního kódu (?)
- **Michal**
   - Sémantický analyzátor

### Deadlines
- Tabulka symbolů (31.10.)
- Lexikální analyzátor (31.10.)  
- Syntaktický analyzátor (15.11.)
- Sémantický analyzátor (22.11.)
- Generátor konečného kódu (27.11.)
