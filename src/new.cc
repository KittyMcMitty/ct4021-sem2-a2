//
// Created by Nicholas Ives on 13/04/2020.
//
//#include <stdlib.h>
#include <new.h>

//volatile uint32_t current_alloc, max_alloc;

void *operator new(size_t size, unsigned int) {

  /*++current_alloc;
  max_alloc = (max_alloc < current_alloc ) ? current_alloc : max_alloc;*/
  return malloc(size);
}

void operator delete(void * ptr, unsigned int) {
  free(ptr);
  //--current_alloc;
}