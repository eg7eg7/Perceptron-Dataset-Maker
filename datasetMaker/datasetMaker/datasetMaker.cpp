// datasetMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#define FILE_NAME_OUTPUT "C://output//data.txt"

#define FILE_NAME_LOG "C://output//datasetMaker_log.txt"
#define version_num "4.0"

const int NUM_CIRCLES = 5;
struct Circle {
	double *center_vector;
	double radius;
	int group;
	double* speed;
}typedef;

int MAX_RANDOM_SIZE = 999;

int getGroup(double* W_vec, double* point_vec, int dim) {
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
int getArgs(int argc, char *argv[], int* data_size, int* dim, double* dT, double* Tmax, double* alpha, int* LIMIT, double* QC) {
	if (argc != 8)
		return 1;

	if (!isInteger(argv[1], data_size))
		return 1;
	if (!isInteger(argv[2], dim))
		return 1;
	if (!isdouble(argv[3], dT))
		return 1;
	if (!isdouble(argv[4], Tmax))
		return 1;
	if (!isdouble(argv[5], alpha))
		return 1;

	if (!isInteger(argv[6], LIMIT))
		return 1;


	if (!isdouble(argv[7], QC))
		return 1;
	return 0;
}
void createDataset(int data_size, int dim, double dT, double Tmax, double alpha, int LIMIT, double QC)
{
	double* W_vec;
	int num_circles_user = 0;
	printf("dataset will be created with the following info: \n");
	printf("N = %d | K= %d | dT = %f | Tmax = %f |alpha = %f | LIMIT = %d | QC =%f | \n", data_size, dim, dT, alpha, Tmax, LIMIT, QC);

	FILE* file_ptr = fopen(FILE_NAME_OUTPUT, "w");
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

	W_vec = (double*)calloc(dim + 1, sizeof(double));

	fflush(stdin);
	printf("\n Choose number of circles (integer) : ");
	scanf("%d", &num_circles_user);
	fprintf(file_ptr_log, "dataset created with datasetMaker version %s by Eden Dupont\n\nMax absolute value : %d\nNumber of circles : %d\n", version_num, MAX_RANDOM_SIZE, num_circles_user);

	printf("\n\nCreating file...please wait\n");
	fprintf(file_ptr, "%d %d %f %f %f %d %f", data_size, dim, dT, Tmax, alpha, LIMIT, QC);
	printf("Writing data line\n");
	int group = 1;
	double x, y;
	double rand_max_div2 = (double)((RAND_MAX) / 2);

	Circle* circles = (Circle*)malloc(num_circles_user * sizeof(Circle));
	for (int i = 0; i < num_circles_user; i++)
	{
		circles[i].center_vector = (double*)malloc(sizeof(double)*dim);
		circles[i].speed = (double*)malloc(sizeof(double)*dim);
		circles[i].radius = (i + 1) * 30;
		circles[i].group = ((i % 2) * 2) - 1;

		for (int j = 0; j < dim; j++)
		{
			circles[i].center_vector[j] = (double)((rand() - rand_max_div2) / (rand_max_div2 / (MAX_RANDOM_SIZE - 50)));
			circles[i].speed[j] = (double)((rand() - rand_max_div2) / (rand_max_div2 / (MAX_RANDOM_SIZE / 100 - 50)));
			if (circles[i].center_vector[j] < 0)
				circles[i].center_vector[j] = -circles[i].center_vector[j];
			if (circles[i].center_vector[j] < circles[i].radius)
				circles[i].center_vector[j] += circles[i].radius;
			if (circles[i].speed[j] < 0 && circles[i].group == 1)
				circles[i].speed[j] *= -1;
			if (circles[i].speed[j] >= 0 && circles[i].group == -1)
				circles[i].speed[j] *= -1;
		}

	}
	printf("Created Circles\n");
	Circle *theCircle;
	double min;
	double max;
	double num;
	double scaled;
	int index;
	for (int i = 0; i < data_size; i++)
	{
		fprintf(file_ptr, "\n");
		index = i % num_circles_user;
		theCircle = &circles[index];
		if (dim != 2)
		{
			for (int k = 0; k < dim; k++)
			{
				x = theCircle->center_vector[k] + (double)((rand() - rand_max_div2) / (rand_max_div2 / theCircle->radius));
				fprintf(file_ptr, "%.2f ", x);
			}
		}
		else // dim ==2
		{
			x = theCircle->center_vector[0] + ((((double) rand())/ RAND_MAX)-0.5)*2*theCircle->radius;
			num = sqrt((theCircle->radius)*(theCircle->radius) - (x - theCircle->center_vector[0])*(x - theCircle->center_vector[0]));
			if (isnan(num))
			{
				printf("num is nan\n");
				break;
			}
			max = theCircle->center_vector[1] + num;
			min = theCircle->center_vector[1] - num;
			scaled = ((double)rand()) / (double) RAND_MAX;
			y = 2*num*scaled + min;
			
			fprintf(file_ptr, "%.2f ", x);
			fprintf(file_ptr, "%.2f ", y);

		}
		for (int k = 0; k < dim; k++)
		{
			fprintf(file_ptr, "%.2f ", theCircle->speed[k]);
		}
		group = theCircle->group;
		fprintf(file_ptr, "%d", group);
	}
	printf("Finished writing to file.. now free\n");

	for (int i = 0; i < num_circles_user; i++)
	{
		printf("Free %d\n", i);
		free(circles[i].center_vector);
		free(circles[i].speed);
	}
	printf("Free log\n");
	fclose(file_ptr_log);
	printf("Free file\n");
	fclose(file_ptr);
	printf("Free W\n");
	free(W_vec);
	printf("\nEnd. created file \"%s\"\n", FILE_NAME_OUTPUT);
}
void getSettings(int* data_size, int* dim, double* dT, double* Tmax, double* alpha, int* LIMIT, double* QC) {

	printf("\n\nSettings input - make sure to define valid types only (integers/doubles)\n");
	printf("data size - N (integer): ");
	scanf("%d", data_size);
	printf("\ndimension - K (integer): ");
	scanf("%d", dim);
	printf("\ndT (double): ");
	scanf("%lf", dT);
	printf("\nTmax (double): ");
	scanf("%lf", Tmax);
	printf("\nalpha (double): ");
	scanf("%lf", alpha);
	printf("\nLIMIT (integer): ");
	scanf("%d", LIMIT);
	printf("\nQC (double): ");
	scanf("%lf", QC);


}
int main(int argc, char *argv[])
{
	srand(time_t(NULL));
	int get_settings = 0;
	int data_size; //number of points
	int dim; //number of coordinates of points - dimension
	double dT; //initial and increment value of alpha
	double Tmax; //max alpha value
	double alpha;
	int LIMIT; //the maximum number of iterations
	double QC; //Quality of Classifier to be reached 
	printf("Perceptron Classifier dataset maker - Summer Edition\n\nmade by Eden Dupont - 2019 version %s\n\n\n", version_num);
	printf("Settings definitions :\nN - number of points\nK - number of coordinates of points\ndT - initial t value and increment value of t\nTmax - max t value\nalpha - conversion ratio\nLIMIT - the maximum number of iterations\nQC - Quality of Classifier to be reached\n");
	printf("\nW vector is [W1,W2,W3,W4.....,b (W0] - W1 corresponds to X coefficient, W2 to Y, W3 to Z and etc. , followed by speed vector which corresponds to each W[i]\n\n");
	get_settings = getArgs(argc, argv, &data_size, &dim, &dT, &Tmax, &alpha, &LIMIT, &QC);
	if (get_settings)
		getSettings(&data_size, &dim, &dT, &Tmax, &alpha, &LIMIT, &QC);

	createDataset(data_size, dim, dT, Tmax, alpha, LIMIT, QC);


	return 0;
}

