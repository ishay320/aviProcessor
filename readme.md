# avi processor

- use openCV for input and output
- use C++11

# the files
|file | usage|
|-----|------|
| dependencies.bash | to install dependencies like openCV|
| makefile | to compile my code|
| main.cpp | the main code|
| test.cpp | the testing with doctest|
<!-- - dockerfile : is for ease of cleaning -->

# running the code

```bash
$ make
$ ./main <input file | -c for camera> <output> -s
```
can add `DEBUG=1` to make to show debug info:
```bash
$ make DEBUG=1
```
- press <kbd>ESC</kbd> to stop from the preview screen.
- <kbd>ctrl</kbd>+<kbd>c</kbd> do graceful exit.
