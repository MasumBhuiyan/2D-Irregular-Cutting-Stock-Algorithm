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
function: SVC()
        
    - initialize OPTIMAL_CUTTING_PATTERN, OPTIMAL_WASTE = FFD(vector<ITEM>)
    - while in each iteration:
    -     calculate COVERED_AREA of each ITEM in current OPTIMAL_CUTTING_PATTERN
    -     generate NEW_CUTTING_PATTERN, NEW_WASTE by function PCA(COVERED_AREA, OPTIMAL_CUTTING_PATTERN)
    -     if OPTIMAL_WASTE > NEW_WASTE: update OPTIMAL_CUTTING_PATTERN
    - end while
    - return OPTIMAL_CUTTING_PATTERN and OPTIMAL_WASTE
    
function: FFD(vector &PIECES): returns initial cutting pattern

    - sort PIECES by decreasing order of area
    - for each ITEM in PIECES:
    -     X = INF, Y, A = 0 find leftmost pivot such that no overlap if mapped into X,Y at A rotation
    -     for each r in rotations:
    -         rotate the ITEM by r degree
    -         BINARY_ITEM = pixelize the ITEM to 0/1 matrix 
    -         hodograph: select a pivot point that maps BINARY_ITEM in BINARY_STOCK no overlap. let say we choose (x, y)
    -         if X > x: X = x, Y = y, A = r;
    -     end
    -     add ITEM in BINARY_STOCK at rotation A into point (X, Y)
    - end
    - return CUTTING_PATTERN, WASTE
    
function: PCA(vector CoveredArea, vector &PIECES)
  
    - calculate material consumption rates, R = CoveredAreas / Areas
    - sort PIECES by value of R
    - PLACED_ITEMS = empty, NON_PLACED_ITEMS = PIECES
    - while NON_PLACED_ITEMS not empty:
    -     V = -INF, ID
    -     for each ITEM in NON_PLACED_ITEMS:
    -         for each r in rotations:
    -             rotate the ITEM by r degree
    -             BINARY_ITEM = pixelize the ITEM to 0/1 matrix 
    -             hodograph: select a pivot point that maps BINARY_ITEM in BINARY_STOCK no overlap. let say we choose (x, y)
    -             calculate V'(x, y, r degree)
    -             if V < V': V = V', X = x, Y = y, ID = ITEM.ID, A = r
    -         end for
    -     end for
    -     add ITEM no ID in BINARY_STOCK at A rotation into point (X, Y)
    -     PLACED.add(ID), NON_PLACED_ITEMS.erase(ID)
    - end while
```
