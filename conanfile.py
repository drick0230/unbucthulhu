from conan import ConanFile
from conan.tools.cmake import cmake_layout

class ConanRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch" # Constraint avalaible settings.
    generators = "CMakeDeps", "CMakeToolchain"
    
    def requirements(self):
        self.requires("sfml/2.6.1")

    def requirements(self):
        self.tool_requires("cmake/3.30.5")  
    
    def layout(self):
        cmake_layout(self)
