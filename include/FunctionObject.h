//
// Created by Nicholas Ives on 13/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_FUNCTIONOBJECT_H_
#define A_TOOLCHAIN_TEST_INCLUDE_FUNCTIONOBJECT_H_

class FunctionObject {
 public:
  virtual void operator()() = 0;
  virtual ~FunctionObject() = default;
};


#endif //A_TOOLCHAIN_TEST_INCLUDE_FUNCTIONOBJECT_H_
