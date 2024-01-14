VCA
---

A prototype for file indexing and discovery.

Prerequisites:

* CMake
* Python3
* Conan: `pip install conan`
* clang-format 10

Clone vca and its submodules:
```
git clone --recursive git@github.com:bloomen/vca.git
```

Note: `conan install` can be skipped from below commands when dependencies have already been built for a given build type.

Build on Linux:

```
conan profile update settings.compiler.libcxx=libstdc++11 default
mkdir build_release && cd build_release
conan install --build=missing .. -s build_type=Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
```

Configure for Visual Studio:
```
mkdir build_release && cd build_release
conan install --build=missing .. -s build_type=Release
cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release ..
```
We currently need different build folders for debug vs release on Windows.
