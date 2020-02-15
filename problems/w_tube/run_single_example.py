from cases import cases
from plate import make_plate
from mesh_params import coarse, refined
from script_utils import get_cl_parser, call_phoenix, vf_format, calc_rel_diff
from tabulate import tabulate

description = "Script for running specific cases of the w-tube problem."
parser = get_cl_parser(description)
parser.add_argument("--case", type=int, default=32,
                    help="name of the case to run (default: 32)")

args = parser.parse_args()
case = args.case

n_cases = len(cases)
err_str = f"unrecognized case {case}. Choose between 1 and {n_cases}."
assert case >= 1 and case <= n_cases, err_str

case = cases[case - 1]

if args.mesh == "COARSE":
    make_plate(case, coarse, "coarse")
else:
    make_plate(case, refined, "refined")

vf_matrix = call_phoenix(args)

table = []
plate_to_tube = "plate -> w-tube"
ref = case["F0W"]
calc = vf_matrix["plate_front"]["w_tube_outside"]
rel_diff = calc_rel_diff(ref, calc)
row = [plate_to_tube, vf_format(ref), vf_format(calc), rel_diff]
table.append(row)

tube_to_plate = "w-tube -> plate"
ref = case["FW0"]
calc = vf_matrix["w_tube_outside"]["plate_front"]
rel_diff = calc_rel_diff(ref, calc)
row = [tube_to_plate, vf_format(ref), vf_format(calc), rel_diff]
table.append(row)

print(tabulate(table,
               headers=["View Factor", "Reference",
                        "Calculated", "Rel. Diff."]))
print()

# vf_F0W
print(f"{plate_to_tube}: {vf_matrix['plate_front']['w_tube_outside']:.20f}")
print(f"{tube_to_plate}: {vf_matrix['w_tube_outside']['plate_front']:.20f}")
