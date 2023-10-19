# ChokbarEngine

Projet n°3 de notre 3ème année en G.Tech.  


## Nomenclature du code

* Namespace : `ThisIsANamespace`
* Class : `ThisIsAClass`
* Field : `m_Name`
* Field pointer : `m_pName`
* Boolean field : `isActive` 
* Method : `ThisIsAMethod()` 
* Getter boolean: `IsActivated()`
* Getter/Setter : `GetName()` / `SetName(type var)`
* TaClasse::GetInstance() -> m_Instance

## Git conventions
Aucun Commit dans dev et master. Pr dans dev.

**Branch Naming :** type/name

### Les types de branches
Le type d’une branche doit être clair afin de comprendre le but de celle-ci. Voici une liste non exhaustive des types de branches :

* feature: Ajout d’une nouvelle fonctionnalité
* bugfix: Correction d’un bug
* hotfix: Correction d’un bug critique
* chore: Nettoyage du code
* experiment: Expérimentation de fonctionnalités

### Commit Naming
Le type nous informe du type de commit. 9 types sont disponibles :

* `feat:` Ajout d’une nouvelle fonctionnalité
* `fix:` Correction d’un bug
* `build:` Changement lié au système de build ou qui concerne les dépendances (npm, grunt, gulp, webpack, etc.)
* `perf:` Amélioration des performances
* `refactor:` Modification n’ajoutant pas de fonctionnalités ni de correction de bug (renommage d’une variable, suppression de code redondant, simplification du code, etc.)
* `style:` Changement lié au style du code (indentation, point virgule, etc.)
* `test:` Ajout ou modification de tests
* `revert:` Annulation d’un précédent commit
* `chore:` Toute autre modification (mise à jour de version par exemple)

#### Le scope
Cet élément facultatif indique simplement le contexte du commit. Il s’agit des composants de notre projet, voici une liste non exhaustive :
* `engine`
* `entities`
* `components`
* `windows`
* `input`
* `graphics`
* `physics`


## Organisation des fichiers
* jeu  
* ChockbarEngine : projet du moteur  
  * Core   : éléments nécessaires au bon fonctionnement du moteur (ex : D3DApp...)  
  * Engine : éléments spécifiques au moteur (exemple : Components, GameTimer...)  
  * Common : éléments en commun, accessible partout (ex : Logger...)  
  * Platform (optionnal) : élément spécifique à la plateforme (fenêtre windows, WinEntry...)  
