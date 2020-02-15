import gmsh
import physical_params

model = gmsh.model


def make_plate(case, mesh_params, mesh_name):
    filename_stem = __file__.split(".")[0]
    case_stem = f"{filename_stem}_{mesh_name}"

    gmsh.initialize()
    model.add(case_stem)

    x_center = 0.0
    y_center = -0.5 * physical_params.l1

    bl_x = x_center - (0.5 * case["a"])
    bl_y = y_center - (0.5 * case["b"])

    point = model.geo.addPoint(bl_x, bl_y, -case["s"])
    line = model.geo.extrude([(0, point)], case["a"], 0.0, 0.0,
                             [mesh_params.n_x], recombine=True)
    surf = model.geo.extrude([line[1]], 0.0, case["b"], 0.0,
                             numElements=[mesh_params.n_y], recombine=True)
    model.geo.extrude(surf, 0.0, 0.0, -4.0 * physical_params.thickness,
                      numElements=[mesh_params.n_z], recombine=True)

    model.geo.synchronize()
    model.mesh.generate(3)

    gmsh.write(f"{case_stem}.msh")
    gmsh.finalize()


if __name__ == "__main__":
    from cases import cases
    from mesh_params import coarse, refined

    make_plate(cases[31], coarse, "coarse")
    make_plate(cases[31], refined, "refined")
