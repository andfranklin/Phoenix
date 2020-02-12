import sys
import gmsh
from meshing_functions import make_elbow, make_tube
from parameters import R, r, l1, l2, thickness
from parameters import n_rings, n_sectors, n_axial, n_layers_2_3, n_layers_6_7

model = gmsh.model

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)
gmsh.option.setNumber("General.Terminal", 1)
model.add(FILENAME_STEM)

inner_radius = r - thickness
outer_radius = r

# 1

origin = -R, 0.0, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = 0.0, 0.0, 0.0
rot_normal = 0.0, 0.0, -1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           n_rings, n_sectors, n_axial)

# 2

origin = R, -l1, 0.0
axis_dir = 0.0, 1.0, 0.0
height = l1
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, n_rings, n_sectors, n_layers_2_3)

# 3

origin = -R, -l1, 0.0
axis_dir = 0.0, 1.0, 0.0
height = l1
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, n_rings, n_sectors, n_layers_2_3)

# 4

origin = -3.0 * R, -l1, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = -2.0 * R, -l1, 0.0
rot_normal = 0.0, 0.0, 1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           n_rings, n_sectors, n_axial)

# 5

origin = 3.0 * R, -l1, 0.0
axis_dir = 0.0, 1.0, 0.0

center_of_rot = 2.0 * R, -l1, 0.0
rot_normal = 0.0, 0.0, -1.0

make_elbow(origin, axis_dir, inner_radius, outer_radius,
           center_of_rot, rot_normal,
           n_rings, n_sectors, n_axial)

# 6

origin = 3.0 * R, l2, 0.0
axis_dir = 0.0, -1.0, 0.0
height = l1 + l2
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, n_rings, n_sectors, n_layers_6_7)

# 7

origin = -3.0 * R, l2, 0.0
axis_dir = 0.0, -1.0, 0.0
height = l1 + l2
make_tube(origin, axis_dir, inner_radius, outer_radius,
          height, n_rings, n_sectors, n_layers_6_7)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()

outside = (3*R) + r
inside = outside - thickness
right_mid = outside - (0.5 * thickness)
print()
print(f"mid left  : {-right_mid} {l2} 0.0")
print(f"mid right : {right_mid} {l2} 0.0")
print()
print(f"   inside : {inside} {l2} 0.0")
print(f"  outside : {outside} {l2} 0.0")
