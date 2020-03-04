import gmsh
import sys

coarse_n_elems = 2
refined_n_elems = 10

n_elems = refined_n_elems

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)

model.add(FILENAME_STEM)

gmsh.option.setNumber("Mesh.Algorithm", 6)
gmsh.option.setNumber("Mesh.CharacteristicLengthMin", 10.0 / n_elems)
gmsh.option.setNumber("Mesh.CharacteristicLengthMax", 10.0 / n_elems)

factory.addBox(10.0, 0.0, 0.0, 1.0, 10.0, 10.0, 1)

factory.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()
