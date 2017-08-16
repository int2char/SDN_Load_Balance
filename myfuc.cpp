#include<math.h>
#include"mex.h"
#include"add.h"
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int i;
	i = mxGetScalar(prhs[0]);
	adder ae;
	i = ae.add(1, 2);
	if (i == 1)
		mexPrintf("hello ,world!\n");
	else
		mexPrintf("´ó¼ÒºÃ£¡%d\n");
	plhs[0] = mxCreateDoubleMatrix(2, 2, mxREAL);
	double *C = mxGetPr(plhs[0]);
	C[0]=1;
    C[1]=2;
    C[2]=3;
    C[3]=4;

}