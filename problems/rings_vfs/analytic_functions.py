from math import atan, log, sqrt, pi


def _c_52_helper(R2, R3, H):
	t1 = (R2 * R2) + (R3 * R3) + (H * H)
	t2 = 2.0 * R2 * R3
	return sqrt((t1 * t1) - (t2 * t2))



def c_52_analytic_function(r1, r2, r3, r4, a):
	"""
	C-52: Ring to parallel coaxial ring.

	http://www.thermalradiation.net/sectionc/C-52.html
	"""
	H  =  a / r1
	R2 = r2 / r1
	R3 = r3 / r1
	R4 = r4 / r1

	t1 = _c_52_helper(R2, R3, H)
	t2 = _c_52_helper(R2, R4, H)
	t3 = _c_52_helper(1.0, R4, H)
	t4 = _c_52_helper(1.0, R3, H)

	denom = 2.0 * ((R2 * R2) - 1.0)

	return (t1 - t2 + t3 - t4) / denom


if __name__ == "__main__":
	# view factor from bottom ring to top ring
	print(c_52_analytic_function(2.0, 4.0, 2.0, 6.0, 15.0))

