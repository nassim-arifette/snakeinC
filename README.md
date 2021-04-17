# Snake en C

Projet Snake en C. L'interface utilise encore la bibliotheque pedagogique `graph.h`, mais le moteur du jeu est maintenant independant de cette bibliotheque.

## Build

```sh
make
./snake
```

Build instrumente avec AddressSanitizer et UndefinedBehaviorSanitizer :

```sh
make debug
./snake
```

## Tests du coeur

Les regles du jeu peuvent etre compilees et testees sans `graph.h` ni `-lgraph` :

```sh
make test
```

Ce test couvre notamment le plateau, les changements de direction, la croissance apres une pomme et les collisions.

## Architecture

```text
include/
  board.h       representation du plateau
  snake.h       modele du serpent
  game.h        etat et regles du jeu
  input.h       commandes d'entree generiques
  renderer.h    interface de rendu
  menu.h        interface du menu
  score.h       persistance des scores

src/
  board.c
  snake.c
  game.c        coeur pur C, sans graph.h
  score.c       I/O standard C, sans graph.h
  main.c        orchestration de l'application
  input_graph.c adaptateur clavier graph.h
  renderer_graph.c renderer graph.h
  menu_graph.c  menu graph.h

tests/
  test_core.c
```

Seuls `input_graph.c`, `renderer_graph.c` et `menu_graph.c` connaissent `graph.h`.

## Migration graphique

La prochaine etape peut ajouter un `renderer_raylib.c` et un `input_raylib.c` derriere les memes interfaces. Le moteur `board/snake/game` n'a pas besoin d'etre reecrit.

Les assets restent volontairement a la racine pour ne pas changer les chemins runtime dans cette etape.
