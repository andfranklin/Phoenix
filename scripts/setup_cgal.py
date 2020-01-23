"""
Sets up CGAL so that it can be built manually. Adds the extraneous
include files.
"""
from __future__ import print_function
from pathlib import Path
from shutil import rmtree
import argparse
import subprocess
import os


class cd:
    def __init__(self, newPath):
        self.newPath = Path(newPath)

    def __enter__(self):
        self.savedPath = Path.cwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)


if __name__ == "__main__":
    DEFAULT_APP_NAME = "sockeye"
    DEFAULT_CGAL_LOCATION = "contrib"
    DEFAULT_CGAL_DIR = "cgal"
    DEFAULT_CONFIG_FILE = __file__.replace(".py", ".config")
    DEFAULT_MINICONDA_DIR = "/opt/moose/miniconda/"
    DEFAULT_BOOST_VERSION = "1.61.0-py27_0"

    # command line arguments
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app-name', type=str, default=DEFAULT_APP_NAME, help=f'the app name (default : {DEFAULT_APP_NAME})')
    parser.add_argument('--cgal-location', type=str, default=DEFAULT_CGAL_LOCATION, help=f'the location of the CGAL directory relative to the app root (default : {DEFAULT_CGAL_LOCATION})')
    parser.add_argument('--cgal-dir', type=str, default=DEFAULT_CGAL_DIR, help=f'the directory name of CGAL (default : {DEFAULT_CGAL_DIR})')
    parser.add_argument('--miniconda-dir', type=str, default=DEFAULT_MINICONDA_DIR, help=f'the directory name of miniconda (default : {DEFAULT_MINICONDA_DIR})')
    parser.add_argument('--boost-version', type=str, default=DEFAULT_BOOST_VERSION, help=f'the version of boost being used (default : {DEFAULT_BOOST_VERSION})')
    parser.add_argument('--dry-run', action='store_true', help="specified actions are printed and not executed")
    parser.add_argument('--verbose', action='store_true', help="specified actions are printed")
    args = parser.parse_args()

    CUR_DIR = Path.cwd()
    ROOT = Path(CUR_DIR.root)
    MINICONDA_DIR = Path(args.miniconda_dir)

    APP_DIR = CUR_DIR
    while (APP_DIR.stem != args.app_name) and (APP_DIR != ROOT):
        APP_DIR = APP_DIR.parent

    cgal_dir = APP_DIR / args.cgal_location / args.cgal_dir

    if not cgal_dir.is_dir():
        err_str  = f"invalid specifiaction for the cgal dir "
        err_str += "(i.e. {cgal_dir} does not exits)."
        raise Exception(err_str)

    BOOST_ROOT = MINICONDA_DIR / "pkgs" / f"boost-{args.boost_version}"
    MINICONDA_LIBS = MINICONDA_DIR / "lib"
    MINICONDA_INCLUDES = MINICONDA_DIR / "include"

    gmp_include_cl = f"GMP_INCLUDE_DIR:PATH={MINICONDA_INCLUDES}"
    GMP_LIB        = MINICONDA_LIBS / "libgmp.dylib"
    gmp_lib_cl     = f"GMP_LIBRARIES:FILEPATH={GMP_LIB}"

    mpfr_include_cl = f"MPFR_INCLUDE_DIR:PATH={MINICONDA_INCLUDES}"
    MPFR_LIB        = MINICONDA_LIBS / "libmpfr.dylib"
    mpfr_lib_cl     = f"MPFR_LIBRARIES:FILEPATH={MPFR_LIB}"

    cmake_cl_args = ["cmake",
        "-D", gmp_include_cl,
        "-D", gmp_lib_cl,
        "-D", mpfr_include_cl,
        "-D", mpfr_lib_cl,
        "-D", "WITH_CGAL_Qt5:BOOL=OFF",
        "-D", "WITH_CGAL_ImageIO:BOOL=OFF",
        "."
    ]

    with cd(cgal_dir):
        os.environ["BOOST_ROOT"] = str(BOOST_ROOT)
        proc = subprocess.run(cmake_cl_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        try:
            proc.check_returncode()
        except subprocess.CalledProcessError as proc_err:
            print("Cmake failed.")
            print("Error Log:\n")
            print(proc.stderr.decode("utf-8"))
            raise proc_err

        proc1 = subprocess.run(["git", "status", "--short"], stdout=subprocess.PIPE)
        changes = ((line[0:2], line[3:]) for line in proc1.stdout.decode("utf-8").split("\n"))

        modified_changes = []
        untracked_changes = []
        for change_type, change in changes:
            if change_type == " M":
                modified_changes.append(change)
            else:
                if change != "" and change != "include/":
                    untracked_changes.append(change)

        for change in untracked_changes:
             subprocess.run(["rm", "-r", change])