# Nelder-Mead
Fast C implementation of the [Nelder-Mead method](http://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method) for unconstrained function minimization. 

## Introduction
The Nelder-Mead method is implemented in `nelder_mead.c`, and it is separated from the implementation of the cost function `cost_fun.c`, which provides a general interface allowing for an easy customization. The function takes as input a (mandatory) array of coordinates `duoble *` and an (optional) pointer to other arguments `void *`, and returns a `double` representing the value of the cost function in the point of minimum.

Currently a simple [sum of squares function](http://www.sfu.ca/~ssurjano/sumsqu.html), which allows function spaces of arbitrary dimension, is implemented in `cost_fun.c`.

In the package, we provide a stand-alone binary with entry point `main.c`, as well as a [MEX interface](http://www.mathworks.com/help/matlab/ref/mex.html) `nelder_mead_mex.c` which can be called inside Matlab. Note that this implementation is analogous to the built-in Matlab function `fminsearch`, but it is considerably faster.



## How to use
The package can be compiled and tested from command line using the bash script provided (it is assumed that `gcc` is installed). The interface of the Nelder-Mead binary is
```
$ ./nm arg_1 arg_2 [...] arg_n
```
where each `arg_i` is a numeric value representing the i-th coordinate of the initial optimization point in the n-dimensional space of the sum of squares cost function.

The header file `nelder_mead.h` contains several customizable `#define` macros. For instance, by enabling the `VERBOSE` mode, the program will output all the steps of the optimization as
```
Iteration 0001  expand      [ -2.20 -3.19 4.05 ]    96.62 
Iteration 0002  reflect     [ -2.20 -3.19 4.05 ]    96.62 
Iteration 0003  expand      [ -2.13 -3.14 3.75 ]    85.30 
Iteration 0004  expand      [ -2.38 -3.24 3.00 ]    64.36 
Iteration 0005  reflect     [ -2.38 -3.24 3.00 ]    64.36 
Iteration 0006  expand      [ -2.34 -3.39 1.65 ]    38.05 
Iteration 0007  expand      [ -2.69 -3.75 0.15 ]    31.85 
Iteration 0008  reflect     [ -2.71 -3.53 0.20 ]    28.76 
[...]
Iteration 0083  contract in [ 0.00 0.00 0.00 ]      0.00 
```

Alternatively, within Matlab, you can compile and run the MEX function by executing the script `test.m`. The interface of the function is
```
>> [x, fx] = nelder_mead_mex( x0[, tolX, tolF, maxIter, maxEval, verbose] )
```
where the outputs `x` and `fx` correspond to the position and value of the function minimum, whereas the input `x0` represents the initial point. Other optional inputs control the [stopping conditions](http://www.mathworks.com/help/matlab/ref/optimset.html) of the minimization. 

## Licence
MIT Licence. Copyright (c) 2015 Matteo Maggioni
