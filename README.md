# Nelder-Mead
Fast C implementation of the [Nelder-Mead method](http://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method) for unconstrained function minimization. 

## Introduction
The Nelder-Mead method is implemented in `nelder_mead.c`. The cost function is passed as a function pointer argument, this provides a general interface allowing for an easy customization. The cost function takes as input a `point_t *` and an (optional) pointer to other constant arguments `void *`. The `point_t` is a custom `struct` composed by a `double *x` (i.e., the n-dimensional position) and a `double fx` (i.e., the value of the cost function calculated at position `x`). On exit the `fx` field of the input will contain the function evaluation. The main `nelder_mead` function takes as input the dimensionality `n` of the function space, the initial n-dimensional `point_t *start` and the final minimizer `point_t *solution`, the pointer to a cost function defined as above, the `void *` pointer to its optional argument, and finally a custom struct `optimset_t` containing the parameters for the optimisation.

Optimization settings are designed to resemble [Matlab optimset](https://www.mathworks.com/help/matlab/ref/optimset.html).

Currently the cost function is a simple [Ackely function](http://www.sfu.ca/%7Essurjano/ackley.html). This function allows function spaces of arbitrary dimension and it also shows how the optional `void *` argument can be used.

## How to use
The package can be compiled and tested from command line using the Makefile provided (run `make` to compile and `make test` to compile and execute an example run). The command line interface of the Nelder-Mead binary is currently very simple and accepts only a sequence of numbers
```
$ ./binary arg_1 arg_2 [...] arg_n
```
where `arg_i` is the i-th coordinate of n-dimensional initial position.

Enabling verbose mode in `nelder_mead.c` would make the program print to console all the steps of the optimization, e.g. the result of running `make test` is the following:
```
Iteration 0001     reflect         [ -2.10 -3.04 4.73 ]    11.00 
Iteration 0002     expand          [ -1.96 -2.89 4.88 ]    10.46 
Iteration 0003     reflect         [ -1.96 -2.89 4.88 ]    10.46 
Iteration 0004     reflect         [ -1.90 -2.97 5.07 ]    10.45 
Iteration 0005     contract out    [ -1.90 -2.97 5.07 ]    10.45 
Iteration 0006     contract in     [ -1.92 -3.01 4.86 ]    10.44 
Iteration 0007     reflect         [ -2.00 -2.96 4.89 ]    10.30 
Iteration 0008     reflect         [ -2.00 -2.96 4.89 ]    10.30 
Iteration 0009     contract in     [ -1.92 -3.00 4.99 ]    10.25 
Iteration 0010     reflect         [ -1.92 -3.00 4.99 ]    10.25 
Iteration 0011     contract out    [ -1.99 -2.94 4.97 ]    10.20 
[...]
Iteration 0040     contract in     [ -1.99 -2.98 4.97 ]    10.17 
Initial point
x = [ -2.10000000 -3.04000000 4.50000000 ], fx = 11.21198731 
Solution
x = [ -1.98940255 -2.98441227 4.97381887 ], fx = 10.16673927 
``` 

## Licence
MIT Licence. Copyright (c) 2017 Matteo Maggioni
