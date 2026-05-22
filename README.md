# INF3135 — Construction et maintenance de logiciels
## Application `tempo` — Travaux pratiques 1, 2 et 3

Cours : INF3135 — Hiver 2025  
Université du Québec à Montréal (UQAM)  
Auteur : Sami Geagea

---

## Description

Ce dépôt regroupe les trois travaux pratiques du cours INF3135, 
qui consistent à concevoir et faire évoluer une application en C 
nommée `tempo`. Cette application permet de manipuler des 
**séries temporelles**, c'est-à-dire des suites de valeurs 
numériques associées à des instants dans le temps.

---

## Dépôts GitHub

Chaque sprint est disponible dans son propre dépôt avec l'historique complet des commits et toutes les branches :

| Sprint | Dépôt | Branches |
|--------|-------|----------|
| Sprint 1 — Mise en place | [Tempo-sprint1](https://github.com/sami136kwe/Tempo-sprint1) | `master`, `correction` |
| Sprint 2 — Extension | [Tempo-sprint2](https://github.com/sami136kwe/Tempo-sprint2) | `master`, `validate-subcommands`, `dynamic-timeseries`, `interpolate-subcommand`, `interpolate-step-option` |
| Sprint 3 — Bibliothèques | [Tempo-sprint3](https://github.com/sami136kwe/Tempo-sprint3) | `master`, `correction`, `from-json`, `test-timeseries-libtap`, `to-gnuplot` |

---

## Télécharger les 3 dépôts

```bash
git clone git@github.com:sami136kwe/Tempo-sprint1.git && \
git clone git@github.com:sami136kwe/Tempo-sprint2.git && \
git clone git@github.com:sami136kwe/Tempo-sprint3.git
```

---

## Structure du dépôt

```
Tempo/
├── Tempo-sprint1/   → TP1 : Mise en place de l'application tempo
├── Tempo-sprint2/   → TP2 : Extension avec nouvelles fonctionnalités
└── Tempo-sprint3/   → TP3 : Intégration de bibliothèques externes
```

---

## Sprint 1 — Mise en place de `tempo`

🔗 [github.com/sami136kwe/Tempo-sprint1](https://github.com/sami136kwe/Tempo-sprint1)

### Description
Implémentation des bases de l'application `tempo` avec 3 
sous-commandes :
- `tempo help` — Affiche le manuel d'utilisation
- `tempo show` — Affiche les observations en ordre chronologique
- `tempo describe` — Affiche les statistiques de la série temporelle

### Compilation
```bash
cd Tempo-sprint1
make
```

### Exécution
```bash
# Afficher l'aide
bin/tempo help

# Afficher les observations
bin/tempo show < examples/6.ts

# Afficher les statistiques
bin/tempo describe < examples/6.ts
```

### Format d'entrée
```
2025-09-01T00:00:00
0 10
28800 40
57600 15
```
La première ligne est l'horodate de référence (format `AAAA-mm-JJTHH:MM:SS`).  
Les lignes suivantes sont des observations : `DÉCALAGE VALEUR`.

### Format de sortie (`show`)
```
2025-09-01T00:00:00 10
2025-09-01T08:00:00 40
2025-09-01T16:00:00 15
```

### Format de sortie (`describe`)
```
Domain: [2025-09-01T00:00:00, 2025-09-03T00:00:00]
Codomain: [10, 50]
Size: 6
Duration: 172800
Amplitude: 40
```

### Tests
```bash
make test
```

---

## Sprint 2 — Extension de `tempo`

🔗 [github.com/sami136kwe/Tempo-sprint2](https://github.com/sami136kwe/Tempo-sprint2)

### Description
Ajout de nouvelles fonctionnalités sur 4 branches indépendantes :
- `validate-subcommands` — Validation stricte des sous-commandes
- `dynamic-timeseries` — Allocation dynamique pour les séries temporelles
- `interpolate-subcommand` — Nouvelle sous-commande `interpolate`
- `interpolate-step-option` — Option `-s|--step` pour le pas d'interpolation

### Compilation
```bash
cd Tempo-sprint2
make
```

### Nouvelle sous-commande : `interpolate`
Interpole linéairement les valeurs entre chaque observation.

```bash
# Interpolation avec pas de 1 seconde (défaut)
bin/tempo interpolate < examples/3_10s.ts

# Interpolation avec pas de 2 secondes
bin/tempo interpolate -s 2s < examples/3_10s.ts

# Interpolation avec pas de 10 minutes
bin/tempo interpolate -s 10m < examples/3_10s.ts

# Interpolation avec pas de 4 heures
bin/tempo interpolate -s 4h < examples/3_10s.ts
```

### Codes de retour
| Code | Signification |
|------|--------------|
| `0`  | Succès |
| `1`  | Erreur d'utilisation |
| `2`  | Erreur dans les données |
| `3`  | Mémoire insuffisante |

### Tests
```bash
make test
```

---

## Sprint 3 — Intégration de bibliothèques

🔗 [github.com/sami136kwe/Tempo-sprint3](https://github.com/sami136kwe/Tempo-sprint3)

### Description
Ajout de fonctionnalités avancées sur 3 branches :
- `test-timeseries-libtap` — Tests unitaires avec Libtap
- `from-json` — Lecture de séries temporelles au format JSON
- `to-gnuplot` — Génération de scripts Gnuplot

### Dépendances
- [Jansson](https://github.com/akheron/jansson) — Lecture JSON
- [Libtap](https://github.com/zorgnax/libtap) — Tests unitaires
- [Gnuplot](http://www.gnuplot.info/) — Visualisation

### Compilation
```bash
cd Tempo-sprint3
make
```

### Option JSON (`-J|--from-json`)
Permet de lire une série temporelle au format JSON :

```bash
bin/tempo show --from-json < examples/serie.json
bin/tempo describe -J < examples/serie.json
bin/tempo interpolate -J < examples/serie.json
```

Format JSON attendu :
```json
{
  "origin": "2025-09-01T00:00:00",
  "observations": [
    {"offset": 0, "value": 10},
    {"offset": 28800, "value": 40}
  ]
}
```

### Sous-commande `gnuplot`
Génère un script Gnuplot pour visualiser la série temporelle :

```bash
# Générer le script
bin/tempo gnuplot < examples/6.ts > plot.gp

# Exécuter le script avec Gnuplot
gnuplot -e "set output 'timeseries.png'" plot.gp
```

### Tests
```bash
make test
```

---

## Technologies utilisées

- **Langage :** C (compilé avec `g++ v12`)
- **Outils :** Git, Make, Bats, Libtap, Jansson, Gnuplot
- **Plateforme :** GitLab UQAM → GitHub

---

## Auteurs

**Sami Geagea**  
**Mehdi Lyafy**  
**Alexandre Blondin Massé**