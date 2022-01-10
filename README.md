Prerequisites:

* Conan: `pip install conan`

Clone vca and its submodules:
```
git clone --recursive git@github.com:bloomen/vca.git
```

Build on Linux:

```
conan profile update settings.compiler.libcxx=libstdc++11 default
mkdir build && cd build
conan install --build=missing ..
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
```

