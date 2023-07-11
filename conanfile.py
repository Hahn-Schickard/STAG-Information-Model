from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.files import load, copy, collect_libs
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain
import re
import os

def to_camel_case(input: str):
    words = input.replace("_", " ").split()
    return '_'.join(word.capitalize() for word in words)

class PackageConan(ConanFile):
    license = "Apache 2.0"
    topics = ("conan", "stag", "modelling", "lwm2m", "technology-adapter")
    requires = [
        "gtest/[~1.11]",
        "variant_visitor/0.1.2@hahn-schickard/stable",
        "nonempty_pointer/0.2.1@hahn-schickard/stable",
    ]
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False],
               "fPIC": [True, False]}
    default_options = {"shared": True,
                       "fPIC": True}
    default_user = "Hahn-Schickard"
    exports_sources = [
        "cmake*",
        "includes*",
        "sources*",
        "CMakeLists.txt"
    ]
    generators = "CMakeDeps"
    short_paths = True

    @property
    def cwd(self):
        return os.path.dirname(os.path.realpath(__file__))

    def set_name(self):
        content = load(self, path=os.path.join(self.cwd, 'CMakeLists.txt'))
        name = re.search('set\(THIS (.*)\)', content).group(1)
        self.name = name.strip().lower()
    
    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, "17")

    def layout(self):
        cmake_layout(self)

    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables['STATIC_CODE_ANALYSIS'] = False
        tc.variables['RUN_TESTS'] = False
        tc.variables['USE_CONAN'] = True
        tc.cache_variables["CMAKE_POLICY_DEFAULT_CMP0077"] = "NEW"
        tc.generate()     

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, pattern='LICENSE', dst='licenses', src=self.cwd)
        copy(self, pattern='NOTICE', dst='licenses', src=self.cwd)
        copy(self, pattern='AUTHORS', dst='licenses', src=self.cwd)

    def package_info(self):
        cmake_target_name = to_camel_case(self.name) + "::" + to_camel_case(self.name)
        self.cpp_info.set_property("cmake_target_name", cmake_target_name)
        self.cpp_info.libdirs = collect_libs(self)
