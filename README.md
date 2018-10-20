Pentago
=======
Game project developed for educational purpose.

## Build

Project is using (modern) CMake as a build system.

Sample run:
```sh
cmake -G"<generator>" -DCMAKE_PREFIX_PATH=<path-to-Qt5-cmake-finders> <path-to-sources>
```

## Unit testing

Google Test is used as a testing framework for C++ code.

All individual tests integrated with CTest.
Sample run (in build directory):
```sh
ctest -T
```
