
#include "model.h"

#define BASE 10

/*
 * ANSI color codes
 * https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
 */
#define GRY "\x1B[1;30m"
#define RED "\x1B[1;31m"
#define GRN "\x1B[1;32m"
#define YLW "\x1B[1;33m"
#define BLU "\x1B[1;34m"
#define MGT "\x1B[1;35m"
#define CYN "\x1B[1;36m"
#define WHT "\x1B[1;37m"
#define NRM "\x1B[0m"
#define GRYBG "\x1B[0;100m"
#define REDBG "\x1B[0;101m"
#define GRNBG "\x1B[0;102m"
#define YLWBG "\x1B[0;103m"
#define BLUBG "\x1B[0;104m"
#define MGTBG "\x1B[0;105m"
#define CYNBG "\x1B[0;106m"
#define WHTBG "\x1B[0;107m"

/*
 * Report program arguments in colour
 */
#define PRINT_ARGS(argc, argv, color) do { \
    fprintf(stderr, "%sargc %s%d%s, argv [ %s", color, NRM, (argc), color, NRM); \
    for (int i = 0; i < (argc); i++) fprintf(stderr, "%s ", (argv)[i]); \
    fprintf(stderr, "%s]%s\n", color, NRM); \
} while (0)

/*
 * Unavoidable "assert", in colour
 */
#define ASSERT(x) do { \
    if(!(x)) { \
        fprintf(stderr, "%sFAIL %s%s %s%s%s %s%s:%s%i\n", RED, WHT, #x, YLW, __func__, NRM, __FILE__, YLW, NRM, __LINE__); \
        exit(1); \
    } \
} while (0)

/*
 * Optimizer settings
 */
typedef struct Optimset {
    int precision;  // significant figures in floats/exponentials
    int format;  // fixed or exponential floating point format
    int verbose;  // toggle verbose output during minimization
    real tol_x;  // tolerance on the simplex solutions coordinates
    real tol_y;  // tolerance on the function value
    unsigned int max_iter;  // maximum number of allowed iterations
    unsigned int max_eval;  // maximum number of allowed function evaluations
    int adaptive;  // simplex updates reduced for dimension > 2
    real scale;  // scaling factor of initial simplex
} optimset;

/*
 * The "simplex" containing an array of n + 1 points each of dimension n
 */
typedef struct Simplex {
    size_t n;
    unsigned int num_iter, num_eval;
    point *vertices;
    point *reflected, *expanded, *contracted, *centroid;
} simplex;

/*
 * "Simplex" or "Amoeba" optimizer
 */
void nelder_mead (const model *, const optimset *, simplex *, point *);

/*
 * Utility functions
 */
real distance (size_t, const point *, const point *);

int compare (const void *, const void *);

void sort (simplex *);

simplex *init_simplex (size_t, real, const point *);

void update_centroid (simplex *);

void update_simplex (real , const point *, const point *, const point *, 
    const model *, simplex *, point *);

real tolerance_x (const simplex *);

real tolerance_y (const simplex *);

int terminated (const simplex *, const optimset *);

point *init_point (size_t);

void copy_point (size_t, const point *, point *);

void print_point (size_t, const point *, int, int);

void print_value (real, int, int);

void print_verbose (int, const char *, ...);

void free_simplex (simplex*);

void free_point (point*);
