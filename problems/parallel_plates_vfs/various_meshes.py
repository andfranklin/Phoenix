from utils import make_subdir, cases
from analytic_functions import c_12_analytic_function as analytic_function
from extract_view_factors import call_phoenix

quadrature_order = "FOURTH"
quadrature_type = "GAUSS"
refinement_level = 0
analytic_solution = analytic_function(10.0, 10.0, 9.0)

for case in cases:
    make_subdir(case)
    results = call_phoenix(case, refinement_level=refinement_level, quadrature_type=quadrature_type, quadrature_order=quadrature_order)
    assert results["lp_right"]["lp_right"] == 0.0
    assert results["rp_left"]["rp_left"] == 0.0
    solution = results["lp_right"]["rp_left"]
    assert abs(solution - results["rp_left"]["lp_right"]) < 1e-12, (solution, results["rp_left"]["lp_right"])
    error = abs(solution - analytic_solution)

    print("case     :", case)
    print("solution :", solution)
    print("error    :", error)
    print()
