# Travail pratique 1: Mise en place de l'application `tempo`

Ce document décrit le travail pratique 1 du cours [INF3135 Construction et
maintenance de logiciels](https://etudier.uqam.ca/cours?sigle=INF3135) au
trimestre d'automne 2025. Le cours est enseigné par Alexandre Blondin Massé,
professeur au [département d'informatique](http://info.uqam.ca/),
à l'[Université du Québec à Montréal](https://uqam.ca/).

Le travail doit être remis au plus tard le **dimanche 12 octobre à 23h59**.
À partir de minuit, une pénalité de **2 points** par heure de retard sera
appliquée.

## Objectifs pédagogiques

Les principaux objectifs pédagogiques visés sont les suivants:

- Vous initier au langage de **programmation C**
- Utiliser adéquatement le logiciel de contrôle de versions **Git** pour
  développer une application
- Vous familiariser à la compilation d'un programme à l'aide d'un **Makefile**
  simple
- Vous habituer à décomposer un programme en **petites fonctions**
- Vous familiariser avec le passage d'arguments par **adresse**
- Vous familiariser avec l'utilisation de l'**entrée standard** (`stdin`) et de
  la **sortie standard** (`stdout`)
- Vous familiariser avec le traitement des **arguments** d'un programme et des
  **codes de retour**

## Description du travail

Vous devez concevoir les bases d'une application nommée `tempo` qui permet de
manipuler des [séries
temporelles](https://fr.wikipedia.org/wiki/S%C3%A9rie_temporelle) (aussi
appelées *séries chronologiques*), c'est-à-dire « une suite de valeurs
numériques représentant l'évolution d'une quantité spécifique au cours du temps
». Cette application sera ensuite étendue dans le cadre des travaux pratiques
2 et 3 du cours.

Afin d'utiliser un vocabulaire uniforme, nous appellerons *horodate* un instant
donné (en anglais, *timestamp*) et *valeur* la quantité associée à cet instant.
Une paire $`(t, v)`$, où $`t`$ est une horodate et $`v`$ une valeur, sera
appelé une *observation*. Une *série temporelle* peut donc être vue comme un
ensemble d'observations.

Votre programme devra lire une série temporelle $`T`$ sur l'entrée standard
(`stdin`), puis ensuite afficher sur la sortie standard (`stdout`) différentes
informations sur $`T`$. Votre programme devra avoir un comportement très précis
vérifié par des tests fonctionnels. Vous devrez donc vous assurer de ne pas
écrire de messages superflus sur `stdout` ou `stderr` et de bien écrire **tels
quels** les messages d'erreurs.

Considérons le flux de texte suivant, qui décrit une série temporelle de six
observations:

```
2025-09-01T00:00:00
0 10
28800 40
57600 15
86400 35
115200 50
172800 25
```

Plus spécifiquement:

* La première ligne du flux (`2025-09-01T00:00:00`) indique l'horodate de
  référence de la série temporelle au format `AAAA-mm-JJTHH:MM:SS`. Ce format
  est notamment reconnu par les standards
  [ISO8601](https://en.wikipedia.org/wiki/ISO_8601) et
  [RFC3339](https://datatracker.ietf.org/doc/html/rfc3339)
* Les lignes suivantes du flux de texte contiennent les observations, une
  observation par ligne
* Une observation est une ligne contenant deux valeurs, séparées par une ou
  plusieurs espaces (par exemple, `28800 40`)
* La première valeur d'une observation (par exemple `28800`) est un entier
  positif ou nul correspondant au décalage (en secondes) du moment où
  l'observation est effectuée par rapport à l'horodate de référence
  (`2025-09-01T00:00:00`)
* La seconde valeur d'une observation (par exemple `40`) est un entier
  correspondant à la valeur observée

Une représentation graphique de la série temporelle ci-haut est disponible dans
le fichier PNG suivant:
![Une série temporelle de 6 observations](doc/timeseries.png)

Plus généralement, pour être valide, un flux de texte doit respecter les
contraintes suivantes:

1. La première ligne du texte doit contenir une horodate valide respectant le
   format `AAAA-mm-JJTHH:MM:SS`
2. Chacune des autres lignes doit contenir une observation, c'est-à-dire
   qu'elle doit avoir une correspondance complète avec l'expression régulière
   étendue (ERE)
   `OFFSET[:blank:]*VALUE[:blank:]*`,
   où
    * `OFFSET` est un entier non négatif
    * `VALUE` est un entier
3. Un *entier* est une chaîne de caractères qui a une correspondance complète
   avec l'ERE `0|([-]?[1-9][0-9]*)`;
4. Un *entier non négatif* est une chaîne de caractères qui a une
   correspondance complète avec l'ERE `0|([1-9][0-9]*)`;

Des exemples de séries temporelles valides (extension `.ts`) et invalides
(extension `.invalid`) sont donnés dans le répertoire [`examples`](examples).

## Sous-commandes

L'application `tempo` utilise des *sous-commandes* afin de préciser
l'information qu'on souhaite afficher à propos d'une série temporelle. Pour ce
premier travail pratique, vous devez supporter les 3 sous-commandes suivantes:

1. `tempo describe`
2. `tempo help`
3. `tempo show`

## La sous-commande `help`

Lorsque vous lancez le programme avec la sous-commande `help`, un manuel
d'utilisation doit être affiché sur la sortie standard:

```text
$ bin/tempo help
Usage: tempo SUBCOMMAND
Displays information about a timeseries.

SUBCOMMAND is mandatory and must take one of the following values:
  describe: describes the timeseries
  help: shows this message
  show: list the observations of the timeseries

A timeseries is a text stream that must satisfy the following syntax:

  1. The first line must contain a valid reference datetime with format
       YYYY-mm-DDTHH:MM:SS
     where YYYY are the 4 digits for the year,
           mm are the 2 digits for the month,
           DD are the 2 digits for the day,
           HH are the 2 digits for the hour,
           MM are the 2 digits for the minutes and
           SS are the 2 digits for the seconds
  2. Each of the remaining line must contain an observation, given with the
     format
       OFFSET VALUE
     where OFFSET is a positive integer indicating the number of seconds offset
                  with respect to the reference datetime and
           VALUE is an integer
```

## La sous-commande `show`

La sous-commande `show` doit afficher la série temporelle sur la sortie
standard dans un format différent de celui considéré lors de la lecture. Plus
précisément, il doit afficher en ordre chronologique chacune des observations,
en commençant par l'horodate de l'observation, suivie par la valeur observée,
en utilisant une espace comme séparateur.

Par exemple, si on reprend la série temporelle illustrée plus haut (voir fichier
[`examples/6.ts`](examples/6.ts)), alors on obtient ceci:

```
$ bin/tempo show < examples/6.ts
2025-09-01T00:00:00 10
2025-09-01T08:00:00 40
2025-09-01T16:00:00 15
2025-09-02T00:00:00 35
2025-09-02T08:00:00 50
2025-09-03T00:00:00 25
```

Ainsi, la commande affiche les 6 observations contenues dans la série
temporelle, en ordre chronologique, en utilisant l'espace comme séparateur.
Chaque observation est donnée par une paire d'horodate et de valeur entière.

## La sous-commande `describe`

La sous-commande `describe` affiche différentes informations à propos de la
série temporelle:

* Son *domaine* (*domain*): le plus petit intervalle temporel recouvrant toutes
  les horodates des observations
* Son *codomaine* (*codomain*): le plus petit intervalle de valeurs recouvrant
  toutes les valeurs des observations
* Sa *taille* (*size*): le nombre d'observations qu'elle contient
* Sa *durée* (*duration*): l'écart (en secondes) entre l'horodate de la
  dernière observation et l'horodate de la première observation
* Son *amplitude* (*amplitude*): l'écart entre la plus grande valeur observée
  et la plus petite valeur observée

Toujours avec l'exemple précédent:

```
$ bin/tempo describe < examples/6.ts
Domain: [2025-09-01T00:00:00, 2025-09-03T00:00:00]
Codomain: [10, 50]
Size: 6
Duration: 172800
Amplitude: 40
```

## Validation

Votre programme doit détecter certaines erreurs d'utilisation et de données.

* Lorsqu'il s'exécute de façon normale (sans erreur), alors le code `0` doit
  être retourné.
* En cas d'erreur d'utilisation du programme, vous devez retourner le code `1`.
* En cas d'erreur dans les données fournies en entrées, vous devez retourner le
  code `2`.

Dans chaque cas d'erreur, un message expliquant l'erreur doit être affiché sur
le canal d'erreur (`stderr`). Plus spécifiquement, les messages d'erreur
doivent être les suivants:

1. Si on oublie de fournir une sous-commande, la valeur `1` doit être retournée
   et le message suivant doit être affiché sur `stderr`:

    ```
    error: subcommand is mandatory
    ```

2. Si on fournit une sous-commande `SUBCOMMAND` non reconnue, la valeur `1`
   doit être retournée et le message doit plutôt être:

    ```
    error: unrecognized subcommand 'SUBCOMMAND'
    ```

   où `SUBCOMMAND` est la sous-commande non reconnue fournie

3. Si l'horodate fournie dans le flux de texte a un format invalide, c'est la
   valeur `2` qui doit être retournée, ainsi que le message:

    ```
    error: invalid datetime format (should be YYYY:mm:DDTHH:MM:SS)
    ```

4. Si le format de l'horodate est valide, mais que l'horodate elle-même est
   invalide (par exemple, la date `2025-02-31` ou l'heure `10:62:17`), on
   retourne `2` ainsi que le message suivant

    ```
    error: invalid datetime (HORODATE)
    ```

   où `HORODATE` est l'horodate invalide apparaissant dans le flux de texte.

5. Si le format d'une observation fournie dans le flux de texte est invalide,
   on retourne `2` et on affiche le message:

    ```
    error: invalid observation format (OBSERVATION)
    ```

   où `OBSERVATION` est la ligne de l'observation invalide.

6. Finalement, si le décalage (*offset*) d'une observation fournie dans le flux
   de texte est négatif, on retourne aussi `2` et on affiche:

    ```
    error: invalid offset (OBSERVATION)
    ```

   où `OBSERVATION` est la ligne de l'observation invalide.

Les tests fonctionnels disponibles dans les fichiers `test*.bats` du répertoire
[`bats`](bats) montrent des exemples pour chacune des 6 situations. En cas de
doute sur le comportement à adopter dans certaines situations, n'hésitez pas
à poser des questions pour avoir des précisions.

## Hypothèses simplificatrices

Vous pouvez faire certaines hypothèses qui réduisent le nombre de cas d'erreur
potentielle à gérer. Plus précisément, vous pouvez supposer, sans le valider,
que:

* Chaque ligne décrivant une série temporelle est de longueur au plus 100
* Le nombre d'observations d'une série temporelle est d'au plus 1000
* Toutes les horodates manipulées sont dans l'intervalle [1902-01-01T00:00:00,
  2037-12-31T23:59:59] (donc pas de problème de représentation d'une horodate
  à l'aide du type `time_t`)

Aussi, vous n'avez pas à gérer les fuseaux horaires, les changements d'heure et
les secondes intercalaires. Autrement dit, vous pouvez faire comme s'il
n'existait qu'un seul fuseau horaire, qu'on ne changeait jamais l'heure et
qu'il n'existait pas de secondes intercalaires.

## Tâches à accomplir

Afin de compléter ce travail pratique, vous devrez suivre les étapes suivantes:

1. Créez un clone (*fork*) du [dépôt du
   projet](https://gitlab.info.uqam.ca/inf31351/20253/inf3135-253-tp1)
2. Assurez-vous que le dépôt soit privé, pour éviter le plagiat
3. Donnez accès à votre dépôt aux utilisateurs `blondin_al` et
   `guite-vinet.julien` en mode *Maintainer* (pour la correction)
4. Familiarisez-vous avec le contenu du dépôt, en étudiant chacun des fichiers
   (`README.md`, `sujet-tp1.md`, répertoire `bats`, `.gitlab-ci.yml`, etc.).
5. Commencez votre développement en versionnant fréquemment l'évolution de
   votre projet avec Git. La qualité de votre versionnement sera évaluée, il
   est donc important de sauvegarder (*commit*) chaque fois que vous avez
   terminé une petite tâche.
6. Complétez le fichier `README.md` en respectant le format Markdown et en
   tenant compte des instructions qui y sont présentes.
7. Complétez le fichier `tempo.c`.

### Clone et création du dépôt

Vous devez cloner le dépôt fourni et l'héberger sur la plateforme [Gitlab du
département](https://gitlab.info.uqam.ca/). Votre dépôt devra se nommer
**exactement** `inf3135-253-tp1` et l'URL devra être **exactement**
`https://gitlab.info.uqam.ca/<utilisateur>/inf3135-253-tp1`, où `<utilisateur>`
doit être remplacé par votre nom d'utilisateur. Il doit être **privé** et
accessible seulement par vous et par les utilisateurs `blondin_al` et
`guite-vinet.julien`.

### Fichier `tempo.c`

L'organisation du contenu de votre fichier `tempo.c` est laissée à votre
discrétion. Des déclarations initiales de constantes et de structures sont
fournies afin de vous aider à vous organiser, mais il est permis de les
modifier ou de les supprimer si vous préférez travailler différemment.

### Makefile

Des Makefiles sont fournis afin d'automatiser certaines tâches. Normalement,
vous ne devriez pas avoir besoin de les modifier, mais vous pouvez le faire si
vous souhaitez automatiser d'autres tâches.

### Tests fonctionnels

Vous pouvez en tout temps lancer les tests fonctionnels à l'aide de la commande

```sh
$ make test
```

Initialement, tous les tests non fonctionnels sont désactivés avec la commande
`skip`. Au fur et à mesure que vous progressez dans votre travail, vous pouvez
activer les tests en supprimant la ligne `skip`.

### Fichier `README.md`

Vous devez compléter le fichier `README.md` livré dans le dépôt en suivant les
instructions qui y sont indiquées. Assurez-vous également de répondre aux
questions suivantes:

- À quoi sert votre programme?
- Comment le compiler?
- Comment l'exécuter?
- Quels sont les formats d'entrées et de sorties?
- Quels sont les cas d'erreur gérés?

Vous devez utiliser la syntaxe Markdown pour écrire une documentation claire et
lisible. Vous pouvez en tout temps vérifier localement le fichier HTML produit
sur votre machine à l'aide de [Pandoc](https://pandoc.org/). Il est aussi
conseillé de vérifier, avant la remise finale, que le résultat produit sur la
page d'accueil de GitLab est celui auquel vous vous attendez.

Il est acceptable de recopier tels quels des passages du fichier `sujet.md`
dans le fichier `README.md` que vous remettrez.

Évitez d'abuser du gras, de l'italique et du HTML brut dans le fichier
`README.md`. Exploitez au maximum les listes à puces et formatez les noms de
fichier et le code à l'aide des apostrophes inversés. Finalement, soignez la
qualité de la langue, qui sera prise en considération dans le fichier
`README.md`.

### Git

Il est important de suivre l'évolution de votre projet à l'aide de Git. Vous
devez cloner (à l'aide du bouton *fork*) le gabarit du projet fourni et ajouter
vos modifications à l'aide de *commits*. En particulier, il est possible que
des corrections ultérieures soient apportées à l'énoncé que vous pourrez
récupérer facilement s'il y a un historique **commun**.

N'oubliez pas de bien configurer votre fichier `.gitconfig` qui permet de vous
identifier comme auteur de *commits*, en y indiquant vos **véritables** prénom,
nom et courriel.

Les messages de *commit* doivent suivre [la convention de Chris
Beams](https://chris.beams.io/posts/git-commit/), adaptée au français:

1. Séparer le sujet (la première ligne) du corps (le reste du message) par une
   ligne vide
2. Limiter le sujet à 50 caractères
3. Commencer le message par une lettre majuscule
4. Ne pas terminer le sujet avec un point
5. Utiliser un verbe à l'indicatif présent comme premier mot du sujet
6. Limiter les lignes du corps du message à 72 caractères
7. Utiliser le corps pour expliquer *ce que* (*what*) la modification apporte
   et *pourquoi* (*why*) elle apporte cette modification, plutôt que *comment*
   (*how*) elle apporte une modification

Vous pouvez vous inspirer des messages de *commit* utilisés dans tous les
dépôts partagés par l'enseignant dans le cadre du cours.

Finalement, n'oubliez pas d'inclure un fichier `.gitignore` en fonction de
votre environnement de développement. Aussi, assurez-vous de ne pas versionner
de fichiers inutiles, incluant les artéfacts produits.

### Langue

Comme vous l'avez constaté, le projet est développé dans deux langues, soit le
français et l'anglais. C'est une réalité avec laquelle vous allez peut-être
devoir composer dans votre future carrière. Dans le cadre de ce travail
pratique, vous devez suivre les règles suivantes:

* Tous les messages de *commit* doivent être rédigés en français
* Le fichier `README.md` doit être rédigé en français
* Les commentaires et la documentation du code source (fichier `.c`, fichier
  `.bats`, etc.) doivent être rédigés en anglais

Cette convention n'est pas universelle, mais elle est souvent employée dans les
milieux bilingues.

### Correction

L'exécution de votre programme sera vérifiée automatiquement grâce à une suite
de tests rédigée en [Bats](https://github.com/bats-core/bats-core). Vous n'avez
pas à installer Bats, qui est livré dans ce dépôt dans le répertoire `bats`.
Pour faciliter votre développement, vous avez accès à un certain nombre de
tests *publics* pour corriger vos travaux (aussi disponibles dans le répertoire
`bats`, dans les fichiers de la forme `test*.bats`). Il suffit d'entrer la
commande

```sh
$ make test
```

pour lancer la suite de tests.

Cependant, prenez note que la couverture de tests n'est pas **complète**. Des
tests seront ajoutés lors de la correction finale. Si vous avez un doute sur le
comportement attendu d'une certaine situation, c'est votre responsabilité de
poser des questions pour obtenir des précisions.

### Barème

Les critères d'évaluation sont les suivants:

| Critère             |  Points |
|:--------------------|--------:|
| Fonctionnalité      |     /60 |
| Qualité du code     |     /15 |
| Documentation       |     /15 |
| Utilisation de Git  |     /10 |
| Total               |    /100 |

Plus précisément, les éléments suivants seront pris en compte:

- **Fonctionnalité (80 points)**: Le programme passe les tests *publics* et
  *privés* en affichant le résultat attendu (1 point par test).

- **Qualité du code (20 points)**: Les identifiants utilisés sont significatifs
  et ont une syntaxe uniforme, le code est bien indenté, il y a de l'aération
  autour des opérateurs et des parenthèses, le programme est simple et lisible.
  Pas de bout de code en commentaire, de commentaires inutiles ou de
  commentaires paraphrasant le code. Pas de valeur magique répétées. Le code
  doit être bien factorisé (pas de redondance). Il est décomposé en petites
  fonctions qui effectuent des tâches spécifiques. *Note*: si votre style est
  impeccable mais que votre travail est peu avancé, vous aurez peu de points
  pour cette partie.

- **Documentation (20 points)**: Le fichier `README.md` contient toute
  l'information demandée et utilise adéquatement le format Markdown. Toutes les
  fonctions sans exception ont une *docstring* qui suit le format suggéré en
  classe. La rédaction est soignée et contient peu ou pas de fautes
  (typographiques, d'orthographe et de grammaire).

- **Utilisation de Git (10 points)**: Les modifications sont réparties en
  *commits* atomiques. Seuls les fichiers pertinents sont versionnés et le
  fichier `.gitignore` est complet. Les messages de *commit* sont
  significatifs, uniformes et suivent la convention suggérée plus haut.

### Pénalités

Des pénalités (jusqu'à hauteur de **100%**) pourront être appliquées si les
contraintes suivantes ne sont pas respectées:

* Votre programme doit compiler sans erreur et sans avertissement lorsqu'on
  lance la commande `make`
* Votre code permanent doit être indiqué dans le fichier `README.md`
* Votre dépôt doit être un *fork* du [gabarit
  fourni](https://gitlab.info.uqam.ca/inf31351/20253/inf3135-253-tp1).
* Votre dépôt doit être **privé**.
* Votre dépôt doit se nommer **exactement** `inf3135-253-tp1`
* L'URL de votre dépôt doit être **exactement**
  `https://gitlab.info.uqam.ca/<utilisateur>/inf3135-253-tp1` où
  `<utilisateur>` doit être remplacé par votre identifiant
* Les utilisateurs `blondin_al` et `guite-vinet.julien` doivent avoir accès
  à votre projet en mode *Maintainer*.
* Il est interdit d'utiliser l'**allocation dynamique** dans ce travail
  (fonctions `malloc`, `calloc`, `realloc`, `free`).

## Remise

La remise se fait automatiquement en ajoutant les utilisateurs `blondin_al` et
`guite-vinet.julien` en mode *Maintainer*, vous n'avez rien de plus à faire.
À moins d'indication contraire de votre part, ce sera le dernier (*commit*)
disponible sur votre branche `master` qui sera considéré pour la correction.

Le travail doit être remis au plus tard le **dimanche 12 octobre à 23h59**.
À partir de minuit, une pénalité de **2 points** par heure de retard sera
appliquée.
