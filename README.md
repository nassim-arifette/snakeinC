# Snake

Snake en C11 avec **raylib** comme unique bibliotheque graphique.

Le projet separe la logique du jeu de l'interface : le coeur (`board`, `snake`, `game`) ne depend pas de raylib et peut etre teste sans ouvrir de fenetre.

## Prerequis

- un compilateur C11 (`gcc` ou `clang`) ;
- `make` ;
- raylib 6.0 ;
- `pkg-config` recommande pour retrouver automatiquement les flags raylib.

## Compiler et lancer

```sh
make
make run
```

L'executable est genere dans `build/bin/snake`. Les assets sont recopies dans `build/bin/assets` afin que le binaire puisse aussi etre lance directement.

La fenetre est dimensionnee une seule fois au demarrage, d'apres la taille de l'ecran. Le menu et la partie partagent ensuite cette meme taille : le menu est mis a l'echelle et le plateau adapte sa taille de case.

## Tests

```sh
make test
make sanitize-test
```

`make test` compile uniquement le coeur du jeu et ne depend pas de raylib. `make sanitize-test` execute les memes tests avec AddressSanitizer et UndefinedBehaviorSanitizer.

Pour compiler l'application complete avec les sanitizers :

```sh
make debug
```

## Structure

```text
.
├── assets/                 # images utilisees par raylib
│   ├── backgrounds/
│   └── sprites/
├── data/                   # donnees runtime (highscore ignore par Git)
├── docs/                   # documentation d'architecture
├── include/snake/          # API publique des modules
├── src/
│   ├── app/                # orchestration menu/partie
│   ├── core/               # logique pure C (jeu et mise en page)
│   ├── persistence/        # sauvegarde du meilleur score
│   ├── platform/           # clavier raylib
│   ├── ui/                 # menu, assets et rendu raylib
│   └── main.c
└── tests/                  # tests unitaires du coeur
```

Voir [`docs/architecture.md`](docs/architecture.md) pour les dependances entre modules.

## Controles

- Fleches ou `WASD` : direction ;
- `Espace` / `Entree` : demarrer ;
- `Echap` : quitter la partie et revenir au menu ;
- bouton de fermeture de la fenetre : quitter l'application.

## Score

Seul le meilleur score est persiste dans `data/highscore.txt`. Ce fichier est cree automatiquement et n'est pas versionne.

## Outils utiles

```sh
make format       # necessite clang-format
make clean        # supprime build/
make dist         # cree build/snake-source.tar.gz
```
