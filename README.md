# Obtention de l'image exemple :

- ./plot 500 500
- 5 clicks gauche pour dessiner grossièrement les traits.
- Touche e pour le passage en mode edge.
- Affiner les traits en ajoutant des sommets grâce au click milieu.
- Touche v pour le passage en mode vertex.
- Touche suppr et click gauche pour enlever les sommets mal placés.
- Touche p pour passer en mode clavier pour sélectionner précisément des sommets trop proches.
- Touche suppr et page suivante/précédente pour la correction.
- Touche c pour fermer le polygone.
- Touche f pour remplir.

# Choix d'implémentation

* Un polynome est représenté par une liste doublement chaînée.
* L'ajout d'un sommet se fait en tête de liste, cela implique que le dernier élément ajouté est toujous à l'indice 0.
* Le remplissage par la méthode scan-line est fait par vérification de la conrrespondace avec dernière couleur rencontrée. Cela permet de ne pas tenir compte des pixels horizontaux de même couleur, car ils sont comptés comme un seul pixel. Les sommets ne sont pas ajoutés à la liste des points formant les couples d'extrémités, sauf s'ils ne sont pas des maximas locaux. Il arrive, dans certains cas, que le derniers pixel tracé avec l'algorithme de bresenham ne soit pas exactement sur les coordonnées du point. Cela entraine une erreur dans le remplissage.
* Les commandes clavier ont été conservées après l'ajout des commandes souris pour plus de précision dans certains choix de sommet ou d'arrêtes.
* Le calcul du sommet le plus proche se fait par simple comparaison de la distance entre les sommets et les coordonnées du pixel qui a été clické.
* Le calcul de l'arrête la plus proche se fait à partir du sommet le plus proche. En effet, cinq distances sont calculées : 
    - La distance entre le point clické (noté A) et le sommet le plus proche (noté B).
    - La distance entre le point clické (noté A) et le point suivant le sommet le plus proche (noté C).
    - La distance entre le point clické (noté A) et le point précédent le sommet le plus proche (noté D).
    - La distance entre le sommet le plus proche (noté B) et son suivant (noté C).
    - La distance entre le sommet le plus proche (noté B) et son précédent (noté D).
Ensuite les angles ABC et ABD sont calculés grâce à la règle de cosinus, et selon quel angle est le plus petit, l'indice de l'arrête est choisie. Du fait que la sélection se fait grâce au sommet le plus proche, il se peut que, dans le cas où un sommet n'appartenant pas à l'arrête la plus proche soit le sommet le plus proche, cela fausse tout le calcul et donc l'affichage. Je n'ai pas tenu compte de ce problème, car il était stipulé dans le sujet qu'il fallait partir du plus proche voisin et, car je n'ai pas trouvé de méthode adéquate pour palier ce problème.

# Toutes les commandes :

## Gestion de l'executable :

*make* pour créer l'executable nommé **plot**.

*make clean* pour tout supprimer sauf le code source, le makfile et le README.

## Lancement du programme :

*./plot _width _height* créé une nouvelle image noire de taille **width**x**height**.

## Changement et fonctionnement des modes :

### Modes principaux :

Les trois modes suivants ne peuvent être activés en même temps. En activer un, désactive automatique celui en cours d'utilisation. Cependant, tous les modes peuvent être désactivés.

#### Mode append :

Basculent on/off avec la touche : *i*.

Ce mode ajoute des sommets à la suite de la ligne brisée avec un *click gauche*.
Ce mode permet également de bouger l'image avec les *touches directionnelles*.

#### Mode vertex :

Basculement on/off avec la touche : *v*.

Ce mode permet de séléctionner un sommet, soit avec un *click gauche*, soit par parcours grâce à *page_up* et *page_down*.
Ce mode permet également de bouger un sommet grâce aux *touches directionnelles*.
Enfin, ce mode permet de supprimer le sommet sélectionné grâce à la touche *suppr*.

#### Mode edge :

Basculement on/off avec la touche : *e*.

Ce mode permet de séléctionner une arrête, soit avec un *click gauche*, soit par parcours grâce à *page_up* et *page_down*.
Ce mode permet également d'insérer un sommet entre les deux points reliés par l'arrête sélectionnée avec un *click milieu*.

### Modes secondaires :

Les deux modes suivants peuvent être activés en même temps. Leurs activations est égalment compatible avec les trois modes principaux cités ci-dessus.

#### Mode close :

Basculement on/off avec la touche : *c*.

Ce mode transforme la ligne brisée en polygone fermé et inversement.

#### Mode fill :

Basculement on/off avec la touche : *f*.

Ce mode rempli le polynome.