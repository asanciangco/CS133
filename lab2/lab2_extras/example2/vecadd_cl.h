const char* vecadd_cl = 
"__kernel\n"
"void vecadd(__global int *A,\n"
"__global int *B,\n"
"__global int *C)\n"
"{\n"
"\n"
"// Get the work-item's unique ID\n"
"int idx = get_global_id(0);\n"
"\n"
"// Add the corresponding locations of\n"
"// 'A' and 'B', and store the result in 'C'.\n"
"C[idx] = A[idx] + B[idx];\n"
"}\n"
;
