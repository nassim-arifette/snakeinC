# Historique de l'audit

Le projet initial etait un prototype pedagogique base sur `graph.h`. La refactorisation a ete faite par etapes afin de conserver les regles du jeu tout en eliminant les principaux risques.

## Problemes corriges

- dereferencement d'un `Serpent *` jamais alloue ;
- ecritures hors limites dans la saisie du menu ;
- boucles recursives entre partie, changement de niveau et menu ;
- fuites de `Game`, serpent et plateau ;
- generation aleatoire pouvant boucler sans fin ;
- reseeding de `rand()` plusieurs fois ;
- lecture fragile du fichier de scores ;
- nombres magiques pour les cases ;
- objets `.o`, binaire et donnees runtime versionnes ;
- couplage de toutes les structures a `graph.h` ;
- `int **` pour le plateau ;
- `usleep()` dans la boucle principale.

## Etat actuel

- raylib est l'unique bibliotheque graphique ;
- `graph.h` et tout son backend ont ete retires ;
- le plateau est contigu ;
- le coeur est testable sans fenetre ;
- les assets, sources, headers, tests et docs sont ranges par responsabilite ;
- les artefacts de compilation vont uniquement dans `build/` ;
- ASan/UBSan font partie des cibles de test ;
- un workflow CI execute les tests du coeur a chaque push/PR.

## Ameliorations possibles plus tard

- remplacer la liste chainee du serpent par un buffer circulaire pour supprimer les allocations a chaque tick ;
- ajouter des tests d'integration du renderer avec screenshots de reference ;
- ajouter une configuration utilisateur persistante ;
- ajouter audio et animations sans modifier le coeur du jeu.
