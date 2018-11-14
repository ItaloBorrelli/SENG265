import sys
import re

# ***** SVG definitions

CANVAS_HEIGHT = 500
CANVAS_WIDTH = 500

'''
purpose
	return a string containing the SVG header
preconditions
	width and height are non-negative integers
'''
def generate_svg_header(width, height):
	SVG_HEADER = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" ' + \
	'width="$width" height="$height">'

	s = SVG_HEADER.replace('$width', str(CANVAS_WIDTH))
	s = s.replace('$height', str(CANVAS_HEIGHT))

	return s

'''
purpose
	return a string containing the SVG for a bounding box of size width by height
preconditions
	width and height are non-negative integers
'''
def generate_svg_bounding_box(width, height):
	SVG_BOUNDING_BOX = '<rect x="0" y="0" width="$width" height="$height" ' + \
	'style="stroke:#000;fill:none" />'

	s = SVG_BOUNDING_BOX.replace('$width', str(CANVAS_WIDTH))
	s = s.replace('$height', str(CANVAS_HEIGHT))

	return s

'''
purpose
	return a string containing the SVG for a line from (x0,y0) to (x1,y1)
preconditions
	 x0, y0, x0, y0 are integers
'''
def generate_svg_line(x0, y0, x1, y1):
	# SVG line with placeholders for x0, y0, x0, y0
	SVG_LINE = '<line x1="$x0" y1="$y0" x2="$x1" y2="$y1" style="stroke:#000" />'
	s = SVG_LINE.replace('$x0', str(x0))
	s = s.replace('$y0', str(y0))
	s = s.replace('$x1', str(x1))
	s = s.replace('$y1', str(y1))

	return s

'''
purpose
	return a string containing the SVG footer
preconditions
	none
'''
def generate_svg_footer():
	return '</svg>'

'''
purpose
	parse line
	if legal
		return [x0, y0, x1, y1] as ints
	else
		return the index in line of the leftmost error
preconditions
	line is a string
	line does not contain tab or newline: too hard to deal with ^ position
'''
def parse_line(line):
	# re note: [ m.start(),m.end() ) for the region just matched

	# *** match preceding whitespace to find initial position
	# note: matches any string
	x = re.compile('\s*')
	m = x.match(line, 0)
	offset = m.end()

	# *** parse line keyword
	x = re.compile('line\s+')
	m = x.match(line, offset)
	if m:
		offset = m.end()
	else:
		return offset

	# *** parse x0 y0 x1 y1
	L = [ ]
	x = re.compile('([+-]?\d+) *')
	while True:
		m = x.match(line, offset)
		if m:
			coordinate = int(m.group(1))

			# on-canvas check
			if coordinate < -CANVAS_HEIGHT/2 or coordinate > CANVAS_HEIGHT/2:
				return offset

			offset = m.end()

			# add coordinate to list
			L.append(coordinate)

			# 4 coordinates already seen?
			if len(L) == 4:
				if offset == len(line):
					return L
				else:
					return offset
		else:
			return offset

# ***** process command line arguments
if len(sys.argv) != 2:
	print >> sys.stderr, 'Usage:', sys.argv[0], 'lines_file'
	sys.exit(1)
try:
	lines_file = open(sys.argv[1], 'r')
except IOError:
	print >> sys.stderr, 'Cannot open file:', sys.argv[1]
	sys.exit(2)

# ***** generate and print header and bounding box
print generate_svg_header(CANVAS_WIDTH, CANVAS_HEIGHT)
print generate_svg_bounding_box(CANVAS_WIDTH, CANVAS_HEIGHT)

# ***** for each line in stdin
#	parse, transpose, generate and print SVG line
line_number = 0
for line in lines_file:
	line_number += 1
	line = line[:-1] # strip newline
	L = parse_line(line)
	if type(L) == list:
		x0, y0, x1, y1 = L[0], L[1], L[2], L[3]
	else:
		print >> sys.stderr, 'Error in line ' + str(line_number) + ':'
		print >> sys.stderr, '   ' + line
		print >> sys.stderr, '   ' + ' '*L + '^'
		continue

	# convert from standard to SVG coordinates
	x0 += CANVAS_WIDTH/2
	y0 = -y0 + CANVAS_HEIGHT/2
	x1 += CANVAS_WIDTH/2
	y1 = -y1 + CANVAS_HEIGHT/2

	print generate_svg_line(x0, y0, x1, y1)

print generate_svg_footer()
