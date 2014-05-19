#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CalculoDWT.h"

float __inline **malloc2D(int width, int high) {
  float **matrix = (float **) malloc(high * sizeof(float *));
  for (int i = 0; i < high; i++)
    matrix[i] = (float *) malloc(width * sizeof(float));
  return matrix;
}

void __inline free2D(int ancho, int high, float **matrix) {
  for (int i = 0; i < high; i++)
    free(matrix[i]);
  free(matrix);
}
void 1D(int x, int y, int width, float *comp, bool even){

	float pixel = 0.0;
	float p[2];

	if(even) {
		pixell = comp[x] + fpb[0];
		for(int i = 0; i < FPB; i++) {
			if(x - i < 0) 
				p[0] = comp[i -x];
			else 
				p[0] = comp[x - i];

			if (x + i >= width)
				p[1] = comp[width-(i-(width-x))-2];
			else
				p[1] = comp[x + i];
			pixel += (p[0] * fpb[i]) + (p[1] * fpb[i]);
		}
	} 
	else {
		pixel = comp[x] * fpa[0];
      for (int i = 1; i < 4; i++) {
        if (x - i < 0)
          p[0] = comp[i - x];
        else
          p[0] = comp[x - i];
        if (x + i >= width)
          p[1] = comp[width-(i-(width-x))-2];
        else
          p[1] = comp[x + i];
        pixel += (p[0] * fpa[i]) + (p[1] * fpa[i]);
	}
	return pixel;
}

void 1Dinv(int x, int y, int width, float *comp, bool even){
	float pixel = 0.0;
	float p[2];

	if(even) 
		pixel = fpb_i[0];
	else 
		pixel = fpa_i[0] * comp[x];
	
	bool odd = !even;
        
    for ( int i = 1; i < 5; i++) {
      if (odd && i < 4) {
        odd = false;
        if (x - i < 0)
          p[0] = comp[i - x];
        else
          p[0] = comp[x - i];
        if (x + i >= width)
          p[1] = comp [width - (i - (width - x)) - 2];
        else
          p[1] = comp[x + i];
        pixel += (p[0] * pbf_i[i]) + (p[1] * pbf_i[i]);
      } else if (!odd){
        odd = true;
        if (x - i < 0)
          p[0] = comp[i - x];
        else
          p[0] = comp[x - i];
        if (x + i >= width)
          p[1] = comp[width - (i - (width - x)) - 2];
        else
          p[1] = comp[x + i];
        pixel += (p[0] * paf_i[i]) + (p[1] * paf_i[i]);
      }
    }
}

void 1D_rows(int width, int high, float **Y420, float **Cb420, float **Cr420) {
  float **newY420 = malloc2D(width, high);
  float **newCb420 = malloc2D(width/2, high/2);
  float **newCr420 = malloc2D(width/2, high/2);

  // llamada a aplica_kernel por cada elemento
  for (int y = 0; y < high; y++) {
    for (int x = 0; x < width; x++) {
      newY420[y][x] = 1D(x, y, width, Y420[y], (x % 2) == 0);
      if (y < (high/2) && x < (width/2)) {
        newCb420[y][x] = 1D(x, y, width/2, Cb420[y], (x % 2) == 0);
        newCr420[y][x] = 1D(x, y, width/2, Cr420[y], (x % 2) == 0);
      }
    }
  }
  // guardar los resultados en el orden correcto
  for (int y = 0; y < high; y++) {
    for (int x = 0; x < width; x++) {
      if (x % 2 == 0) { // es par => resultado del paso bajo
        Y420[y][x/2] = newY420[y][x];
        if (y < (high/2) && x < (width/2)) {
          Cb420[y][x/2] = newCb420[y][x];
          Cr420[y][x/2] = newCr420[y][x];
        }
      }
      else { // es impar => resultado del paso high
        Y420[y][x/2 + width/2] = newY420[y][x];
        if (y < (high/2) && x < (width/2)) {
          int width_2 = width/2;
          Cb420[y][x/2 + width_2/2] = newCb420[y][x];
          Cr420[y][x/2 + width_2/2] = newCr420[y][x];
        }
      }
    }
  }
  free_2d_f(width, high, newY420);
  free_2d_f(width/2, high/2, newCb420);
  