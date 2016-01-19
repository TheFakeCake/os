#include "task.h"

#include "gdt.h"
#include "pfs.h"

int exec(char *file)
{
    stat_t stats;

    if (file_stat(file, &stats))
    {
        
    }
}
