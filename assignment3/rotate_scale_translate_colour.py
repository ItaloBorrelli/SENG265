import sys
import Line_Point_colour

'''
purpose
	parse argv:
		[-a angle] [-f factor] [-n count] [-x delta_x] [-y delta_y] [-c1] [-c2] [file(s)]
	if legal
		return dictionary of option/option value pairs
		use None for option value if option not present
		use 'file_names':L
			where L is a list of the file names and [ ] if no files present
	else
		return error message
'''
def parse_argv(argv):
	D = { '-a':None, '-f':None, '-n':None, '-x':None, '-y':None, '-c1':None, '-c2':None, 'file_names':[] }

	i = 1
	while i < len(sys.argv) and sys.argv[i][0] == '-':
		# *** duplicate option, illegal option
		if sys.argv[i] in D:
			if D[sys.argv[i]] != None:
				return 'Duplicate option: ' + sys.argv[i]
		else:
			return 'Illegal option: ' + sys.argv[i]

		# *** extract option value
		option = sys.argv[i]
		i = i + 1
		if i >= len(sys.argv):
			return 'Missing option value for: ' + sys.argv[i]
		option_value = sys.argv[i]

		# *** -a option
		if option == '-a':
			try:
				D['-a'] = float(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value
		# *** -f option
		elif option == '-f':
			try:
				D['-f'] = float(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value
		# *** -x option
		elif option == '-x':
			try:
				D['-x'] = float(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value
		# *** -y option
		elif option == '-y':
			try:
				D['-y'] = float(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value
		# *** -n option: int
		elif option == '-n':
			try:
				D['-n'] = int(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value
		elif option == '-c1':
			try:
				D['-c1'] = str(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value
		elif option == '-c2':
			try:
				D['-c2'] = str(option_value)
			except ValueError:
				return 'Illegal option value: ' + option + ' ' + option_value

		# advance to next option
		i = i + 1

	# add file_names to D
	D['file_names'] = sys.argv[i:]
	if D['file_names'] == [ ]:
		D['file_names'] = None

	return D

# ***** apply rotate, scale, translate
def process_lines_file(file_object, options):
	colours = {}
	colour_file = open('css_colours.txt', 'r')
	for line in colour_file:
		colours[line.strip()] = line.strip()
	colour_file.close
	#checks if -c1 is a legal colour
	if options['-c1'] not in colours and options['-c1'] != None:
		#if -c1 is not legal checks if -c2 is also not legal
		if options['-c2'] not in colours:
			print >> sys.stderr, 'Error -c1 "', options['-c1'], '" and -c2 "', options['-c2'], '" are not a valid colours'
			sys.exit(2)
		print >> sys.stderr, 'Error -c1 "',options['-c1'], '" is not a valid colour'
		sys.exit(2)
	#checks if -c2 is a legal colour
	if options['-c2'] not in colours and options['-c1'] != None:
		print >> sys.stderr, 'Error -c2 "',options['-c2'], '" is not a valid colour'
		sys.exit(2)

	k = 0
	for line in file_object:
		# convert L to a Line object
		L = line.split()
		point0 = Line_Point_colour.Point(float(L[1]), float(L[2]))
		point1 = Line_Point_colour.Point(float(L[3]), float(L[4]))
		#If -c1 is specified checks if -c2 is specified
		if options['-c1']:
			if options['-c2']:
				j = 0
			else:
				print >> sys.stderr, 'Error must define c2 if c1 is defined'
				sys.exit(2)
		#If -c2 is specified checks if -c1 is specified
		if options['-c2']:
			if options['-c1']:
				j = 0
			else:
				print >> sys.stderr, 'Error must define c1 if c2 is defined'
				sys.exit(2)
		if L[5] == options['-c1']:
			colour = options['-c2']
			k += 1
		else:
			colour = L[5]
		line = Line_Point_colour.Line(point0, point1, colour)
		# rotate, scale, translate and write line count times
		for i in range(options['-n']):
			line.rotate(options['-a'])
			line.scale(options['-f'])
			line.translate(options['-x'], options['-y'])
			print 'line', line
	#Says if no colours were changed
	if options['-c1'] and options['-c2']:
		if k < 1:
			print >> sys.stderr, 'No colours were changed, "',options['-c1'], '" is not used in the file specified'


# *** handle command-line arguments
options = parse_argv(sys.argv)
if type(options) == str:
	print >> sys.stderr, options
	sys.exit()

# *** apply defaults where needed
default_options = { '-a':0.0, '-f':1.0, '-n':1, '-x':0.0, '-y':0.0, '-c1':None, '-c2':None}
for option in default_options:
	if options[option] == None:
		options[option] = default_options[option]

# *** process each input file

if options['file_names'] == None:
	process_lines_file(sys.stdin, options)
else:
	for file_name in options['file_names']:
		try:
			file_object = open(file_name, 'r')
		except IOError:
			print >> sys.stderr, 'Cannot open file:', file_name
			sys.exit()
process_lines_file(file_object, options)
