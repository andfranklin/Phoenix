import gmsh
import sys
from meshing_functions import make_elbow, half_cir, quarter_cir

model = gmsh.model

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)
gmsh.option.setNumber("General.Terminal", 1)
model.add(FILENAME_STEM)

origin   = 0.0, 0.0, 0.0
axis_dir = 0.0, 0.0, 1.0
inner_radius = 8.0
outer_radius = 10.0
center_of_rot = 20.0, 0.0, 0.0
rot_normal    =  0.0, 1.0, 0.0
n_rings = 1
n_sectors = 1
n_axial = 1
torus_angle = quarter_cir

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           n_rings, n_sectors, n_axial,
           torus_angle=torus_angle)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()
