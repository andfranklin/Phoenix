from math import atan, log, sqrt, pi


def _c_11_middle_terms(X, Y):
	tmp = sqrt(1.0 + (Y *Y))
	return X * tmp * atan(X / tmp)


def c_11_analytic_function(a, b, c):
	"""
	C-11: Identical, parallel, directly opposed rectangles

	http://www.thermalradiation.net/sectionc/C-11.html
	"""
	X = a / c
	X2 = X * X

	Y = b / c
	Y2 = Y * Y

	t1_numer = (1.0 + X2) * (1.0 + Y2)
	t1_denom = 1.0 + X2 + Y2
	t1 = log(sqrt(t1_numer / t1_denom))

	t2 = _c_11_middle_terms(X, Y)
	t3 = _c_11_middle_terms(Y, X)

	t4 = X * atan(X)
	t5 = Y * atan(Y)

	multiplier = 2.0 / (pi * X * Y)

	return multiplier * (t1 + t2 + t3 - t4 - t5)


def _c_12_aux(thing, base):
	return thing * atan(thing / sqrt((base * base) + 4.0))


def _c_12_middle_terms(X, Y):
	multi = sqrt((Y * Y) + 4.0)
	t1 = _c_12_aux(Y, Y)
	t2 = _c_12_aux(X, Y)
	return multi * (t1 - t2)


def c_12_analytic_function(a, b, c):
	"""
	C-12: Coaxial, parallel squares of different edge length

	http://www.thermalradiation.net/sectionc/C-12.html
	"""
	if b > a:
		a, b = b, a

	A = a / c
	A2 = A * A

	B = b / a
	B2 = B * B

	if A < 0.2:
		return (A2 * B2) / pi

	X = A * (1.0 + B)
	X2 = X * X

	Y = A * (1.0 - B)
	Y2 = Y * Y

	sqrt_numer = (A2 * (1.0 + B2)) + 2.0
	denom = (Y2 + 2.0) * (X2 + 2.0)
	t1 = log((sqrt_numer * sqrt_numer) / denom)

	t2 = _c_12_middle_terms(X, Y)
	t3 = _c_12_middle_terms(Y, X)

	return (t1 + t2 + t3) / (pi * A * A)


if __name__ == "__main__":
	expected = 0.2285656684427083418231774203377426601946
	c_11_solution = c_11_analytic_function(10.0, 10.0, 9.0)
	c_12_solution = c_12_analytic_function(10.0, 10.0, 9.0)

	print(f"Expected Solution : {expected:.25}")
	print(f"    C-11 Solution : {c_11_solution:.25}")
	print(f"    C-12 Solution ; {c_12_solution:.25}")