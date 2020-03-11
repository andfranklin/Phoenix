from utils import make_command_line_parser, make_subdir
from analytic_functions import c_12_analytic_function as analytic_function
from extract_view_factors import call_phoenix
import matplotlib.pyplot as plt

parser = make_command_line_parser("run mesh refinement on a case")
args = parser.parse_args()
case = args.case[0]
make_subdir(case)

analytic_solution = analytic_function(10.0, 10.0, 9.0)


quadrature_orders = ["FIRST", "SECOND", "THIRD", "FOURTH",
                     "FIFTH", "SIXTH", "SEVENTH", "EIGHTH",
                     "NINTH", "TENTH", "ELEVENTH", "TWELFTH",
                     "THIRTEENTH", "FOURTEENTH", "FIFTEENTH",
                     "SIXTEENTH", "SEVENTEENTH", "EIGHTTEENTH"]

quadrature_numerical_orders = list(range(1, len(quadrature_orders) + 1))

# quadrature_types = ["CLOUGH", "CONICAL", "GAUSS", "GRID", "MONOMIAL",
#                     "SIMPSON", "TRAP", "GAUSS_LOBATTO"]

quadrature_types = ["GAUSS", "GRID", "MONOMIAL", "SIMPSON", "TRAP"]

refinement_level = 0
for quadrature_type in quadrature_types:
    print(quadrature_type)
    errors = []
    for quadrature_order in quadrature_orders:
        print(" ", quadrature_order)
        results = call_phoenix(case, refinement_level=refinement_level, quadrature_type=quadrature_type, quadrature_order=quadrature_order)
        assert results["lp_right"]["lp_right"] == 0.0
        assert results["rp_left"]["rp_left"] == 0.0
        solution = results["lp_right"]["rp_left"]
        assert abs(solution - results["rp_left"]["lp_right"]) < 1e-12, (solution, results["rp_left"]["lp_right"])
        error = abs(solution - analytic_solution)
        errors.append(error)
    label = quadrature_type.replace("_", "-").capitalize()
    plt.plot(quadrature_numerical_orders, errors, label=label, linestyle="-", marker=".")

plt.xlabel("Quadrature Order")
plt.ylabel("Absolute Error")
plt.yscale("log")
plt.xticks(ticks=quadrature_numerical_orders, labels=[str(order) for order in quadrature_numerical_orders])
plt.legend()
plt.savefig(f"quadrature_refinement_{case}.png")
plt.show()
