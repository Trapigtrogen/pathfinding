Pathfinding using A* algorithm. Takes bitmap as input and reads blue pixel as start, red as goal and green ones as walls.

# Building

## Linux

```
cd <path to pathfinding>

mkdir build
cd build
mkdir linux
cd linux
cmake ..
make
```
Will create a file called Pathfinding into the build directory

## Window

### I don't have Windows so I cannot test for it

* Build Windows version of Drawpp and replace external/Drawpp: https://gitlab.com/Trapigtrogen/graphics-library
* Prebuilt library exists but is out-of-date: https://gitlab.com/Trapigtrogen/graphics-library/-/releases/v1.0.0
* Same Cmake steps for Windows. It works if it works idk
* Use Linux on live boot, virtual machine or WSL
