import gmsh
import sys

R = 1.0

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)

model.add(FILENAME_STEM)

gmsh.option.setNumber("Mesh.Algorithm", 6)
gmsh.option.setNumber("Mesh.CharacteristicLengthMin", 1.0)
gmsh.option.setNumber("Mesh.CharacteristicLengthMax", 1.0)

factory.addBox(0.0, 0.0, 0.0, 1.0, 10.0, 10.0, 1)

factory.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()