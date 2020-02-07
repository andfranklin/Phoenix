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

p1 = model.geo.addPoint(0.0,  0.0,  0.0, 1.0)
p2 = model.geo.addPoint(0.0, 10.0,  0.0, 1.0)
p3 = model.geo.addPoint(0.0, 10.0, 10.0, 1.0)
p4 = model.geo.addPoint(0.0,  0.0, 10.0, 1.0)

l1 = model.geo.addLine(p1, p2)
l2 = model.geo.addLine(p2, p3)
l3 = model.geo.addLine(p3, p4)
l4 = model.geo.addLine(p4, p1)

c1 = model.geo.addCurveLoop([l1, l2, l3, l4])
s1 = model.geo.addPlaneSurface([c1])

model.geo.extrude([(2, s1)], 1.0, 0.0, 0.0, numElements=[1], recombine=True)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()