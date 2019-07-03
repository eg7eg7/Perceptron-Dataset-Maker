// datasetMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#define FILE_NAME "dataset.txt"
#define version_num "2.1"

static int MAX_RANDOM_SIZE = 1000;

int getGroup(float* W_vec, float* point_vec,int dim) {
	float sum = W_vec[dim];
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
int isFloat(char* string, float* the_num) {
	float num;

	num = (float)atof(string);

	if (num == 0 && string[0] != '0')
		return 0;
	*the_num = num;
	return 1;
}
int getArgs(int argc, char *argv[], int* data_size, int* dim, float* alpha_zero, float* alpha_max, int* LIMIT, float* QC) {
	if (argc != 7)
		return 1;

	if (!isInteger(argv[1], data_size))
		return 1;
	if (!isInteger(argv[2], dim))
		return 1;
	if (!isInteger(argv[5], LIMIT))
		return 1;
	if (!isFloat(argv[3], alpha_zero))
		return 1;
	if (!isFloat(argv[4], alpha_max))
		return 1;
	if (!isFloat(argv[6], QC))
		return 1;
	return 0;
}
void createDataset(int data_size, int dim, float alpha_zero, float alpha_max, int LIMIT, float QC)
{
	
	int RANDOM_GROUP = 1;
	float* W_vec;
	float* point_vec;
	printf("dataset will be created with the following info: \n");
	printf("N = %d | K= %d | alpha_zero = %f | alpha_max = %f | LIMIT = %d | QC =%f | \n", data_size, dim, alpha_zero, alpha_max, LIMIT, QC);

	FILE* file_ptr = fopen(FILE_NAME, "w");
	if (file_ptr == NULL)
	{
		printf("Error opening file!\n");
		return;
	}

	W_vec = (float*)calloc(dim+1, sizeof(float));
	point_vec = (float*)calloc(dim, sizeof(float));
	char ans;
	printf("\nK is %d, do you want to assign random groups? (y/n)   ", dim);
	fflush(stdin);
	scanf(" %c", &ans);
	if ((ans == 'n' || ans == 'N'))
		RANDOM_GROUP = 0;
	else
		RANDOM_GROUP = 1;
	if (!RANDOM_GROUP)
	{
		fflush(stdin);
		
		for (int i = 0;i < dim; i++)
		{
			printf("\nW%d = ",i+1);
			scanf("%f", &W_vec[i]);
		}
		printf("\nW0 (b) = ");
		scanf("%f", &W_vec[dim]);
		
		printf("\nW vector is (");
		for (int i = 0; i < dim; i++)
		{
			printf(" W%d = %f", (i+1),W_vec[i]);
		}
		printf(" b = %f)\n", W_vec[dim]);
	}
	printf("\n\nCreating file...please wait");
	fprintf(file_ptr, "%d %d %f %f %d %f", data_size, dim, alpha_zero, alpha_max, LIMIT, QC);
	int group = 1;
	int rand_group;
	float x;
	float rand_max_div2 = (float)(RAND_MAX / 2);
	float division = rand_max_div2 / MAX_RANDOM_SIZE;
	for (int i = 0; i < data_size; i++)
	{
		fprintf(file_ptr, "\n");
		for (int k = 0; k < dim; k++)
		{
			x = (float)((rand() - rand_max_div2) / division);
			point_vec[k] = x;
			fprintf(file_ptr, "%.2f ", x);
		}

		if (RANDOM_GROUP)
		{
			rand_group = (int)(rand() - rand_max_div2);
			if (rand_group > 0)
				group = 1;
			else
				group = -1;
		}
		else
			group = getGroup(W_vec, point_vec,dim);
		fprintf(file_ptr, "%d", group);
	}

	fclose(file_ptr);
	free(W_vec);
	free(point_vec);
	printf("\nEnd. created file \"%s\"\n", FILE_NAME);
}
void getSettings(int* data_size, int* dim, float* alpha_zero, float* alpha_max, int* LIMIT, float* QC) {

	printf("\n\nSettings input - make sure to define valid types only (integers/floats)\n");
	printf("data size - N (integer): ");
	scanf("%d", data_size);
	printf("\ndimension - K (integer): ");
	scanf("%d", dim);
	printf("\nalpha_zero (float): ");
	scanf("%f", alpha_zero);
	printf("\nalpha_max (float): ");
	scanf("%f", alpha_max);
	printf("\nLIMIT (integer): ");
	scanf("%d", LIMIT);
	printf("\nQC (float): ");
	scanf("%f", QC);


}
int main(int argc, char *argv[])
{
	int get_settings = 0;
	int data_size; //number of points
	int dim; //number of coordinates of points - dimension
	float alpha_zero; //initial and increment value of alpha
	float alpha_max; //max alpha value
	int LIMIT; //the maximum number of iterations
	float QC; //Quality of Classifier to be reached 
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

