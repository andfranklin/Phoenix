import gmsh
import parameters as params

model = gmsh.model


def make_plate(case, ofilename):
    gmsh.initialize()
    model.add(ofilename.replace(".msh", ""))

    x_center = 0.0
    y_center = -0.5 * params.l1

    bl_x = x_center - (0.5 * case["a"])
    bl_y = y_center - (0.5 * case["b"])

    point = model.geo.addPoint(bl_x, bl_y, -case["s"])
    line = model.geo.extrude([(0, point)], case["a"], 0.0, 0.0, [params.n_x],
                             recombine=True)
    surf = model.geo.extrude([line[1]], 0.0, case["b"], 0.0,
                             numElements=[params.n_y], recombine=True)
    model.geo.extrude(surf, 0.0, 0.0, -4.0 * params.thickness,
                      numElements=[params.n_z], recombine=True)

    model.geo.synchronize()
    model.mesh.generate(3)

    gmsh.write(ofilename)
    gmsh.finalize()


if __name__ == "__main__":
    from cases import cases
    make_plate(cases[31], __file__.replace(".py", ".msh"))
