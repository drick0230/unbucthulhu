# unbucthulhu
Jeu vidéo de type mini-jeux ou "party game"

## Installation des dépendances

Puisque certaines dépendances ne sont pas disponible sur les versions plus récentes que c++11, il faut spécifier la version du compilateur.

### Fedora

```SH
sudo dnf install conan
conan profile detect
sed -Ei 's/(compiler.version)=.*$/\1=11/' $(conan profile path default)
conan install . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True --profile drick0230_unbucthulhu
```

## Compilation et exécution

```SH
conan install . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True --profile drick0230_unbucthulhu
source ./build/Release/generators/conanbuild.sh
cmake --preset conan-release
cmake --build ./build/Release
source ./build/Release/generators/deactivate_conanbuild.sh
./build/Release/unbucthulhu
```
