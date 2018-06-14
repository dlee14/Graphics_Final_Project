#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
//contsants can simply be chars (small space needed)
color get_lighting( double *normal, char *constants, double * view ) {
  //initializations
  color i;
  //jk cant do dis part
  //int * a, d, s;

  normalize(normal);
  normalize(view);

  int loop = 0;
  if(constants != NULL) {loop = 1;}

  //if entries exist
  if(loop){
    SYMTAB * reflect = lookup_symbol(constants);
    int * a = calculate_ambient(reflect);
    int * d = calculate_diffuse(reflect, normal);
    int * s = calculate_specular(reflect, normal, view);
    i.red = a[RED] + d[RED] + s[RED];
    i.green = a[GREEN] + d[GREEN] + s[GREEN];
    i.blue = a[BLUE] + d[BLUE] + s[BLUE];
    limit_color(i.red);
    limit_color(i.green);
    limit_color(i.blue);
  }

  //else, set up base light, which is the only thing needed
  else{
    extern double alight[3];
    i.red = alight[RED];
    i.green = alight[GREEN];
    i.blue = alight[BLUE];
  }
  return i;
}

int * calculate_ambient(SYMTAB * reflect) {
  extern double alight[3];
  int * al = malloc(3 * sizeof(double));
  //fetching entries from symtab!
  al[RED] = alight[RED] * reflect->s.c->r[0];
  al[GREEN] = alight[GREEN] * reflect->s.c->g[0];
  al[BLUE] = alight[BLUE] * reflect->s.c->b[0];
  return al;
  /**
  a.red = ambient.red * constants[RED][0];
  a.green = ambient.green * constants[GREEN][0];
  a.blue = ambient.blue * constants[BLUE][0];
  return a;
  **/
}

int * calculate_diffuse(SYMTAB * reflect, double * normal) {
  //acessing setup values
  extern int num_lights;
  extern char light_names[100][100];

  int i;

  int * dl = malloc(3 * sizeof(double));
  double dot;

  for(i = 0; i < num_lights; i++){
    SYMTAB * li = lookup_symbol(light_names[i]);
    normalize(li->s.l->l);
    dot = dot_product(normal, li->s.l->l);
    dl[RED] = dl[RED] + li->s.l->c[0] * reflect->s.c->r[1] * dot;
    dl[GREEN] = dl[GREEN] + li->s.l->c[1] * reflect->s.c->g[1] * dot;
    dl[BLUE] = dl[BLUE] + li->s.l->c[2] * reflect->s.c->b[1] * dot;
  }

  return dl;
}

int * calculate_specular(SYMTAB * reflect, double * normal, double * view) {
  //acessing setup values yet again
  extern int num_lights;
  extern char light_names[100][100];

  int * sl = malloc(3 * sizeof(double));
  double n[3];
  double dot;
  int i;

  for(i = 0; i < num_lights; i++){
    SYMTAB * li = lookup_symbol(light_names[i]);

    normalize(li->s.l->l);
    dot = 2 * dot_product(normal, li->s.l->l);

    n[0] = (normal[0] * dot) - li->s.l->l[0];
    n[1] = (normal[1] * dot) - li->s.l->l[1];
    n[2] = (normal[2] * dot) - li->s.l->l[2];

    //checking
    dot = dot_product(n, view);
    if(dot > 0){
      pow(dot, SPECULAR_EXP);
    }
    else{
      dot = 0;
    }

    sl[RED] = sl[RED] + li->s.l->c[RED] * reflect->s.c->r[2] * dot;
    sl[GREEN] = sl[GREEN] + li->s.l->c[GREEN] * reflect->s.c->g[2] * dot;
    sl[BLUE] = sl[BLUE] + li->s.l->c[BLUE] * reflect->s.c->b[2] * dot;

  }

  return sl;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red = c->red > 255 ? 255 : c->red;
  c->green = c->green > 255 ? 255 : c->green;
  c->blue = c->blue > 255 ? 255 : c->blue;

  c->red = c->red < 0 ? 0 : c->red;
  c->green = c->green < 0 ? 0 : c->green;
  c->blue = c->blue < 0 ? 0 : c->blue;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
