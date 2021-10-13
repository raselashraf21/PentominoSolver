# Solver API

Solver API for Pentomino games

# Build

- make clean
- make
- ./Solver [arguments]

# Arguments Option

-b [board]

	-b "0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000" 
	
	Each row separated by newline character '\n' 
	Empty cell denoted by '0'
	Ex. 6x10 board
		0000000000
		0000000000
		0000000000
		0000000000
		0000000000
		0000000000
	
	Blocked cell denoted by '-'
	Ex. 8x8 board
		00000000
		00000000
		00000000
		000--000
		000--000
		00000000
		00000000
		00000000
	
	FilledUP by Pentomino pieces denoted by pentomino character e.g. 'T'
	Ex. 3x4 board
		0TTT
		00T0
		00T0
	
	If no board defined, default is 8x9 board
	
-o [output directory]
	
	Default is ./data/

-v : [verbose output] 

	print solution in console

# Run

	./Solver - b "0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000"


## Imporvement:
	
	Current Implementation does not work board cell number less < 60
	

