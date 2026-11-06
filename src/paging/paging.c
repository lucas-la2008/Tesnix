#include "paging/paging.h"
#include <lib/kstddef.h>

void set_paging(){
    for (int i = 0; i < 1024; i++){
        page_directory[i] = 0x00000002;
    }

    for (size_t i = 0; i < 1024; i++){
        page_table[i] = (i);
    }
}
