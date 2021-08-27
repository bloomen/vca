Prerequisites:

* Boost
* Qt5.12

Clone vca and its submodules:
```
git clone --recursive git@github.com:bloomen/vca.git
```

Configure for Xcode:
```
cd vca
mkdir build
export Qt5_DIR=<Path_to_Qt5Config.cmake>
cmake -GXcode ..
```
