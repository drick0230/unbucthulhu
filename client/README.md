# unbucthulhu_client

Client du jeu vidéo Unbucthulhu.

## Installation des dépendances

Puisque certaines dépendances ne sont pas disponible sur les versions plus récentes que c++11, il faut spécifier la version du compilateur.

### Fedora

```SH
sudo dnf install conan
conan profile detect --name drick0230_unbucthulhu_client
sed -Ei 's/(compiler.version)=.*$/\1=11/' $(conan profile path drick0230_unbucthulhu_client)
conan install . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True --profile drick0230_unbucthulhu_client
```

## Compilation

Le projet peut être compilé avec Conan.
Pour ce faire, il suffit d'exécuter la commande suivante:


```SH
conan build . --profile drick0230_unbucthulhu_client
```

Cela va générer un exécutable dans le dossier ciblé par l'outil de génération utilisé.
Par exemple, avec CMake sur Fedora, l'exécutable se trouve à `./build/Release/unbucthulhu_client`.

## Création du paquet Conan

Le projet peut aussi être générer sous formes de paquet Conan.
La commande suivante crée un paquet Conan à partir du projet et l'ajoute au cache de Conan.
À noter que cela exécute aussi les tests du paquet, incluant l'éxécution de celui-ci.
C'est donc une command pratique pour compiler et éxécuter le code par la suite.

```SH
conan create . --profile drick0230_unbucthulhu_client
```

Pour afficher les paquets du projet disponibles sur votre machine, il suffit d'exécuter:

```SH
conan list unbucthulhu_client
```
