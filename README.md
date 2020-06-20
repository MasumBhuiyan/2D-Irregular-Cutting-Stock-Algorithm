# 2D Irregular Cutting Stock Algorithm &middot; ![Build Status](https://img.shields.io/travis/npm/npm/latest.svg?style=flat-square) ![Language](https://img.shields.io/badge/C%2B%2B-11-blue) ![CMake](https://img.shields.io/badge/CMake-V%203.10%2B-blue)
## Building and Testing ![os](https://img.shields.io/badge/os-linux-orange)
* Install ![cmake](https://img.shields.io/badge/CMake-V%203.10%2B-blue) if not installed.
* Open terminal and go to the target folder.
* Run the following commands <br>
  > `$ mkdir build` <br>
    `$ cd build` <br>
    `$ cmake ..`<br>
    `$ make`<br>
* In order to run the tests
  > `$ make test`
* An executable file **(run)** will be created.
* Run command `$ ./run` to execute the program.


## Proposed Algorithm &middot; ![Build Status](https://img.shields.io/travis/npm/npm/latest.svg?style=flat-square) <br>
```
function merge(Item A, Item B):
  Matrix Best
  for (x, y) in A:
    C = translate B to (x, y)
    if A and C does not intersect:
      D = insert C into A
      R = find bounding rectangle with minimum area
      if area of R < area of Best:
        update Best with R
  return Best
```
