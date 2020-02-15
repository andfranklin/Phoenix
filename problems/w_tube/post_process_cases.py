from script_utils import get_cased_cl_parser
from cases import cases
import json
from script_utils import vf_format, calc_rel_diff
from tabulate import tabulate

description = "script for post processing the results of all cases."
parser = get_cased_cl_parser(description)
args = parser.parse_args()

calc_data = None
in_file = f"results_{args.mesh.lower()}.json"
with open(in_file, "r") as input:
    calc_data = json.loads(input.read())

headers = ("No.", "s, mm", "a, mm", "b, mm",
           r"F_{0W}^{ref}", r"F_{0W}^{calc}", r"\overline{\Delta F_{0W}}",
           r"F_{W0}^{ref}", r"F_{W0}^{calc}", r"\overline{\Delta F_{W0}}")

table = []
for i, (ref, calc) in enumerate(zip(cases, calc_data), 1):
    row = [i, ref["s"], ref["a"], ref["b"]]
    F0W_ref = ref["F0W"]
    F0W_calc = calc["plate_front"]["w_tube_outside"]
    delta_F0W = calc_rel_diff(F0W_ref, F0W_calc)
    row += [vf_format(F0W_ref), vf_format(F0W_calc), delta_F0W]

    FW0_ref = ref["FW0"]
    FW0_calc = calc["w_tube_outside"]["plate_front"]
    delta_FW0 = calc_rel_diff(FW0_ref, FW0_calc)
    row += [vf_format(FW0_ref), vf_format(FW0_calc), delta_FW0]
    table.append(row)

table = tabulate(table, headers=headers, tablefmt="psql")
print(table)
