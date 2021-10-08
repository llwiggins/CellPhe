#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NUMFRAMES 481
#define IMAGEWIDTH 615
#define IMAGEHEIGHT 615
/* maximum number of frames */
#define MAXF 1000
/* maximum number of cells in total */
#define MAXC 3000
/* maximum number of cells on a frame */
#define MAXCF 200
/* maximum boundary length */
#define MAXB 2000
/* maximum number of pixels within cell (area) */
#define MAXA 20000
/* minimum tracklength for cells to be considered */
#define MINLENGTH 50
/* number of levels (bins) for co-occurrence values */
#define NCOOC 10
/* number of wavelet levels */
#define LEVELS 4

#define PI 3.14159265

typedef struct varnames{
   char var[100];
}NAMES;

typedef struct inputvars{
  double *frame;
  double *stats;
  double *vars;
}INVARS;

typedef struct bpix{
  int *xpix;
  int *ypix;
  int blength;
  int xlim1;
  int xlim2;
  int ylim1;
  int ylim2;
}BOUND;

typedef struct apix{
  int *xpix;
  int *ypix;
  int *intensity;
  int npix;
  int width;
  int height;
  int *miniImage;
  int lev0num;
  int lev1num;
  int lev2num;
  double *lev0Pix;
  double *lev1Pix;
  double *lev2Pix;
  double *cooc01;
  double *cooc12;
  double *cooc02;
}AREA;

typedef struct cellinfo{
  int num;
  int cellID;
  int lineID;
  int startframe;
  int *framenum;
  int *missingframe;
  INVARS *input;
  BOUND *boundary;
  AREA *object;
  double *wave;  
  int *wl;
  double areaoftraject;
}C;

typedef struct frameinfo{
  int num;
  int *cells;
}F;


#endif
