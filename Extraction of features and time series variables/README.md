# CellPhe - Feature Extraction and Selection 

## Overview

## System Requirements

### Operating Systems
* macOS - ADD TESTED VERSIONS
* Linux - ADD TESTED VERSIONS

### Software Dependencies
* C and C++ compilers - ADD TESTED VERSIONS
* CMake - ADD TESTED VERSIONS
* LibTiff - ADD TESTED VERSIONS

### Hardware Requirements
* DESCRIBE MEMORY REQUIREMENTS

## Installation

#### Linux
Install Libtiff and CMake from your distribution package repositories, e.g:

```
$ sudo apt update && sudo apt install cmake libtiff libtiff-dev
```

on Ubuntu, or:

```
$ sudo dnf install cmake libtiff libtiff-devel
```

on Fedora.

On other Linux distributions, you will need use the distribution package management tool and find the appropriate package names in order to install. If you are using a shared computing facility, it is likely that you will already have both Libtiff and CMake available to you via an Environment Modules system or similar - consult the documentation for the computing facility to find out how to properly use available system software.

#### macOS

First, install Libtiff using [homebrew](https://brew.sh/):

```
$ brew update && brew install cmake libtiff
```

CMake and Libtiff packages are also available via [MacPorts](https://www.macports.org/) (`cmake` and `tiff`) if you prefer.

### Compiling with CMake
Take the following steps to build this component with CMake:

1. Clone a copy of this repository with `git`:

```
$ git clone https://github.com/llwiggins/CellPhe
```

2. Change your working directory to the directory of this component:

```
$ cd "CellPhe/Extraction of features and time series variables"
```

3. Create a build directory and move into it:

```
$ mkdir build && cd build
```

4. Configure the build for your system:

```
$ cmake .. .
```

5. Compile the software:

```
$ make
```

A typical build of this software should take fewer than 60 seconds. All being well, you should have a `bin` directory in the `build` directory, containing a single executable file `CellPhe`. This can be copied to wherever you would like to use it from, or can be added to your `PATH`.

## Example Usage

### Running the Program

ADD EXAMPLE RUN USING PROVIDED SAMPLE DATA

### Expected Output

DESCRIBE PROGRAM OUTPUT
