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
                fprintf(stderr, "%s\n", dlerror());
                exit(EXIT_FAILURE);
        }

        if ((error = dlerror()) != NULL)  {
                fprintf(stderr, "%s\n", error);
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
                fprintf(stderr, "%s\n", error);
                exit(EXIT_FAILURE);
        }   

        return func;
}

void
destroy_lib(void *handle)
{
        dlclose(handle);
}

