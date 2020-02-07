.PHONY: all clean

all: bottom_ring.msh top_ring.msh

bottom_ring.msh: bottom_ring.py
	@ python3 bottom_ring.py

top_ring.msh: top_ring.py
	@ python3 top_ring.py

clean:
	@ rm -vf *.msh
	@ rm -vf *.e