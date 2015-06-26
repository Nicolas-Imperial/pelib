#ifndef PELIB_DL_H
#define PELIB_DL_H

void* load_lib(const char* library);
void* load_function(void *handle, const char *function);
void destroy_lib(void *handle);

#endif
