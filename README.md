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

# Toutes les commandes :

## Gestion de l'executable :

*make* pour créer l'executable nommé **plot**.

*make clean* pour tout supprimer sauf le code source, le makfile et le README.

## Lancement du programme :

*./plot _width _height* créé une nouvelle image noire de taille **width**x**height**.

Le programme dispose de deux modes principaux:

- le mode clavier
- le mode souris

Par défaut le programme se lance en mode **souris**.

## Changement et fonctionnement des modes :

### Modes principaux :

Pour basculer entre le mode **clavier** et le mode **souris**, pressez la touche : *p*.

Le mode **souris** prend en compte les fonctionnalités de la sélection à la souris.
Le mode **clavier** prend en compte les fonctionnalités de la sélection au clavier.

### Modes secondaires :

Les trois modes suivants ne peuvent être activés en même temps. En activer un, désactive automatique celui en cours d'utilisation. Cependant, tous les modes peuvent être désactivés. Cela permet, en mode **clavier** de bouger l'image avec les *touches directionnelles*.

#### Mode append :

Basculent on/off avec la touche : *i*.

Ce mode ajoute des sommets à la suite de la ligne brisée avec un *click gauche*.

#### Mode vertex :

Basculement on/off avec la touche : *v*.

Ce mode permet de séléctionner un sommet, soit avec un *click gauche* en mode **souris**, soit par parcours grâce à *page_up* et *page_down* en mode **clavier**.
Ce mode permet également de bouger, en mode **clavier**, un sommet grâce aux *touches directionnelles*.
Enfin, ce mode permet de supprimer le sommet sélectionné grâce à la touche *suppr*.

#### Mode edge :

Basculement on/off avec la touche : *e*.

Ce mode permet de séléctionner une arrête, soit avec un *click gauche* en mode **souris**, soit par parcours grâce à *page_up* et *page_down* en mode **clavier**.
Ce mode permet également d'insérer un sommet entre les deux points reliés par l'arrête sélectionnée avec un *click milieu*.

### Modes semi-permanents :

Les deux modes suivants peuvent être activés en même temps. Leurs activations est égalment compatible avec les trois modes secondaires cités ci-dessus.

#### Mode close :

Basculement on/off avec la touche : *c*.

Ce mode transforme la ligne brisée en polygone fermé et inversement.

#### Mode fill :

Basculement on/off avec la touche : *f*.

Ce mode rempli le polynome.