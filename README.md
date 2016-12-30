# World

The intent is to build a game such as TTD with modern opengl approach.
Anyway, don't expect it to be finished soon :-D
This is a big project to learn and increase my c++ dev skills.

## Compilation

See required libraries before trying to build World !

cd build && cmake .. && make

Tested on :

* Kubuntu 16.04

## Required Libraries

* libglfw3-dev

* https://github.com/hsaturn/SaturnLib
This library should be cloned in the same folder as World.

cd SaturnLib && cmake . && sudo make install