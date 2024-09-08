#!/bin/bash

make

./ppmcvt -o out01.pbm clock.ppm

./ppmcvt -g 1000 -o out02.pgm clock.ppm

./ppmcvt -i red -o out03.pbm clock.ppm

./ppmcvt -i green -o out04.pbm clock.ppm

./ppmcvt -i blue -o out05.pbm clock.ppm

./ppmcvt -r red -o out06.ppm clock.ppm

./ppmcvt -r green -o out07.ppm clock.ppm

./ppmcvt -r blue -o out08.ppm clock.ppm

./ppmcvt -s -o out09.pgm clock.ppm

./ppmcvt -m -o out10.pbm clock.ppm

./ppmcvt -t 4 -o out11.pgm clock.ppm

./ppmcvt -n 4 -o out12.ppm clock.ppm
