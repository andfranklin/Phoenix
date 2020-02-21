from utils import make_command_line_parser, make_subdir
from analytic_functions import c_12_analytic_function as analytic_function
from extract_view_factors import call_phoenix
import matplotlib.pyplot as plt

parser = make_command_line_parser("run mesh refinement on a case")
args = parser.parse_args()
case = args.case[0]
make_subdir(case)

analytic_solution = analytic_function(10.0, 10.0, 9.0)

quadrature_types = ["GAUSS", "GRID", "MONOMIAL",
                    "SIMPSON", "TRAP"]

quadrature_order = "FOURTH"

refinement_levels = list(range(6))
for quadrature_type in quadrature_types:
    print(quadrature_type)
    errors = []
    for refinement_level in refinement_levels:
        print(" ", refinement_level)
        results = call_phoenix(case, refinement_level=refinement_level,
                               quadrature_type=quadrature_type,
                               quadrature_order=quadrature_order)
        assert results["lp_right"]["lp_right"] == 0.0
        assert results["rp_left"]["rp_left"] == 0.0
        solution = results["lp_right"]["rp_left"]
        assert abs(solution - results["rp_left"]["lp_right"]) < 1e-12, (solution, results["rp_left"]["lp_right"])
        error = abs(solution - analytic_solution)
        errors.append(error)
    label = quadrature_type.replace("_", "-").capitalize()
    plt.plot(refinement_levels, errors, label=label, linestyle="-", marker=".")

plt.xlabel("Refinement Level")
plt.ylabel("Absolute Error")
plt.yscale("log")
plt.xticks(ticks=refinement_levels, labels=[str(level)[0] for level in refinement_levels])
plt.legend()
plt.savefig(f"mesh_refinement_{case}.png")
plt.show()
