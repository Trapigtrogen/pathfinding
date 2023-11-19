Pathfinding using A* algorithm. Takes bitmap as input and reads blue pixel as start, red as goal and green ones as walls.
* one-list branch has algorithm where all the checked and unchecked nodes are in single vector and it's working as intended though quite slow. A single frame will take several seconds to draw.
~~* I started the two-list branch where the unchecked and checked nodes are on their own vectors. This version turned out broken and even slower. The blue colour in this one is for debug and indicates how many times the node has been checked.~~ Starting this from scratch. Every time I try to just convert the one-list to two-list it turns into a mess.

# Building
You need to have glut installed

## Windows

```
cd <path to pathfinding>

md build
cd build
md win32
cd win32
cmake ..\
```
Build with your compiler


## Linux

**Cmake doesn't necessarily work. Build script made for Linux**

```
cd <path to pathfinding>

mkdir build
cd build
mkdir linux
cd linux
cmake ..\
make
```
Will create a file called Pathfinding into the build directory
