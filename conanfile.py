from conan import ConanFile
import os


# from conan.tools.files import clear_folder


class GladGlfwConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = ["glad/0.1.36", "glfw/3.3.2", "freetype/2.13.0", "glm/cci.20230113", "nlohmann_json/3.11.2",
                "assimp/5.0.1"]
    generators = "CMakeDeps", "CMakeToolchain"

    def package(self):
        self.copy("*.h", dst="include", src="glad")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def configure(self):
        self.options['glad'].shared = False
        self.options['glad'].fPIC = True
        self.options['glad'].no_loader = False
        self.options['glad'].spec = 'gl'
        self.options['glad'].gl_profile = 'core'
        self.options['glad'].gl_version = '4.3'
    # def layout(self):
    #     # We make the assumption that if the compiler is msvc the
    #     # CMake generator is multi-config
    #     multi = True if self.settings.get_safe("compiler") == "msvc" else False
    #     if multi:
    #         self.folders.generators = os.path.join("build", "generators")
    #     else:
    #         self.folders.generators = os.path.join("build", str(self.settings.build_type), "generators")