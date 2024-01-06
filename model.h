typedef long double real;
typedef enum { false, true } bool;

/*
 * "Low-noise" squaring for arguments with no side-effects
 */
#define SQR(x) ((x) * (x))

/*
 * Function definition
 */
typedef struct Model model;

/*
 * Point definition 
 *   x: n-dimensional array with point coordinates
 *   y: value of a function f applied to the coordinates x, y = f(x)
 */
typedef struct Point {
    real *x;
    real  y;
} point;

/*
 * Initialize function
 */
model *init_model (void);

/*
 * Return expected number of dimensions
 */
size_t dimensions (void);

/*
 * Cost function implementation
 *   model: model to optimize
 *   point: point where to evaluate the function
 */
void cost (const model *, point *);
