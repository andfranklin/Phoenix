import gmsh
import sys
from meshing_functions import make_tube

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)

model.add(FILENAME_STEM)

origin = 0.0, 0.0, 1.0
axis_dir = 0.0, 0.0, 1.0
inner_radius = 1.0
outer_radius = 2.0
height = 15.0
n_rings = 1
n_sectors = 8
n_layers = 30

make_tube(origin, axis_dir, inner_radius, outer_radius, height, n_rings, n_sectors, n_layers)

model.geo.synchronize()
model.mesh.generate(3)
gmsh.write(MSH_FILENAME)
gmsh.finalize()