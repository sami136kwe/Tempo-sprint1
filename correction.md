# Évaluation du travail pratique 1

## Identification

* Nom d’utilisation: geagea.sami
* Prénom et nom: Sami Geagea
* Code Permanent: GEAS72090507

## Total: 87/100

## Vérification de base: -5

* L'URL du dépôt est correcte? oui
* Le dépôt est privé? oui
* Le dépôt est un *fork*? oui
* Le rôle accordé est *Maintainer*? oui
* Le projet compile sans erreur et sans avertissement avec la commande `make
  build`? non -5pts
* Le fichier README contient le code permanent? oui
* Nombre d'heures de retard? 0

## Fonctionnalités: 60/60

* Nombre de tests réussis? 60
* Voir le rapport Bats en annexe

## Qualité du code: 15/15

* Identifications significatifs? oui
* Syntaxe uniforme? oui
* Indentation correcte? oui
* Aération correcte? oui
* Code simple et lisible? oui
* Présence de code mort (inutilisé ou en commentaire)? non
* Présence de valeurs magiques répétées? non
* Code bien factorisé? oui
* Bonne décomposition fonctionnelle? oui

## Documentation: 15/15

* Le fichier README est complet? oui
* Il respecte le format Markdown? oui
* Les fonctions sont documentées avec des *docstrings*? oui
* Les *docstrings* suivent le format demandé? oui
* Il y a peu ou pas de fautes? oui

## Git: 2/10

* Les *commits* sont atomiques? oui
* Seuls les fichiers pertinents sont versionnés? non
* Il y a un fichier `.gitignore`? oui
* Les messages de *commit* sont significatifs? oui
* La syntaxe demandée des messages de *commit* est respectée? non
* On retrouve un répertoire de configuration dans le dépôt: ./.vscode -2pts
* Nombre de messages qui ne commencent pas par une majuscule? 2 -2pts
* Nombre de messages dont le premier mot est un verbe à l’indicatif? 3 -2pts
* Le message du commit `9ab22da ` n’est pas significatif -2pts

## Annexe 1: Rapport de tests Bats

```
1..60
ok 1 tempo describe with empty timeseries displays the domain
ok 2 tempo describe with timeseries of size 1 displays the domain
ok 3 tempo describe with chronological timeseries of size 2 displays the domain
ok 4 tempo describe with antichronological timeseries of size 2 displays the domain
ok 5 tempo describe with timeseries with repeated offsets displays the domain
ok 6 tempo describe with empty timeseries displays the codomain
ok 7 tempo describe with timeseries of size 1 displays the codomain
ok 8 tempo describe with chronological timeseries of size 2 displays the codomain
ok 9 tempo describe with antichronological timeseries of size 2 displays the codomain
ok 10 tempo describe with timeseries with repeated offsets displays the codomain
ok 11 tempo describe with empty timeseries displays the size
ok 12 tempo describe with timeseries of size 1 displays the size
ok 13 tempo describe with chronological timeseries of size 2 displays the size
ok 14 tempo describe with antichronological timeseries of size 2 displays the size
ok 15 tempo describe with timeseries with repeated offsets displays the size
ok 16 tempo describe with empty timeseries displays the duration
ok 17 tempo describe with timeseries of size 1 displays the duration
ok 18 tempo describe with chronological timeseries of size 2 displays the duration
ok 19 tempo describe with antichronological timeseries of size 2 displays the duration
ok 20 tempo describe with timeseries with repeated offsets displays the duration
ok 21 tempo describe with empty timeseries displays the amplitude
ok 22 tempo describe with timeseries of size 1 displays the amplitude
ok 23 tempo describe with chronological timeseries of size 2 displays the amplitude
ok 24 tempo describe with antichronological timeseries of size 2 displays the amplitude
ok 25 tempo describe with timeseries with repeated offsets displays the amplitude
ok 26 tempo show with datetime missing the character T returns an error
ok 27 tempo show with wrong character in date returns an error
ok 28 tempo show with wrong character in time returns an error
ok 29 tempo show with wrong day returns an error
ok 30 tempo show with wrong month returns an error
ok 31 tempo show with wrong year returns an error
ok 32 tempo show with another wrong day returns an error
ok 33 tempo show with wrong character in observation returns an error
ok 34 tempo show with wrong offset format returns an error
ok 35 tempo show with wrong value format returns an error
ok 36 tempo show with negative offset format returns an error
ok 37 tempo show with empty observation line returns an error
ok 38 tempo describe runs without memory problem
ok 39 tempo help runs without memory problem
ok 40 tempo show runs without memory problem on empty timeseries
ok 41 tempo show runs without memory problem on nonempty timeseries
ok 42 tempo show with empty timeseries works
ok 43 tempo show with timeseries of size 1 works
ok 44 tempo show with timeseries of size 1 with spaces works
ok 45 tempo show with chronological timeseries of size 2 works
ok 46 tempo show with antichronological timeseries of size 2 works
ok 47 tempo show with timeseries with repeated offsets works
ok 48 tempo show with timeseries with multiple repeated offsets works
ok 49 tempo show with year-changing timeseries works
ok 50 tempo show with month-changing timeseries works
ok 51 tempo show with day-changing timeseries works
ok 52 tempo show with hour-changing timeseries works
ok 53 tempo show with minute-changing timeseries works
ok 54 tempo help shows the help
ok 55 tempo help describes a valid timeseries text stream
ok 56 tempo help mentions the describe subcommand
ok 57 tempo help mentions the help subcommand
ok 58 tempo help mentions the show subcommand
ok 59 tempo without subcommand returns an error
ok 60 tempo with unrecognized subcommand returns an error
```
