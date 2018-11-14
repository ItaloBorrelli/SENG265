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
def generate_svg_line(x0, y0, x1, y1, colour):
	# SVG line with placeholders for x0, y0, x0, y0
	SVG_LINE = '<line x1="$x0" y1="$y0" x2="$x1" y2="$y1" style="stroke:$colour" />'
	s = SVG_LINE.replace('$x0', str(x0))
	s = s.replace('$y0', str(y0))
	s = s.replace('$x1', str(x1))
	s = s.replace('$y1', str(y1))
	s = s.replace('$colour', str(colour))

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
	# *** parse 'line' keyword
	x = re.compile(' *line +')
	m = x.match(line)
	if m:
		offset = m.end()
	else: # error: illegal keyword
		return None

	# ***** parse x0 y0 x1 y1 colour
	L = [ ]
	x = re.compile('([+-]?\d+)(\s)*(?: +|$)([A-Za-z]*)( )*')

	# create a list of legal colours from file
	colour_file = open('css_colours.txt', 'r')
	legal_colours = [colour[:-1] for colour in colour_file]
	colour_file.close()

	while True:
		# *** match
		m = x.match(line, offset)
		if m:
			x_y = int(m.group(1))
			if x_y < -CANVAS_HEIGHT/2 or x_y > CANVAS_HEIGHT/2: # error: out of range
				return offset
			offset = m.end()
			L.append(x_y)

			if offset == len(line): # no more tokens remaining
				if len(L) < 4: # error: too few x/y values
					return offset
				else:
					if m.group(3) in legal_colours: # use inputed colour if legal
						L.append(m.group(3))
						return L
					else: # use default colour
						L.append('Black')
						return L
			else:
				if len(L) == 4:
					break
		# *** no match
		else: # error: illegal token value
			return offset

	# ***** check for non-space after last correct token
	x = re.compile(' *$')
	m = x.match(line, offset)
	if m:
		return L
	return offset # error: non-space after last x/y token

# purpose
#	write to stdout the lines in lines_file converted to SVG
# preconditions
#	lines_file is a reference to a text file opened for reading
def process_lines_file(lines_file):
	# ***** for each line in lines_file
	#	parse, transpose, generate and print SVG line
	line_number = 0
	for line in lines_file:
		line_number += 1
		if line[-2:] == '\r\n': # Windows
			line = line[:-2] # strip carriage return and newline
		elif line[-1] == '\n': # Linux
			line = line[:-1] # strip newline
		L = parse_line(line)
		if type(L) == list:
			x0, y0, x1, y1, colour = L[0], L[1], L[2], L[3], L[4]
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
		#print L
		print generate_svg_line(x0, y0, x1, y1, colour)

# ***** generate and print header and bounding box
print generate_svg_header(CANVAS_WIDTH, CANVAS_HEIGHT)
print generate_svg_bounding_box(CANVAS_WIDTH, CANVAS_HEIGHT)

# ***** process command line arguments
if len(sys.argv) == 1:
	process_lines_file(sys.stdin)
else:
	for lines_file_name in sys.argv[1:]:
		try:
			lines_file = open(lines_file_name, 'r')
		except IOError:
			print >> sys.stderr, 'Cannot open file:', sys.argv[1]
			sys.exit(2)
		process_lines_file(lines_file)
		lines_file.close()

# ***** generate and print footer
print generate_svg_footer()
