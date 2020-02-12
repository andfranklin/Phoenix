import gmsh
import sys
from dimensions import l1, a, b, s, thickness

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)

model.add(FILENAME_STEM)

x_center = 0.0
y_center = -0.5 * l1

bl_x = x_center - (0.5 * a)
bl_y = y_center - (0.5 * b)

point = model.geo.addPoint(bl_x, bl_y, -s)
line = model.geo.extrude([(0, point)], a, 0.0, 0.0, [15], recombine=True)
surf = model.geo.extrude([line[1]], 0.0, b, 0.0, numElements=[10], recombine=True)
model.geo.extrude(surf, 0.0, 0.0, -4.0 * thickness, numElements=[1], recombine=True)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()
