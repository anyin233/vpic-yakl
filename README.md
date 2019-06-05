# GPU Specific Instructions

## Obtaining and Using Kokkos

This project relies on Kokkos. There are a few options for a user to obtain
kokkos, documented below.

### Quickstart

1) Do a *recursive* clone of this repo, this will pull down a copy of Kokkos
for you
2) Load modules for a) Cuda, and b) MPI
3) Build the project by passing the CMake option `-DBUILD_INTERNAL_KOKKOS=ON`.
This will request VPIC to build and handle Kokkos for you.
4) If you want GPU functionally, also pass `-DENABLE_KOKKOS_CUDA=ON`. One
should manually revied the `set(KOKKOS_ARCH "...")` line in `CMakeLists.txt` to
ensure it meets their needs (the default is for Volta)

This should give you a simple working of the code, but be aware it does come
with caveats. Most notably, one is expected to main a version of Kokkos per
target device (one per GPU device, one per CPU platform, etc), where as the
above builds for the specific platform that you're currently on. Additionally,
the above approach can be quite brittle when changing compile flags
between builds because Kokkos doesn't treat CMake as a first class
citizen (set to change in early 2020)

### Manual Kokkos Install (more powerful, more effort)

It is typical to maintain many different installs of Kokkos (CPU, older
GPU, new GPU, Debug, etc), so it's worth while learning how to install Kokkos
manually. Breifly:

1) Clone Kokkos (or use ./kokkos in the recursive clone) from
https://github.com/kokkos/kokkos
2) Make a build folder, and execute `../generate_makefile.bash`, passing the
appropriate options for platform and device architecture. These look something
like:
  - CPU: `../generate_makefile.bash --with-serial --with-openmp
  --prefix=$KOKKOS_INSTALL_DIR`
  - GPU: `../generate_makefile.bash --with-serial --with-openmp --with-cuda
  --arch=Kepler30 --with-cuda-options=enable_lambda
  --compiler=$KOKKOS_SRC_DIR/bin/nvcc_wrapper --prefix=$KOKKOS_INSTALL_DIR`

### Further Reading

One can cherry pick the Kokkos specific details from
[here](https://github.com/ECP-copa/Cabana/wiki/Build-Instructions) to get
detailed build instructions for Kokkos (ignore the things about Cabana)

The advanced user should review `CMakeLists.txt` for the Kokkos specific
options that are available. These include:

1. `ENABLE_KOKKOS_OPENMP`
2. `ENABLE_KOKKOS_CUDA`
3. `BUILD_INTERNAL_KOKKOS`
4. `VPIC_KOKKOS_DEBUG`
5. `KOKKOS_ARCH`

## Building VPIC + Kokkos

Then when we build VPIC we need to make sure we using the GPU, you need to
specify the Kokkos `nvcc_wrapper` to be the compiler. This typically looks
something like:

`CXX=$HOME/tools/kokkos_gpu/bin/nvcc_wrapper cmake -DENABLE_KOKKOS_CUDA=ON ..`

# VANILLA VPIC README

## Vector Particle-In-Cell (VPIC) Project

VPIC is a general purpose particle-in-cell simulation code for modeling
kinetic plasmas in one, two, or three spatial dimensions. It employs a
second-order, explicit, leapfrog algorithm to update charged particle
positions and velocities in order to solve the relativistic kinetic
equation for each species in the plasma, along with a full Maxwell
description for the electric and magnetic fields evolved via a second-
order finite-difference-time-domain (FDTD) solve. The VPIC code has been
optimized for modern computing architectures and uses Message Passing
Interface (MPI) calls for multi-node application as well as data
parallelism using pthreads. VPIC employs a variety of short-vector,
single-instruction-multiple-data (SIMD) intrinsics for high performance
and has been designed so that the data structures align with cache
boundaries. The current feature set for VPIC includes a flexible input
deck format capable of treating a wide variety of problems. These
include: the ability to treat electromagnetic materials (scalar and
tensor dielectric, conductivity, and diamagnetic material properties);
multiple emission models, including user-configurable models; arbitrary,
user-configurable boundary conditions for particles and fields; user-
definable simulation units; a suite of "standard" diagnostics, as well
as user-configurable diagnostics; a Monte-Carlo treatment of collisional
processes capable of treating binary and unary collisions and secondary
particle generation; and, flexible checkpoint-restart semantics enabling
VPIC checkpoint files to be read as input for subsequent simulations.
VPIC has a native I/O format that interfaces with the high-performance
visualization software Ensight and Paraview. While the common use cases
for VPIC employ low-order particles on rectilinear meshes, a framework
exists to treat higher-order particles and curvilinear meshes, as well
as more advanced field solvers.

## Attribution

Researchers who use the VPIC code for scientific research are asked to cite
the papers by Kevin Bowers listed below.

1. Bowers, K. J., B. J. Albright, B. Bergen, L. Yin, K. J. Barker and
D. J. Kerbyson, "0.374 Pflop/s Trillion-Particle Kinetic Modeling of
Laser Plasma Interaction on Road-runner," Proc. 2008 ACM/IEEE Conf.
Supercomputing (Gordon Bell Prize Finalist Paper).
http://dl.acm.org/citation.cfm?id=1413435

2. K.J. Bowers, B.J. Albright, B. Bergen and T.J.T. Kwan, Ultrahigh
performance three-dimensional electromagnetic relativistic kinetic
plasma simulation, Phys. Plasmas 15, 055703 (2008);
http://dx.doi.org/10.1063/1.2840133

3. K.J. Bowers, B.J. Albright, L. Yin, W. Daughton, V. Roytershteyn,
B. Bergen and T.J.T Kwan, Advances in petascale kinetic simulations
with VPIC and Roadrunner, Journal of Physics: Conference Series 180,
012055, 2009

## Getting the Code

VPIC uses nested submodules.  This requires the addition of the *--recursive*
flag when cloning the repository:

    % git clone https://github.com/lanl/vpic.git

This command will check out the VPIC source code.

## Requirements

The primary requirement to build VPIC is a C++11 capable compiler and
an up-to-date version of MPI.

## Build Instructions

    % cd vpic 

VPIC uses the CMake build system. To configure a build, do the following from
the top-level source directory:
  
    % mkdir build
    % cd build

Then call the curses version of CMake:

    % ccmake ..

The `./arch` directory also contains various cmake scripts (including specific build options) which can help with building

They can be invoked using something like:

    % ../arch/generic-Release

GCC users should ensure the `-fno-strict-aliasing` compiler flag is set (as shown in `./arch/generic-gcc-sse`)

After configuration, simply type 'make'.

## Building an example input deck

After you have successfully built VPIC, you should have an executable in
the *bin* directory called *vpic*.  To build an executable from one of
the sample input decks, simply run:

    % bin/vpic input_deck

where *input_deck* is the name of your sample deck.  For example, to build
the *harris* input deck in the *sample* subdirectory
*(assuming that your build directory is located in the top-level
source directory)*:

    % bin/vpic ../sample/harris

Beginners are advised to read the harris deck thoroughly, as it provides many examples of common uses cases.

## Command Line Arguments

Note: Historic VPIC users should note that the format of command line arguments was changed in the first open source release. The equals symbol is no longer accepted, and two dashes are mandatory. 

In general, command line arguments take the form `--command value`, in which two dashes are followed by a keyword, with a space delimiting the command and the value.

The following specific syntax is available to the users:

### Threading

Threading (per MPI rank) can be enabled using the following syntax: 

`./binary.Linux --tpp n`

Where n specifies the number of threads

#### Example:

`mpirun -n 2 ./binary.Linux --tpp 2`

To run with VPIC with two threads per MPI rank.

### Checkpoint Restart

VPIC can restart from a checkpoint dump file, using the following syntax:

`./binary.Linux --restore <path to file>`

#### Example:

`./binary.Linux --restore ./restart/restart0`

To restart VPIC using the restart file `./restart/restart0`

## Feedback

Feedback, comments, or issues can be raised through [GitHub issues](https://github.com/lanl/vpic/issues)

## Release

This software has been approved for open source release and has been assigned **LA-CC-15-109**.

## Copyright

Copyright (c) 2016, Los Alamos National Security, LLC
All rights reserved.

Copyright 2016. Los Alamos National Security, LLC. This software was produced under U.S. Government contract DE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security, LLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.  NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works, such modified software should be clearly marked, so as not to confuse it with the version available from LANL.

Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
