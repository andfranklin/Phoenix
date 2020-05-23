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
refinement_levels = list(range(5))

if case == "hex_hex":
    n_surf_elems = [4 * (4 ** i) for i in refinement_levels]
elif case == "pyr_pyr":
    n_surf_elems = [14 * (4 ** i) for i in refinement_levels]
else:
    raise Exception(f"unsupported case ({case}).")

surface_areas = [100.0 / n_elems for n_elems in n_surf_elems]

print("N. Surface Elems")
print(n_surf_elems)
print()

print("Surface Areas")
print(surface_areas)
print()

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
    plt.plot(n_surf_elems, errors, label=f"{label}", linestyle="-", marker=".")

plt.xlabel("Number of Surface Elements")
plt.ylabel("Absolute Error")
plt.yscale("log")
plt.xscale("log")
plt.legend(prop={"family":"monospace"})
plt.savefig(f"mesh_refinement_{case}.png")
plt.show()
