Phoenix
=======

![logo](https://upload.wikimedia.org/wikipedia/commons/4/43/Phoenix-Fabelwesen.jpg "Phoenix")

A MOOSE app for radiation heat transfer. It deterministically calculates view factors between/of
arbitrary geometries. Occlusion and self-occlusion can be accounted for. A variety of options
exist for users to select between using (fast, but potentially buggy) floating point numbers as
the basis for their computation or "exact" numbers (slow, but avoids bugs common with floating points).

Disclaimer: Of course, the "exact" numbers are not exact. See
[CGAL's documentation](https://doc.cgal.org/latest/Manual/tutorial_hello_world.html) for more information.

Setup
=====

Before compiling Phoenix one should first follow the
[Getting Started](https://www.mooseframework.org/moose/getting_started/index.html)
instructions for MOOSE. Proceed only after the MOOSE environment
is installed.

Phoenix requires a few additional libraries to be installed before it can be
built. Perhapse the simplest way to install the libraries is with miniconda. First
setup a custom conda environment:

    conda create -n phoenix --clone=/opt/moose/miniconda

The new environment then needs to be activated:

    source activate phoenix

Run the following commands to install the dependancies into the phoenix
environment:

    sudo -E conda install gmp
    sudo -E conda install mpfr
    sudo -E conda install boost

This will install the [GNU Multiple Precision Arithmetic Library (GMP)](https://gmplib.org/),
[GNU Multiple Precision Floating-Point Reliably library (GNU MPFR)](http://www.mpfr.org/),
and [Boost](http://www.boost.org/), respectively.
These dependancies are required for the modified version of
[The Computational Geometry Algorithm Library (CGAL)](http://www.cgal.org/)
that is included as a submodule in Phoenix.

After the MOOSE environment is installed, clone Phoenix into the
desired directory:

    git clone https://github.com/andfranklin/Phoenix.git

The submodules (CGAL, MOOSE and transitive submodules) then need to be
initialized and updated:

	cd phoenix
	git submodule update --init

Next libMesh must be built (note that MOOSE updates will require often require libMesh to be rebuilt):

    make libmesh_update

Finally, Phoenix can be built:

    make -j <number of cores>

To avoid getting burned by Phoenix, verify that everything has been installed properly by running the tests:

	./run_tests

and the unit tests:
	
	cd unit
	make -j <number of cores>
	./run_tests

Note: The phoenix conda-environment must be activated before Phoenix can be compiled.  