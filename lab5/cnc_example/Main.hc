
   /***** AUTO-GENERATED FILE from file vadd.cnc - only generated if file does not exist (on running cncc_t the first time) - feel free to edit *****/

#include "Dispatch.h"
#include <string.h>
#include <stdio.h>

#define N 8

int main(int argc, char** argv)
{

	Context* context = initGraph();

	// initialization
	float **va1 = (float **) malloc ( N*sizeof(float*));
	float **vb0 = (float **) malloc ( N*sizeof(float*));

	int i;
	for(i = 0; i < N - 0; i++){
		vb0[i] = (float* ) malloc ( 1 * sizeof (float) );
		va1[i] = (float* ) malloc ( 1 * sizeof (float) );

		*va1[i] = i;
		*vb0[i] = N-i;
	}

	finish{
		int _index0_0;
		for(_index0_0 = 0; _index0_0 < N - 0; _index0_0++){
			char* tagvb0 = createTag(1, _index0_0 + 0);
			Put(vb0[_index0_0], tagvb0, context->vb);
		}

		int _index1_0;
		for(_index1_0 = 0; _index1_0 < N - 0; _index1_0++){

			char* tagva1 = createTag(1, _index1_0 + 0);
			Put(va1[_index1_0], tagva1, context->va);
		}

		int _index2_0;
		for(_index2_0 = 0; _index2_0 < N; _index2_0++){
			char* tagaddTag2 = createTag(1, _index2_0);
			prescribeStep("addStep", tagaddTag2, context);
		}

	}

	Step* step = NULL;

	int _index3_0;
	float* vctemp3;
	float* vc3;
	vc3 = (float*) malloc ( sizeof(float) * (N - 0) );

	for(_index3_0 = 0; _index3_0 < N - 0; _index3_0++){
		char* tagvc3 = createTag(1, _index3_0 + 0);
		CNC_GET((void**) & (vctemp3), tagvc3, context->vc, step);
		vc3[_index3_0] = vctemp3[0];
		printf("%.0f ", vc3[_index3_0]);
	}
	printf("\n");

	deleteGraph(context);
}

