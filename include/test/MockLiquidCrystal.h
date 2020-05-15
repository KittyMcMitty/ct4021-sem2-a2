//
// Created by Nicholas Ives on 13/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKLIQUIDCRYSTAL_H_
#define A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKLIQUIDCRYSTAL_H_

#include <gmock/gmock.h>

class MockLiquidCrystal {
 public:
  MockLiquidCrystal();
  ~MockLiquidCrystal();
  MOCK_METHOD(void, LiquidCrystal, (uint8_t, uint8_t, uint8_t, uint8_t,
                                    uint8_t, uint8_t));
  MOCK_METHOD(void, setCursor, (uint8_t , uint8_t));
  MOCK_METHOD(void, print, (const char []));
  MOCK_METHOD(void, print, (int));
  MOCK_METHOD(void, clear, ());
  MOCK_METHOD(void, begin, (uint8_t, uint8_t));
};

class LiquidCrystalMockInterface {
 public:
  inline static MockLiquidCrystal* mock_lcd_;

  LiquidCrystalMockInterface(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1,
                             uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
    //mock_lcd_->LiquidCrystal(rs, enable, d0, d1, d2, d3);
  }

  void setCursor(uint8_t col, uint8_t row) {
    mock_lcd_->setCursor(col, row);
  }
  void print(const char str[]) {
    mock_lcd_->print(str);
  }
  void print(int num) {
    mock_lcd_->print(num);
  }
  void clear() {
    mock_lcd_->clear();
  }
  void begin(uint8_t col, uint8_t row) {
    mock_lcd_->begin(col, row);
  }
};

#endif //A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKLIQUIDCRYSTAL_H_
