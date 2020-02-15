from cases import cases
from plate import make_plate
from extract_view_factors import call_phoenix, vf_format, calc_rel_diff
from tabulate import tabulate
from argparse import ArgumentParser

description = "Script for running specific cases of the w-tube problem."

parser = ArgumentParser(description=description)
parser.add_argument("--case", type=int, default=32,
                    help="name of the case to run (default: 32)")

args = parser.parse_args()
case = args.case

n_cases = len(cases)
err_str = f"unrecognized case {case}. Choose between 1 and {n_cases}."
assert case >= 1 and case <= n_cases, err_str

case = cases[case - 1]
plate_filename = "plate.msh"

make_plate(case, plate_filename)
vf_matrix = call_phoenix(plate_filename=plate_filename)

print(vf_matrix.matrix)
print()

table = []
vf_label = "F0W"
ref = case[vf_label]
calc = vf_matrix["plate_front"]["w_tube_outside"]
rel_diff = calc_rel_diff(ref, calc)
row = [vf_label, vf_format(ref), vf_format(calc), rel_diff]
table.append(row)

vf_label = "FW0"
ref = case[vf_label]
calc = vf_matrix["w_tube_outside"]["plate_front"]
rel_diff = calc_rel_diff(ref, calc)
row = [vf_label, vf_format(ref), vf_format(calc), rel_diff]
table.append(row)

print(tabulate(table,
               headers=["View Factor", "Reference",
                        "Calculated", "Rel. Diff."]))
