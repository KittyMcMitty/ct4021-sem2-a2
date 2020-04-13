//
// Created by Nicholas Ives on 13/04/2020.
//
#include <stdlib.h>
void *operator new(size_t size, unsigned int) {
  return malloc(size);
}

void operator delete(void * ptr, unsigned int) {
  free(ptr);
}