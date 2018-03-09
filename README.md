# NuMC

This repo contains a new Monte Carlo simulation developed for the ANITA collaboration to simulate the detection of ultra-high energy neutrino induced air showers in the Antarctic ice by ANITA. 

The `master` branch should always compile and run succesfully, but is under heavy development, so we suggest using one of the official [releases](https://github.com/rprechelt/numc/releases) as they are guaranteed to compile, run, and produce validated neutrino data.

## Installation
**Before cloning the repo**, you will need to install the Git [LFS](https://git-lfs.github.com/) extension; this allows us to store large data files in the repo without including them in the regular Git log _significantly_ improving push/pull performance. 

To install and run the code, you will need GCC>=5.2 (GCC>=6 is preferred), a working installation of Boost, and ROOT. 

Note for MacOS users: on a default MacOS installation, `g++` is _aliased_ to Clang. This code has not currently been tested to compile under Clang, but _may_ work, although you may miss out on a few compiler optimizations due to our use of GCC-specific attributes. 

Once you have met these requirements, you can setup the repo as follows

     # clone the repo
     git clone --recursive https://github.com/rprechelt/numc
     
     # change into the repo directory
     cd numc
     
     # you can then compile the code with make
     # the first time you run make, it will automatically download several large data files
     # and unzip them into the data/ directory
     make
     
     # you should now have a functioning binary
     # you should see a command line help message when you run the below command
     ./build/NuMC 
     
Once you have a functioning binary, you should run the built-in test suite to verify that there are no issues. Please see the **Running Tests** section below.
     
### Using Conda

If you do not wish to install these into your system for fear of conflicts with other projects, you can use [Conda](https://conda.io/docs/) to create a local virtual environment for GCC and Boost. However, you will still need a system provided ROOT. 

To create a Conda environment for this project, 

    conda create --name numc
    conda activate numc  # to enable the conda environment in your current shell
    conda install -c creditx gcc-7
    conda install -c conda-forge boost
    source deactivate  # to disable the conda environment
    
You can then follow the instructions above to get the code compiled and ready to run. 
    
    
## Running Tests
The project has an ever-growing test suite that attempts to test many of the functions and methods
within numc. To compile and run the test suite, 

    # compile the test suite
    make test 
    
    # you can then run it with
    ./build/Test_NuMC
    
The test code is organized into suites, cases, and subcases. As you are developing `numc`, you may find your self oly wanting to run tests for a particular piece of code. For example, we only want to run tests on the `Particle` class; this can be achieved by passing the test-suite argument to `Test_NuMC`

    # only test Bedmap
    ./build/Test_NuMC --test-suite=particle
    
    # or in short form
    ./build/Test_NuMC -ts=particle
    
You can also only run one particular test case in the Particle test suite (see the corresponding test file for the names of the various test cases). To only run a particular test case, 

    # only run "Create ElectronNeutrino" test case
    ./build/Test_NuMC --test-case="Create ElectronNeutrino"
        
    # or in short form
    ./build/Test_NuMC -tc="Create ElectronNeutrino"

For more information about how to run the various test suites, cases and subcases, and the format that results are displayed in, please see the `Test_NuMC` help

     # more info
     ./build/Test_NuMC -h
