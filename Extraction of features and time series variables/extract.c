/*
this file considers cells with trackength > MINLENGTH and outputs various parameters
from the wavelet transforms of these - only uses those for which use == 1

example usage:
make
./image 05062019_B3_3 B
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"

/* C++ procedures */
int* loadImage  (char*, int*, int*, int*);

int readLine(FILE *fp);
void waveTran(double *inputArray, int arraynum, double *outputArray, int *detlength);
void wavevars(double *inputArray, int start, int end, double *vars, int lev);
void summarystats(C *cell, int ncells, int nvars);
void densitycalc(C *cell, int ncells, int inputnum);
void VarFromCentre(C *cell, int ncells, int inputnum);
void minBox(C *cell, int ncells,  int inputnum);
void curvature(C *cell, int ncells, int gap, int inputnum);
void fillobject(char *name, C *cell, F *frame, int nframes, int inputnum);
void atob(C *cell, int ncells,  int inputnum);
void polyClass(C *cell, int ncells,  int inputnum);
void interpolate(C *cell, int ncells, int numinput);
void textureVariables(C *cell, int ncells, int inputnum);
void firstOrderOriginal(C *cell, int ncells, int inputnum);
void firstOrderApprox(C *cell, int ncells, int inputnum);
void cooccurVariables(C *cell, int ncells, int inputnum);
void writedata(C *cell, int ncells, int numinput, int nstats, char *cellgroupname, NAMES *vname,  char *classlabel);

int main (int argc, char *argv[])
{
  int i, j, k, n, m, nf, keep, xpix, ypix, cellID, frameID, bmax, numvars;
  float X, Y, Volume, Thickness, Radius, Area, Sphericity, Vel1, Vel2;
  float Length, Width, Orientation, Mass, Displacement, Velocity, TrackLength;
  int dtmp, dtmp1, dtmp2, dtmp3, dtmp4;
  int ncells, nframes, cellnummax, keepframe;
  double diff;
  int numinput = 49;
  int nstats = 3;
  char cellgroupname[100];
  char inputname[100];
  char boundname[100];
  char filelistname[100];
  char ch;
  char classlabel[10];

  if (argc != 3) printf("correct usage: ./cell inputgroup_name classlabel\n");
  /* will add _Phase-FullFeatureTable.csv to inputgroup_name and boundaries.csv to boundary_filename */
  strcpy(cellgroupname, argv[1]);
  strcpy(classlabel, argv[2]);

  /* open filelist */
  strcpy(inputname, cellgroupname);
  strcat(inputname, "_Phase-FullFeatureTable.csv\0");

  FILE *fp = NULL;
  fp = fopen(inputname, "r");
  printf("opened %s\n", inputname);

  /* allocate space for cellinfo */
  C *cell= NULL;
  cell= (C *) malloc (MAXC * sizeof(C));

  for (i = 0; i < MAXC; i++)
  {
    cell[i].framenum = (int *) malloc (MAXF * sizeof(int));
    cell[i].missingframe = (int *) malloc (MAXF * sizeof(int));
    for (j = 0; j < MAXF; j++)
    {
      cell[i].missingframe[j] = 0;
    }
    cell[i].input = (INVARS *) malloc (numinput * sizeof(INVARS));
    for (j = 0; j < numinput; j++)
    {
      cell[i].input[j].frame = (double *) malloc (MAXF * sizeof(double));
      cell[i].input[j].stats = (double *) malloc (nstats * sizeof(double));
      cell[i].input[j].vars = (double *) malloc ((LEVELS + 1) * 3 * sizeof(double));
      for (k = 0; k < (LEVELS + 1) * 3; k++)
      {
        cell[i].input[j].vars[k] = 0.0;
      }
    }
    cell[i].boundary = (BOUND *) malloc (MAXF * sizeof(BOUND));
    for (j = 0; j < MAXF; j++)
    {
      cell[i].boundary[j].xpix = (int *) malloc (MAXB * sizeof(int));
      cell[i].boundary[j].ypix = (int *) malloc (MAXB * sizeof(int));
    }
    cell[i].object = (AREA *) malloc (MAXF * sizeof(AREA));
    for (j = 0; j < MAXF; j++)
    {
      cell[i].object[j].xpix = (int *) malloc (MAXA * sizeof(int));
      cell[i].object[j].ypix = (int *) malloc (MAXA * sizeof(int));
      cell[i].object[j].intensity = (int *) malloc (MAXA * sizeof(int));
      cell[i].object[j].miniImage = (int *) malloc (MAXA * sizeof(int));
      cell[i].object[j].lev0Pix = (double *) malloc (MAXA * sizeof(double));
      cell[i].object[j].lev1Pix = (double *) malloc (MAXA * sizeof(double));
      cell[i].object[j].lev2Pix = (double *) malloc (MAXA * sizeof(double));
      cell[i].object[j].cooc01 = (double *) malloc (NCOOC*NCOOC * sizeof(double));
      cell[i].object[j].cooc12 = (double *) malloc (NCOOC*NCOOC * sizeof(double));
      cell[i].object[j].cooc02 = (double *) malloc (NCOOC*NCOOC * sizeof(double));
    }
    cell[i].wave = (double *) malloc ((MAXF) * sizeof(double));
    cell[i].wl = (int *) malloc ((LEVELS + 1) * sizeof(int));
  }

  NAMES *vname= NULL;
  vname = (NAMES *) malloc (numinput * sizeof(NAMES));

  /* read past first two lines */
  readLine(fp);

  readLine(fp);
  keep = 1;
  /* n is the cell counter */
  n = 0;
  /* m is the frame counter for each cell */
  m = 0;
  cellnummax = 0;
  while (fscanf(fp, "%c%d%c,%c%d%c,%c%d%c,%c%d%c,", &ch, &dtmp, &ch, &ch, &dtmp1, &ch, &ch, &dtmp2, &ch, &ch,  &dtmp3, &ch) != EOF)
  {
    fscanf(fp, "%c%f%c,%c%f%c,%c%d%c,%c%d%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c,%c%f%c\n", &ch, &X, &ch, &ch, &Y, &ch, &ch, &xpix, &ch, &ch, &ypix, &ch, &ch, &Volume, &ch, &ch, &Thickness, &ch, &ch, &Radius, &ch, &ch, &Area, &ch, &ch, &Sphericity, &ch, &ch, &Length, &ch, &ch, &Width, &ch, &ch, &Orientation, &ch, &ch, &Mass, &ch, &ch, &Displacement, &ch, &ch, &Velocity,  &ch, &ch, &Vel1,  &ch, &ch, &Vel2, &ch, &ch, &TrackLength, &ch);
    if (dtmp2 != keep)
    {
      keep = dtmp2;
      n++;
      m = 0;
    }
    cell[n].cellID = keep;
    if (cell[n].cellID > cellnummax) cellnummax = cell[n].cellID;
    if (cellnummax > MAXC) printf("PROBLEM: too many cells for array %d %d\n", cell[n].cellID, cellnummax);
    cell[n].lineID = dtmp3;
    if (m == 0)
    {
      cell[n].startframe = dtmp;
      cell[n].framenum[m] = dtmp;
      keepframe = dtmp;
    }
    else
    {
      while (dtmp != keepframe)
      {
        for (j = 0; j < numinput; j++)
        {
          cell[n].input[j].frame[keepframe] = -1.0;
        }
        cell[n].missingframe[m] = 1;
        cell[n].framenum[m] = keepframe;
        if (m > MAXF) printf("PROBLEM: too many frames for array %d %d \n", cell[n].cellID, m);
        m++;
        keepframe++;
      }
    }
    /* NOTE numvars 0-3 are used in densitycalc and varfromcentre */
    cell[n].input[0].frame[m] = X;
    cell[n].input[1].frame[m] = Y;
    cell[n].input[2].frame[m] = Volume;
    strcpy(vname[2].var, "Vol\0");
    cell[n].input[3].frame[m] = Radius;
    strcpy(vname[3].var, "Rad\0");
    cell[n].input[4].frame[m] = Sphericity;
    strcpy(vname[4].var, "Sph\0");
    cell[n].input[5].frame[m] = Length;
    strcpy(vname[5].var, "Len\0");
    cell[n].input[6].frame[m] = Width;
    strcpy(vname[6].var, "Wid\0");
    cell[n].input[7].frame[m] = Velocity;
    strcpy(vname[7].var, "Velocity\0");
    cell[n].input[8].frame[m] = Displacement;
    nframes = cell[n].framenum[m] - cell[n].framenum[0];
    if (nframes > 0) cell[n].input[8].frame[m] /= (float)nframes;
    strcpy(vname[8].var, "Dis\0");
    cell[n].input[9].frame[m] = TrackLength;
    strcpy(vname[9].var, "Trac\0");
    cell[n].input[10].frame[m] = 0.0;
    if (TrackLength > 0.0) cell[n].input[10].frame[m] = Displacement/TrackLength;
    strcpy(vname[10].var, "D2T\0");
    cell[n].framenum[m] = keepframe;
    m++;
    keepframe++;
    cell[n].num = m;
  }
  nf = dtmp+1;
  ncells = n+1;
  
  fclose(fp);
  
  numvars = 11;

  /* info on which cells are in which frames */
  F *frame = NULL;
  frame = (F *) malloc (nf * sizeof(F));
  for (k = 0; k < nf; k++)
  {
    frame[k].cells = (int *) malloc (MAXCF * sizeof(int));
    frame[k].num = 0;
    for (i = 0; i < ncells; i++)
    {
      if ((k >= cell[i].framenum[0] - 1) && (k <= cell[i].framenum[cell[i].num - 1] - 1))
      {
        frame[k].cells[frame[k].num] = i;
        frame[k].num++;
      }
    }

  }

  /* get positions in cell array of each cellID */
  int *cellnums = NULL;
  cellnums= (int *) malloc (cellnummax * sizeof(int));
  for (i = 0; i < cellnummax; i++)
  {
    cellnums[i] = -1;
  }
  for (i = 0; i < ncells; i++)
  {
    cellnums[cell[i].cellID] = i;
  }

  /* open boundary file */
  strcpy(boundname, cellgroupname);
  strcat(boundname, "_boundaries.csv\0");

  fp = fopen(boundname, "r");
  printf("opened %s\n", boundname);

  /* read in boundary data */
  fscanf(fp, "%d,", &dtmp);
  bmax = dtmp;
  for (i = 0; i < 2*bmax + 1; i++)
  {
    fscanf(fp, "%d,", &dtmp);
  }
  fscanf(fp, "%d", &dtmp);
  while (fscanf(fp, "%d,%d,%d,", &dtmp, &dtmp1, &dtmp2) != EOF)
  {
    frameID = dtmp;
    cellID = dtmp1;
    if (dtmp2 > MAXB) printf("PROBLEM: boundary too long for array %d %d\n", cellID, bmax);
    m = frameID-cell[cellnums[cellID]].startframe;
    cell[cellnums[cellID]].boundary[m].blength = dtmp2;
    if (frameID != cell[cellnums[cellID]].framenum[m]) printf("mismatch of framenumbers %d %d %d\n", cellID, frameID, m);
    for (i = 0; i < bmax; i++)
    {
      if (i != bmax-1) fscanf(fp, "%d,%d,", &dtmp3, &dtmp4);
      else fscanf(fp, "%d,%d", &dtmp3, &dtmp4);
      cell[cellnums[cellID]].boundary[m].xpix[i] = dtmp3;
      cell[cellnums[cellID]].boundary[m].ypix[i] = dtmp4;
    }
  }
  fclose(fp);

  /* image filelist  */
  strcpy(filelistname, cellgroupname);
  strcat(filelistname, "_imagelist.txt\0");
  printf("opened %s\n", filelistname);

  minBox (cell, ncells, numvars);
  strcpy(vname[11].var, "Box\0");
  strcpy(vname[12].var, "Rect\0");
  numvars = numvars + 2;
  VarFromCentre (cell, ncells, numvars);
  strcpy(vname[13].var, "VfC\0");
  numvars = numvars + 1;
  curvature (cell, ncells, 4, numvars);
  strcpy(vname[14].var, "Cur\0");
  numvars = numvars + 1;
  densitycalc(cell, ncells, numvars);
  strcpy(vname[15].var, "Den\0");
  numvars = numvars + 1;
  fillobject(filelistname, cell, frame, nf, numvars);
  strcpy(vname[16].var, "Area\0");
  numvars = numvars + 1;
  atob (cell, ncells, numvars);
  strcpy(vname[17].var, "A2B\0");
  numvars = numvars + 1;
  polyClass(cell, ncells, numvars);
  strcpy(vname[18].var, "poly1\0");
  strcpy(vname[19].var, "poly2\0");
  strcpy(vname[20].var, "poly3\0");
  strcpy(vname[21].var, "poly4\0");
  numvars = numvars + 4;
  textureVariables(cell, ncells, numvars);
  strcpy(vname[22].var, "IQ1\0");
  strcpy(vname[23].var, "IQ2\0");
  strcpy(vname[24].var, "IQ3\0");
  strcpy(vname[25].var, "IQ4\0");
  strcpy(vname[26].var, "IQ5\0");
  strcpy(vname[27].var, "IQ6\0");
  strcpy(vname[28].var, "IQ7\0");
  strcpy(vname[29].var, "IQ8\0");
  strcpy(vname[30].var, "IQ9\0");
  numvars = numvars + 9;
  firstOrderOriginal(cell, ncells, numvars);
  strcpy(vname[31].var, "FOmean\0");
  strcpy(vname[32].var, "FOsd\0");
  strcpy(vname[33].var, "FOskew\0");
  numvars = numvars + 3;
  cooccurVariables(cell, ncells, numvars);
  strcpy(vname[34].var, "Hf1_01\0");
  strcpy(vname[35].var, "Hf2_01\0");
  strcpy(vname[36].var, "Hf3_01\0");
  strcpy(vname[37].var, "Hf4_01\0");
  strcpy(vname[38].var, "Hf5_01\0");
  strcpy(vname[39].var, "Hf1_12\0");
  strcpy(vname[40].var, "Hf2_12\0");
  strcpy(vname[41].var, "Hf3_12\0");
  strcpy(vname[42].var, "Hf4_12\0");
  strcpy(vname[43].var, "Hf5_12\0");
  strcpy(vname[44].var, "Hf1_02\0");
  strcpy(vname[45].var, "Hf2_02\0");
  strcpy(vname[46].var, "Hf3_02\0");
  strcpy(vname[47].var, "Hf4_02\0");
  strcpy(vname[48].var, "Hf5_02\0");

  interpolate (cell, ncells, numinput);

  /* calculate total ascent and descent */
  for (i = 0; i < ncells; i++)
  {
    if (cell[i].num > MINLENGTH)
    {
      for (j = 0; j < numinput; j++)
      {
        cell[i].input[j].vars[2] = cell[i].input[j].frame[0];
        for (k = 1; k < cell[i].num; k++)
        {
          diff = cell[i].input[j].frame[k]-cell[i].input[j].frame[k-1];
          if (diff > 0) cell[i].input[j].vars[0] += diff;
          else cell[i].input[j].vars[1] += diff;
          if (cell[i].input[j].frame[k] > cell[i].input[j].vars[2]) cell[i].input[j].vars[2] = cell[i].input[j].frame[k];
        }
        cell[i].input[j].vars[0] /= (float)cell[i].num;
        cell[i].input[j].vars[1] /= (float)cell[i].num;
        cell[i].input[j].vars[2] /= (float)cell[i].num;
      }
    }
  }

  /* calculate wavelet detail coefficients */
  for (i = 0; i < ncells; i++)
  {
    if (cell[i].num > MINLENGTH)
    {
      for (j = 0; j < numinput; j++)
      {
        waveTran(cell[i].input[j].frame, cell[i].num, cell[i].wave, cell[i].wl);
        for (k = 0; k < LEVELS; k++)
        {
          wavevars(cell[i].wave, cell[i].wl[k], cell[i].wl[k+1], cell[i].input[j].vars, k);
        }
      }
    }
  }

  summarystats(cell, ncells, numinput);

  /*write out all values (timeseries for each cell) for particular variables */
  writedata(cell, ncells, numinput, nstats, cellgroupname, vname, classlabel);

  for (i = 0; i < MAXC; i++)
  {
    free(cell[i].framenum);
    free(cell[i].missingframe);
    for (j = 0; j < numinput; j++)
    {
      free(cell[i].input[j].frame);
      free(cell[i].input[j].stats);
      free(cell[i].input[j].vars);
    }
    free(cell[i].input);
    free(cell[i].wave);
    free(cell[i].wl);
    for (j = 0; j < MAXF; j++)
    {
      free(cell[i].boundary[j].xpix);
      free(cell[i].boundary[j].ypix);
    }
    free(cell[i].boundary);
    for (j = 0; j < MAXF; j++)
    {
      free(cell[i].object[j].xpix);
      free(cell[i].object[j].ypix);
      free(cell[i].object[j].intensity);
      free(cell[i].object[j].miniImage);
      free(cell[i].object[j].lev0Pix);
      free(cell[i].object[j].lev1Pix);
      free(cell[i].object[j].lev2Pix);
      free(cell[i].object[j].cooc01);
      free(cell[i].object[j].cooc12);
      free(cell[i].object[j].cooc02);
    }
    free(cell[i].object);
  }
  free(cell);
  free(cellnums);
  for (i = 0; i < MAXCF; i++)
  {
    free(frame[i].cells);
  }
  free (frame);
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
Procedure:  fSort
Description:    Sorts an array of doubles  into increasing order
****************************************************************/
void fSort (double *array, int *index, int nRef)
{
  int nInt, ii, i, j, nf;
  double tf;
  nInt = nRef;
  if (nInt == 1)
    return;

  do
  {
    nInt = nInt / 2;
    if ((nInt % 2) == 0)
    {
      nInt = nInt-1;
    }
    for (ii= 0; ii < nRef - nInt; ii++)
    {
      i = ii;
      j = i + nInt;
      if (array[i] > array[j])
      {
        tf = array[j];
        nf = index[j];
        do
        {
          array[j] = array[i];
          index[j] = index[i];
          j = i;
          i = i - nInt;
        }
        while ((i >= 0) && (array[i] > tf));
        array[j] = tf;
        index[j] = nf;
      }
    }
  }
  while (nInt > 1);
}


/*****************************************************************************************
Procedure:    code for Daubechies' 2-coefficient (Haar) wavelet from Numerical Recipes

******************************************************************************************/
void daub2(double *a, int n, int isign)
{
  double *wksp=NULL;
  int nh, i,j;
  double D0 = 0.70710678;
  double D1 = 0.70710678;

  wksp = (double *) malloc (n* sizeof(double));

  if (n < 4) return;
  nh = n/2;
  if (isign >= 0)
  {
    i = 0;
    for ( j = 0; j < n-1; j+=2)
    {
      wksp[i] = D0*a[j] + D1*a[j+1];
      wksp[i+nh] = D1*a[j] - D0*a[j+1];
      i++;
    }
  }
  else
  {
    j = 0;
    for (i=0;i < nh-1; i++)
    {
      wksp[j] = D0*a[i] + D1*a[i+nh];
      wksp[j+1] = D1*a[i] - D0*a[i+nh];
      j=j+2;
    }
  }
  for (i = 0; i < n; i++)
  {
    a[i] = wksp[i];
  }
  free (wksp);
}

/*****************************************************************************************
Procedure:    wavelet transform
Description:  performs 3-level wavelet transform
******************************************************************************************/
void waveTran(double *inputArray, int arraynum, double *outputArray, int *detlength)
{
  int x, k, length;
  double root2 = sqrt(2.0);
  int ww;

  detlength[0] = 0;
  length = arraynum;
  ww = 0;
  if (length % 2 != 0) ww = 1;

  double *xVector   = NULL;
  xVector = (double *) malloc ((length + ww) * sizeof(double));
  for (x = 0; x < length; x++)
  {
    xVector[x] = inputArray[x];
  }

  for (k = 0; k < LEVELS; k++)
  {
    ww = 0;
    if (length % 2 != 0) ww = 1;

    /* do transform */
    if (ww == 1) xVector[length] = xVector[length - 1];
    length = length + ww;

    daub2(xVector,length, 1);

    /* rescale for next level */
    for (x = 0; x < length - ww; x++)
    {
      xVector[x] /= root2;
    }

    length = length/2;
    /* output detail coefficients */
    for (x = 0; x < length - ww; x++)
    {
     outputArray[x+detlength[k]] = inputArray[x + length];
    }
    detlength[k+1] = detlength[k] + length - ww;
  }

 free(xVector);
}

/*****************************************************************************************
Procedure:    wavevars
Description:  calculate variables from wavelet coefficients
******************************************************************************************/
void wavevars(double *inputArray, int start, int end, double *vars, int lev)
{
  int k, n;
  int length = end-start;
  double diff, ascent, descent, max;

  double *tempArray = NULL;
  tempArray = (double *) malloc (length * sizeof(double));

  n = 0;
  for (k = start; k < end; k++)
  {
    tempArray[n] = inputArray[k];
    n++;
  }

  /* calculate total ascent and descent */
  ascent = 0.0;
  descent = 0.0;
  max = 0.0;
  for (k = 1; k < length; k++)
  {
    diff = tempArray[k] - tempArray[k-1];
    if (diff > 0) ascent += diff;
    else descent += diff;

    if (fabs(tempArray[k]) > max) max = fabs(tempArray[k]);
  }
  vars[3*(lev + 1)] = ascent/(float)length;
  vars[3*(lev + 1) + 1] = descent/(float)length;
  vars[3*(lev + 1) + 2] = max/(float)length;
  free (tempArray);
}

/******************************************************************
Procedure: densitycalc
Description: calculates local density for each cell in each frame
This is the newest version
*******************************************************************/
void densitycalc(C *cell, int ncells, int inputnum)
{
  int i, j, k, l, num;
  double x, y, r, x1, y1, r1, avrad, dist, vol1, vol;
  int swap;
  double standard, d, prop, alpha;

  double twoPI = 8.0 * atan(1.0);

  /* get average cell radius */
  avrad = 0.0;
  num = 0;
  for (i = 0; i < ncells; i++)
  {
    for (j = 0; j < cell[i].num; j++)
    {
      avrad += cell[i].input[3].frame[j];
      num++;
    }
  }
  avrad /= (float)num;
  avrad = 2.0*avrad;

  for (i = 0; i < ncells; i++)
  {
    /* for each frame */
    for (j = 0; j < cell[i].num; j++)
    {
      cell[i].input[inputnum].frame[j] = 0.0;
      x = cell[i].input[0].frame[j];
      y = cell[i].input[1].frame[j];
      r = cell[i].input[3].frame[j];
      /* get distance to other cells */
      for (k = 0; k < ncells; k++)
      {
        for (l = 0; l < cell[k].num; l++)
        {
          if ((cell[k].framenum[l] == cell[i].framenum[j]) && (k != i))
          {
            x1 = cell[k].input[0].frame[l];
            y1 = cell[k].input[1].frame[l];
            r1 = cell[k].input[3].frame[l];
            dist = sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
            standard = r + avrad + r1;
            if (dist < standard)
            {
              /* d is the height of the circular segment of the nearby cell to be included in the density calculation */
              d = standard - dist;
              swap = 0;
              if (d > r1)
              {
                 d = d-r1;
                 swap = 1;
              }
              /* angle subtended at centre of "circle" */
              alpha = fabs(2.0*cos((r1-d)/r1));
              /* proportion of cell within close region */
              prop = 1.0/(twoPI*(alpha - sin(alpha)));
              /* if more than half the cell is in close region */
              if (swap == 1)
              {
                prop = 1-prop;
              }
              /* if entire cell inside close region */
              if (standard > dist + r1) prop = 1.0;
              vol1 = cell[k].input[2].frame[l];
              /* volume of area around first cell */
              vol = 2.0*twoPI*avrad*(r*r + r*avrad +avrad*avrad);
              cell[i].input[inputnum].frame[j] += 100.0*prop*vol1/vol;
            }
          }
        }
      }
    }
  }
}

/*****************************************************
Procedure: summarystats
Description: calculates various stats from cell variables
******************************************************/
void summarystats(C *cell, int ncells, int nvars)
{
  int i, j, k, nummissing, notmissing;
  double m1, m2, m3, en, enm1, enm2;

  for (i = 0; i < ncells; i++)
  {
    nummissing = 0;
    for (j = 0; j < cell[i].num; j++)
    {
       if (cell[i].missingframe[j] == 1) nummissing++;
    }
    notmissing = cell[i].num - nummissing;
    if (notmissing > 5)
    {
      en = (float)(notmissing);
      enm1 = en - 1.0;
      enm2 = en - 2.0;
      for (k = 0; k < nvars; k++)
      {
        /* get means */
        cell[i].input[k].stats[0] = 0.0;
        for (j = 0; j < cell[i].num ; j++)
        {
          if (cell[i].missingframe[j] != 1)
          {
            cell[i].input[k].stats[0] += cell[i].input[k].frame[j];
          }
        }
        /* stat 0 is mean */
        cell[i].input[k].stats[0] /= en;

        /* get moments */
        m1 = cell[i].input[k].stats[0];
        m2 = 0.0;
        m3 = 0.0;
        for (j = 0; j < cell[i].num ; j++)
        {
          if (cell[i].missingframe[j] != 1)
          {
            m2 += (cell[i].input[k].frame[j]-m1) * (cell[i].input[k].frame[j]-m1);
            m3 += (cell[i].input[k].frame[j]-m1) * (cell[i].input[k].frame[j]-m1) * (cell[i].input[k].frame[j]-m1);
          }
        }
        /* stat 1 is standard deviation */
        cell[i].input[k].stats[1] = sqrt(m2/enm1);
        /* stat 2 is skewness */
        if (m2 == 0.0) cell[i].input[k].stats[2] = 0.0;
        else cell[i].input[k].stats[2] = (m3 * en * sqrt(enm1))/(enm2 * m2 * sqrt(m2));
      }

    }
  }

}

/*****************************************************************************
 Procedure:  getVar
 Description: calcultes the variance/mean of an array
*******************************************************************************/
double getVar(double *array, int num)
{
  int i;
  double var, sum1, sum2, fnum;

  sum1 = 0.0;
  sum2 = 0.0;
  fnum = (float)num;
  for (i = 0; i < num; i++)
  {
    sum1 += array[i];
    sum2 += array[i]*array[i];
  }
  sum1 /= fnum;
  sum2 /= fnum;
  var = sum2 - (sum1*sum1);
  var = var/sum1;
  return(var);
}

/*****************************************************************************
 Procedure:  VarFromCentre
 Description:    finds variance of the didtance from the boundary to the centre
******************************************************************************/
void VarFromCentre(C *cell, int ncells, int jj)
{
  int i, j, k, x, y, icx, icy;

  double *dist   = NULL;
  dist = (double *) malloc (MAXB * sizeof(double));

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        for (j = 0; j < cell[i].boundary[k].blength; j++)
        {
          x = cell[i].boundary[k].xpix[j];
          y = cell[i].boundary[k].ypix[j];
          icx = cell[i].input[0].frame[k];
          icy = cell[i].input[1].frame[k];
          dist[j] = sqrt((x-icx)*(x-icx) + (y-icy)*(y-icy));
        }
        cell[i].input[jj].frame[k] = getVar(dist,cell[i].boundary[k].blength);
      }
      else cell[i].input[jj].frame[k] = -1.0;

    }
  }
  free(dist);
}

/*****************************************************************************
 Procedure:  minBox
 Description:    calculates minimal rectangular box that the cell will fit in
******************************************************************************/
void minBox(C *cell, int ncells, int inputnum)
{
  int i, j, k, l;
  int x1, x2, y1, y2;
  double dist, maxdist = 0.0;
  int keepx1 = 0, keepx2 = 0, keepy1 = 0, keepy2 = 0;
  double xlength, ylength;
  double alpha, rotx, roty, maxx, minx, maxy, miny, max;

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        /* find maximum distance between two boundary points */
        maxdist = 0.0;
        for (j = 0; j < cell[i].boundary[k].blength; j++)
        {
          for (l = j+1; l < cell[i].boundary[k].blength; l++)
          {
            x1 = cell[i].boundary[k].xpix[j];
            y1 = cell[i].boundary[k].ypix[j];
            x2 = cell[i].boundary[k].xpix[l];
            y2 = cell[i].boundary[k].ypix[l];
            dist = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
            if (dist > maxdist)
            {
              maxdist = dist;
              keepx1 = x1;
              keepx2 = x2;
              keepy1 = y1;
              keepy2 = y2;
            }
          }
        }

        if (keepx1 == keepx2) alpha = PI/2.0;
        else if (keepy1 == keepy2) alpha = 0.0;
        else alpha = atan2((double)(keepy1 - keepy2), (double)(keepx1 - keepx2));

        /* rotating points by -alpha makes keepx1 -keepx2 lie along x-axis */
        /* get rotated y values and find max-min for ylength */
        maxy = 0.0;
        miny = IMAGEHEIGHT;
        /* double rotx */
        for (j = 0; j < cell[i].boundary[k].blength; j++)
        {
          roty = keepy1 - sin(alpha)*(float)(cell[i].boundary[k].xpix[j] - keepx1) + cos(alpha)*(float)(cell[i].boundary[k].ypix[j] - keepy1);
          rotx = keepx1 + cos(alpha)*(float)(cell[i].boundary[k].xpix[j] - keepx1) + sin(alpha)*(float)(cell[i].boundary[k].ypix[j] - keepy1);
          if (roty < miny) miny = roty;
          if (roty > maxy) maxy = roty;
          if (rotx < minx) minx = rotx;
          if (rotx > maxx) maxx = rotx;
        }
        xlength = maxx-minx;
        ylength = maxy-miny;
        cell[i].input[inputnum].frame[k] = (float)(xlength*ylength)/(2.0*PI*cell[i].input[3].frame[k]);
        max = xlength;
        if (ylength > max) max = ylength;
        cell[i].input[inputnum+1].frame[k] = max/(xlength + ylength);
      }
      else
      {
        cell[i].input[inputnum].frame[k] = -1.0;
        cell[i].input[inputnum+1].frame[k] = -1.0;
      }
    }
  }

}

/*****************************************************************************
Procedure:  curvature
Description:    calculates the curvature of the boundary
******************************************************************************/
void curvature(C *cell, int ncells, int gap, int inputnum)
{
  int i, j, k, x, y, n, xmgap, ymgap, xpgap, ypgap;
  int i1, i2, i3;
  double d1, d2, d3;
  double curv, new;

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        n = cell[i].boundary[k].blength;
        curv = 0.0;
        for (j = 0; j < n; j++)
        {
          x = cell[i].boundary[k].xpix[j];
          y = cell[i].boundary[k].ypix[j];
          xmgap = cell[i].boundary[k].xpix[(j-gap+n)%n];
          ymgap = cell[i].boundary[k].ypix[(j-gap+n)%n];
          xpgap = cell[i].boundary[k].xpix[(j+gap+n)%n];
          ypgap = cell[i].boundary[k].ypix[(j+gap+n)%n];
          i1 = (x-xmgap)*(x-xmgap) + (y-ymgap)*(y-ymgap);
          i2 = (x-xpgap)*(x-xpgap) + (y-ypgap)*(y-ypgap);
          i3 = (xmgap-xpgap)*(xmgap-xpgap) + (ymgap-ypgap)*(ymgap-ypgap);
          d1 = sqrt((float)i1);
          d2 = sqrt((float)i2);
          d3 = sqrt((float)i3);
          new = d1 + d2 - d3;
          curv = curv + new;
        }
        cell[i].input[inputnum].frame[k] = curv/(float)n;
      }
      else cell[i].input[inputnum].frame[k] = -1.0;

    }
  }
}

/*************************************************************************************************************
Procedure:  reScaleInt
Description:  input values are re-scaled to have a minimum equal to 0 and a maximum equal to 255
************************************************************************************************************/
void reScaleInt (int num, int *inputArray)
{
  int ind, max, min;
  double scale;
  min = 1000;
  max = -1000;

  for (ind = 0; ind < num ; ind++)
  {
    if (inputArray[ind] < min)  min = inputArray[ind];
    if (inputArray[ind] > max)  max = inputArray[ind];
  }

  if (max != min)
  {
    scale = 255.0/(double)(max-min);
    for (ind = 0; ind < num; ind++)
    {
      inputArray[ind] = (int)((inputArray[ind] - min)*scale);
    }
  }
}
/*****************************************************
Procedure:  writeImage
Description:    Writes out image data in pgm format
******************************************************/
void writeImage (const char *filename, int width, int height, int *pixels)
{
  FILE *fp = NULL;
  int i = 0, imageSize = width * height;

  int *temp = NULL;
  temp = (int *) malloc (imageSize * sizeof(int));
  for(i = 0; i < imageSize; i++)
  {
    temp[i] = (int)pixels[i];
  }
  reScaleInt (width*height, temp);

  unsigned char *copyPixels = NULL;
  copyPixels = (unsigned char *) malloc (imageSize * sizeof(unsigned char));
  for(i = 0; i < imageSize; i++)
  {
    copyPixels[i] = temp[i];
  }

  fp = fopen(filename, "w");

  for(i = 0; i < height; i++)
    fwrite(copyPixels + i * width, sizeof(unsigned char), width , fp);

  free(temp);
  free(copyPixels);
  fclose(fp);
}

/***********************************************************************************
Procedure:  firstNay
Description:    gives indices of eight immediate neighbours from x and y
************************************************************************************/
void firstNay (int x, int y, int width, int height, int *ip)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    ip[i] = -1;
  }
  if (y > 0) ip[0] = x + width * (y - 1);
  if (x < width-1) ip[1] = x + 1 + width * y;
  if (y < height-1) ip[2] = x + width * (y + 1);
  if (x > 0) ip[3] = x - 1 + width * y;
}

/***********************************************************************************************
Procedure:	1-level 2D wavelet transform
Description:    does 1-level x and y wavelet transform
************************************************************************************************/
void waveTran2D(int width, int height, double *inputImage, double *levImage)
{
  int x, y, ind, yind, jnd;
  int xLength = width;
  int yLength = height;

  double *xVector   = NULL;
  double *yVector   = NULL;
  xVector   = (double *) malloc (width * sizeof(double));
  yVector   = (double *) malloc (height * sizeof(double));
  double  *tempImage = NULL;
  tempImage = (double *) malloc (width * height * sizeof(double));

  /* do one level x transform */
  for (y = 0; y < yLength; y++)
  {
    yind = width*y;
    for (x = 0; x < xLength; x++)
    {
      ind = x + yind;
      xVector[x] = inputImage[ind];
    }
    daub2(xVector,xLength,1);
    for (x = 0; x < xLength; x++)
    {
      ind = x + yind;
      tempImage[ind] = xVector[x]/sqrt(2.0);
    }
  }
  /* do one level y transform */
  for (x = 0; x < xLength; x++)
  {
    for (y = 0; y < yLength; y++)
    {
      ind = x + width*y;
      yVector[y] = tempImage[ind];
    }
    daub2(yVector,yLength,1);
    for (y = 0; y < yLength; y++)
    {
      ind = x + width*y;
      tempImage[ind] = yVector[y]/sqrt(2.0);
    }
  }

  xLength = xLength/2;
  yLength = yLength/2;
  /* output smoothed image */
  for (x = 0; x < xLength; x++)
  {
    for (y = 0; y < yLength; y++)
    {
      ind = x + width*y;
      jnd = x + (width/2)*y;
      levImage[jnd] = tempImage[ind];
    }
  }
  free  (xVector);
  free  (yVector);
  free (tempImage);
}

/*************************************************************************************
Procedure:  shrinkmask
Description: produces a mask at a particular level from the one at the previous level
new mask is only 0 if all 4 pixels in previous level are 0
***************************************************************************************/
void shrinkmask(int width, int height, int *mask, int *newmask)
{
  int i, j, inds, x, y, indb, n;

  for (i = 0; i < width; i++)
  {
    for (j = 0; j < height; j++)
    {
      inds = i + j*width;
      newmask[inds] = 1;
      n = 0;
      for (x = 0; x < 2; x++)
      {
        for (y = 0; y < 2; y++)
        {
          indb = 2*i + x + 2*width * (2*j + y);
          if (mask[indb] == 0) n++;
        }
      }
      if (n == 4) newmask[inds] = 0;
    }
  }
}

/*************************************************************************************************************
Procedure:  reScale
Description: real-valued input values are re-scaled to have a minimum equal to 0 and a maximum equal to 255
************************************************************************************************************/
void reScale (int num, double *inputArray, int *mask)
{
  int ind;
  double max, min, scale;
  min = 1000.0;
  max = -1000.0;

  for (ind = 0; ind < num ; ind++)
  {
    if ((mask[ind] == 1) && (inputArray[ind] < min))  min = inputArray[ind];
    if (inputArray[ind] > max)  max = inputArray[ind];
  }

  if (max != min)
  {
    scale = 255.0/(max-min);
    for (ind = 0; ind < num; ind++)
    {
       if (mask[ind] == 1)
       {
         inputArray[ind] = (inputArray[ind] - min)*scale;
      }
    }
  }

  for (ind = 0; ind < num ; ind++)
  {
    if ((mask[ind] == 1) && (inputArray[ind] < min)) min = inputArray[ind];
    if (inputArray[ind] > max)  max = inputArray[ind];
  }
}

/*********************************************************************************************
Procedure:  getCoocMatrix
Description: creates co-occurrence matrices images on two levels
***********************************************************************************************/
void getCoocMatrix(double *cooc, double *bigimage, int *bigmask, double *smallimage, int *smallmask, int w, int h, int numlevs)
{
  int i, j, ls, lb, ind, inds, indb, x, y, nc, twos;
  float n;

  nc = NCOOC;
  n = (float)nc;
  twos = pow(2,numlevs);
  /* rescale the two images */
  reScale (w*h, smallimage, smallmask);
  reScale (twos*twos*w*h, bigimage, bigmask);

  for (j = 0; j < NCOOC*NCOOC; j++)
  {
    cooc[j] = 0;
  }

  for (i = 0; i < w; i++)
  {
    for (j = 0; j < h; j++)
    {
      inds = i + j*w;
      for (x = 0; x < twos; x++)
      {
        for (y = 0; y < twos; y++)
        {
          indb = twos*i + x + twos*w * (twos*j + y);
          if (bigmask[indb] != 0)
          {
            ls = (int)(n*smallimage[inds]/256.0);
            lb = (int)(n*bigimage[indb]/256.0);
            ind = lb + ls*NCOOC;
            cooc[ind]++;
          }
        }
      }
    }
  }
}

/*****************************************************************************
 Procedure:  coocur
 Description:
*******************************************************************************/
void cooccur(int *image, int *mask, int width, int height, C *cell, int cellind, int frameind)
{
  int j, x, y, ind, jnd;
  int newwidth, newheight;

  /* check the image size is correct for a 2-level transform */
  newwidth = (width/4)*4;
  if (width - newwidth > 0) newwidth += 4;
  newheight = (height/4)*4;
  if (height - newheight > 0) newheight += 4;

  double  *newimage = NULL;
  newimage = (double *) malloc (newwidth * newheight * sizeof(double));
  int  *newmask = NULL;
  newmask = (int *) malloc (newwidth * newheight * sizeof(int));
  double  *lev1image = NULL;
  lev1image = (double *) malloc ((newwidth/2) * (newheight/2) * sizeof(double));
  int  *lev1mask = NULL;
  lev1mask = (int *) malloc ((newwidth/2) * (newheight/2)  * sizeof(int));
  double  *lev2image = NULL;
  lev2image = (double *) malloc ((newwidth/4) * (newheight/4) * sizeof(double));
  int  *lev2mask = NULL;
  lev2mask = (int *) malloc ((newwidth/4) * (newheight/4) * sizeof(int));

  /* transfer the image to the double array, extending if necessary */
  for (j = 0; j < newwidth * newheight; j++)
  {
    newimage[j] = 0.0;
    newmask[j] = 0;
  }
  for (x = 0; x < width; x++)
  {
    for (y = 0; y < height; y++)
    {
      ind = x + y*width;
      jnd = x + y*newwidth;
      newimage[jnd] = (float)image[ind];
      newmask[jnd] = mask[ind];
    }
  }
  /* store non-mask pixels as real numbers */
  cell[cellind].object[frameind].lev0num = 0;
  for (x = 0; x < newwidth*newheight; x++)
  {
    if (newmask[x] != 0)
    {
      cell[cellind].object[frameind].lev0Pix[cell[cellind].object[frameind].lev0num] = newimage[x];
      cell[cellind].object[frameind].lev0num++;
    }
  }
  /* do both level wavelet transforms and store non-mask pixels (as real numbers) */
  waveTran2D(newwidth, newheight, newimage, lev1image);
  shrinkmask(newwidth/2, newheight/2, newmask, lev1mask);
  cell[cellind].object[frameind].lev1num = 0;
  for (x = 0; x < (newwidth/2)*(newheight/2); x++)
  {
    if (lev1mask[x] != 0)
    {
      cell[cellind].object[frameind].lev1Pix[cell[cellind].object[frameind].lev1num] = lev1image[x];
      cell[cellind].object[frameind].lev1num++;
    }
  }
  waveTran2D(newwidth/2, newheight/2, lev1image, lev2image);
  shrinkmask(newwidth/4, newheight/4, lev1mask, lev2mask);
  cell[cellind].object[frameind].lev2num = 0;
  for (x = 0; x < (newwidth/4)*(newheight/4); x++)
  {
    if (lev2mask[x] != 0)
    {
      cell[cellind].object[frameind].lev2Pix[cell[cellind].object[frameind].lev2num] = lev2image[x];
      cell[cellind].object[frameind].lev2num++;
    }
  }

  /* calculate co-occurrence matrix between image and first level wavelet approximation */
  double  *cooc01 = NULL;
  cooc01 = (double *) malloc (NCOOC*NCOOC * sizeof(double));
  getCoocMatrix(cooc01, newimage, newmask, lev1image, lev1mask, newwidth/2, newheight/2, 1);

  /* calculate co-occurrence matrix between first and second level wavelet approximations */
  double  *cooc12 = NULL;
  cooc12 = (double *) malloc (NCOOC*NCOOC * sizeof(double));
  getCoocMatrix(cooc12, lev1image, lev1mask, lev2image, lev2mask, newwidth/4, newheight/4, 1);

  /* calculate co-occurrence matrix between image and second level wavelet approximation */
  double  *cooc02 = NULL;
  cooc02 = (double *) malloc (NCOOC*NCOOC * sizeof(double));
  getCoocMatrix(cooc02, newimage, newmask, lev2image, lev2mask, newwidth/4, newheight/4, 2);

  for (x = 0; x < NCOOC*NCOOC; x++)
  {
    cell[cellind].object[frameind].cooc01[x] = cooc01[x];
    cell[cellind].object[frameind].cooc12[x] = cooc12[x];
    cell[cellind].object[frameind].cooc02[x] = cooc02[x];
  }

  free(newimage);
  free(newmask);
  free(lev1image);
  free(lev1mask);
  free(lev2image);
  free(lev2mask);
  free(cooc01);
  free(cooc12);
  free(cooc02);

 }



/*****************************************************************************
Procedure:  fillobject
Description:  finds indices for pixels within the cell boundary
*******************************************************************************/
void fillobject(char *filelist, C *cell, F *frame, int nframes, int inputnum)
{
  int k, j, i, l, x, y, npix, ind, jnd, width, height;
  int xlim1, xlim2, ylim1, ylim2, n, m;
  int offsetX, offsetY, colour, cellind, frameind;
  int iw = IMAGEWIDTH;
  int ih = IMAGEHEIGHT;
  char name[1024];
  int ip[4];
  FILE *fin = NULL;
  fin = fopen(filelist, "r");

  int    *image = NULL;
  image = (int *) malloc (iw * ih * sizeof(int));

  for (k = 0; k < nframes; k++)
  {
    /*read image name */
    fscanf(fin, "%s\n", name);
    image = loadImage(name, &iw, &ih, &colour);
    for (i = 0; i < frame[k].num; i++)
    {
      /* cellind is the index of the cell in the main (overall) cell array */
      cellind = frame[k].cells[i];
      /*frameind gives the frame numbers for a particular cell*/
      frameind = k + 1 - cell[cellind].startframe;

      if (cell[cellind].missingframe[frameind] != 1)
      {
        /* get limits for boundary "image" */
        xlim1 = IMAGEWIDTH;
        xlim2 = 0;
        ylim1 = IMAGEHEIGHT;
        ylim2 = 0;
        for (j = 0; j < cell[cellind].boundary[frameind].blength; j++)
        {
          x = cell[cellind].boundary[frameind].xpix[j];
          y = cell[cellind].boundary[frameind].ypix[j];
          if (x < xlim1) xlim1 = x;
          if (x > xlim2) xlim2 = x;
          if (y < ylim1) ylim1 = y;
          if (y > ylim2) ylim2 = y;
        }
        cell[cellind].boundary[frameind].xlim1 = xlim1;
        cell[cellind].boundary[frameind].xlim2 = xlim2;
        cell[cellind].boundary[frameind].ylim1 = ylim1;
        cell[cellind].boundary[frameind].ylim2 = ylim2;
        width = xlim2-xlim1+1;
        height = ylim2-ylim1+1;
        npix = width*height;
        offsetX = xlim1;
        offsetY = ylim1;

        int *miniImage = NULL;
        miniImage = (int *) malloc (npix * sizeof(int));
        /* set "image" to 1 for boundary and -1 for non-boundary */
        for (j = 0; j < npix; j++)
        {
          miniImage[j] = -1;
        }
        for (j = 0; j < cell[cellind].boundary[frameind].blength; j++)
        {
          x = cell[cellind].boundary[frameind].xpix[j];
          y = cell[cellind].boundary[frameind].ypix[j];
          ind = (x-xlim1) + width*(y-ylim1);
          miniImage[ind] = 1;
        }

        for (j = 0; j < cell[cellind].boundary[frameind].blength; j++)
        {
          x = cell[cellind].boundary[frameind].xpix[j];
          y = cell[cellind].boundary[frameind].ypix[j];
        }

        int *xmin = NULL;
        xmin = (int *) malloc (height * sizeof(int));
        int *xmax = NULL;
        xmax = (int *) malloc (height* sizeof(int));
        int *ymin = NULL;
        ymin = (int *) malloc (width * sizeof(int));
        int *ymax = NULL;
        ymax = (int *) malloc (width* sizeof(int));
        /* get minimum and maximum x and y values for each x for "filling" */
        for (j = 0; j < height; j++)
        {
          xmin[j] = width;
          xmax[j] = 0;
          for (l = 0; l < width; l++)
          {
            ind = l + width*j;
            if (miniImage[ind] == 1)
            {
              if (l < xmin[j]) xmin[j] = l;
              if (l > xmax[j]) xmax[j] = l;
            }
          }
        }
        for (j = 0; j < width; j++)
        {
          ymin[j] = height;
          ymax[j] = 0;
          for (l = 0; l < height; l++)
          {
            ind = j + width*l;
            if (miniImage[ind] == 1)
            {
              if (l < ymin[j]) ymin[j] = l;
              if (l > ymax[j]) ymax[j] = l;
            }
          }
        }
        /* fill background from edges of image */
        for (j = 0; j < width; j++)
        {
          for (l = 0; l < ymin[j]; l++)
          {
            ind = j + width*l;
            miniImage[ind] = 0;
          }
          for (l = ymax[j] + 1; l < height; l++)
          {
            ind = j + width*l;
            miniImage[ind] = 0;
          }
        }
        for (j = 0; j < height; j++)
        {
          for (l = 0; l < xmin[j]; l++)
          {
            ind = l + width*j;
            miniImage[ind] = 0;
          }
          for (l = xmax[j] + 1; l < width ; l++)
          {
            ind = l + width*j;
            miniImage[ind] = 0;
          }
        }
        /* check for non-boundary pixels with background pixels as neighbours */
        n = 1;
        while (n > 0)
        {
          n = 0;
          for (j = 0; j < width; j++)
          {
            for (l = 0; l < height; l++)
            {
              ind = j + width*l;
              if (miniImage[ind] == 0)
              {
                firstNay (j, l, width, height, ip);
                for (m = 0; m < 4; m++)
                {
                  if ((ip[m] != -1) && (miniImage[ip[m]] == -1))
                  {
                    miniImage[ip[m]] = 0;
                    n++;
                  }
                }
              }
            }
          }
        }
        /* now fill interior pixels and make a mask for background (0 for background, 1 for cell) */
        int *mask = NULL;
        mask = (int *) malloc (width*height * sizeof(int));
        n = 0;
        cell[cellind].object[frameind].width = width;
        cell[cellind].object[frameind].height = height;
        for (j = 0; j < width; j++)
        {
          for (l = 0; l < height; l++)
          {
            ind = j + width*l;
            jnd = j+offsetX + IMAGEWIDTH*(l+offsetY);
            mask[ind] = 1;
            if (miniImage[ind] != 0)
            {
              cell[cellind].object[frameind].xpix[n] = j;
              cell[cellind].object[frameind].ypix[n] = l;
              cell[cellind].object[frameind].intensity[n] = image[jnd];
              if (n > MAXA) printf("PROBLEM: cell with too many pixels for array %d %d\n", cell[cellind].cellID, n);
              n++;
              miniImage[ind] = image[jnd];
            }
            else mask[ind] = 0;
            cell[cellind].object[frameind].miniImage[ind] = miniImage[ind];
          }
        }
        cell[cellind].object[frameind].npix = n;
        cell[cellind].input[inputnum].frame[frameind] = n;


        cooccur(miniImage, mask, width, height, cell, cellind, frameind);


        free (miniImage);
        free (xmin);
        free (xmax);
        free (ymin);
        free (ymax);
        free (mask);
      }
      else cell[cellind].input[inputnum].frame[frameind] = -1.0;
    }
  }

  fclose(fin);
  free (image);
}

/*****************************************************************************
 Procedure:  atob
 Description:    calculates the ratio of the area to the boundary squared
 *******************************************************************************/
 void atob(C *cell, int ncells, int inputnum)
 {
    int i, k;

    for (i = 0; i < ncells; i++)
    {
      for (k = 0; k < cell[i].num; k++)
      {
        if (cell[i].missingframe[k] != 1)
        {
          cell[i].input[inputnum].frame[k] = cell[i].input[inputnum-1].frame[k]/(float)(cell[i].boundary[k].blength*cell[i].boundary[k].blength);
        }
        else cell[i].input[inputnum].frame[k] = -1.0;
      }
    }

 }

/*****************************************************************************
Procedure:  textureVariables
Description:
*******************************************************************************/
void textureVariables(C *cell, int ncells, int inputnum)
{
  int width, height, c, i, k, minx, miny, maxx, maxy, a, term1, term2, b, xp, xc, yp, yc, Indicator, floorh, num;
  double r, d, h, q, polyarea;
  float quantile;

  for (i = 0; i < ncells; i++)
  {
    printf("cellID: %d\n", i+1);
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        int input = inputnum;
        minx = 0;
        miny = 0;
        term1 = 0;
        term2 = 0;
        Indicator = 0;
        d = 0;

        width = cell[i].object[k].width;
        height = cell[i].object[k].height;
        maxx = width - 1;
        maxy = height - 1;
        if((maxx - minx) < (maxy - miny))
        {
          r = (maxx-minx) * 0.25;
        }
        else
        {
          r = (maxy-miny) * 0.25;
        }

        int *pointstoevaluatex = NULL;
        pointstoevaluatex = (int *) malloc (cell[i].object[k].npix * sizeof(int));
        int *pointstoevaluatey = NULL;
        pointstoevaluatey = (int *) malloc (cell[i].object[k].npix * sizeof(int));

        int *sortintensities = NULL;
        sortintensities = (int *) malloc (cell[i].object[k].npix * sizeof(int));

        for(c = 0; c < cell[i].object[k].npix; c++)
        {
          sortintensities[c] = cell[i].object[k].intensity[c];
        }

        for (b = 0; b < cell[i].object[k].npix; b++)
        {
          for (c = b + 1; c < cell[i].object[k].npix; c++)
          {
            if (sortintensities[b] > sortintensities[c])
            {
              a =  sortintensities[b];
              sortintensities[b] = sortintensities[c];
              sortintensities[c] = a;
            }
          }
        }

        for(q = 0.1; q < 0.95; q+=0.1)
        {
          Indicator = 0;
          h = ((cell[i].object[k].npix) - 1)*q + 1;
          floorh = floor(h);
          quantile = sortintensities[floorh - 1] + (h - floorh)*(sortintensities[floorh] - sortintensities[floorh-1]);

          num = 0;

          for(c = 0; c < cell[i].object[k].npix; c++)
          {
            if(cell[i].object[k].intensity[c] >= quantile)
            {
              pointstoevaluatex[num] = cell[i].object[k].xpix[c];
              pointstoevaluatey[num] = cell[i].object[k].ypix[c];
              num++;
            }
          }
          term1 = 0;
          term2 = 0;
          polyarea = 0;
          for(a = 0; a < (cell[i].boundary[k].blength - 1); a++)
          {
            term1 = term1 + (cell[i].boundary[k].xpix[a]*cell[i].boundary[k].ypix[a+1]);
            term2 = term2 + (cell[i].boundary[k].xpix[a+1]*cell[i].boundary[k].ypix[a]);
          }
          term1 = term1 + (cell[i].boundary[k].xpix[cell[i].boundary[k].blength - 1]*cell[i].boundary[k].ypix[0]);
          term2 = term2 + (cell[i].boundary[k].xpix[0]*cell[i].boundary[k].ypix[cell[i].boundary[k].blength - 1]);
          polyarea = 0.5 * abs(term1 + (cell[i].boundary[k].xpix[0] * cell[i].boundary[k].ypix[0]) - term2 - (cell[i].boundary[k].xpix[0] * cell[i].boundary[k].ypix[0]));

          for(a = 0; a < num; a++)
          {
            for(b = 0; b < num; b++)
            {
              if(a!=b)
              {
                xp = pointstoevaluatex[b];
                yp = pointstoevaluatey[b];
                xc = pointstoevaluatex[a];
                yc = pointstoevaluatey[a];

                d = sqrt(((xp - xc)*(xp - xc))+(yp - yc)*(yp - yc));
                if(d < r)
                {
                  Indicator++;
                }
              }
            }
          }
          double Kemp = (polyarea/(num * (num - 1)))*Indicator;
          double Ktheo = PI * r * r;
          cell[i].input[input].frame[k] = Kemp - Ktheo;
          input = input + 1;
        }

        free(sortintensities);
        free(pointstoevaluatex);
        free(pointstoevaluatey);

      }
    }
  }

}

/***************************************************************************
Procedure: FOstats
Description: calculates mean, stdev and skewness from an array of values
****************************************************************************/
void FOstats(double *array, int num, double *stats)
{
  int j;
  double m1, m2, m3, en, enm1, enm2;

  en = (float)(num);
  enm1 = en - 1.0;
  enm2 = en - 2.0;

  stats[0] = 0.0;
  stats[1] = 0.0;
  stats[2] = 0.0;
  for (j = 0; j < num ; j++)
  {
    stats[0] += array[j];
  }
  /* stat 0 is mean */
  stats[0] /= en;

  /* get moments */
  m1 = stats[0];
  m2 = 0.0;
  m3 = 0.0;
  for (j = 0; j < num ; j++)
  {
    m2 += (array[j]-m1) * (array[j]-m1);
    m3 += (array[j]-m1) * (array[j]-m1) * (array[j]-m1);
  }
  /* stat 1 is standard deviation */
  stats[1] = sqrt(m2/enm1);
  /* stat 2 is skewness */
  if ((m2 == 0.0) || (enm2 <= 0.0)) stats[2] = 0.0;
  else stats[2] = (m3 * en * sqrt(enm1))/(enm2 * m2 * sqrt(m2));

}

/***************************************************************************************************
Procedure:  firstOrderOriginal
Description: Extracts first order features from pixel intensity histogram of each cell in each frame
*****************************************************************************************************/
void firstOrderOriginal(C *cell, int ncells, int inputnum)
{
  int c, i, k;

  double *stats = NULL;
  stats = (double *) malloc (3 * sizeof(double));

  double *array = NULL;
  array = (double *) malloc (MAXA * sizeof(double));

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        for(c = 0; c < cell[i].object[k].npix; c++)
        {
          array[c] = (float)cell[i].object[k].intensity[c];
        }
        FOstats(array, cell[i].object[k].npix, stats);

        cell[i].input[inputnum].frame[k] = stats[0];
        cell[i].input[inputnum + 1].frame[k] = stats[1];
        cell[i].input[inputnum + 2].frame[k] = stats[2];
      }
      else
      {
        cell[i].input[inputnum].frame[k] = -1.0;
        cell[i].input[inputnum + 1].frame[k] = -1.0;
        cell[i].input[inputnum + 2].frame[k] = -1.0;
      }
    }
  }
  free(stats);
  free(array);

}

/*****************************************************************************
Procedure:  haralick
Description: calculates Haralick features from a co-occurrence matrix
*******************************************************************************/
void haralick(double *cooc, int num, double *hf)
{
  int a, b, c;
  double energy = 0.0;
  double contrast = 0.0;
  double homogeneity = 0.0;
  double correlation = 0.0;
  double entropy = 0.0;
  double rowmean = 0.0;
  double colmean = 0.0;
  double sdevx = 0.0;
  double sdevy = 0.0;
  double corrsum = 0.0;

  a = 0;
  for (c = 0; c < (NCOOC*NCOOC); c++)
  {
    energy = energy + ((cooc[c]/num)*(cooc[c]/num));
  }

  for (b = 0; b < NCOOC; b++)
  {
    for(c = 0; c < NCOOC; c++)
    {
      if(c == 0 && b > 0)
      {
        a = a + 9;
      }
      contrast = contrast + ((b - c)*(b - c)*cooc[b+c+a]/num);
      homogeneity = homogeneity + (cooc[b+c+a]/num)/(1 + (b-c)*(b-c));
      rowmean = rowmean + (b+1)*(cooc[b+c+a]/num);
      colmean = colmean + (c+1)*(cooc[b+c+a]/num);
      if(cooc[b+c+a] != 0)
      {
        entropy = entropy + ((cooc[b+c+a]/num)*(log(cooc[b+c+a]/num)));
      }
    }
  }

  a = 0;
  for(b = 0; b < NCOOC; b++)
  {
    for(c = 0; c < NCOOC; c++)
    {
      if (c == 0 && b > 0)
      {
        a = a + 9;
      }
      sdevx = sdevx + (((b+1)-rowmean)*((b+1)-rowmean)*cooc[b+c+a]/num);
      sdevy = sdevy + (((c+1)-colmean)*((c+1)-colmean)*cooc[b+c+a]/num);
      corrsum = corrsum + (((b+1)-rowmean)*((c+1)-colmean)*cooc[b+c+a]/num);
    }
  }
  sdevx = sqrt(sdevx);
  sdevy = sqrt(sdevy);
  correlation = corrsum/(sdevx*sdevy);
  entropy = entropy * -1.0;

  hf[0] = energy;
  hf[1] = contrast;
  hf[2] = homogeneity;
  hf[3] = correlation;
  hf[4] = entropy;
}

/**************************************************************************************************
Procedure:  cooccurVariables
Description: extract Haralick features from co-occurrence matrices between different wavelet levels
***************************************************************************************************/
void cooccurVariables(C *cell, int ncells, int inputnum)
{
  int i, k;

  double *hf = NULL;
  hf = (double *) malloc (5 * sizeof(double));

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        haralick(cell[i].object[k].cooc01, cell[i].object[k].lev0num, hf);
        cell[i].input[inputnum].frame[k] = hf[0];
        cell[i].input[inputnum+1].frame[k] = hf[1];
        cell[i].input[inputnum+2].frame[k] = hf[2];
        cell[i].input[inputnum+3].frame[k] = hf[3];
        cell[i].input[inputnum+4].frame[k] = hf[4];
      }
    }
  }

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        haralick(cell[i].object[k].cooc12, cell[i].object[k].lev1num, hf);
        cell[i].input[inputnum+5].frame[k] = hf[0];
        cell[i].input[inputnum+6].frame[k] = hf[1];
        cell[i].input[inputnum+7].frame[k] = hf[2];
        cell[i].input[inputnum+8].frame[k] = hf[3];
        cell[i].input[inputnum+9].frame[k] = hf[4];
      }
    }
  }

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        haralick(cell[i].object[k].cooc02, cell[i].object[k].lev0num, hf);
        cell[i].input[inputnum+10].frame[k] = hf[0];
        cell[i].input[inputnum+11].frame[k] = hf[1];
        cell[i].input[inputnum+12].frame[k] = hf[2];
        cell[i].input[inputnum+13].frame[k] = hf[3];
        cell[i].input[inputnum+14].frame[k] = hf[4];
      }
    }
  }
}


/*****************************************************************************
Procedure: pointttolinedist
Description:  calculates the distance between the point (x0, y0) and the line
*******************************************************************************/
double pointttolinedist(int x0, int y0, int x1, int y1, int x2, int y2)
{
  double fnum, fden, dist;
  int numer, denom;

  numer = (y2 - y1)*x0 - (x2 - x1)*y0 + x2*y1 - y2*x1;
  denom = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
  fnum = fabs((float)numer);
  fden = sqrt((float)denom);
  dist = fnum/fden;

  return(dist);
}

/*****************************************************************************
Procedure: polygon
Description: extracts a set of points from the cell boundary (xArray and yArray)
that form a polygon
where the sides are within thresh of the boundary
*******************************************************************************/
void polygon(int *xArray, int *yArray, int num, double thresh, int *xPoints, int *yPoints, int *nPoints)
{
  int k, i, xs, ys, x, y, numpoints, indkeep, n;
  int x0, y0, x1, y1, x2, y2;
  int alldone;
  double dist, distMax;

  xs = xArray[0];
  ys = yArray[0];
  distMax = 0.0;
  indkeep = 0;
  for (k = 0; k < num; k++)
  {
    x = xArray[k];
    y = yArray[k];
    dist = sqrt((double)((x - xs) * (x - xs) + (y - ys) * (y - ys)));
    if (dist > distMax)
    {
      distMax = dist;
      indkeep = k;
    }
  }

  int *pointArray = NULL;
  pointArray = (int *) malloc (num * sizeof(int));
  int *tempArray = NULL;
  tempArray = (int *) malloc (num * sizeof(int));
  int *done = NULL;
  done = (int *) malloc (num * sizeof(int));

  numpoints = 2;
  pointArray[0] = 0;
  pointArray[1] = indkeep;

  alldone = 0;
  while (alldone == 0)
  {
    alldone = 1;
    n = 0;
    tempArray[n] = 0;
    n++;
    for (k = 1; k < numpoints; k++)
    {
      x1 = xArray[pointArray[k-1]];
      y1 = yArray[pointArray[k-1]];
      x2 = xArray[pointArray[k]];
      y2 = yArray[pointArray[k]];
      distMax = 0.0;
      for (i = pointArray[k-1] + 1; i < pointArray[k] - 1; i++)
      {
         x0 = xArray[i];
         y0 = yArray[i];
         dist = pointttolinedist(x0, y0, x1, y1, x2, y2);
         if (dist > distMax)
         {
           distMax = dist;
           indkeep = i;
         }
       }
       if (distMax > thresh)
       {
         tempArray[n] = indkeep;
         n++;
         alldone = 0;
       }
       tempArray[n] = pointArray[k];
       n++;
    }
    done[numpoints] = 0;
    x1 = xArray[pointArray[numpoints-1]];
    y1 = yArray[pointArray[numpoints-1]];
    x2 = xArray[pointArray[0]];
    y2 = yArray[pointArray[0]];
    distMax = 0.0;
    for (i = pointArray[numpoints-1] + 1; i < num; i++)
    {
      x0 = xArray[i];
      y0 = yArray[i];
      dist = pointttolinedist(x0, y0, x1, y1, x2, y2);
      if (dist > distMax)
      {
        distMax = dist;
        indkeep = i;
      }
    }
    if (distMax > thresh)
    {
      tempArray[n] = indkeep;
      n++;
      alldone = 0;
    }
    numpoints = n;
    for (i = 0; i < numpoints; i++)
    {
      pointArray[i] = tempArray[i];
    }
  }

  for (i = 0; i < numpoints; i++)
  {
    xPoints[i] = xArray[pointArray[i]];
    yPoints[i] = yArray[pointArray[i]];
  }
  xPoints[i] = xPoints[0];
  yPoints[i] = yPoints[0];

  (*nPoints) = numpoints;

  free (pointArray);
  free (tempArray);
  free (done);
}

/*****************************************************************************
Procedure: polyclass
Description:  calculates variables from the polygonal estimates of the cells
*******************************************************************************/
void polyClass(C *cell, int ncells, int inputnum)
{
  int i, j, k, x0, y0, x1, y1, x2, y2, num;
  double dist, distMax;
  double angle, angleMin;
  double meana, vara, meand, vard;
  double Asq, Bsq, Csq, a, b;
  double thresh = 2.0;
  int nPoints;

  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      if (cell[i].missingframe[k] != 1)
      {
        num = cell[i].boundary[k].blength;
        int *xArray = NULL;
        xArray = (int *) malloc (num * sizeof(int));
        int *yArray = NULL;
        yArray = (int *) malloc (num * sizeof(int));
        for (j = 0; j < num; j++)
        {
          xArray[j] = cell[i].boundary[k].xpix[j];
          yArray[j] = cell[i].boundary[k].ypix[j];
        }
        int *xPoints = NULL;
        xPoints = (int *) malloc (num+2 * sizeof(int));
        int *yPoints = NULL;
        yPoints = (int *) malloc (num+2 * sizeof(int));
        polygon(xArray, yArray, num, thresh, xPoints, yPoints, &nPoints);

        distMax = 0.0;
        angleMin = 2.0*PI;
        meana = 0.0;
        vara = 0.0;
        meand = 0.0;
        vard = 0.0;
        xPoints[nPoints] = xPoints[0];
        yPoints[nPoints] = yPoints[0];
        xPoints[nPoints+1] = xPoints[1];
        yPoints[nPoints+1] = yPoints[2];
        for (j = 1; j < nPoints+1; j++)
        {
          x0 = xPoints[j];
          y0 = yPoints[j];
          x1 = xPoints[j-1];
          y1 = yPoints[j-1];
          x2 = xPoints[j+1];
          y2 = yPoints[j+1];
          Asq =  (double)((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
          Bsq =  (double)((x0 - x2) * (x0 - x2) + (y0 - y2) * (y0 - y2));
          Csq =  (double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
          a = sqrt(Asq);
          b = sqrt(Bsq);
          dist = a;
          if (dist > distMax) distMax = dist;
          meand = meand + dist;
          vard = vard + dist*dist;
          if (fabs((Asq + Bsq - Csq)/(2.0*a*b) - 1.0) > 0.001)
          {
            angle = acos((Asq + Bsq - Csq)/(2.0*a*b));
            if (angle < angleMin) angleMin = angle;
            meana = meana + angle;
            vara = vara + angle*angle;
          }
        }
        meand /= (float)(nPoints+1);
        vard /= (float)(nPoints+1);
        meana /= (float)(nPoints+1);
        vara /= (float)(nPoints+1);
        cell[i].input[inputnum].frame[k] = distMax;
        cell[i].input[inputnum + 1].frame[k] = angleMin;
        cell[i].input[inputnum + 2].frame[k] = vara - (meana*meana);
        cell[i].input[inputnum + 3].frame[k] = vard - (meand*meand);
        free(xArray);
        free(yArray);
      }
      else
      {
        cell[i].input[inputnum].frame[k] = -1;
        cell[i].input[inputnum + 1].frame[k] = -1;
        cell[i].input[inputnum + 2].frame[k] = -1;
        cell[i].input[inputnum + 3].frame[k] = -1;
      }
    }
  }
}

/*****************************************************************************
Procedure:  interpolate
Description:  gives values to missing frames by interpolation
******************************************************************************/
void interpolate(C *cell, int ncells, int numinput)
{
  int i, j, k, r, m;
  double value;
  for (i = 0; i < ncells; i++)
  {
    for (k = 0; k < cell[i].num; k++)
    {
      r = 0;
      if (cell[i].missingframe[k] == 1)
      {
        r = 1;
        while (cell[i].missingframe[k + r] == 1) r++;
      }
      for (j = 0; j < numinput; j++)
      {
        value = (cell[i].input[j].frame[k+r]-cell[i].input[j].frame[k-1])/(float)(r+1);
        for (m = 0; m < r; m++)
        {
          cell[i].input[j].frame[k+m] = cell[i].input[j].frame[k-1] + (m+1)*value;
        }
      }
    }
  }
}

/*****************************************************
Procedure: writedata
Description: write out all values for particular variables
******************************************************/
void writedata(C *cell, int ncells, int numinput, int nstats, char *cellgroupname, NAMES *vname,  char *classlabel)
{
  int i, j, k;
  char inputname[100];
  char varname[100];
  char levname[100];

  double minX, maxX, minY, maxY, area;

  strcpy(inputname, cellgroupname);
  strcat(inputname, "outputdata.txt\0");

  NAMES *str = NULL;
  str = (NAMES *) malloc (3*sizeof(NAMES));
  strcpy(str[0].var, "_asc\0");
  strcpy(str[1].var, "_des\0");
  strcpy(str[2].var, "_max\0");

  NAMES *lev = NULL;
  lev = (NAMES *) malloc (3*sizeof(NAMES));
  strcpy(lev[0].var, "_l1\0");
  strcpy(lev[1].var, "_l2\0");
  strcpy(lev[2].var, "_l3\0");

  NAMES *stat = NULL;
  stat = (NAMES *) malloc (nstats*sizeof(NAMES));
  strcpy(stat[0].var, "_mean\0");
  strcpy(stat[1].var, "_std\0");
  strcpy(stat[2].var, "_skew\0");
  strcpy(stat[3].var, "_kurt\0");
  strcpy(stat[4].var, "_med\0");
  strcpy(stat[5].var, "_q1\0");
  strcpy(stat[6].var, "_q2\0");

  FILE *fp = NULL;
  fp = fopen(inputname, "w");

  for (i = 0; i < ncells; i++)
  {
    minX = cell[i].input[0].frame[0];
    maxX = 0.0;
    minY = cell[i].input[1].frame[0];
    maxY = 0.0;

    for (j = 0; j < cell[i].num; j++)
    {
      if(cell[i].input[0].frame[j] < minX)
      {
        minX = cell[i].input[0].frame[j];
      }
      if(cell[i].input[1].frame[j] < minY)
      {
        minY = cell[i].input[1].frame[j];
      }
      if(cell[i].input[0].frame[j] > maxX)
      {
        maxX = cell[i].input[0].frame[j];
      }
      if(cell[i].input[1].frame[j] > maxY)
      {
        maxY = cell[i].input[1].frame[j];
      }
    }
    area = (maxX - minX) * (maxY - minY);
    cell[i].areaoftraject = area/cell[i].num;
  }

  fprintf(fp, "class cellname frames ");
  for (j = 2; j < numinput; j++)
  {
    for (k = 0; k < 3; k++)
    {
      strcpy(varname, vname[j].var);
      strcat(varname, str[k].var);
      if ((j != 9) || ((j == 9) && (k != 1))) fprintf(fp, "%s ", varname);
    }
    for (k = 0; k < 3; k++)
    {
      strcpy(varname, vname[j].var);
      strcat(varname, lev[k].var);
      for (i = 0; i < 3; i++)
      {
        strcpy(levname, varname);
        strcat(levname, str[i].var);
        if ((j != 9) || ((j == 9) && (i != 1))) fprintf(fp, "%s ", levname);
      }
    }
  }
  for (j = 2; j < numinput; j++)
  {
    for (k = 0; k < nstats; k++)
    {
      strcpy(varname, vname[j].var);
      strcat(varname, stat[k].var);
      fprintf(fp, "%s ", varname);
    }
  }
  fprintf(fp, "trajarea\n");

  for (i = 0; i < ncells; i++)
  {
    if (cell[i].num > MINLENGTH)
    {
      fprintf(fp, "%s %s%d %d ", classlabel, cellgroupname, cell[i].cellID, cell[i].num);
      for (j = 2; j < numinput; j++)
      {
        for (k = 0; k < 3; k++)
        {
          if ((j != 9) || ((j == 9) && (k != 1))) fprintf(fp, "%f ", cell[i].input[j].vars[k]);
        }
        for (k = 6; k < 3*(LEVELS + 1); k++)
        {
          if ((j != 9) || ((j == 9) && (k % 3 != 1))) fprintf(fp, "%f ", cell[i].input[j].vars[k]);
        }
      }
      for (j = 2; j < numinput; j++)
      {
        for (k = 0; k < nstats; k++)
        {
          fprintf(fp, "%f ", cell[i].input[j].stats[k]);
        }
      }
      fprintf(fp, "%f\n", cell[i].areaoftraject);
    }
  }
  fclose(fp);
}
