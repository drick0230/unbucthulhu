"""
Recette Conan pour le protocole du jeu Unbucthulhu.
"""

import conan


class UnbuchtulhuProtocolRecipe(conan.ConanFile):
    """
    Classe de la recette Conan.
    C'est ici que l'on retrouve l'information relative au projet et
    le code responsable de sa compilation.
    """

    name = "unbucthulhu_protocol"
    version = "0.0.0"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "drick0230 61423988+drick0230@users.noreply.github.com"
    url = "https://github.com/drick0230/unbucthulhu/"
    description = 'Projet du protocole du jeu vidéo Unbucthulhu.'
    topics = ("game",)

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

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

        # Variable used to propagate the project's name.
        cmake.configure(variables=dict([("CMAKE_PROJECT_NAME", self.name)]))
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
        self.requires("protobuf/5.27.0", transitive_headers=True)
        self.tool_requires("protobuf/5.27.0")

    def package_info(self):
        """
        Permet de définir le nom de la librairie.
        Example, inclusion dans CMakeList.txt:
        ```CMAKE
        target_link_libraries(example unbucthulhu_protocol::unbucthulhu_protocol)
        ```
        """
        self.cpp_info.libs = [self.name]
