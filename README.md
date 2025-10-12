# Travail pratique 1: Mise en place de l'application `tempo`

## Description

L'application `tempo` est un programme en C permettant de lire, afficher et analyser des
**séries temporelles** fournies sur l'entrée standard (`stdin`). Une série temporelle est
composée d'une horodate de référence suivie de plusieurs observations `(OFFSET, VALUE)`.

Les fonctionnalités offertes sont :
* `tempo help` : affiche le guide d’utilisation
* `tempo show` : affiche les observations avec horodates absolues
* `tempo describe` : affiche les caractéristiques de la série (domaine, codomaine, etc.)


## Auteur

Sami Geagea (GEAS72090507)

## Fonctionnement

1. Compiler le projet :

```sh
$ cd src
$ make build
```

2. Utiliser l’exécutable avec l’une des sous-commandes suivantes :

```sh
$ bin/tempo help
$ bin/tempo show < examples/6.ts
$ bin/tempo describe < examples/6.ts
```

## Tests

Pour exécuter la suite de tests fonctionnels :

si on est dans le repertoire inf3135-253-tp1
```sh
$ make test
```

si on est dans le repertoire inf3135-253-tp1/src
```sh
$ cd ..
$ make test
```

Les tests utilisent le framework Bats.

## Dépendances

* [GCC](https://gcc.gnu.org/) — compilateur C standard
* [Bats (Bash Automated Testing System)](https://github.com/bats-core/bats-core) pour les tests fonctionnels
* `make` — pour l'automatisation de la compilation et des tests

## Références
* cours 2 et 3 de inf3135 
* guides sur [Stack Overflow](https://stackoverflow.com)
* documentation c [tutorialspoint] (https://www.tutorialspoint.com/c_standard_library/index.htm)
* Aide pour ecire la documentation  et aider a deboger avec la IA ChatGPT

## État du projet

* [X] Le nom du dépôt GitLab est exactement `inf3135-253-tp1`
* [X] L'URL du dépôt GitLab est exactement `https://gitlab.info.uqam.ca/utilisateur/inf3135-253-tp1`
* [X] Les utilisateurs `blondin_al` et `guite-vinet.julien` on accès au projet en mode *Maintainer*
* [X] Le dépôt GitLab est un *fork* du gabarit fourni
* [X] Le dépôt GitLab est privé
* [X] Le dépôt contient au moins un fichier `.gitignore`
* [X] Les sections incomplètes de ce fichier (`README.md`) ont été complétées
* [X] Toutes les fonctions du fichier `tempo.c` sont documentées (*docstring*)
