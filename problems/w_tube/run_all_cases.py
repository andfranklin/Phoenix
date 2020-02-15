from cases import cases
from plate import make_plate
from mesh_params import coarse, refined
from script_utils import get_cl_parser, call_phoenix, vf_format, calc_rel_diff
from tabulate import tabulate

description = "Script for running all cases of the w-tube problem."
parser = get_cl_parser(description)

parser.add_argument("--quiet", action="store_true",
                    help="suppress info printing")

args = parser.parse_args()
verbose = not args.quiet

for i, case in enumerate(cases[:10], 1):
    if verbose:
        if i != 1:
            print()
            print()

        header = f"Case {i}"
        print(header)
        print("=" * len(header))
        print()

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

    if verbose:
        table = tabulate(table,
                         headers=["View Factor", "Reference",
                                  "Calculated", "Rel. Diff."],
                         tablefmt="psql")
        print(table)
        print()
        print(f"{plate_to_tube}:",
              f"{vf_matrix['plate_front']['w_tube_outside']:.20f}")
        print(f"{tube_to_plate}:",
              f"{vf_matrix['w_tube_outside']['plate_front']:.20f}")
