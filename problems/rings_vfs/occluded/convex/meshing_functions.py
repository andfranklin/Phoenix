import gmsh
import numpy as np

model = gmsh.model

def magnitude(vect):
    vect = np.array(vect)
    return np.sqrt(vect @ vect)

def normalize(vect):
    vect = np.array(vect)
    mag = magnitude(vect)
    return tuple(vect / mag)

def orthogonal(a, b):
    return np.isclose(np.array(a) @ np.array(b), 0.0)

def find_ortho_normal_vect(vect):
    vect = normalize(vect)
    orth = (1.0, 0.0, 0.0)
    while not orthogonal(vect, orth):
        orth = np.random.rand(3)
        orth = normalize(orth - ((orth @ vect) * vect))
    return tuple(orth)

def scale(scaler, vect):
    vect = np.array(vect)
    return tuple(scaler * vect)

def add(a, b):
    return tuple(np.array(a) + np.array(b))

half_cir = np.pi
quarter_cir = 0.5 * half_cir

def make_tube(origin, axis_dir, inner_radius, outer_radius, height, n_rings, n_sectors, n_layers):
    assert outer_radius > inner_radius
    assert inner_radius > 0.0
    assert height > 0.0
    assert n_rings > 0
    assert n_sectors > 0
    assert n_layers > 0
    assert magnitude(axis_dir) > 0.0

    axis_dir = normalize(axis_dir)
    radial_dir = find_ortho_normal_vect(axis_dir)

    delta = scale(inner_radius, radial_dir)
    point = add(origin, delta)
    point = model.geo.addPoint(*point)

    thickness = outer_radius - inner_radius
    radial_axis = scale(thickness, radial_dir)
    line = model.geo.extrude([(0, point)], *radial_axis, [n_rings], recombine=True)

    circ1 = model.geo.revolve([line[1]],  *origin, *axis_dir, quarter_cir, [n_sectors], recombine=True)
    circ2 = model.geo.revolve([circ1[0]], *origin, *axis_dir, quarter_cir, [n_sectors], recombine=True)
    circ3 = model.geo.revolve([circ2[0]], *origin, *axis_dir, quarter_cir, [n_sectors], recombine=True)
    circ4 = model.geo.revolve([circ3[0]], *origin, *axis_dir, quarter_cir, [n_sectors], recombine=True)

    tube_axis = scale(height, axis_dir)
    tube1 = model.geo.extrude(circ1, *tube_axis, [n_layers], recombine=True)
    tube2 = model.geo.extrude(circ2, *tube_axis, [n_layers], recombine=True)
    tube3 = model.geo.extrude(circ3, *tube_axis, [n_layers], recombine=True)
    tube4 = model.geo.extrude(circ4, *tube_axis, [n_layers], recombine=True)