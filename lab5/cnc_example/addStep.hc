
#include "Common.h"
void addStep( int i, float vb0, float va1, Context* context){
	float* vc2;
	vc2 = (float* ) malloc ( 1 * sizeof (float) );

	*vc2 = va1 + vb0;

	char* tagvc2 = createTag(1, i);
	Put(vc2, tagvc2, context->vc);
}


