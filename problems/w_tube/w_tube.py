import sys
import gmsh
from meshing_functions import make_elbow, make_tube
import parameters as params

model = gmsh.model

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)
gmsh.option.setNumber("General.Terminal", 1)
model.add(FILENAME_STEM)

inner_radius = params.r - params.thickness
outer_radius = params.r

# 1

origin = -params.R, 0.0, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = 0.0, 0.0, 0.0
rot_normal = 0.0, 0.0, -1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           params.n_rings, params.n_sectors, params.n_axial)

# 2

origin = params.R, -params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0
height = params.l1

make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, params.n_rings, params.n_sectors, params.n_layers_2_3)

# 3

origin = -params.R, -params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0
height = params.l1

make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, params.n_rings, params.n_sectors, params.n_layers_2_3)

# 4

origin = -3.0 * params.R, -params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = -2.0 * params.R, -params.l1, 0.0
rot_normal = 0.0, 0.0, 1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           params.n_rings, params.n_sectors, params.n_axial)

# 5

origin = 3.0 * params.R, -params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = 2.0 * params.R, -params.l1, 0.0
rot_normal = 0.0, 0.0, -1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           params.n_rings, params.n_sectors, params.n_axial)

# 6

origin = 3.0 * params.R, params.l2, 0.0
axis_dir = 0.0, -1.0, 0.0
height = params.l1 + params.l2
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, params.n_rings, params.n_sectors, params.n_layers_6_7)

# 7

origin = -3.0 * params.R, params.l2, 0.0
axis_dir = 0.0, -1.0, 0.0
height = params.l1 + params.l2
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, params.n_rings, params.n_sectors, params.n_layers_6_7)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()
