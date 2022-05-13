# Go command

```bash
# compile the code into an excutable
go build

# discover the Go install path
go list -f '{{.Target}}' # GOBIN

# change the install target by setting the GOBIN variable
go env -w GOBIN=/path/to/your/bin

# install the package
go install
```
