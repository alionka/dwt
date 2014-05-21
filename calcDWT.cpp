#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calcDWT.h"

float __inline **malloc2D(int width, int high) {
  float **matrix = (float **) malloc(high * sizeof(float *));
  for (int i = 0; i < high; i++)
    matrix[i] = (float *) malloc(width * sizeof(float));
  return matrix;
}

void __inline free2D(int width, int high, float **matrix) {
  for (int i = 0; i < high; i++)
    free(matrix[i]);
  free(matrix);
}

void traspose_matrix(int width, int high, float **matrix, float **matrixT) {
  for (int y = 0; y < high; y++)
    for (int x = 0; x < width; x++)
      matrixT[x][y] = matrix[y][x];
}

float oneD(int x, int y, int width, float *comp, bool even){

	float pixel = 0.0;
	float p[2];

	if(even) {
		pixel = comp[x] + fpb[0];
		for(int i = 0; i < 5; i++) {
			if(x - i < 0) 
				p[0] = comp[i - x];
			else 
				p[0] = comp[x - i];

			if (x + i >= width)
				p[1] = comp[width - ( i - ( width - x )) - 2];
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
          p[1] = comp[width - (i - (width - x)) -2];
        else
          p[1] = comp[x + i];
        pixel += (p[0] * fpa[i]) + (p[1] * fpa[i]);
	   }
  }
	return pixel;
}

float oneDinv(int x, int y, int width, float *comp, bool even){
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
        pixel += (p[0] * fpb_i[i]) + (p[1] * fpb_i[i]);
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
        pixel += (p[0] * fpa_i[i]) + (p[1] * fpa_i[i]);
      }
    }
    return pixel;
}

//fin oneD

// Hola 2D

void oneD_rows(int width, int high, float **Y420, float **Cb420, float **Cr420) {
  float **newY420 = malloc2D(width, high);
  float **newCb420 = malloc2D(width/2, high/2);
  float **newCr420 = malloc2D(width/2, high/2);

  // llamada a aplica_kernel por cada elemento
  for (int y = 0; y < high; y++) {
    for (int x = 0; x < width; x++) {
      newY420[y][x] = oneD(x, y, width, Y420[y], (x % 2) == 0);
      if (y < (high/2) && x < (width/2)) {
        newCb420[y][x] = oneD(x, y, width/2, Cb420[y], (x % 2) == 0);
        newCr420[y][x] = oneD(x, y, width/2, Cr420[y], (x % 2) == 0);
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
  free2D(width, high, newY420);
  free2D(width/2, high/2, newCb420);
  free2D(width/2, high/2, newCr420);
}

void oneD_rows_inv(int width, int high, float **Y420, float **Cb420, float **Cr420) {
  float **newY420 = malloc2D(width, high);
  float **newCb420 = malloc2D(width/2, high/2);
  float **newCr420 = malloc2D(width/2, high/2);
  
  // reorganizar el orden de los elementos
  for (int y = 0; y < high; y++) {
    for (int x = 0; x < width; x++) {
      if (x % 2 == 0) { // es par => resultado del paso bajo
        newY420[y][x] = Y420[y][x/2];
        if (y < (high/2) && x < (width/2)) {
          newCb420[y][x] = Cb420[y][x/2];
          newCr420[y][x] = Cr420[y][x/2];
        }
      }
      else { // es impar => resultado del paso high
        newY420[y][x] = Y420[y][x/2 + width/2];
        if (y < (high/2) && x < (width/2)) {
          int width_2 = width/2;
          newCb420[y][x] = Cb420[y][x/2 + width_2/2];
          newCr420[y][x] = Cr420[y][x/2 + width_2/2];
        }
      }
    }
  }
  
  // llamada a aplica_kernel (inverso) por cada elemento
  for (int y = 0; y < high; y++) {
    for (int x = 0; x < width; x++) {
      Y420[y][x] = oneDinv(x, y, width, newY420[y], (x % 2) == 0);
      if (y < (high/2) && x < (width/2)) {
        Cb420[y][x] = oneDinv(x, y, width/2, newCb420[y], (x % 2) == 0);
        Cr420[y][x] = oneDinv(x, y, width/2, newCr420[y], (x % 2) == 0);
      }
    }
  }
  free2D(width, high, newY420);
  free2D(width/2, high/2, newCb420);
  free2D(width/2, high/2, newCr420);
}

void oneD_columns(int width, int high, float **Y420, float **Cb420, float **Cr420) {
  float **newY420 = malloc2D(high, width);
  float **newCb420 = malloc2D(high/2, width/2);
  float **newCr420 = malloc2D(high/2, width/2);

  // trasponer matrices
  traspose_matrix(width, high, Y420, newY420);
  traspose_matrix(width/2, high/2, Cb420, newCb420);
  traspose_matrix(width/2, high/2, Cr420, newCr420);

  oneD_rows(high, width, newY420, newCb420, newCr420);

  // retrasponer matrices
  traspose_matrix(high, width, newY420, Y420);
  traspose_matrix(high/2, width/2, newCb420, Cb420);
  traspose_matrix(high/2, width/2, newCr420, Cr420);

  free2D(high, width, newY420);
  free2D(high/2, width/2, newCb420);
  free2D(high/2, width/2, newCr420);
}

void oneD_Columns_inv(int width, int high, float **Y420, float **Cb420, float **Cr420) {
  float **newY420 = malloc2D(high, width);
  float **newCb420 = malloc2D(high/2, width/2);
  float **newCr420 = malloc2D(high/2, width/2);
  
  // trasponer matrices
  traspose_matrix(width, high, Y420, newY420);
  traspose_matrix(width/2, high/2, Cb420, newCb420);
  traspose_matrix(width/2, high/2, Cr420, newCr420);
  
  oneD_rows_inv(high, width, newY420, newCb420, newCr420);
  
  // retrasponer matrices
  traspose_matrix(high, width, newY420, Y420);
  traspose_matrix(high/2, width/2, newCb420, Cb420);
  traspose_matrix(high/2, width/2, newCr420, Cr420);
  
  free2D(high, width, newY420);
  free2D(high/2, width/2, newCb420);
  free2D(high/2, width/2, newCr420);
}


void YCbCr420_DWT(int ancho, int alto, float **Y420, float **Cb420, float **Cr420) {
  for (int nivel=0, an = ancho, al = alto; nivel < 6; nivel++, an /= 2, al /= 2) {
    oneD_rows(an, al, Y420, Cb420, Cr420);
    oneD_columns(an, al, Y420, Cb420, Cr420);
  }
}

void DWT_YCbCr420(int ancho, int alto, float **Y420, float **Cb420, float **Cr420) {
  for (int nivel=5, an = ancho/pow(2,nivel), al = alto/pow(2,nivel); nivel >= 0; nivel--, an *= 2, al *= 2) {
    oneD_rows_inv(an, al, Y420, Cb420, Cr420);
    oneD_Columns_inv(an, al, Y420, Cb420, Cr420);
  }
}
