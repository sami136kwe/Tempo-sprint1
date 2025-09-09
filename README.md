# Travail pratique 1: Mise en place de l'application `tempo`

## Format Markdown (supprimer cette section avant la remise)

N'oubliez pas de bien exploiter le format Markdown.

Sauter une ligne pour changer de paragraphe.

Mettre les noms de fichier et bout de code courts entre apostrophes inversés.
Par exemple, si vous parlez du fichier `Makefile`.

Mettre les longs bouts de code dans des blocs de code (triples apostrophes
inversés). Par exemple, vous pouvez donner un exemple de commande comme suit:

```sh
$ make
$ ls -a
```

Utiliser des listes à puces ou des énumérations le plus possible (plus agréable
à lire). Par exemple, pour décrire le contenu du projet:

* `README.md`: ce fichier
* `Makefile`: permet d'automatiser la compilation
* etc.

Bien aérer le contenu du fichier source (`README.md`). Éviter les longues
lignes dans le fichier Markdown (par exemple, limiter à 80) pour une meilleure
lisibilité avec un éditeur de texte.

## Description

Décrivez ici le projet. Commencez d'abord par une description générale, puis
donnez ensuite des détails. Indiquez le contexte dans lequel ce travail est
accompli (cours, sigle, enseignant, université).

Aussi, insérer un lien vers le [sujet du travail](sujet.md).

## Auteur ou autrice

Indiquez ici votre prénom et nom, puis votre code permanent entre parenthèses,
sans mettre en gras ou en italique.

Par exemple,

Alexandre Blondin Massé (BLOA12345678)

## Fonctionnement

Expliquez d'abord en mots comment faire fonctionner le projet (imaginez que la
personne qui l'utilisera est dans un environnement Linux, mais ne connaît pas
C, ne connaît pas le projet et souhaite seulement saisir une série de commandes
pour l'exécuter). En particulier, indiquez les commandes qui doivent être
entrées pour la compilation et l'exécution.

## Tests

Expliquez ici comment lancer la suite de tests automatiques avec la commande
`make test`, ainsi que le résultat que vous obtenez.

## Dépendances

Indiquez les dépendances du projet, avec lien officiel. Il faudrait au moins
mentionner GCC et [Bats](https://github.com/bats-core/bats-core). Utiliser une
liste à puces pour donner la liste des dépendances.

## Références

Indiquez ici les références que vous avez utilisées pour compléter le projet,
avec l'hyperlien vers la référence. Pas besoin de mentionner les diapositives
du cours, mais si vous avez eu recours à un site d'aide, un fragment de code,
une discussion sur un forum ou une IA générative, mentionnez-le.

## État du projet

Indiquez toutes les tâches qui ont été complétés en insérant un `X` entre les
crochets. Si une tâche n'a pas été complétée, expliquez pourquoi.

* [ ] Le nom du dépôt GitLab est exactement `inf3135-253-tp1`
* [ ] L'URL du dépôt GitLab est exactement (remplacer `utilisateur` par votre
  nom identifiant GitLab)
  `https://gitlab.info.uqam.ca/utilisateur/inf3135-253-tp1`
* [ ] Les utilisateurs `blondin_al` et `guite-vinet.julien` on accès au projet
  en mode *Maintainer*
* [ ] Le dépôt GitLab est un *fork* du gabarit fourni
* [ ] Le dépôt GitLab est privé
* [ ] Le dépôt contient au moins un fichier `.gitignore`
* [ ] Les sections incomplètes de ce fichier (`README.md`) ont été complétées
* [ ] Toutes les fonctions du fichier `tempo.c` sont documentées (*docstring*)
