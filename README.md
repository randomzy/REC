# robotics_v1_entry

## Requirements
  + CMake 3.13 or above (for the command line)
  + Compiler that supports c++ 17 (tested with GCC 10 and Clang 13 on Linux)
## Build

Run the following commands in the root directory of this repository

```
cmake -S cpp -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
## Run
In the directory where input.bin and output.bin are located, run
```
path-to-repo/build/RedEyeCorrection
```
where path-to-repo is path to the root of this repository