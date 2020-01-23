#!/usr/bin/env python

"""
Parses output-logs saved in a text file. Records information about the time
stepping. Plots and prints pertinent information regarding the time stepping.
"""

from __future__ import print_function
import matplotlib.pyplot as plt
import re

# regex definitions
float_pre_regex = r"(?:(?:\d+[.]\d*)|(?:\d+))"
float_pos_regex = r"(?:[eE][+-]\d+)?"
float_regex = "(" + float_pre_regex +float_pos_regex + ")"

int_regex = r"([+-]?[\d]+)"

# "Time Step <int>, time = <float>"
timestep_regex = r"(?:Time Step\s*)" + int_regex + r"(?:, time =\s*)" + float_regex
timestep_regex = re.compile(timestep_regex)

# "dt = <float>"
dt_regex = "(?:dt =\s*)" + float_regex
dt_regex = re.compile(dt_regex)

convergence_regex = "((?:Solve Did NOT Converge!)|(?:Solve Converged!))"
convergence_regex = re.compile(convergence_regex)

def get_time_step_info(log_file):
    with open(log_file) as run_log:
        text = run_log.read()
        timesteps_times = re.findall(timestep_regex, text)[1:]
        convergences = re.findall(convergence_regex, text)
        _dts = re.findall(dt_regex, text)[1:]

    timesteps = []
    times = []
    dts = []

    for (timestep, time), convergence, dt in zip(timesteps_times, convergences, _dts):
        if convergence == "Solve Converged!":
            timesteps.append(int(timestep))
            times.append(float(time))
            dts.append(float(dt))

    total_timesteps  = len(_dts)
    valid_timesteps  = len(dts)
    failed_timesteps = total_timesteps - valid_timesteps
    print(f"total timesteps  : {total_timesteps}")
    print(f"valid timesteps  : {valid_timesteps}")
    print(f"failed timesteps : {failed_timesteps}")
    print()

    return timesteps, dts, times


# overriding print because why not
original_print = print
def print(*args, **kwargs):
    if "sep" not in kwargs:
        kwargs["sep"] = ""
    original_print(*args, **kwargs)


def visualize_time_step_info(timesteps, dts, times):
    mindt, maxdt = min(dts), max(dts)
    miny, maxy = 0.9 * mindt, 1.1 * maxdt

    print("Simulation End Time : ", times[-1], "s")
    print("Number of timesteps : ", timesteps[-1])
    print()

    print("Min dt : ", mindt, "s")
    print("Max dt : ", maxdt, "s")

    # Time vs. dt plot
    maxx = times[-1]
    minx = times[0]
    plt.semilogy(times, dts)
    plt.title("Time vs. dt")
    plt.xlabel("Time [s]")
    plt.xlim(minx, maxx)
    plt.ylim(miny, maxy)
    plt.ylabel("dt [s]")
    plt.show()

    # Timestep vs. dt plot
    minx, maxx = timesteps[0], timesteps[-1]
    plt.semilogy(timesteps, dts)
    plt.title("Timestep vs. dt")
    plt.xlabel("Timestep [#]")
    plt.xlim(minx, maxx)
    plt.ylim(miny, maxy)
    plt.ylabel("dt [s]")
    plt.show()


if __name__ == '__main__':
    import argparse

    # process command line
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('log-file', metavar='lf', type=str,
                        help='The log file to be parsed.')
    parser.add_argument('--visualize', action='store_true')
    parser.add_argument('--print-times', action='store_true')

    args = parser.parse_args()
    log_file = args.__getattribute__("log-file")

    if args.visualize or args.print_times:
        timesteps, dts, times = get_time_step_info(log_file)

        if args.print_times:
            print("\'" + str(times)[1:-1].replace(", "," ") + "\'\n")

        if args.visualize:
            visualize_time_step_info(timesteps, dts, times)

    else:
        print("Error: an action was not specified.")