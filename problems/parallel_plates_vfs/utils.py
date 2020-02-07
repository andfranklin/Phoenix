from argparse import ArgumentParser
from subprocess import check_output
from os import getcwd, chdir

cases = {"hex_hex", "pyr_pyr", "pri_pri"}

def make_command_line_parser(description):
	parser = ArgumentParser(description=description)
	parser.add_argument('case', choices=cases, nargs=1, help='name of the case to run')
	return parser


def make_subdir(case):
	cwd = getcwd()
	chdir(case)
	check_output(["make", "all"])
	chdir(cwd)