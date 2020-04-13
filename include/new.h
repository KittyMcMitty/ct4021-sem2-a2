//
// Created by Nicholas Ives on 13/04/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_NEW_H_
#define A_TOOLCHAIN_TEST_INCLUDE_NEW_H_

#include <stdlib.h>

void *operator new(size_t size, unsigned int);
void operator delete(void * ptr, unsigned int);


#endif //A_TOOLCHAIN_TEST_INCLUDE_NEW_H_
