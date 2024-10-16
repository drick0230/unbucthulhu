# unbucthulhu

Jeu vidéo de type mini-jeux ou "party game"

## Description

Le projet est composé d'un client et d'un serveur.
Le serveur est celui qui est responsable de la logique et de la physique du jeu.
De son côté, le client est responsable de l'affichage du jeu et de la capture des entrées pour un utilisateur.
C'est deux systèmes communiquent entre eux par TCP avec l'aide de ProtoBuf pour la structure des données de l'API.
Pour plus de détails, voir les répertoires des différents sous-projets.

## Structure du projet

- [./client/](./client/): Répertoire du projet du client du jeu.
- [./server/](./server/): Répertoire du projet du serveur du jeu.
- [./protocol/](./protocol/): Répertoire du projet du protocole du jeu.

## Conan

Les projets de ce répertoire Git utilisent le gestionnaire de paquets C/C++ Conan.

### Configuration du profile

Sur Fedora, Conan requiert quelques ajustements pour bien fonctionner.

Pour commencer, le profile générer avec `conan profile detect` spécifie une version de compilateur qui n'est pas disponible sur Conan Center. De ce fait il faut la descendre à 11. Dans le cas contraire, il vous faudra spécifier `--build=missing` pour créer les paquets Conan au lieu de simplement téléchargé le binaire.

```SH
sed -Ei 's/(compiler.version)=.*$/\1=11/' $(conan profile path default)
```

\* Le profile peut-être changé selon les besoins.

### Compilation d'un projet

Le projet se compile et ses dépendances (Conan ou système) s'installent avec la commande suivante:

```SH
conan build path/to/project \
--profile:all default \
--conf tools.system.package_manager:mode=install \
--conf tools.system.package_manager:sudo=True
```

\* Le profile peut-être changé selon les besoins.

### Création du paquet Coann d'un projet

Le projet peut aussi être générer sous formes de paquet Conan.
La commande suivante crée un paquet Conan à partir du projet et l'ajoute au cache de Conan, mais ne génère pas de fichiers dans le projet.
À noter que cela exécute aussi les tests du paquet, incluant l'éxécution de celui-ci.
C'est donc une commande pratique pour compiler et éxécuter le code par la suite.

```SH
conan create path/to/project \
--profile:all default \
--conf tools.system.package_manager:mode=install \
--conf tools.system.package_manager:sudo=True
```

\* Le profile peut-être changé selon les besoins.

Pour afficher les paquets du projet disponibles sur votre machine, il suffit d'exécuter:

```SH
conan list projectName
```

`projectName` peut être par exemple, unbucthulhu_client, unbucthulhu_server, unbucthulhu_protocole, etc.
