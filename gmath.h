#ifndef GMATH_H
#define GMATH_H

#include "matrix.h"
#include "ml6.h"
#include "symtab.h"

#define AMBIENT_C 0
#define DIFFUSE 1
#define SPECULAR 2
#define LOCATION 0
#define COLOR 1
#define RED 0
#define GREEN 1
#define BLUE 2
#define SPECULAR_EXP 4

//lighting functions
color get_lighting( double *normal, char *constants, double *view);
int * calculate_ambient(SYMTAB * reflect);
int * calculate_diffuse(SYMTAB * reflect, double * normal );
int * calculate_specular(SYMTAB * reflect, double * normal, double * view);
int limit_color( int x );

//vector functions
void normalize( double *vector );
double dot_product( double *a, double *b );
double *calculate_normal(struct matrix *polygons, int i);

#endif
