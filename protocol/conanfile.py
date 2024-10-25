"""
Recette Conan pour le protocole du jeu Unbucthulhu.
"""

import shutil
import conan
import os

import conan.tools
import conan.tools.files


class UnbuchtulhuProtocolRecipe(conan.ConanFile):
    """
    Classe de la recette Conan.
    C'est ici que l'on retrouve l'information relative au projet et
    le code responsable de sa compilation.
    """

    name = "unbucthulhu_protocol"
    version = "0.0.0"
    package_type = "header-library"

    # Optional metadata
    license = "MIT"
    author = "drick0230 61423988+drick0230@users.noreply.github.com"
    url = "https://github.com/drick0230/unbucthulhu/"
    description = 'Projet du protocole du jeu vidéo Unbucthulhu.'
    topics = ("game",)

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "src/*"

    def layout(self):
        """
        Définit la structure de dossier du projet.
        Par exemple, l'emplacement des sources, du fichier compilé,
        des configurations du générateur, etc.
        """
        self.folders.source = "./"
        self.folders.build = "./build/release"
        self.folders.generators = os.path.join(self.folders.build, "generators")

    def build(self):
        """
        Définit les instructions pour compilé
        le projet.
        """
        flatc = self.dependencies["flatbuffers"].cpp_info.bindirs[0] + "/flatc"
        schema_file = os.path.join(self.source_folder, "src/unbucthulhu_protocol.fbs")
        self.run(
            f"{flatc} --cpp --no-includes --reflect-names -o {self.build_folder} {schema_file}"
        )

    def requirements(self):
        """
        Définit les dépendances du projet.
        """
        self.requires("flatbuffers/24.3.25", transitive_headers=True)
        self.tool_requires("flatbuffers/24.3.25")

    def package(self):
        conan.tools.files.copy(
            self, "*.h", self.build_folder, os.path.join(self.package_folder, "include")
        )

    def package_info(self):
        """
        Permet de définir le nom de la librairie.
        Example, inclusion dans CMakeList.txt:
        ```CMAKE
        target_link_libraries(example unbucthulhu_protocol::unbucthulhu_protocol)
        ```
        """
        self.cpp_info.libs = [self.name]
