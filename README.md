# How to build this  
## Prerequisites
This has been tested on linux on the Debian Buster distro, but should work on all linux systems.  
Also has been tested on MacOS.  
The base path of the program is assumed to be the project root. E.g. if the dataset.csv is passed  
as a command line argument, then the file dataset.csv must be present in the project root.
## Build instructions
* Run the build_project.sh script in the project root.
* Run the executable named project in the Release directory. This MUST be run from the project root  
like ./Release/project, or the paths will be wrong.

# Testing
## Instructions
* Run the run_tests.sh script in the project root.

# Command line arguments
* --in: This controls the input file name. Default: W_Dataset.csv
* --out: This controls the output file name. Default: W_Out_Pairs.csv
* --type: This controls whether the results are written as pairs or as a list matching ids.  
Default value: pairs  
Allowed values: pairs, all