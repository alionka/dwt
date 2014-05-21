#ifndef CALCDWT_H
#define CALCDWT_H


const float fpb[5] = { 0.602949018236360, 0.266864118442875, -0.078223266528990, -0.016864118442875, 0.026748757410810 };
const float fpa[4] = { 0.057543526228500, -0.295635881557125, -0.028771763114250, 0.045635881557125 };
//Inverse 
const float fpb_i[4] = { 0.057543526228500, -0.295635881557125, -0.028771763114250, 0.045635881557125 };
const float fpa_i[5] = { 0.602949018236360, -0.266864118442875, -0.078223266528990, 0.016864118442875, 0.026748757410810 };

void traspose_matrix(int width, int high, float **matrix, float **matrixT);
float oneD(int x, int y, int width, float *comp, bool even);
float oneDinv(int x, int y, int width, float *comp, bool even);
void oneD_rows(int width, int high, float **Y420, float **Cb420, float **Cr420);
void oneD_rows_inv(int width, int high, float **Y420, float **Cb420, float **Cr420);
void oneD_columns(int width, int high, float **Y420, float **Cb420, float **Cr420);
void oneD_Columns_inv(int width, int high, float **Y420, float **Cb420, float **Cr420);
void YCbCr420_DWT(int ancho, int alto, float **Y420, float **Cb420, float **Cr420);
void DWT_YCbCr420(int ancho, int alto, float **Y420, float **Cb420, float **Cr420);

#endif