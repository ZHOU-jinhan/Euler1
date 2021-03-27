#/bin/bash

sudo rm -r output
mkdir output
gcc user/main.c library/*/*.c -o ./output/main -lwiringPi -lpthread
sudo ./output/main
