//
// Created by Nicholas Ives on 10/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_COMMANDS_H_
#define A_TOOLCHAIN_TEST_INCLUDE_COMMANDS_H_


#include "RadarState.h"
#include <FunctionObject.h>

// All our action objects can share this context pointer
class RadarAction : public FunctionObject {
 protected:
  friend class CommandsTest;

  explicit RadarAction();
  inline static RadarContext* context_;

 public:
  ~RadarAction() override = default;
};

// move radar
class DoMove : public RadarAction {
 private:
  inline static RadarAction* instance_ {nullptr};
  explicit DoMove() = default;
  ~DoMove() final = default;

 public:
  static RadarAction* instance(RadarContext* c);
  static void delete_instance();
  void operator()() final;
};

// ping the radar and stick it back into the context
class DoPing : public RadarAction {
 private:
  inline static RadarAction* instance_ {nullptr};
  explicit DoPing() = default;
  ~DoPing() final = default;

 public:
  static RadarAction* instance(RadarContext* c);
  static void delete_instance();
  void operator()() final;

};

// pulse LED
class DoLEDPulse : public RadarAction {
 private:
  inline static RadarAction* instance_ {nullptr};
  explicit DoLEDPulse() = default;
  ~DoLEDPulse() final = default;

 public:
  static RadarAction* instance(RadarContext* c);
  static void delete_instance();
  void operator()() final;
};


class DoPIRCheck : RadarAction {
 private:
  inline static RadarAction* instance_ {nullptr};
  uint32_t count {0};
  explicit DoPIRCheck() = default;
  ~DoPIRCheck() final = default;

 public:
  static RadarAction* instance(RadarContext* c);
  static void delete_instance();
  void operator()() final;

};
/*
class DoMemStats : RadarAction {
 private:
  inline static RadarAction* instance_ {nullptr};
  explicit DoMemStats() = default;
  ~DoMemStats() final = default;

 public:
  static RadarAction* instance(RadarContext* c);
  static void delete_instance();
  void operator()() final;
};
*/
#endif //A_TOOLCHAIN_TEST_INCLUDE_COMMANDS_H_
