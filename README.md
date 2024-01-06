# Nelder-Mead
Fast C implementation of the [Nelder-Mead method](http://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method) for unconstrained function minimization. 

## Introduction
A custom optimization using Nelder-Mead can be implemented by following the abstract interface in [`model.h`](model.h), which includes:
 * `point` (struct): defines a `n`-dimensional point;
 * `model` (struct): defines the model to optimize;
 * `dimensions` (function): returns the number of dimensions `n` of the `model` domain;
 * `init_model` (function): initializes the `model` to optimize;
 * `cost` (function): computes the cost function of the `model` to optimize at a given input point;

A `point` is a structure containing:
 * `x`: array of coordinates;
 * `y`: value of the `cost` function `f(x)` evaluated at `x`;

The `cost` function accepts the following arguments:
 * `model *mdl`: `model` to optimize;
 * `point *pnt`: the coordinates `pnt->x` will be used to evaluate the cost function whose value will be then stored to `pnt->y` (updated on output);

An extremely simple implementation of a `cost` function is provided in [`abs.c`](abs.c) which implements a 1-dimensional absolute value. We also provide implementations of several other functions:
* [Abs](https://en.wikipedia.org/wiki/Absolute_value) in [`abs.c`](abs.c);
* [Ackely](http://www.sfu.ca/%7Essurjano/ackley.html) in [`ackley.c`](ackley.c);
* [Beale](http://www.sfu.ca/%7Essurjano/beale.html) in [`beale.c`](beale.c);
* [Hartmann 3-D](http://www.sfu.ca/%7Essurjano/hart3.html) in [`hartmann3.c`](hartmann3.c);
* [Hartmann 6-D](http://www.sfu.ca/%7Essurjano/hart6.html) in [`hartmann6.c`](hartmann6.c);
* [Himmelblau](https://en.wikipedia.org/wiki/Himmelblau%27s_function) in [`himmelblau.c`](himmelblau.c);
* [Rosenbrock](http://www.sfu.ca/%7Essurjano/rosen.html) in [`rosenbrock.c`](rosenbrock.c);
* [Sphere](http://www.sfu.ca/%7Essurjano/spheref.html) in [`sphere.c`](sphere.c).

The optimization is based on a `simplex` structure defined in [`nelder_mead.h`](nelder_mead.h), containing:
 * `point *vertices`: the `n+1` list of vertices in the simplex ordered according to their `cost` function values;
 * `point *reflected, *expanded, *contracted, *centroid`: the transformed `point` of the simplex;
 * `size_t n`: number of dimensions of the `model` domain;
 * `unsigned int num_iter, num_eval`: counters for the number of iterations and evaluations executed during the optimization;

The main `nelder_mead` method is implemented in [`nelder_mead.c`](nelder_mead.c), and accepts the following arguments:
 * `model *mdl`: model to optimize
 * `optimset *opt`: optimization settings
 * `simplex *smpl`: simplex used for optimization (updated on exit)
 * `point *out`: minimizer found by the optimization (output argument)

## Compilation
The package can be compiled and tested from command line using the [Makefile](Makefile) provided. Run `make` to compile all provided functions. Run `make test` to compile *and* run optimizations for all functions. The output of `make test` should be
```bash
$ make test
...
argc 11, argv [ ./bin/nm-abs 9 0 0 1e-9 1e-9 5000 5000 0 1.0 -7.60 ]
        Input [ -7.600000000 ]  7.600000000
       Output [ -0.000000000 ]  0.000000000
  Tolerance-x 0.000000001
  Tolerance-y 0.000000000
   Iterations 34
  Evaluations 70
argc 13, argv [ ./bin/nm-ackley 9 0 0 1e-9 1e-9 5000 5000 0 1.0 -2.10 -3.04 4.50 ]
        Input [ -2.100000000 -3.040000000  4.500000000 ]  11.211987305
       Output [ -0.000000000 -0.000000000  0.000000000 ]  0.000000001
  Tolerance-x 0.000000001
  Tolerance-y 0.000000001
   Iterations 123
  Evaluations 237
...
argc 12, argv [ ./bin/nm-himmelblau 9 0 0 1e-9 1e-9 5000 5000 0 1.0 -3.0 -3.0 ]
        Input [ -3.000000000 -3.000000000 ]  26.000000000
       Output [ -3.779310253 -3.283185991 ]  0.000000000
  Tolerance-x 0.000000001
  Tolerance-y 0.000000000
   Iterations 72
  Evaluations 142
argc 12, argv [ ./bin/nm-beale 9 0 0 1e-9 1e-9 5000 5000 0 1.0 0.0 0.0 ]
        Input [  0.000000000  0.000000000 ]  14.203125000
       Output [  3.000000000  0.500000000 ]  0.000000000
  Tolerance-x 0.000000001
  Tolerance-y 0.000000000
   Iterations 82
  Evaluations 158
```

Note that after running `make`, all the generated binaries will be inside a newly created `bin` folder. Running `make clean` will remove all binaries. Compilation has been tested on Linux (various distributions) using `gcc`, as well as on MacOS using `clang`.

## Usage
The command line interface of the Nelder-Mead binary accepts a sequence of 10+ positional arguments 
```bash
$ ./binary precision format verbose tol_x tol_y max_iter max_eval adaptive scale coord_1 ... coord_n
```
where `coord_i` is the i-th coordinate of the n-dimensional initial point used to start the optimization. A `point` must be at least 1-dimensional, that is `n >= 1`. All the other optimization settings are designed to resemble [Matlab optimset](https://www.mathworks.com/help/matlab/ref/optimset.html).

| Position | Parameter | Type | Valid Values | Comment |
| - | - | - | - | - |
| 1 | `precision` | `int` | (3..36) | Significant figures to display |
| 2 | `format` | `bool` | [0,1] | Number format to display: 0 = fixed point, 1 = exponential |
| 3 | `verbose` | `bool` | [0,1] | Verbose mode: 0 = off, 1 = on |
| 4 | `tol_x` | `float` | (1e-36..1e-3) | Termination tolerance on simplex coordinates |
| 5 | `tol_y` | `float` | (1e-36..1e-3) | Termination tolerance on function value |
| 6 | `max_iter` | `int` | (1..100000) | Maximum number of iterations |
| 7 | `max_eval` | `int` | (1..100000) | Maximum number of function evaluations |
| 8 | `adaptive` | `bool` | [0,1] | [Adaptive Nelder-Mead parameters](https://link.springer.com/article/10.1007/s10589-010-9329-3): 0 = off, 1 = on |
| 9 | `scale` | `float` | (1e-12..1e3) | [Scaling factor of initial simplex](https://link.springer.com/article/10.1007/s11590-022-01953-y) |
| 10+ | `coord_i` | `float` | - | Space-separated coordinates of the initial point |

Optimization will stop when both of these conditions are satisfied:
 * distance between coordinates of best and worst point in the simplex is lower than `tol_x`;
 * difference between cost function value at best and worst point in the simplex is lower than `tol_y`;

Additionally, optimization will stop when either of these conditions are satisfied:
 * current number of iterations is larger than `max_iter`;
 * current number of evaluations is larger than `max_eval`;

For example, this is a valid call of the binary to optimize the Ackley function:
```bash
$ ./bin/nm-ackley 9 0 0 1e-9 1e-9 5000 5000 0 1.0 -2.10 -3.04 4.50
argc 13, argv [ ./bin/nm-ackley 9 0 0 1e-9 1e-9 5000 5000 0 1.0 -2.10 -3.04 4.50 ]
        Input [ -2.100000000 -3.040000000  4.500000000 ]  11.211987305
       Output [ -0.000000000 -0.000000000  0.000000000 ]  0.000000001
  Tolerance-x 0.000000001
  Tolerance-y 0.000000001
   Iterations 123
  Evaluations 237
```
Note that the program will display the coordinate and cost function value of both input and output points. The output point is the minimizer found by the algorithm. Additionally all values used to compute termination conditions are also shown, including the final tolerance on `x` and `y`, as well as the iteration and function evaluation counts.

We also provide a simplified interface to interact with the binary via the [`run.sh`](run.sh) shell script. Effort has been made to ensure POSIX compatibility of the script. Complete usage instructions can be seen by by executing `./run.sh -h`. In a nutshell, all optimization settings are optional and only two arguments are required:
 * `-b` the binary name of the cost function to minimize;
 * `-p` the initial point for the optimization given as a comma-separated list of floating point values (i.e., the coordinates of the point).

Here are some examples:
 * Optimize Ackley function with all default parameters (this is equivalent to the example above):
   ```bash
   $ ./run.sh -b nm-ackley -p -2.10,-3.04,4.50
   ```
 * Limit the number of iterations to `10`:
   ```bash
   $ ./run.sh -b nm-ackley -p -2.10,-3.04,4.50 -i 10
   ```
 * Enable verbose mode:
   ```bash
   $ ./run.sh -b nm-ackley -p -2.10,-3.04,4.50 -v 1
   ```

When verbose mode is enabled (`-v 1`), the following output will be generated:
```bash
$ ./run.sh -b nm-ackley -p -2.10,-3.04,4.50 -v 1
argc 13, argv [ ./bin/nm-ackley 9 0 1 1e-9 1e-9 5000 5000 0 1.0 -2.10 -3.04 4.50 ]
 iter  eval  operation     tol_x        tol_y        coordinates                                 value       
    1     5  expand        2.828427125  4.943346793  [ -1.100000000 -1.625786438  2.050510257 ]  6.886132039 
    2     7  reflect       2.828427125  4.575352355  [ -1.100000000 -1.625786438  2.050510257 ]  6.886132039 
    3     8  reflect       2.828427125  3.988386500  [ -1.100000000 -1.625786438  2.050510257 ]  6.886132039 
    4     9  reflect       3.187194313  5.532257925  [  0.282716049  0.172534512  0.871126307 ]  3.384738018 
    5    11  reflect       2.364962968  4.510011288  [  0.282716049  0.172534512  0.871126307 ]  3.384738018 
    6    12  reflect       2.556718290  3.501394021  [  0.282716049  0.172534512  0.871126307 ]  3.384738018 
    7    13  contract_out  1.830851818  0.970952465  [  0.282716049  0.172534512  0.871126307 ]  3.384738018 
    8    15  contract_in   1.151545708  1.340949404  [  0.267169639  0.016692963 -0.435940236 ]  2.876504207 
    9    17  contract_in   1.035004043  0.815016512  [  0.267169639  0.016692963 -0.435940236 ]  2.876504207 
   10    19  contract_in   1.218398115  1.416047086  [ -0.258334603 -0.063347372 -0.194762005 ]  1.968690932 
   11    21  contract_in   0.648065875  0.962345027  [ -0.258334603 -0.063347372 -0.194762005 ]  1.968690932 
   12    23  reflect       0.583719199  0.907813276  [ -0.258334603 -0.063347372 -0.194762005 ]  1.968690932 
   13    24  contract_in   0.455579538  1.153756473  [  0.107594305  0.076588163 -0.216149600 ]  1.424936702 
   14    26  contract_out  0.404802287  0.802674609  [  0.048929867 -0.237382477  0.004745503 ]  1.301830586 
   15    28  contract_in   0.435007423  1.447537999  [  0.070779482 -0.041159018  0.088827581 ]  0.521152933 
   16    30  contract_in   0.328984514  0.903783769  [  0.070779482 -0.041159018  0.088827581 ]  0.521152933 
  ...
  108   207  reflect       0.000000014  0.000000007  [ -0.000000000  0.000000001 -0.000000007 ]  0.000000016 
  109   208  contract_in   0.000000008  0.000000012  [ -0.000000003 -0.000000001  0.000000001 ]  0.000000008 
  110   210  contract_in   0.000000011  0.000000010  [ -0.000000003 -0.000000001  0.000000001 ]  0.000000008 
  111   212  contract_in   0.000000008  0.000000008  [ -0.000000003 -0.000000001  0.000000001 ]  0.000000008 
  112   214  contract_in   0.000000006  0.000000003  [ -0.000000003 -0.000000001  0.000000001 ]  0.000000008 
  113   216  reflect       0.000000005  0.000000001  [  0.000000001 -0.000000002 -0.000000002 ]  0.000000007 
  114   218  contract_in   0.000000003  0.000000005  [  0.000000001  0.000000001 -0.000000001 ]  0.000000004 
  115   220  contract_in   0.000000005  0.000000004  [  0.000000001  0.000000001 -0.000000001 ]  0.000000004 
  116   222  contract_in   0.000000003  0.000000004  [ -0.000000001 -0.000000001 -0.000000000 ]  0.000000003 
  117   224  contract_out  0.000000002  0.000000001  [ -0.000000001 -0.000000001 -0.000000000 ]  0.000000003 
  118   226  reflect       0.000000001  0.000000001  [ -0.000000000  0.000000001  0.000000001 ]  0.000000003 
  119   228  reflect       0.000000002  0.000000002  [  0.000000000 -0.000000001  0.000000000 ]  0.000000002 
  120   230  contract_in   0.000000002  0.000000002  [  0.000000000  0.000000000 -0.000000000 ]  0.000000001 
  121   232  contract_in   0.000000001  0.000000002  [  0.000000000  0.000000000 -0.000000000 ]  0.000000001 
  122   234  contract_in   0.000000001  0.000000001  [  0.000000000  0.000000000 -0.000000000 ]  0.000000001 
  123   236  contract_in   0.000000001  0.000000001  [ -0.000000000 -0.000000000  0.000000000 ]  0.000000001 
        Input [ -2.100000000 -3.040000000  4.500000000 ]  11.211987305 
       Output [ -0.000000000 -0.000000000  0.000000000 ]  0.000000001 
  Tolerance-x 0.000000001 
  Tolerance-y 0.000000001 
   Iterations 123
  Evaluations 237
```
where, from left to right, each row contains:
 * iteration number;
 * number of function evaluations;
 * operation on the simplex executed by the algorithm;
 * current diameter (Euclidean distance between coordinates of best and worst point in the simplex);
 * coordinates (`x`) of current best point, i.e. the current minimizer;
 * value (`y`) of current best point, i.e. the current minimum.

## Licence
MIT Licence. Copyright (c) 2017-2024 [Matteo Maggioni](https://github.com/matteotiziano), [Ian Smith](https://github.com/m4r35n357)
