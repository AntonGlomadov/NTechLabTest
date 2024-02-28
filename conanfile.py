from conans import ConanFile, CMake, tools
import os


class NtechLab(ConanFile):
    name = "NtechLab"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_find_package"
    requires = [("gtest/1.14.0")]