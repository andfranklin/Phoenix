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

p1 = model.geo.addPoint(0.0,  0.0, 0.0, 10.0 / n_elems)
p2 = model.geo.addPoint(0.0, 10.0, 0.0, 10.0 / n_elems)
l1 = model.geo.addLine(p1, p2)

s1 = model.geo.extrude([(1, l1)], 0.0, 0.0, 10.0, numElements=[n_elems],
                       recombine=True)
model.geo.extrude(s1, 1.0, 0.0, 0.0, numElements=[1], recombine=True)

model.geo.synchronize()
model.mesh.generate(3)

gmsh.write(MSH_FILENAME)
gmsh.finalize()
