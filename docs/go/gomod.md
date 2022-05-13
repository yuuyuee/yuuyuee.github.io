# go mod

## Create module

```bash
# 1. Create a greetings directory for your Go module source code.
mkdir greetings
cd greetings

# 2. Start your module using the go mod init command.
go mod init example.com/greetings

# 3. Create a file in which to write your code and call it greetings.go.
# ...
```

## Call your code from another module

```bash
# 1. Create a directory for your caller
mkdir hello
tree
# <HOME>/
#   | -- greetings/
#   | -- hello/
cd hello

# 2. Enable dependency tracking for the code you're about to write.
go mod init example.com/hello

# 3. Create a file in which to write your code and call it hello.go.

# replace example.com/greetings with ../greetings for the purpose of
# locating dependency.
go mod edit -replace example.com/greetings=../greetings

# Synchronize the example.com/hello module's dependencies, adding those
# required by the code, but not yet tracked in the module.
go mod tidy
```
