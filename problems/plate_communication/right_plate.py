import gmsh
import sys

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)
gmsh.option.setNumber("Mesh.Algorithm", 6)
gmsh.option.setNumber("Mesh.CharacteristicLengthMin", 1.0)
gmsh.option.setNumber("Mesh.CharacteristicLengthMax", 1.0)

model.add(FILENAME_STEM)

p1 = model.geo.addPoint(10.0,  0.0, 0.0, 1.0)
p2 = model.geo.addPoint(10.0, 10.0, 0.0, 1.0)
l1 = model.geo.addLine(p1, p2)

s1 = model.geo.extrude([(1, l1)], 0.0, 0.0, 10.0, numElements=[10], recombine=True)
model.geo.extrude(s1, 1.0, 0.0, 0.0, numElements=[1], recombine=True)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()