#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include <pelib/dl.h>

void*
load_lib(const char* library)
{
        void *handle;
        char *error;

        handle = dlopen(library, RTLD_LAZY);
        if (!handle) {
                fprintf(stderr, "[%s:%s:%d] Cannot open shared library: %s\n", __FILE__, __FUNCTION__, __LINE__, dlerror());
                exit(EXIT_FAILURE);
        }

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "[%s:%s:%d] Error while opening shared library: %s\n", __FILE__, __FUNCTION__, __LINE__, error);
                exit(EXIT_FAILURE);
        }   

        return handle;
}

void*
load_function(void *handle, const char *function)
{
        dlerror();    /* Clear any existing error */

        void *func = (void*)dlsym(handle, function);
        char *error;

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "[%s:%s:%d] Error while loading function: %s\n", __FILE__, __FUNCTION__, __LINE__, error);
                exit(EXIT_FAILURE);
        }   

        return func;
}

void
destroy_lib(void *handle)
{
        dlclose(handle);
}

