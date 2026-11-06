#include "paging/paging.h"

void set_paging(){
    for (int i = 0; i < 1024; i++){
        page_directory[i] = 0x00000002;
    }
}
