import subprocess


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


def call_phoenix(refinement_level=0,
                 quadrature_type="GAUSS", quadrature_order="FOURTH",
                 plate_filename="plate.msh"):
    call_list = ["../../phoenix-opt", "-i", f"view_factors.i"]
    call_list.append(f"refinement_level={refinement_level}")
    call_list.append(f"quadrature_type={quadrature_type}")
    call_list.append(f"quadrature_order={quadrature_order}")
    call_list.append(f"plate_mesh={plate_filename}")
    results = subprocess.check_output(call_list)

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
