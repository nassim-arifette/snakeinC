# Architecture

## Regle principale

Le coeur du jeu ne depend d'aucune API graphique. raylib reste a la frontiere de l'application : fenetre, input, rendu, assets et persistance de fichier.

```text
main.c
  |
  v
app/app.c
  |-----------------------> ui/menu.c
  |-----------------------> ui/renderer.c ---> ui/assets.c ---> raylib
  |-----------------------> platform/input.c -----------------> raylib
  |-----------------------> persistence/score.c --------------> raylib filesystem
  |
  v
core/game.c ---> core/level.c
  |                 |
  v                 v
core/snake.c ---> core/board.c

core/layout.c (calcul pur : taille de fenetre, geometrie du plateau)
  ^                    ^
  |                    |
app/app.c        ui/menu.c, ui/renderer.c
```

## `src/core`

### `board.c`

Possede un tableau contigu de `CellType`. Aucun code de rendu, aucun fichier, aucune entree utilisateur.

### `snake.c`

Gere la liste chainee des segments et maintient la coherence avec le `Board`.

### `game.c`

Expose le cycle de vie d'une partie et `game_step()`. Un appel a `game_step()` represente un tick de simulation.

### `level.c`

Contient les details internes d'un niveau : murs, portails, spawn du serpent, pommes/obstacles, progression et RNG deterministe par partie.

`level.h` est volontairement prive dans `src/core` : ce n'est pas une API publique.

### `layout.c`

Calcule la mise en page sans toucher a raylib :

- `layout_window_size()` choisit une taille de fenetre adaptee a l'ecran (avec des bornes minimum et maximum) ;
- `layout_board_geometry()` place le plateau dans la fenetre : taille de case entiere, plateau centre sous le HUD ;
- `layout_ui_transform()` met une interface concue dans un espace de reference a l'echelle de la fenetre.

Une seule taille de fenetre est appliquee, au demarrage : le menu comme la partie gardent la meme fenetre pendant toute la vie du programme. Comme ce module est du calcul pur, il est teste par `make test` comme le reste du coeur.

## `src/app`

`app.c` possede la boucle applicative. Il :

1. initialise une seule fenetre raylib ;
2. affiche le menu ;
3. cree une partie ;
4. accumule le temps de frame et appelle `game_step()` au rythme demande par le moteur ;
5. revient au menu sans recreer le contexte raylib ;
6. sauvegarde le meilleur score.

Le moteur conserve son rythme en microsecondes mais l'application utilise `GetFrameTime()` : aucun `usleep()` ou appel POSIX n'est necessaire.

## `src/ui`

- `assets.c` centralise le chargement/dechargement des textures ;
- `menu.c` ne contient que l'interface du menu ;
- `renderer.c` traduit l'etat `Game` en primitives raylib.

`menu.c` dessine une interface concue en 720x760 puis la met a l'echelle de la fenetre courante (`layout_ui_transform()`), ce qui garde la meme mise en page sur tous les ecrans.

Le renderer ne change jamais la taille de la fenetre : il choisit la taille de case qui fait tenir le plateau courant dans cette fenetre et le centre.

## `src/platform`

`input.c` transforme les touches raylib en commandes metier (`INPUT_UP`, `INPUT_BACK`, etc.). Le coeur ne connait jamais les constantes `KEY_*`.

## `src/persistence`

`score.c` gere un seul entier : le meilleur score. Il n'y a plus de fichier historique parcouru a chaque frame.

## Tests

Les tests compilent uniquement :

```text
board.c + snake.c + game.c + level.c
```

Cela garantit que le coeur reste independant de raylib. Les suites sont separees dans `tests/test_board.c`, `tests/test_snake.c` et `tests/test_game.c`.
