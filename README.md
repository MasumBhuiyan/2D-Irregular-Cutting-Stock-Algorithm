# 2D Irregular Cutting Stock Algorithm &middot; ![Build Status](https://img.shields.io/travis/npm/npm/latest.svg?style=flat-square) ![Language](https://img.shields.io/badge/C%2B%2B-11-blue) ![CMake](https://img.shields.io/badge/CMake-V%203.10%2B-blue)

## Table of Contents
1. [ Description ](#description)
2. [ Installation ](#installation)
3. [ Build ](#build)
4. [ Algorithm ](#algorithm)
5. [ Credits ](#credits)
6. [ License ](#license)
7. [ Conclusion ](#conclusion) 


<a name="description"></a>
## Description
2D Irregular Cutting Stock Algorithm.<br><br>
**Applications**
* A
* B
* C
---
<a name="installation"></a>
## Installation ![os](https://img.shields.io/badge/os-linux-orange) ![editor](https://img.shields.io/badge/sublime_text-3-blue)
* Install **python 3** if not installed. <br>
To install open terminal `Ctrl+Alt+T` and type the following commands<br>
> `$ sudo apt-get update` <br>
> `$ sudo apt-get install python3.7` <br>
> `$ python --version` <br>
* Clone the repository by the following command <br>
> `git clone [repository url]` <br>
* Open the repository with **sublime text 3**
* Open file **__main__.py** <br> 
* Run the program by `Cntrl+B` <br>
---
<a name="build"></a>
## Build ![os](https://img.shields.io/badge/os-linux-orange)
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
---
<a name="algorithm"></a>
## Algorithm &middot; ![Build Status](https://img.shields.io/travis/npm/npm/latest.svg?style=flat-square) <br>
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
7.     for r1 in rotations:
8.         D = rotate A in r1 degree
9.         for r2 in rotations:
10.            E = rotate B in r2 degree
11.            F = merge(D, E)
12.            if area of F < area of C then update C with F
13.    return C
```
<a name="credits"></a>
## Credits
- Masum Bhuiyan
- B.Sc. in Computer Science (Expected September 2020)
- Jahangirnagar University, Dhaka, Bangladesh
- Reach me @[LinkedIn](https://www.linkedin.com/in/masumbhuiyan577/)
<a name="license"></a>
## Lincense ![license](https://img.shields.io/badge/license-MIT-green) <br>
MIT License

Copyright (c) 2020 MasumBhuiyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
<a name="conclusion"></a>
## Conclusion
Checkout the [wiki](https://www.demo.com) for detail description of the project.
