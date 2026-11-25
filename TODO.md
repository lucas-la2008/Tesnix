# Tesnix todo list

## **• Kernel Inicialization**
- [x] Stack config
- [x] Multiboot magic number verification
- [x] Call main

## **• Memory**
### Heap
  - Initialization
    - [x] Define the heap area and set all as free
  - Allocation
    - [x] Loop through the headers in memory searching a free block
    - [x] Verify the size and allocate the desired size
    - [x] Create another memory block with the remaining memory and return the pointer
  - Deallocation
    - [x] Get the header in the memory block
    - [x] Set it as free
    - [x] Merge it with the next free block
### Physical memory manager
  - Initialization
    - [x] Bitmap sections creation using only free memory
    - [x] Set bits in all bitmaps as 0
    - [x] Protect the kernel in the bitmap
  - Allocation
    - [x] Search a free memory space
    - [x] Set it as occupied
    - [x] Return the page address
  - Deallocation
    - [x] Find the bit in the bitmap
    - [x] Set it as free
  ### Virtual memory manager
  - Allocation
    - [ ] Get the physical memory address from the [PMM](#physical-memory-manager)
