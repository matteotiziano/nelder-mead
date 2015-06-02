
mex nelder_mead_mex.c nelder_mead.c cost_fun.c utils.c -DMATLAB_MEX_FILE

[x,fx] = nelder_mead_mex([-2.10 -3.04 4.50 ])

