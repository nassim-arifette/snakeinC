# Snake en C

Projet Snake en C utilisant actuellement la bibliotheque pedagogique `graph.h` / `-lgraph`.

## Build

```sh
make
./snake
```

Pour compiler avec AddressSanitizer et UndefinedBehaviorSanitizer :

```sh
make debug
./snake
```

## Organisation actuelle

- `Snake.c` : cycle de vie du programme, creation/destruction d'une partie et changement de niveau.
- `Spawn.c` : creation/destruction du serpent, pommes et obstacles.
- `Deplacement.c` : deplacement et collisions.
- `Event.c` : clavier et progression de la partie.
- `Dessin.c` : rendu du plateau et du serpent.
- `Menu.c` : menu et options.
- `Info.c` : HUD et high score.
- `Snake.h` : types et API partagee.

Les fichiers generes (`*.o`, executable `snake`) et le fichier runtime `test.txt` sont ignores par Git.

## Prochaine refactorisation

Le coeur du jeu doit progressivement devenir independant de `graph.h`. L'objectif est de separer :

1. `game` / `snake` / `board` : logique pure sans appel graphique ;
2. `input` : conversion des touches vers des commandes de jeu ;
3. `renderer` : implementation graphique interchangeable.

Une fois cette separation faite, un renderer raylib pourra remplacer `graph.h` sans reecrire la logique du Snake.
