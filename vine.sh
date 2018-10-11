#!/bin/bash

python2 vine_generator.py 5 0.39269908169872414 0.9 4 > vine0.txt
python2 lines_to_svg_colour.py vine0.txt > vine0.svg
python2 vine_generator.py 3 0.3141592653589793 0.6 6 > vine1.txt
python2 lines_to_svg_colour.py vine1.txt > vine1.svg
python2 vine_generator.py 4 0.39269908169872414 0.9 4 > vine2.txt
python2 lines_to_svg_colour.py vine2.txt > vine2.svg
python2 vine_generator.py 6 0.5235987755982988 0.6 5 > vine3.txt
python2 lines_to_svg_colour.py vine3.txt > vine3.svg

python2 mirror.py vine0.txt 4 > vine0mirror.txt
python2 lines_to_svg_colour.py vine0mirror.txt > vine0mirror.svg
python2 mirror.py vine1.txt 1 > vine1mirror.txt
python2 lines_to_svg_colour.py vine1mirror.txt > vine1mirror.svg
python2 mirror.py vine2.txt 3 > vine2mirror.txt
python2 lines_to_svg_colour.py vine2mirror.txt > vine2mirror.svg
python2 mirror.py vine3.txt 6 > vine3mirror.txt
python2 lines_to_svg_colour.py vine3mirror.txt > vine3mirror.svg
