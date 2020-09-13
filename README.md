# RetroLFO
an 8 bit lfo module

## Project Structure
- `RetroLFO/*` is the production code in the folder format expected by arduino ide
- `RetroLFOTest/*` is a cmake project that pulls in gtest for unit testing the pure functional, hardware independent parts of the codebase

## Building
1. open `RetroLFO` folder in the arduino ide. 
1. connect and configure a programmer for the ATTiny85. 
1. compile and flash the code to the ATTiny85.

## Running tests
1. open the `RetroLFOTest` folder in a terminal. 
1. run `cmake CMakeLists.txt` (this will download and install gtest for the project, if you dont have cmake 2.8.11+ you may need to install cmake)
1. run `make`
1. run `make test` to run all tests
1. run `./fixed_test` or one of the other test binaries to run a particular suite of tests