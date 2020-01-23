"""
Filters-out CGAL sub-packages which are under the GPL.
"""
from __future__ import print_function
from pathlib import Path
from shutil import rmtree
import argparse
import yaml
import re

if __name__ == "__main__":
    DEFAULT_APP_NAME = "sockeye"
    DEFAULT_CGAL_LOCATION = "contrib"
    DEFAULT_CGAL_DIR = "cgal"
    DEFAULT_CONFIG_FILE = __file__.replace(".py", ".config")

    # command line arguments
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app-name', type=str, default=None, help=f'the app name (default : {DEFAULT_APP_NAME})')
    parser.add_argument('--cgal-location', type=str, default=None, help=f'the location of the CGAL directory relative to the app root (default : {DEFAULT_CGAL_LOCATION})')
    parser.add_argument('--cgal-dir', type=str, default=None, help=f'the directory name of CGAL (default : {DEFAULT_CGAL_DIR})')
    parser.add_argument('--dry-run', action='store_true', help="specified actions are printed and not executed")
    parser.add_argument('--verbose', action='store_true', help="specified actions are printed")
    parser.add_argument('--show-reason', action='store_true', help="shows reason for action during a dry run")
    parser.add_argument('--config-file', type=str, default=DEFAULT_CONFIG_FILE, help=f'the yaml configuration file (default : {DEFAULT_CONFIG_FILE})')
    parser.add_argument('--force-keep', type=str, nargs='+', help="additional sub-packages that are suppose to be kept regardless of whether they are GPL or LGPL")
    parser.add_argument('--force-delete', type=str, nargs='+', help="additional sub-packages that are suppose to be deleted regardless of whether they are GPL or LGPL")
    args = parser.parse_args()

    # process configuration file
    with open(args.config_file, "r") as cfg_file:
        try:
            configuration = yaml.load(cfg_file)
        except yaml.YAMLError as exc:
            print(exc)

    # to handle force keeps and deletes
    def get_forcing_specification(container, name):
        extraction = container.get(name, [])
        return set() if extraction is None else set(extraction)

    ## command line specification
    cl_force_keep   = get_forcing_specification(args.__dict__, "force_keep")
    cl_force_delete = get_forcing_specification(args.__dict__, "force_delete")

    ## configuration file specification
    cfg_force_keep   = get_forcing_specification(configuration, "force_keep")
    cfg_force_delete = get_forcing_specification(configuration, "force_delete")

    ## combining command line and configuration file specification
    force_keep   = cl_force_keep.union(cfg_force_keep)
    force_delete = cl_force_delete.union(cfg_force_delete)

    ## checking for conflicts
    conflicts = force_keep.intersection(force_delete)
    if len(conflicts) > 0:
        raise Exception(f"conflicts between force keep and delete: {conflicts}")


    # deducing file structure for the app and the location of CGAL
    CUR_DIR = Path.cwd()
    ROOT = Path(CUR_DIR.root)

    def resolve_between_config_and_cl(config_spec, cl_spec):
        return cl_spec if cl_spec is not None else config_spec

    config_app_name = configuration.get("app_name", DEFAULT_APP_NAME)
    cl_app_name = args.app_name
    app_name = resolve_between_config_and_cl(config_app_name, cl_app_name)

    APP_DIR = CUR_DIR
    while (APP_DIR.stem != app_name) and (APP_DIR != ROOT):
        APP_DIR = APP_DIR.parent

    if APP_DIR == ROOT:
        raise Exception(f"couldn't find {app_name}'s root")

    def verify_dir(dir_, dir_name):
        if not dir_.exists():
            err_str  = f"{dir_name} directory does not exits. "
            err_str += f"Tried looking for \"{dir_}\""
            raise Exception(err_str)

    config_cgal_location = configuration.get("cgal_location", DEFAULT_CGAL_LOCATION)
    cl_cgal_location = args.cgal_location
    cgal_relative_location = resolve_between_config_and_cl(config_cgal_location, cl_cgal_location)
    CGAL_LOCATION = APP_DIR / cgal_relative_location
    verify_dir(CGAL_LOCATION, cgal_relative_location)

    config_cgal_dir = configuration.get("cgal_dir", DEFAULT_CGAL_DIR)
    cl_cgal_dir = args.cgal_dir
    cgal_relative_dir = resolve_between_config_and_cl(config_cgal_dir, cl_cgal_dir)
    CGAL_DIR = CGAL_LOCATION / cgal_relative_dir
    verify_dir(CGAL_DIR, cgal_relative_dir)

    def is_exposed_dir(item):
        return item.is_dir() and item.stem[0] != "."

    sub_dirs = (item for item in CGAL_DIR.iterdir() if is_exposed_dir(item))


    def get_license_path(directory):
        directory_name = directory.stem
        return directory / "package_info" / directory_name / "license.txt"


    def is_subpackage(directory):
        license_file = get_license_path(directory)
        return license_file.exists()


    sub_packages = []
    misc_dirs = []
    for sub_dir in sub_dirs:
        if is_subpackage(sub_dir):
            sub_packages.append(sub_dir)
        else:
            misc_dirs.append(sub_dir)

    def is_LGPL(sub_package):
        LGPL_pattern = "LGPL"
        license_file = get_license_path(sub_package)
        with open(license_file, "r") as open_file:
            license_file_text = open_file.read()

        matches = re.findall(LGPL_pattern, license_file_text)
        return len(matches) >= 1

    show_reason = args.show_reason
    dry_run = args.dry_run
    verbose = dry_run or args.verbose

    def verbose_printout(removing, sub_package, reason):
        if verbose:
            dry_run_str  = "removing" if removing else "keeping"
            dry_run_str += " " + sub_package.stem
            dry_run_str += " because " + reason if show_reason else ""
            print(dry_run_str)

    force_delete_print = lambda sub_package : verbose_printout(True, sub_package, "specified as force delete")
    lgpl_print = lambda sub_package : verbose_printout(False, sub_package, "is LGPL")
    force_keep_print = lambda sub_package : verbose_printout(False, sub_package, "specified as force keep")
    gpl_print = lambda sub_package : verbose_printout(True, sub_package, "is GPL")

    not_dry_run = not dry_run

    def rmdir(dir_):
        if not_dry_run:
            rmtree(dir_, ignore_errors=True)

    for sub_package in sub_packages:
        sub_package_stem = sub_package.stem

        if is_LGPL(sub_package):
            if sub_package.stem in force_delete:
                force_delete_print(sub_package)
                rmdir(sub_package)
            else:
                lgpl_print(sub_package)

        else:
            if sub_package.stem in force_keep:
                force_keep_print(sub_package)

            else:
                gpl_print(sub_package)
                rmdir(sub_package)