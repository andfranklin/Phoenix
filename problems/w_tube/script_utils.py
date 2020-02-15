import subprocess
from argparse import ArgumentParser

quad_types = ["CLOUGH", "CONICAL", "GAUSS", "GRID", "MONOMIAL",
              "SIMPSON", "TRAP", "GAUSS_LOBATTO", "DEFAULT"]

quad_orders = ["CONSTANT", "FIRST", "SECOND", "THIRD", "FOURTH", "FIFTH",
               "SIXTH", "SEVENTH", "EIGHTH", "NINTH", "TENTH", "ELEVENTH",
               "TWELFTH", "THIRTEENTH", "FOURTEENTH", "FIFTEENTH", "SIXTEENTH",
               "SEVENTEENTH", "EIGHTTEENTH", "NINTEENTH", "TWENTIETH",
               "DEFAULT"]

meshes = ["COARSE", "REFINED"]


def get_cased_cl_parser(description):
    parser = ArgumentParser(description=description)
    parser.add_argument('--mesh', choices=meshes, default="COARSE",
                        help='the mesh to use (default: COARSE)')
    return parser


def get_cl_parser(description):
    parser = get_cased_cl_parser(description)
    parser.add_argument("--refinement-level", type=int, default=0,
                        help="the mesh refinement level (default: 0)")
    parser.add_argument('--quad-type', choices=quad_types, default="GAUSS",
                        help='the quadrature type (default: GAUSS)')
    parser.add_argument('--quad-order', choices=quad_orders, default="FIRST",
                        help='the quadrature order (default: FIRST)')
    return parser


def is_viewfactor_line(line):
    line_str = str(line)
    return " -> " in line_str or "  view factor : " in line_str


class VFInfo:
    def __init__(self, from_, to_, value_):
        self.from_ = str(from_).strip()
        self.to_ = str(to_).strip()
        self.value_ = float(value_)

    def __str__(self):
        return f"{self.from_} -> {self.to_} : {self.value_}"


class VFMatrix:
    def __init__(self, vf_infos):
        matrix = {}
        for vf_info in vf_infos:
            if vf_info.from_ not in matrix:
                matrix[vf_info.from_] = {}

            from_row = matrix[vf_info.from_]
            assert vf_info.to_ not in from_row
            from_row[vf_info.to_] = vf_info.value_
        self.matrix = matrix

    def __getitem__(self, key):
        return self.matrix[key]


def call_phoenix(args, quadrature_order="FOURTH"):
    assert args.refinement_level >= 0
    call_list = ["../../phoenix-opt", "-i", f"view_factors.i"]
    call_list.append(f"refinement_level={args.refinement_level}")
    call_list.append(f"quadrature_type={args.quad_type}")
    call_list.append(f"quadrature_order={args.quad_order}")
    call_list.append(f"mesh={args.mesh}")
    results = subprocess.check_output(call_list, stderr=subprocess.DEVNULL)

    split_results = results.splitlines()
    vf_lines = list(filter(is_viewfactor_line, split_results))

    vf_infos = []
    for spec, val in zip(vf_lines[::2], vf_lines[1::2]):
        from_, to_ = spec.decode("utf-8").split(" -> ")
        _, value_ = val.decode("utf-8").split(" : ")
        vf_info = VFInfo(from_, to_, value_)
        vf_infos.append(vf_info)

    vf_matrix = VFMatrix(vf_infos)

    return vf_matrix


def vf_format(vf):
    return f"{vf:.4}"


def calc_rel_diff(ref, calc):
    diff = abs(calc - ref) / ref
    return f"{diff:.3%}"
