# Audit technique

## Phase 1 - stabilisation

La premiere passe a corrige :

- l'allocation invalide du serpent ;
- plusieurs ecritures hors limites dans le menu ;
- les boucles recursives entre jeu, niveaux et menu ;
- les fuites principales et les destructeurs manquants ;
- le RNG reseede plusieurs fois ;
- les boucles potentiellement infinies de spawn ;
- la lecture fragile du high score ;
- les artefacts compiles versionnes ;
- le Makefile et une cible ASan/UBSan.

## Phase 2 - separation du coeur

Cette passe extrait un moteur pur C :

- `Board` utilise maintenant un stockage contigu ;
- `Game` possede directement le `Snake` ;
- `game_step()` concentre mouvement, collisions, pommes et progression ;
- le moteur ne dessine plus pendant un mouvement ou un spawn ;
- le high score n'est plus relu depuis le disque a chaque frame ;
- `score.c` isole la persistance ;
- `input_graph.c`, `renderer_graph.c` et `menu_graph.c` sont les seuls fichiers qui dependent de `graph.h` ;
- `make test` compile et execute le coeur sans `-lgraph`.

## Frontiere d'architecture

```text
          +-----------------------+
          |       main.c          |
          +-----------+-----------+
                      |
       +--------------+--------------+
       |              |              |
   input.h        renderer.h       menu.h
       |              |              |
 input_graph.c  renderer_graph.c  menu_graph.c
       \              |              /
        \-------------+-------------/
                      |
              +-------v-------+
              |    game.h     |
              +-------+-------+
                      |
              +-------+-------+
              |               |
           board.h          snake.h
```

Le coeur (`board.c`, `snake.c`, `game.c`) ne depend d'aucune bibliotheque graphique.

## Suite recommandee

1. Valider le runtime avec la vraie bibliotheque `graph.h` et `make debug`.
2. Etendre les tests unitaires aux portails et aux changements de niveau.
3. Ajouter `renderer_raylib.c` et `input_raylib.c`.
4. Porter le menu vers raylib.
5. Une fois la parite fonctionnelle obtenue, retirer `-lgraph`.


## Phase 3 - backend raylib parallele

Un second backend est maintenant disponible sans modifier le moteur :

- `input_raylib.c` traduit les touches raylib vers `InputCommand` ;
- `renderer_raylib.c` implemente le rendu via raylib ;
- `menu_raylib.c` fournit un menu natif raylib ;
- `make raylib` produit `snake-raylib` ;
- le backend historique reste le build par defaut avec `make`.

Le changement de bibliotheque n'affecte donc plus `board.c`, `snake.c`, `game.c`, `score.c` ni `main.c`.

### Politique de migration

Le backend `graph.h` reste disponible tant que la version raylib n'a pas ete validee visuellement et fonctionnellement. Une fois la parite confirmee, raylib pourra devenir le backend par defaut dans un commit distinct.
