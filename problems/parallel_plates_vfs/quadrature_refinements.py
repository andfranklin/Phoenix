from extract_view_factors import call_phoenix, analytic_solution
import matplotlib.pyplot as plt

quadrature_orders = ["FIRST", "SECOND", "THIRD", "FOURTH", "FIFTH", "SIXTH",
                     "SEVENTH", "EIGHTH", "NINTH", "TENTH", "ELEVENTH",
                     "TWELFTH", "THIRTEENTH", "FOURTEENTH", "FIFTEENTH",
                     "SIXTEENTH", "SEVENTEENTH", "EIGHTTEENTH", "NINTEENTH",
                     "TWENTIETH"]

quadrature_numerical_orders = list(range(1, len(quadrature_orders) + 1))

# quadrature_types = ["CLOUGH", "CONICAL", "GAUSS", "GRID", "MONOMIAL",
#                     "SIMPSON", "TRAP", "GAUSS_LOBATTO"]

quadrature_types = ["GAUSS", "GRID", "MONOMIAL",
                    "SIMPSON", "TRAP"]

refinement_level = 0
# quadrature_type = "GAUSS"
for quadrature_type in quadrature_types:
    errors = []
    for quadrature_order in quadrature_orders:
        results = call_phoenix(refinement_level=refinement_level, quadrature_type=quadrature_type, quadrature_order=quadrature_order)
        assert results["lp_front"]["lp_front"] == 0.0
        assert results["rp_back"]["rp_back"] == 0.0
        assert abs(results["lp_front"]["rp_back"] - results["rp_back"]["lp_front"]) < 1e-15
        solution = results["lp_front"]["rp_back"]
        error = abs(solution - analytic_solution)
        errors.append(error)
    label = quadrature_type.replace("_", "-").capitalize()
    plt.semilogy(quadrature_numerical_orders, errors, label=label)

plt.xlabel("Quadrature Order")
plt.ylabel("Error")
plt.legend()
plt.savefig('quadrature_refinement.pdf')
plt.show()
