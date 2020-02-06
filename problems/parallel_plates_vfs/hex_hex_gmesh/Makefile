.PHONY: all clean

all: hex_hex_0_FOURTH_GAUSS.e

hex_hex_0_FOURTH_GAUSS.e: left_plate.msh right_plate.msh
	./../../../phoenix-opt -i view_factors.i

left_plate.msh: left_plate.py
	@ python3 left_plate.py

right_plate.msh: right_plate.py
	@ python3 right_plate.py

clean:
	@ rm -v *.msh
	@ rm -v *.e