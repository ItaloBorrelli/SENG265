import sys
import Line_Point_colour

CANVAS_HEIGHT = 500.0

'''
purpose
	copy and mirror all lines giving two copies on either side of the x-axis
preconditions
	None
'''

def mirror_horizontal(lines):
	length = len(lines)

	for i in range(length):
		line = lines[i]

		line.translate(0.0, CANVAS_HEIGHT/4)

		new_point0 = Line_Point_colour.Point(line.point0.x, -line.point0.y)
		new_point1 = Line_Point_colour.Point(line.point1.x, -line.point1.y)

		new_line = Line_Point_colour.Line(new_point0, new_point1, line.colour)

		lines.append(new_line)

	return lines

'''
purpose
	copy and mirror all lines giving two copies on either side of the y-axis after being scaled
preconditions
	None
'''

def mirror_vertical(lines):
	length = len(lines)

	for i in range(length):
		line = lines[i]

		line.scale(0.5)
		line.translate(CANVAS_HEIGHT/4, 0.0)

		new_point0 = Line_Point_colour.Point(-line.point0.x, line.point0.y)
		new_point1 = Line_Point_colour.Point(-line.point1.x, line.point1.y)

		new_line = Line_Point_colour.Line(new_point0, new_point1, line.colour)

		lines.append(new_line)

	return lines

'''
purpose
	take lines from a line file and turn them into a Line class objects
preconditions
	None
'''

def line_to_line_point(line):
	L = line.split()
	point0 = Line_Point_colour.Point(float(L[1]), float(L[2]))
	point1 = Line_Point_colour.Point(float(L[3]), float(L[4]))
	line = Line_Point_colour.Line(point0, point1, L[5])

	return line

# ********** process command line arguments
if len(sys.argv) != 3:
	print >> sys.stderr, 'Syntax: ' + sys.argv[0] + ' file num'
	sys.exit(2)

try:
        file = open(sys.argv[1], 'r')
	num = int(sys.argv[2])
except ValueError:
        print >> sys.stderr, sys.argv[2] + ': num must be an integer'
	sys.exit(2)
except IOError:
        print >> sys.stderr, sys.argv[1] + ': file could not be opened.'
	sys.exit(2)

lines = [line_to_line_point(line) for line in file]
file.close()

for i in range(num):
	if i % 2 == 0:
		lines = mirror_vertical(lines)
	else:
		lines = mirror_horizontal(lines)


for line in lines:
	print 'line', line
