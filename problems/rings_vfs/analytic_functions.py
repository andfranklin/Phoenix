from math import acos, atan, sqrt, pi


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


def _c_53_atan_helper(C, D, numer_multi, denom_multi):
	C2 = C * C
	C2P1 = 1.0 + C2
	
	D2 = D * D
	D2P1 = 1.0 + D2

	multi = sqrt(C2P1 * D2P1)
	meat = atan(sqrt((C2P1 * numer_multi) / (D2P1 * denom_multi)))

	return multi * meat


def _c_53_t4(C, D, Y):
	C2 = C * C
	D2 = D * D
	Y2 = Y * Y
	return _c_53_atan_helper(C, D, Y2 - D2, C2 - Y2)


def _c_53_t5_t6(R1, Rc):
	C = R1 + Rc
	D = R1 - Rc
	return _c_53_atan_helper(C, D, D, C)


def c_53_analytic_function(r1, r2, rc, h):
	"""
	C-53: Annulus to coaxial annulus of different outer radius;
	both annuli have inner radius of blocking coaxial cylinder.

	http://www.thermalradiation.net/sectionc/C-53.html
	"""
	R1 = r1 / h
	R2 = r2 / h
	Rc = rc / h

	A = (R1 * R1) - (Rc * Rc)
	B = (R2 * R2) - (Rc * Rc)

	C = R2 + R1
	D = R2 - R1

	Y = sqrt(A) + sqrt(B)

	t1 = 0.5 * A * acos(Rc / R2)
	t2 = 0.5 * B * acos(Rc / R1)
	t3 = 2.0 * Rc * (atan(Y) - atan(sqrt(A)) - atan(sqrt(B)))
	t4 = _c_53_t4(C, D, Y)
	t5 = _c_53_t5_t6(R1, Rc)
	t6 = _c_53_t5_t6(R2, Rc)

	return (t1 + t2 + t3 - t4 + t5 + t6) / (pi * A)


if __name__ == "__main__":
	br_outer_rad = 4.0
	tr_outer_rad = 6.0
	inner_rad = 2.0
	sep_dist = 15.0

	# view factor from bottom ring to top ring
	unoccluded = c_52_analytic_function(inner_rad, br_outer_rad, inner_rad, tr_outer_rad, sep_dist)
	occluded = c_53_analytic_function(br_outer_rad, tr_outer_rad, inner_rad, sep_dist)
	
	print(f"Unoccluded Solution : {unoccluded:.20}")
	print(f"  Occluded Solution : {occluded:.20}")

