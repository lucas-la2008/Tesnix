# Tesnix todo list
---

## **• Kernel Initialization**
- [x] Stack config
- [x] Multiboot magic number verification
- [x] Call main
---
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
    - [x] Bitmap creation
    - [x] Set bits in the bitmap as 1
    - [x] Free usable memory
    - [x] Protect the kernel in the bitmap
  - Allocation
    - [ ] Search a free memory space
    - [ ] Set it as occupied
    - [ ] Return the page address
  - Deallocation
    - [ ] Find the bit in the bitmap
    - [ ] Set it as free
  ### Virtual memory manager
  - Allocation
    - [ ] Get the physical memory address from the [PMM](#physical-memory-manager)
