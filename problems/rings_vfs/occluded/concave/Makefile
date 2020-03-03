.PHONY: all clean

all: bottom_ring.msh top_ring.msh blocking_cylinder.msh

bottom_ring.msh: bottom_ring.py
	@ python3 bottom_ring.py

top_ring.msh: top_ring.py
	@ python3 top_ring.py

blocking_cylinder.msh: blocking_cylinder.py
	@ python3 blocking_cylinder.py

clean:
	@ rm -vf *.msh
	@ rm -vf *.e