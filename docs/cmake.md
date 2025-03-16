#cmake

## cmake

### Generate a project build system

cmake [<options>] -B <path-to-buid> [-s <path-to-source>]
cmake [<options>] <path-to-source | path-to-existing-build>

### Build a project

cmake --build <dir> [<options>] [-- <build-too-options>]

### Open a project

cmake --open <dir>

### Run a script

cmake [-D <var>=<value>]... -P <cmake-script-file>

### Run a command-line tool

cmake -E <command> [<options>]

### Run the Find-Package tool

cmake --find-package [<options>]

### Run a workflow preset

cmake --workflow <options>

### View help

cmake --help [-<topic>]
