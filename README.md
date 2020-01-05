# Theremultime

Theremultime is the ultimate theremine player system !

## Description

From your laptop camera, use your right hand to set the pitch and your left hand to set the volume.

## Setup

You need a working version of opencv.
From the root repository, type the following command in a terminal to compile the program:
```
mkdir build
cd build
cmake ..
make
```
Then use `./pgm` to run it.

## Usage

Use the trackbar to change the color detected by the program.
The default parameters allow playing while wearing light blue dish washing gloves.
Be aware that the color you want to detect must refer exclusively to your hand; any moving object of the same color may cause interferences.

When the right color is chosen, move your hands in the air to make sound !

To quit the program press Ctrl + C in the terminal.