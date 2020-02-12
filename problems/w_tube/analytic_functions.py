from math import acos, atan, sqrt, pi


def c_78_analytic_function(r, h):
	"""
	C-78: Inside surface of right circular cylinder to itself.

	http://www.thermalradiation.net/sectionc/C-78.html
	"""
	H  =  h / (2.0 * r)
	t1 = 1.0 + H
	t2 = sqrt(1.0 + (H * H))
	return t1 - t2


if __name__ == "__main__":
	r  = 8.0
	h  = 10.0
	vf = c_78_analytic_function(r, h)
	print(f"Solution : {vf:.20}")
