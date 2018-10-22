Pentago
=======
Game project developed for educational purpose.

## Build

Project is using (modern) CMake as a build system.

1. Make sure git submodules are up-to-date:
    ```sh
    git submodule update --init
    ```

2. Configure CMake:
    ```sh
    cmake -G"<generator>" -DCMAKE_PREFIX_PATH=<path-to-Qt5-cmake-finders> <path-to-sources>
    ```

3. Build, using IDE, or from command-line:
    ```sh
    cmake --build .
    ```

## Unit testing

Google Test is used as a testing framework for C++ code.

All individual tests integrated with CTest.
<br>Sample run (from build directory):
```sh
ctest -T
```
