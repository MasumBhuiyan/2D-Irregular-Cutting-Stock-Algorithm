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
1 function merge(Item A, Item B):
2     Item C
3     for (x, y) in A:
4         T = translate B to (x, y)
5          if A and T does not intersect:
6              S = insert B into A
7              R = find axis parallel rectangle that encloses S using minimum area
8              if area of R < area of C then update C with R
10    return C
```
```
1. function split(vector<Item> items, int l, int r):
2.     if l == r then return items[ l ]
3.     m = (l + r) / 2
4.     Item A = split(items, l, m)
5.     Item B = split(items, m + 1, r)
6      Item C
6.     for r1 in rotations:
7.         D = rotate A in r1 degree
8.         for r2 in rotations:
9.             E = rotate B in r2 degree
10.            F = merge(D, E)
12.            if area of F < area of C then update C with F
13.    return C
```
