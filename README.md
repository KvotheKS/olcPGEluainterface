A simple 'wrapper' program to use the olcPixelGameEngine directly from lua. the game loop is called from C++.

to compile the main program (cpp_base.cpp) use :

In	Linux compile :	g++ -o cpp_base cpp_base.cpp -llua -ldl -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
In  Windows	msvc compile : cl /EHsc cpp_base.cpp

There's a helper script that contains all constant keys and colors featured in the engine. To include it from C++ just #define AUTOMATIC_REQUIRE and assign the path of the script to the variable "path".
