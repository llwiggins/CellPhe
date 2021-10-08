#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#define MINSEP 0

typedef struct sinfo{
    double *vars;
    char name[100];
    int group;
    char treatment[10];
    int numframes;
}SAMPLE;

typedef struct vinfo{
    int use;
    double sep;
    char name[100];
}VARINF;

int readLine(FILE *fp);
void choosevars(VARINF *vars, int nvars);
void sepcalc(SAMPLE *data, int nsamples1, int nsamples2, VARINF *vars, int nvars);

int main (int argc, char *argv[])
{
    int i, j, k, tmp, dtmp, nsamples, nvars, nsamples1, nsamples2;
    FILE *fp = NULL;
    char treatment[10];
    char inputfile1[100];
    char inputfile2[100];
    char outputfile[100];
    char name[20];
    float ftmp;

    printf("Please enter nvars,nsamples1,nsamples2:\n");
    scanf("%d,%d,%d", &nvars, &nsamples1, &nsamples2);
    /* if the number of arguments on the command line is not equal to 2, then print an error statement */
    if (argc != 3) printf("correct usage: ./cell inputgroup1_name inputgroup2_name\n");

    /* allocate space for data */
    nsamples = nsamples1 + nsamples2;
    SAMPLE *data = NULL;
    data = (SAMPLE *) malloc (nsamples * sizeof(SAMPLE));
    for (i = 0; i < nsamples; i++)
    {
      data[i].vars = (double *) malloc (nvars * sizeof(double));
    }    

    /* allocate space for variable information */
    VARINF *vars = NULL;
    vars = (VARINF *) malloc (nvars * sizeof(VARINF));
    for (j = 0; j < nvars; j++)
    {
	  vars[j].use = 0;
    }

    strcpy(inputfile1, argv[1]);
    strcpy(inputfile2, argv[2]);
    
 	fp = fopen(inputfile1, "r");
    printf("opened %s\n", inputfile1);
    tmp = fscanf(fp, "%s ", name);
	tmp = fscanf(fp, "%s ", name);
    tmp = fscanf(fp, "%s ", name);
    for (j = 0; j < nvars; j++)
    {
	  tmp = fscanf(fp, "%s ", name);
	  strcpy(vars[j].name, name);
    }
    for (i = 0; i < nsamples1; i++)
    {
      data[i].group = 1;
	  tmp = fscanf(fp, "%s %s %d ", treatment, name, &dtmp);
      strcpy(data[i].treatment, treatment);
	  strcpy(data[i].name, name);
	  data[i].numframes = dtmp;
      for (j = 0; j < nvars; j++)
      {
        tmp = fscanf(fp, "%f ", &ftmp);
        data[i].vars[j] = ftmp;
      }  
    }
    fclose(fp);

 	fp = fopen(inputfile2, "r");
    printf("opened %s\n", inputfile2);
    tmp = readLine(fp);
    for (i = 0; i < nsamples2; i++)
    {
      data[nsamples1 + i].group = 2;
	  tmp = fscanf(fp, "%s %s %d ", treatment, name, &dtmp);
      strcpy(data[nsamples1 + i].treatment, treatment);
	  strcpy(data[nsamples1 + i].name, name);
	  data[nsamples1 + i].numframes = dtmp;
      for (j = 0; j < nvars; j++)
      {
        tmp = fscanf(fp, "%f ", &ftmp);
        data[nsamples1 + i].vars[j] = ftmp;
      }  
    }
    fclose(fp);

    /* calculate separation for each feature */
    sepcalc(data, nsamples1, nsamples2, vars, nvars);
	
    choosevars(vars, nvars);

 	fp = fopen("separationscores.txt", "w");
    for (j = 0; j < nvars; j++)
    {
      if (vars[j].use == 1) fprintf(fp, "%d %s %lf\n", j, vars[j].name, vars[j].sep);
    }
    fclose(fp);

 
    for (i = 0; i < nsamples; i++)
    {
      free(data[i].vars);
    }
    free(data);

    free(vars);

    return 0;
}

/*****************************************************
Procedure: readLine
Description: reads past a line
******************************************************/
int readLine(FILE *fp)
{
	int ch;

    ch = fgetc(fp);

	while (ch != 10)
	{
	    ch = fgetc(fp);
	}

	return ch;
}

/****************************************************************
Procedure:  choosevars
Description:    chooses variables for which separation is > MINSEP
****************************************************************/
 void choosevars(VARINF *vars, int nvars)
 {
    int j, k;

    for (j = 0; j < nvars; j++)
    {
	  vars[j].use = 0;
      if (vars[j].sep > MINSEP) vars[j].use = 1;
    }

 }

/*****************************************************
 Procedure: varsep
 Description: calculates separation for variable jj
 NOTE: this currently only works for two groups
 ***************************************************/
double varsep(SAMPLE *data, int nsamples1, int nsamples2, int jj)
{
    int i, l, nsamples;
    int ngroups = 2;
    double vb, vw, overmean, sep;
    double mean[ngroups];
    double var[ngroups];

     nsamples = nsamples1 + nsamples2;
    /* calculate group means */
    mean[0] = 0.0;
    mean[1] = 0.0;
    for (i = 0; i < nsamples; i++)
    {
      if (data[i].group == 1) mean[0] += data[i].vars[jj]/(float)nsamples1;
      else if (data[i].group == 2) mean[1] += data[i].vars[jj]/(float)nsamples2;
    }
    overmean = (nsamples1*mean[0] + nsamples2*mean[1])/(nsamples);
   
    /* calculate group variances */
    var[0] = 0.0;
    var[1] = 0.0;
    for (i = 0; i < nsamples; i++)
    {
      if (data[i].group == 1) var[0] += (data[i].vars[jj]-mean[0])*(data[i].vars[jj]-mean[0])/(float)(nsamples1-1);
      else if (data[i].group == 2) var[1] += (data[i].vars[jj]-mean[1])*(data[i].vars[jj]-mean[1])/(float)(nsamples2-1);
    }

    vw = ((nsamples1-1)*var[0] + (nsamples2-1)*var[1])/(nsamples-2);
    vb = (nsamples1*(mean[0]-overmean)*(mean[0]-overmean) + nsamples2*(mean[1]-overmean)*(mean[1]-overmean))/(nsamples-2);
    sep = vb/vw;

    return(sep);
}

/*****************************************************
 Procedure: sepcalc
 Description: calculates separation for all variables
****************************************************/
void sepcalc(SAMPLE *data, int nsamples1, int nsamples2, VARINF *vars, int nvars)
{
    int j;

    for (j = 0; j < nvars; j++)
    {
      vars[j].sep = varsep(data, nsamples1, nsamples2, j);
    }

}

