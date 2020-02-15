R  = 165.0  # mm
r  = 109.0  # mm
l1 = 1683.0 # mm
l2 = 469.0  # mm
thickness = 25.0 # mm

# w-tube meshing parameters
n_rings = 1
n_sectors = 8     # 2
n_axial = 24      # 14
n_layers_2_3 = 36 # 9
n_layers_6_7 = 48 # 9

# plate meshing parameters
n_x = 40 # 15
n_y = 40 # 10
n_z = 4

if __name__ == "__main__":
    outside = (3*R) + r
    inside = outside - thickness
    right_mid = outside - (0.5 * thickness)

    print(f"mid left  : {-right_mid} {l2} 0.0")
    print(f"mid right : {right_mid} {l2} 0.0")
    print()
    print(f"   inside : {inside} {l2} 0.0")
    print(f"  outside : {outside} {l2} 0.0")
