#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "ImagenES.h"
#include "CalcDWT.h"

#define FORMAT_420 0
#define FORMAT_DWT 1


#define MAX_MATCHES 1

#define _420_DWT 0
#define DWT_420 1


static int width = 512;
static int high = 512;
static *char input_file = "lena.420";
static *char output_file = "lena.dwt";
static int option = _420_DWT;

int get_format (char *name){
	regex_t format_420;
	regex_t format_dwt;
	int ret420,ret420i,retdwt,retdwti;

	/* Compile the regular expression */
	ret420 = regcomp(&format_420,"*.420",0);
	retdwt = regcomp(&format_dwt,"*.dwt",0);

	if (ret420 || retdwt) printf("Something goes wrong with compiling formats\n"); 
	if(regexec(format_420,name,MAX_MATCHES,0)){
		regfree($format_420);
		regfree($format_dwt);
		return FORMAT_420;
	} 
	if(regexec(format_dwt,name,MAX_MATCHES,0)) {
		regfree($format_420);
		regfree($format_dwt);
		return FORMAT_DWT;
	}
	
	return -1;
}

void get_args (int argc, char **argv) {

	int input_format,output_format;
	if(argc!=5) {
		printf("Usage:\ndwt input_file output_file width high\n");
	}

	width = atoi(argv[3]);
	high = atoi(argv[4]);

	input_file = argv[1];
	output_file = argv[2];

	input_format = get_format(input_file);
	output_format = get_format(output_format);

	if(input_format == FORMAT_420 && output_format == FORMAT_DWT) option = _420_DWT;
	if(input_format == FORMAT_DWT && output_format == FORMAT_420) option = DWT_420;
	
}

int int main(int argc, char const *argv[])
{
	float **Y420, **Cb420, **Cr420;

	get_args(argc,argv);

  	ReservaPlano(width, high, &Y420);
    ReservaPlano(width/2, high/2, &Cb420);
    ReservaPlano(width/2, high/2, &Cr420);

    LiberaPlano(width, high, &Y420);
    LiberaPlano(width/2, high/2, &Cb420);
    LiberaPlano(width/2, high/2, &Cr420);
	return 0;
}