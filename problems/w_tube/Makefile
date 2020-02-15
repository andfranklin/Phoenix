.PHONY: all clean

all: w_tube.msh

w_tube.msh: w_tube.py physical_params.py mesh_params.py meshing_functions.py
	@ python3 w_tube.py

clean:
	@ rm -vf *.msh
	@ rm -vf *.e
	@ rm -vfr __pycache__
