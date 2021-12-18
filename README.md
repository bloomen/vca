Prerequisites:

* Curl
* Boost
* Qt5.12

Clone vca and its submodules:
```
git clone --recursive git@github.com:bloomen/vca.git
```

Configure for Xcode:
```
cd vca
mkdir build && cd build
export Qt5_DIR=<Path_to_Qt5Config.cmake>
cmake -GXcode ..
```

Configure for QtCreator on Windows:
* Install Visual Studio 2017
* Tell QtCreator to use VS2017
