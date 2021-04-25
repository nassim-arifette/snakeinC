# Snake en C

Projet Snake en C avec un coeur de jeu independant de la bibliotheque graphique.

Deux backends sont disponibles :

- `graph.h` / `-lgraph` pour conserver la compatibilite avec l'environnement du cours ;
- raylib 6.0 pour une version plus moderne et multiplateforme.

## Backend du cours

```sh
make
./snake
```

Build instrumente :

```sh
make debug
./snake
```

## Backend raylib

Installer raylib et `pkg-config`, puis :

```sh
make raylib
./snake-raylib
```

Le Makefile utilise `pkg-config --cflags --libs raylib` lorsqu'il est disponible, avec un fallback `-lraylib -lm`.

## Tests du coeur

Les regles du jeu se compilent et se testent sans aucune bibliotheque graphique :

```sh
make test
```

Les tests couvrent le plateau, les changements de direction, la croissance, les portails et les collisions.

## Architecture

```text
include/
  board.h
  snake.h
  game.h
  input.h
  renderer.h
  menu.h
  score.h

src/
  board.c
  snake.c
  game.c
  score.c
  main.c

  input_graph.c
  renderer_graph.c
  menu_graph.c

  input_raylib.c
  renderer_raylib.c
  menu_raylib.c

tests/
  test_core.c
```

`main.c`, `game.c`, `board.c`, `snake.c` et `score.c` sont communs aux deux versions. Le backend se choisit uniquement au link.

Les assets restent a la racine pour conserver des chemins runtime identiques entre les deux backends.
