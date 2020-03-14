import gmsh
import sys

model = gmsh.model
factory = model.occ

FILENAME_STEM = __file__.split(".")[0]
MSH_FILENAME = f"{FILENAME_STEM}.msh"

gmsh.initialize(sys.argv)

gmsh.option.setNumber("General.Terminal", 1)

model.add(FILENAME_STEM)

mesh_size = 1.0

gmsh.option.setNumber("Mesh.Algorithm", 6);
gmsh.option.setNumber("Mesh.CharacteristicLengthMin", mesh_size);
gmsh.option.setNumber("Mesh.CharacteristicLengthMax", mesh_size);

inner_rad = 2.0
start_point = 0.0, 0.0, 1.0
cyl_vector  = 0.0, 0.0, 15.0 

factory.addCylinder(*start_point, *cyl_vector, inner_rad)

factory.synchronize()
model.mesh.generate(3)
gmsh.write(MSH_FILENAME)
gmsh.finalize()