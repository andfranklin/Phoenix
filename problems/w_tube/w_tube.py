import sys
import gmsh
from meshing_functions import make_elbow, make_tube
from mesh_params import coarse as mesh_params
import physical_params


model = gmsh.model

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)
gmsh.option.setNumber("General.Terminal", 1)
model.add(FILENAME_STEM)

inner_radius = physical_params.r - physical_params.thickness
outer_radius = physical_params.r

# 1

origin = -physical_params.R, 0.0, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = 0.0, 0.0, 0.0
rot_normal = 0.0, 0.0, -1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_axial)

# 2

origin = physical_params.R, -physical_params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0
height = physical_params.l1

make_tube(origin, axis_dir, inner_radius, outer_radius,
          height,
          mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_layers_2_3)

# 3

origin = -physical_params.R, -physical_params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0
height = physical_params.l1

make_tube(origin, axis_dir, inner_radius, outer_radius,
          height,
          mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_layers_2_3)

# 4

origin = -3.0 * physical_params.R, -physical_params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = -2.0 * physical_params.R, -physical_params.l1, 0.0
rot_normal = 0.0, 0.0, 1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_axial)

# 5

origin = 3.0 * physical_params.R, -physical_params.l1, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = 2.0 * physical_params.R, -physical_params.l1, 0.0
rot_normal = 0.0, 0.0, -1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_axial)

# 6

origin = 3.0 * physical_params.R, physical_params.l2, 0.0
axis_dir = 0.0, -1.0, 0.0
height = physical_params.l1 + physical_params.l2
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height,
          mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_layers_6_7)

# 7

origin = -3.0 * physical_params.R, physical_params.l2, 0.0
axis_dir = 0.0, -1.0, 0.0
height = physical_params.l1 + physical_params.l2
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height,
          mesh_params.n_rings, mesh_params.n_sectors, mesh_params.n_layers_6_7)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()
