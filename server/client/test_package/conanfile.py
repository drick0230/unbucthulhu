"""
Recette Conan pour tester le paquet Conan généré.
C'est une sorte de sous-projet Conan qui vérfie que
le paquet Conan généré est valide.
Ce n'est pas destiné à faire des tests unitaires.
"""

import conan

class UnbucthulhuTestConan(conan.ConanFile):
    """
    Classe de la recette Conan.
    C'est ici que l'on retrouve l'information relative au projet et
    le code responsable de sa compilation.
    """
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        """
        Définit les dépendances du projet de test.
        """
        self.requires(self.tested_reference_str)

    def test(self):
        """
        Définit les instructions pour testé le paquet Conan généré.
        """
        if conan.tools.build.can_run(self):
            self.run(self.tested_reference_str.split(sep="/")[0], env="conanrun")
