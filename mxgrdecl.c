#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <mex.h>


#include "grdecl.h"



/* Get COORD, ZCORN, ACTNUM and DIMS from mxArray.       */
/*-------------------------------------------------------*/
void mxInitGrdecl(struct Grdecl *g, const mxArray *prhs[])
{
  int i,j,k;


  g->coord    = mxGetPr(mxGetField(prhs[0], 0, "COORD"));
  double *tmp = mxGetPr(mxGetField(prhs[0], 0, "cartDims"));
  g->n = 1;
  for (i=0; i<3; ++i){
    g->dims[i] = tmp[i];
    g->n      *= tmp[i];
  }
  mexPrintf("dimensions: %d %d %d\n",
	    g->dims[0],
	    g->dims[1],
	    g->dims[2]);



  /* grdecl.actnum = permute(actnum, [3,1,2]);   */
  int *actnum  = mxGetData(mxGetField(prhs[0], 0, "ACTNUM"));
  g->actnum    = malloc(g->n*  sizeof(*g->actnum));
  int    *iptr = g->actnum;
  for (j=0; j<g->dims[1]; ++j){
    for (i=0; i<g->dims[0]; ++i){
      for (k=0; k<g->dims[2]; ++k){
	*iptr++ = actnum[i+g->dims[0]*(j+g->dims[1]*k)];

      }
    }
  }

  /* grdecl.zcorn = permute(zcorn, [3,1,2]);   */
  double *zcorn = mxGetPr(mxGetField(prhs[0], 0, "ZCORN"));
  g->zcorn      = malloc(g->n*8*sizeof(*g->zcorn));
  double *dptr  = g->zcorn;
  for (j=0; j<2*g->dims[1]; ++j){
    for (i=0; i<2*g->dims[0]; ++i){
      for (k=0; k<2*g->dims[2]; ++k){
	*dptr++ = zcorn[i+2*g->dims[0]*(j+2*g->dims[1]*k)];
      }
    }
  }
}



/* Free stuff that was allocated in initgrdecl.          */
/*-------------------------------------------------------*/
void freeGrdecl(struct Grdecl *g)
{
  free(g->zcorn);  g->zcorn  = NULL;
  free(g->actnum); g->actnum = NULL;
}
