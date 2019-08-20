// datasetMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#define FILE_NAME "input.txt"
#define FILE_NAME_LOG "datasetMaker_log.txt"
#define version_num "3.0"
const int NUM_CIRCLES = 5;
struct Circle {
	double *center_vector;
	double radius;
	int group;
}typedef;
int MAX_RANDOM_SIZE = 1000;

int getGroup(double* W_vec, double* point_vec,int dim) {
	double sum = W_vec[dim];
	for (int i = 0; i < dim; i++) {
		sum += W_vec[i] * point_vec[i];
	}

	if (sum >= 0)
		return 1;
	else 
		return -1;
}
int isInteger(char* string, int* the_num) {
	int num;

	num = atoi(string);

	if (num == 0 && string[0] != '0')
		return 0;
	*the_num = num;
	return 1;
}
int isdouble(char* string, double* the_num) {
	double num;

	num = (double)atof(string);

	if (num == 0 && string[0] != '0')
		return 0;
	*the_num = num;
	return 1;
}
int getArgs(int argc, char *argv[], int* data_size, int* dim, double* alpha_zero, double* alpha_max, int* LIMIT, double* QC) {
	if (argc != 7)
		return 1;

	if (!isInteger(argv[1], data_size))
		return 1;
	if (!isInteger(argv[2], dim))
		return 1;
	if (!isInteger(argv[5], LIMIT))
		return 1;
	if (!isdouble(argv[3], alpha_zero))
		return 1;
	if (!isdouble(argv[4], alpha_max))
		return 1;
	if (!isdouble(argv[6], QC))
		return 1;
	return 0;
}
void createDataset(int data_size, int dim, double alpha_zero, double alpha_max, int LIMIT, double QC)
{
	
	int RANDOM_GROUP = 1;
	double* W_vec;
	double* point_vec;
	printf("dataset will be created with the following info: \n");
	printf("N = %d | K= %d | alpha_zero = %f | alpha_max = %f | LIMIT = %d | QC =%f | \n", data_size, dim, alpha_zero, alpha_max, LIMIT, QC);

	FILE* file_ptr = fopen(FILE_NAME, "w");
	if (file_ptr == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	FILE* file_ptr_log = fopen(FILE_NAME_LOG, "w");
	if (file_ptr_log == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	W_vec = (double*)calloc(dim+1, sizeof(double));
	point_vec = (double*)calloc(dim, sizeof(double));
	char ans;
	printf("\nK is %d, do you want to create random values or create groups?\n\n Press R for random\nPress G for groups)  ", dim);
	fflush(stdin);
	scanf(" %c", &ans);
	if ((ans == 'r' || ans == 'R'))
		RANDOM_GROUP = 1;
	else
		RANDOM_GROUP = 0;

		fflush(stdin);
		if (RANDOM_GROUP == 1)
		{

		
		printf("\nChoose the W vector for the data to be based on.\n");
		for (int i = 0;i < dim; i++)
		{
			printf("\nW%d = ",i+1);
			scanf("%lf", &W_vec[i]);
		}
		printf("\nW0 (b) = ");
		scanf("%lf", &W_vec[dim]);
		
		printf("\nW vector is (");
		for (int i = 0; i < dim; i++)
		{
			printf(" W%d = %f", (i+1),W_vec[i]);
		}
		printf(" b = %f)\n", W_vec[dim]);
		}
	fprintf(file_ptr_log,"dataset created with datasetMaker version %s by Eden Dupont\n\nMax absolute value : %d\n",version_num,MAX_RANDOM_SIZE);
	if (RANDOM_GROUP == 1)
		fprintf(file_ptr_log, "data points assigned with random values.\n");
	else
		fprintf(file_ptr_log, "data points classified with groups W:\n");
	for (int i = 0; i <= dim; i++)
	{
		fprintf(file_ptr_log, "W[%d] = %f\n", i,W_vec[i]);
	}
	printf("\n\nCreating file...please wait");
	fprintf(file_ptr, "%d %d %f %f %d %f", data_size, dim, alpha_zero, alpha_max, LIMIT, QC);
	int group = 1;
	double x;
	double rand_max_div2 = (double)(RAND_MAX / 2);
	double division = rand_max_div2 / MAX_RANDOM_SIZE;
	if(RANDOM_GROUP == 1)
	for (int i = 0; i < data_size; i++)
	{
		fprintf(file_ptr, "\n");
		for (int k = 0; k < dim; k++)
		{
			x = (double)((rand() - rand_max_div2) / division);
			point_vec[k] = x;
			fprintf(file_ptr, "%.2f ", x);
		}
	group = getGroup(W_vec, point_vec,dim);
		fprintf(file_ptr, "%d", group);
	}
	else
	{
		Circle circles[NUM_CIRCLES];
		for (int i = 0; i < NUM_CIRCLES; i++)
		{
			circles[i].center_vector = (double*)malloc(sizeof(double)*dim);
			circles[i].radius = (i + 1) * 30;
			circles[i].group = ((i % 2)*2)-1;
			for (int j = 0; j < dim; j++)
			{
				circles[i].center_vector[j] = (double)((rand() - rand_max_div2) / (rand_max_div2 / (MAX_RANDOM_SIZE - 50)));
				if (circles[i].center_vector[j] < 0)
					circles[i].center_vector[j] = -circles[i].center_vector[j];
				if (circles[i].center_vector[j] < circles[i].radius)
					circles[i].center_vector[j] += circles[i].radius;
			}
			
		}
		Circle *theCircle;
		for (int i = 0; i < data_size; i++)
		{
			fprintf(file_ptr, "\n");
			theCircle = &circles[i%NUM_CIRCLES];
			for (int k = 0; k < dim; k++)
			{
				x = theCircle->center_vector[k] + (double)((rand() - rand_max_div2) / (rand_max_div2 / theCircle->radius));
				point_vec[k] = x;
				fprintf(file_ptr, "%.2f ", x);
			}
			group = theCircle->group;
			fprintf(file_ptr, "%d", group);
		}





		for (int i = 0; i < NUM_CIRCLES; i++)
		{
			free(circles[i].center_vector);
		}
	}
	fclose(file_ptr_log);
	fclose(file_ptr);
	free(W_vec);
	free(point_vec);
	printf("\nEnd. created file \"%s\"\n", FILE_NAME);
}
void getSettings(int* data_size, int* dim, double* alpha_zero, double* alpha_max, int* LIMIT, double* QC) {

	printf("\n\nSettings input - make sure to define valid types only (integers/doubles)\n");
	printf("data size - N (integer): ");
	scanf("%d", data_size);
	printf("\ndimension - K (integer): ");
	scanf("%d", dim);
	printf("\nalpha_zero (double): ");
	scanf("%lf", alpha_zero);
	printf("\nalpha_max (double): ");
	scanf("%lf", alpha_max);
	printf("\nLIMIT (integer): ");
	scanf("%d", LIMIT);
	printf("\nQC (double): ");
	scanf("%lf", QC);


}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	int get_settings = 0;
	int data_size; //number of points
	int dim; //number of coordinates of points - dimension
	double alpha_zero; //initial and increment value of alpha
	double alpha_max; //max alpha value
	int LIMIT; //the maximum number of iterations
	double QC; //Quality of Classifier to be reached 
	printf("Perceptron Classifier dataset maker\n\nmade by Eden Dupont - 2019 version %s\n\n\n", version_num);
	printf("Settings definitions :\nN - number of points\nK - number of coordinates of points\nalpha_zero - initial value and increment value of alpha\nalpha_max - max alpha value\nLIMIT - the maximum number of iterations\nQC - Quality of Classifier to be reached\n");
	printf("\nW vector is [W1,W2,W3,W4.....,b (W0] - W1 corresponds to X coefficient, W2 to Y, W3 to Z and etc.\n\n");
	printf( "\n\nYou can choose between two types of outputs:\n1. Choose the desired W vector to which the final result of the perceptron should tend to.\nFor example, in 2D space W=(1,2,3), plug to y=mx+b ==> 2=m+3 ==> m=3 ==> the line that cuts between the points in the dataset should be y=3x+3\n\n\n2. Random grouping between 1 and -1\n" );
	get_settings = getArgs(argc, argv, &data_size, &dim, &alpha_zero, &alpha_max, &LIMIT, &QC);
	if (get_settings)
		getSettings(&data_size, &dim, &alpha_zero, &alpha_max, &LIMIT, &QC);

	createDataset(data_size, dim, alpha_zero, alpha_max, LIMIT, QC);


	return 0;
}

