#!/bin/sh

display_value() {
    if [ -z $1 ]; then
        value="None"
    else
        value=$1
    fi
    printf "default = %-5s" "${value}"
}
FAIL="\e[1;31mFAIL\e[0m"
WARNING="\e[1;33mWARNING\e[0m"

root="./bin"

bin=""
point=""

precision=9
format=0
verbose=0
tol_x=1e-9
tol_f=1e-9
max_iter=5000
max_eval=5000
adaptive=0
scale=1.0

usage="
Usage:
$ $0 [-r PRECISION] [-t FORMAT] [-v VERBOSE] 
           [-x TOL_X] [-f TOL_F] [-i MAX_ITER] [-e MAX_EVAL]
           [-a ADAPTIVE] [-s SCALE]
           -p POINT -b BINARY

Arguments:
 -r  PRECISION  int    optional  $(display_value ${precision}) Significant figures to display
 -t  FORMAT     bool   optional  $(display_value ${format}) Number format: 0 = fixed point, 1 = exponential
 -v  VERBOSE    bool   optional  $(display_value ${verbose}) Verbose mode: 0 = off, 1 = on
 -x  TOL_X      float  optional  $(display_value ${tol_x}) Termination tolerance on the current point
 -f  TOL_F      float  optional  $(display_value ${tol_f}) Termination tolerance on the function value
 -i  MAX_ITER   int    optional  $(display_value ${max_iter}) Maximum number of iterations
 -e  MAX_EVAL   int    optional  $(display_value ${max_eval}) Maximum number of function evaluations
 -a  ADAPTIVE   bool   optional  $(display_value ${adaptive}) Adaptive Nelder-Mead parameters: 0 = off, 1 = on
 -s  SCALE      float  optional  $(display_value ${scale}) Scaling factor of initial simplex
 -p  POINT      str    required  $(display_value ${point}) Comma-separated list of coordinates of initial point
 -b  BINARY     str    required  $(display_value ${bin}) Binary name of the objective function to optimize

Examples:
  $0 -b nm-ackley -p -2.10,-3.04,4.50
"

user_defaults='./user-defaults'
[ -f $user_defaults ] && . $user_defaults

while getopts "r:t:v:x:f:i:e:a:s:p:b:h" opt; do
    case $opt in
        r) precision=${OPTARG} ;;
        t) format=${OPTARG} ;;
        v) verbose=${OPTARG} ;;
        x) tol_x=${OPTARG} ;;
        f) tol_f=${OPTARG} ;;
        i) max_iter=${OPTARG} ;;
        e) max_eval=${OPTARG} ;;
        a) adaptive=${OPTARG} ;;
        s) scale=${OPTARG} ;;
        p) point=${OPTARG} ;;
        b) bin="${OPTARG}" ;;
        h | *) echo "${usage}"; exit 0 ;;
    esac
done

if [ ! -d "${root}" ]; then
    echo "${FAIL} Binary directory '${root}' does not exist. Did you run 'make'?"
    exit 1
fi
files=$(ls ${root})
num_files="$(echo "${files}" | wc -l)"
if [ "${num_files}" -eq 0 ]; then
    echo "${FAIL} No available binary files in '${root}'. Did you run 'make'?"
    exit 1
fi

if [ -z "${point}" ]; then
    echo "${FAIL} Argument '-p' is required."
    exit 1
fi

if [ -z "${bin}" ] || [ ! -f "${root}/${bin}" ]; then
    echo "Invalid binary name '${bin}'. Select one option:"
    echo "${files}" | nl -w5 -s ") "
    echo "    q) quit"
    while true; do
        read -p "#? " opt
        case ${opt} in
            q) exit 0 ;;
            ''|*[!0-9]*) ;;
            *) ( [ "${opt}" -le 0 ] || [ "${opt}" -gt "${num_files}" ] ) || break ;;
        esac
        echo "${FAIL} Invalid option '${opt}'"
    done
    bin=$(echo "$files" | sed -n "${opt}p")
fi

# workaround, as getops cannot read multiple argument for the same option
# we pass the coordinates as numbers separated by commas and here we
# replace all commas with spaces and then use set to then conveniently
# read them again as command line arguments
IFS=','
set -- ${point}

${root}/${bin} \
    ${precision} ${format} ${verbose} \
    ${tol_x} ${tol_f} ${max_iter} ${max_eval} \
    ${adaptive} ${scale} \
    $@
