import meshio
import gmsh
import sys

R = 1.0

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"
EXD_FILENAME = f"{FILENAME_STEM}.e"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)

model.add(FILENAME_STEM)

gmsh.option.setNumber("Mesh.Algorithm", 6)
gmsh.option.setNumber("Mesh.CharacteristicLengthMin", 0.4)
gmsh.option.setNumber("Mesh.CharacteristicLengthMax", 0.4)

factory.addBox(-R, 0.0, 0.0, R, R, R, 1)

factory.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()

mesh = meshio.read(MSH_FILENAME)
meshio.write(EXD_FILENAME, mesh)
