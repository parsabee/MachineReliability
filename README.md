# Machine Reliability problem solver
reads from `stdin` the input of the problem of format:
```
<budget>
<number of machines>
<cost of machine 1> <reliability of machine 1>
<cost of machine 2> <reliability of machine 2>
.
.
.
<cost of machine n> <reliability of machine n>
```
## build
```
make
```
## execute
```
reliability --iter # iterative version
reliability --memo # recursive memoized version
reliability # both memoized and iterative
```
