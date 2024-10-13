"""
Recette Conan pour le client du jeu Unbucthulhu.
"""

import conan

class UnbucthulhuRecipe(conan.ConanFile):
    """
    Classe de la recette Conan.
    C'est ici que l'on retrouve l'information relative au projet et
    le code responsable de sa compilation.
    """
    name = "unbucthulhu_client"
    version = "0.0.0"
    package_type = "application"

    # Optional metadata
    license = "MIT"
    author = "drick0230 61423988+drick0230@users.noreply.github.com"
    url = "https://github.com/drick0230/unbucthulhu/"
    description = 'Jeu vidéo de type mini-jeux ou "party game"'
    topics = ("game", )

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"
    cmake_variables = dict([("CMAKE_PROJECT_NAME", name)])


    def layout(self):
        """
        Définit la structure de dossier du projet.
        Par exemple, l'emplacement des sources, du fichier compilé,
        des configurations du générateur, etc.
        """
        conan.tools.cmake.cmake_layout(self)

    def generate(self):
        """
        Définit les instructions pour générer les fichiers
        nécéssaires pour compilé le projet.
        """
        deps = conan.tools.cmake.CMakeDeps(self)
        deps.generate()
        tc = conan.tools.cmake.CMakeToolchain(self)
        tc.generate()

    def build(self):
        """
        Définit les instructions pour compilé
        le projet.
        """
        cmake = conan.tools.cmake.CMake(self)
        cmake.configure(variables=self.cmake_variables)
        cmake.build()

    def package(self):
        """
        Définit les fichiers à transférer du dossier de sortie (build folder)
        vers celui du paquet Conan généré.
        """
        cmake = conan.tools.cmake.CMake(self)
        cmake.install()

    def requirements(self):
        """
        Définit les dépendances du projet.
        """
        self.requires("sfml/2.6.1")
