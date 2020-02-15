R = 165.0    # mm
r = 109.0    # mm
l1 = 1683.0  # mm
l2 = 469.0   # mm
thickness = 25.0  # mm


if __name__ == "__main__":
    outside = (3*R) + r
    inside = outside - thickness
    right_mid = outside - (0.5 * thickness)

    print(f"mid left  : {-right_mid} {l2} 0.0")
    print(f"mid right : {right_mid} {l2} 0.0")
    print()
    print(f"   inside : {inside} {l2} 0.0")
    print(f"  outside : {outside} {l2} 0.0")
