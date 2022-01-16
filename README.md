Prerequisites:

* CMake
* Python3
* Conan: `pip install conan`

Clone vca and its submodules:
```
git clone --recursive git@github.com:bloomen/vca.git
```

Note: `--build=missing` can be skipped from below commands when dependencies have already been built.


Build on Linux:

```
conan profile update settings.compiler.libcxx=libstdc++11 default
mkdir build && cd build
conan install --build=missing .. -s build_type=Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
```

Configure for Visual Studio:
```
mkdir build_release && cd build_release
conan install --build=missing .. -s build_type=Release
cmake -G "Visual Studio 16 2019" ..
```
We currently need different build folders for debug vs release on Windows.
