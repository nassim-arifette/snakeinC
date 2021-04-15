# Audit technique

## Problemes corriges dans cette branche

### Memoire

- allocation manquante de `Serpent` dans `SpawnSerpent` ;
- destructeurs explicites pour les noeuds du serpent et la grille ;
- suppression des ecritures hors limites dans la saisie du menu ;
- gestion des echecs d'allocation de la grille et du serpent.

### Cycle de vie

- suppression de la boucle de jeu recursive dans `lvlup` ;
- suppression du rappel de `MenuBase` depuis la boucle de jeu ;
- une partie quitte maintenant reellement sa boucle lorsque `game->end` est positionne ;
- le menu est rouvert par `main` apres la fin d'une partie.

### Robustesse

- generation des pommes et obstacles bornee par le nombre de cases libres ;
- `srand` appele une seule fois au demarrage ;
- vitesse minimale afin d'eviter un delai negatif ;
- lecture du high score basee uniquement sur le resultat de `fscanf` ;
- enum pour les types de cases et les modes de jeu ;
- include guard et prototypes complets dans `Snake.h`.

### Depot / build

- suppression des `.o`, du binaire `snake`, du fichier de scores et du fichier objet parasite ;
- ajout de `.gitignore` ;
- warnings GCC plus stricts ;
- cible `make debug` avec ASan/UBSan ;
- correction de la cible `compress`.

## Points encore a traiter

1. Separer completement la logique du rendu : `Spawn.c` et d'autres fichiers appellent encore directement `graph.h`.
2. Sortir les structures dans plusieurs headers (`game.h`, `snake.h`, `renderer.h`, etc.).
3. Remplacer `int **plat` par une representation contigue ou une structure `Board`.
4. Remplacer le fichier de scores par une persistance plus propre et ne pas relire le disque a chaque frame.
5. Ajouter des tests unitaires de logique sans fenetre graphique.
6. Ajouter une abstraction `Renderer`, puis une implementation raylib.

## Ordre recommande pour la suite

- Phase 1 : stabiliser cette branche avec le vrai environnement `graph.h` et les sanitizers.
- Phase 2 : extraire un coeur de jeu pur C testable sans interface graphique.
- Phase 3 : ajouter un renderer raylib en parallele de l'ancien renderer.
- Phase 4 : supprimer `graph.h` lorsque la parite fonctionnelle est validee.
