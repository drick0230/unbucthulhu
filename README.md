# unbucthulhu

Jeu vidéo de type mini-jeux ou "party game"

## Description

Le projet est composé d'un client et d'un serveur.
Le serveur est celui qui est responsable de la logique et de la physique du jeu.
De son côté, le client est responsable de l'affichage du jeu et de la capture des entrées pour un utilisateur.
C'est deux systèmes communiquent entre eux par TCP avec l'aide de ProtoBuf pour la structure des données de l'API.
Pour plus de détails, voir les répertoires des différents sous-projets.

## Structure du projet

- `./client/`: Répertoire du projet du client du jeu.
- `./server/`: Répertoire du projet du serveur du jeu.
- `./common/`: Répertoire des sources communes aux projets.