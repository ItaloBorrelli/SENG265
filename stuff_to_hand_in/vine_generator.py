import sys
import Line_Point_colour

from math import pi
from random import randint

'''
purpose
	create and move the next line into place
preconditions
	None
'''

def next(line, angle, scale, trunk):
	branch = Line_Point_colour.Line(line.point0, line.point1)
	branch.translate(-line.point0.x, -line.point0.y)

	if trunk:	# a trunk is not scaled
		branch.rotate(angle)
	else:
		branch.rotate(angle * 2)
		branch.scale(scale)

	branch.translate(line.point1.x, line.point1.y)
	# branch.translate(line.point0.x, line.point0.y)
	# branch.translate(line.point1.x-line.point0.x, line.point1.y-line.point0.y)

	return branch

'''
purpose
	recursively creates and draws the branches of a tree
preconditions
	None
'''

def draw(line, num, angle, scale, depth):
	# base case reached
	if depth == 0:
		return

	# draw $num branches rotated evenly spaced around the origin
	for i in range(num):
		curr_line = Line_Point_colour.Line(line.point0, line.point1, line.colour)
		curr_line.rotate(2 * pi/num * i)

		curr_line.colour = colours[(random_start + random_space * depth) % len(colours)]

		print "line", curr_line

	trunk = next(line, angle, scale, True)
	draw(trunk, num, angle, scale, depth-1)

	branch = next(line, angle, scale, False)
	draw(branch, num, angle, scale, depth-1)

# ********** process command line arguments
if len(sys.argv) != 5:
	print >> sys.stderr, 'Syntax: ' + sys.argv[0] + ' num angle scale depth'
	sys.exit(2)

try:
	num = int(sys.argv[1])
	angle = float(sys.argv[2])
	scale = float(sys.argv[3])
	depth = int(sys.argv[4])
except ValueError:
	print >> sys.stderr, sys.argv[0] + ' inputs must numbers'
	sys.exit(2)

if num < 1:
	print >> sys.stderr, sys.argv[0] + ' number of branches must be at least 1'
	sys.exit(2)
if scale <= 0:
	print >> sys.stderr, sys.argv[0] + ' scale must be greater than 0'
	sys.exit(2)
if depth < 1:
	print >> sys.stderr, sys.argv[0] + ' depth must be at least 1'
	sys.exit(2)

# create initial line
point0 = Line_Point_colour.Point(0.0, 0.0)
point1 = Line_Point_colour.Point(0.0, 50.0)

start = Line_Point_colour.Line(point0, point1)

# create list of usable colours
colour_file = open('css_colours_modified.txt', 'r')
colours = [colour[:-1] for colour in colour_file]
colour_file.close()

# random colour position start and spacing for consecutive colours
random_start = randint(0,len(colours))
random_space = randint(0,len(colours))

draw(start, num, angle, scale, depth)
