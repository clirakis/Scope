/*
 *                            COPYRIGHT
 *
 *  Copyright (C) 2021 Exstrom Laboratories LLC
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  A copy of the GNU General Public License is available on the internet at:
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  or you can write to:
 *
 *  The Free Software Foundation, Inc.
 *  675 Mass Ave
 *  Cambridge, MA 02139, USA
 *
 *  Exstrom Laboratories LLC contact:
 *  stefan(AT)exstrom.com
 *
 *  Exstrom Laboratories LLC
 *  Longmont, CO 80503, USA
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Calculates component values for the nth order analog low pass Butterworth filter.
// Compile: gcc -lm -o alpbw alpbw.c

int main(int argc, char *argv[])
{
  int n, m, i, j, k, tb;
  int na, nb, nc, nu, nv;
  double x, a[3], r;
  double *b, *c, *d, *u, *v;

  if(argc < 4){
    printf("Usage: %s n t f\n", argv[0]);
    printf("  n = filter order\n");
    printf("  t = termination\n");
    printf("     0 = same source and load resistance\n");
    printf("     1 = either source or load resistance\n");
    printf("  f = 3db cutoff frequency\n");
    return(1);}
  
  n = (int)strtol(argv[1], NULL, 10);
  char t = argv[2][0];
  double w = strtod(argv[3],NULL);
  w *= 2.0*M_PI;

  if(t == '0'){
    for(i=0; i<n; ++i){
      x = 2.0*sin(M_PI*(double)(2*i+1)/(double)(2*n))/w;
      printf("%1.10lf\n", x);}
    return(0);}

  b = (double *)malloc((n+1) * sizeof(double));
  c = (double *)malloc((n+1) * sizeof(double));

  // initialize the Butterworth polynomial
  if(n % 2 == 1){
    b[0] = b[1] = 1.0;
    nb = 2;
    m = (n - 1)/2;}
  else{
    b[0] = 1.0;
    nb = 1;
    m = n/2;}

  // calculate the Butterworth polynomial
  for(k=0; k<m; ++k){
    a[0] = 1.0;
    a[1] = 2.0*sin(M_PI*(double)(2*k+1)/(double)(2*n));
    a[2] = 1.0;
    na = 3;
    nc = na + nb - 1;
    // perform the convolution: a[0,...,na-1] * b[0,...,nb-1]
    // producing the sequence c[0,...,nc-1] nc=na+nb-1
    for(i = 0; i < nb; ++i){
      c[i] = 0.0;
      for(j = i < na ? 0 : i - na + 1; j <= i; ++j)
        c[i] += a[i-j] * b[j];}
    for(i = nb; i < nc; ++i){
      c[i] = 0.0;
      for(j = i < na ? 0 : i - na + 1; j < nb; ++j)
        c[i] += a[i-j]*b[j];}
    d = b;
    b = c;
    c = d;
    nb = nc;}

  // print Butterworth polynomial coefficient values
  //  printf("\nPolynomial coefficient values\n");
  //  for(i=0; i<nb; ++i) printf("%1.6lf ", b[i]);
  //  printf("\n\n");

  // put odd coefficients in c and even coefficients in b
  for(i=1, nb=1, nc=0; i<=n; i+=2, ++nc){
    c[(i-1)/2] = b[i];
    if(i < n){
      b[(i+1)/2] = b[i+1];
      ++nb;}}

  // make u the larger of b and c
  // make v the smaller of b and c
  if(nb > nc){
    u = b;
    nu = nb;
    v = c;
    nv = nc;}
  else{
    u = c;
    nu = nc;
    v = b;
    nv = nb;}

  // calculate component values
  printf("Filter component values\n");
  tb=1;
  while(nu>0 && nv>0){
    if(tb == 1){
      r=u[nu-1]/v[nv-1];
      printf("%lf ",r/w);
      for(i=1; i<nv; ++i) u[nu-i-1]=u[nu-i-1]-r*v[nv-i-1];
      --nu;
      tb=0;}
    else{
      r=v[nv-1]/u[nu-1];
      printf("%lf ",r/w);
      for(i=1; i<nu; ++i) v[nv-i-1]=v[nv-i-1]-r*u[nu-i-1];
      --nv;
      tb=1;}
  }
  printf("\n\n");
  free(u);
  free(v);
  return(0);
}
