#!/bin/bash
#Vine application

echo 'Would you like to generate a new image or edit an existing one? '
echo

read -p  "Type [g] to generate, [e] to edit or any other key to exit: " str
echo
	if [[ "$str" =~ ^[Gg]$ ]]
	then
		#Do generate code, prompts
		read -p 'What would you like your new file to be called? ' filenameIn
		echo
		read -p 'How many branches: ' branches
		read -p 'What is the angle in rads: ' angle
		read -p 'What Scale: ' scale
		read -p 'What depth: ' depth

		python2 vine_generator.py $branches $angle $scale $depth > $filename_Out.txt
		python2 vine_generator.py $branches $angle $scale $depth > $filenameIn.txt
		#python2 lines_to_svg_colour.py $filenameIn.txt > $filenameIn.svg


	elif [[ "$str" =~ ^[Ee]$ ]]
	then
		#Propt name
			read -p 'Please type the name of the line file you would like the edit: ' filenameIn
echo

		##### CHECK IF FILE EXISTS
			while true;
			do
				if [ -f "./$filenameIn.txt" ]; then
					echo "The file exists; proceed."
					break

				else
					echo "Whoa, cowboy! That ain't a file in this dir."
					read -p 'Please make sure the file is typed correctly ' filenameIn
				fi
			done


	else
		echo 'You exited the program'
		exit 0
	fi





read -p  'Type [Y] if you would like to mirror, otherwise press any key to continue: ' str
	if [[ "$str" =~ ^[Yy]$ ]]
	then

		#Do mirror stuff
		read -p 'Please type an integer greater than 0, the amount of time you want the image to mirrored: ' int


		echo 'You choose to mirror the image [ '$filenameIn' ] [' $int '] times'
		python2 mirror.py $filenameIn.txt $int > $filename_Out.txt

	fi

read -p  'Type [Y] if you would like to change colour, otherwise press any key to continue: ' str
echo
	if [[ "$str" =~ ^[Yy]$ ]]
		then
			read -p 'Please type the colour you would like to change: ' C1
			read -p 'Please type the colour you would like to change it to: ' C2

			python2 rotate_scale_translate_colour.py -c1 $C1 -c2 $C2 $filenameIn.txt > $filename_Out.txt


			#Do colour stuff

	fi

read -p 'What do you want your outputted file to be called?: ' fileout
python2 lines_to_svg_colour.py $filename_Out.txt > $fileout.svg

echo "output to $fileout"
