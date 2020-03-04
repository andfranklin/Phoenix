.PHONY: all clean

all: left_plate.msh right_plate.msh

left_plate.msh: left_plate.py
	@ python3 left_plate.py

right_plate.msh: right_plate.py
	@ python3 right_plate.py

clean:
	@ rm -vf *.msh
	@ rm -vf *.e