# Ядро - Тестовое задание

# Requirements
 * C++ Compiler
   * GNU 10+
     * tested: CI [ 10, 11, 12 ], locally [ 13 ]
   * Clang 12+
     * tested: CI [ 12, 13, 14 ], locally [ 15 ]
 * CMake 3.8+

## Build with CMake

In project's root run:

```shell
cmake -B build_dir
cmake --build build_dir --target task
```

## Run

```shell
./build_dir/task input.txt
```

## Test

````shell
cmake --build build_dir --target test
./build_dir/test
````

