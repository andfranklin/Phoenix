from extract_view_factors import call_phoenix, analytic_solution

quadrature_order = "FOURTH" #["FIRST", "SECOND", "THIRD", "FOURTH", "FIFTH", "SIXTH",
                    # "SEVENTH", "EIGHTH", "NINTH", "TENTH", "ELEVENTH",
                     #"TWELFTH", "THIRTEENTH", "FOURTEENTH", "FIFTEENTH",
                     #"SIXTEENTH", "SEVENTEENTH", "EIGHTTEENTH", "NINTEENTH",
                     #"TWENTIETH"]

quadrature_type = "GAUSS"
cases = ["hh_plates", "ht_plates", "htp_plates", "tpt_plates", "tptp_plates", "tt_plates"]
refinement_level = 0

# quadrature_type = "GAUSS"
for case in cases:
    results = call_phoenix(case=case, refinement_level=refinement_level, quadrature_type=quadrature_type, quadrature_order=quadrature_order)
    assert results["lp_front"]["lp_front"] == 0.0
    assert results["rp_back"]["rp_back"] == 0.0
    assert abs(results["lp_front"]["rp_back"] - results["rp_back"]["lp_front"]) < 1e-12, (results["lp_front"]["rp_back"], results["rp_back"]["lp_front"])
    solution = results["lp_front"]["rp_back"]
    error = abs(solution - analytic_solution)

    print("case     :", case)
    print("solution :", solution)
    print("error    :", error)
    print()
