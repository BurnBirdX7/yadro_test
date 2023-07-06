# Ядро - Тестовое задание

# Requirements
 * Compiler
   * GNU C++ Compiler 13+ (tested)
   * Clang 16+ (theoretically)
     * C++20's Ranges are used, that are not supported until Clang 16

## Build

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

