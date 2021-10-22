# CellPhe - Feature Extraction and Selection 

## Overview

## System Requirements

### Operating Systems
* macOS - tested on Big Sur 10.6
* Linux - test on CentOS 7.9.2009

### Software Dependencies
* C and C++ compilers - tested with Apple clang 11.0.3 ( macOS) & gcc 10.3.0 (Linux)
* LibTiff - test with version 4.2.0, 4.3.0

### Hardware Requirements
* This is a memory intensive program - the example provided in this repository ran with ~10GB of memory on macOS, but up to ~120G on Linux with the tested compilers.

## Installation

#### macOS
Install the XCode Command Line tools using the following terminal command:

```
$ xcode-select --install
```
taking care to select the Command Line Developer Tools as part of the installation process.

Install Libtiff using [homebrew](https://brew.sh/):

```
$ brew update && brew install libtiff
```

Libtiff packages are also available via [MacPorts](https://www.macports.org/) (`tiff`) if you prefer.

#### Linux
Install compilers and GNU make from your distribution package repositories, e.g:

```
$ sudo apt update && sudo apt install build-essential
```

on Ubuntu, or:

```
$ sudo dnf install gcc gcc-c++ make
```

on Fedora.

Install Libtiff from your distribution package repositories, e.g:

```
$ sudo apt update && sudo apt install libtiff libtiff-dev
```

on Ubuntu, or:

```
$ sudo dnf install libtiff libtiff-devel
```

on Fedora.

On other Linux distributions, you will need use the distribution package management tool and find the appropriate package names in order to install. If you are using a shared computing facility, it is likely that you will already have both compilers and Libtiff available to you via an Environment Modules system or similar - consult the documentation for the computing facility to find out how to properly use available system software.

### Compiling this software:
Take the following steps to build this component: 

1. Clone a copy of this repository with `git`:

```
$ git clone https://github.com/llwiggins/CellPhe
```

2. Change your working directory to the directory of this component:

```
$ cd "CellPhe/Extraction of features and time series variables"
```

3. Modify line 18 of the top-level `Makefile` to correctly locate LibTiff, i.e. change `/usr/local/lib` to the correct path for your system, if not already correct

4. Modify lines 5 and 6 of the `image_info/Makefile` to correctly locate LibTiff libraries and include files, i.e. change `/usr/local/lib` and `/usr/local/include` to the correct path for your system, if not already correct

5. Compile the software by running:

```
$ make
```

A typical build of this software should take fewer than 60 seconds. All being well, you should have an executable file called `image` in component directory. This can be copied to wherever you would like to use it from, or can be added to your `PATH`.

## Example Usage

### Running the Program

ADD EXAMPLE RUN USING PROVIDED SAMPLE DATA

### Expected Output

DESCRIBE PROGRAM OUTPUT