#include "sabertooth.hpp"
#include <gtest/gtest.h>

      TEST(SabertoothTest, MaxMotorSpeed) {
      auto s = rml::Sabertooth<rml::SimplifiedSerialConfig, rml::LinearInterpolation, rml::ReturnOutput>();
        auto vel = s.command(0,0,1.0f);
        vel = s.command(0, 1,1.0f);
        EXPECT_EQ(vel[0], 127);
        EXPECT_EQ(vel[1], 255);
      }
      TEST(SabertoothTest, BoundaryInvariants) {
      auto s = rml::Sabertooth<rml::SimplifiedSerialConfig, rml::LinearInterpolation, rml::ReturnOutput>();
        for (int i = -100; i <= 100; i++) {
          auto vel = s.command(0, 0, (float)i/100);
          vel = s.command(0, 1, (float)i/100);
          EXPECT_LT(vel[0], 128);
          EXPECT_GT(vel[0], 0);
          EXPECT_LT(vel[1], 256);
          EXPECT_GT(vel[1], 127);
        }
      }
  // rc::check("[Sabertooth] Check max motor speed", [&s] () {
  //     const auto full_spd = 1;
  //     RC_ASSERT(s.construct_command(0, 255)[0] == 127);
  //     RC_ASSERT(s.construct_command(0, full_spd)[1] == 255);
  //     });
  // rc::check("[Sabertooth] Command invariants", [&s]() {
  //     const auto motor1_spd = rc::gen::inRange<float>(0,2);
  //     const auto motor2_spd = rc::gen::inRange<float>(0,2);
  //     auto vel = s.command(0, *motor1_spd-1);
  //     vel = s.command(1, *motor2_spd-1);
  //     RC_ASSERT(vel[0] < 128 && vel[0] > 0);
  //     RC_ASSERT(vel[1] < 256 && vel[1] > 127);
  //     });
  // rc::check("[Sabertooth] Linear command correlation", [&s]() {
  //     const auto motor1_spd = rc::gen::inRange<float>(0,2);
  //     const auto motor2_spd = rc::gen::inRange<float>(0,2);
  //     auto vel = s.command(0, *motor1_spd-1);
  //     vel = s.command(1, *motor2_spd-1);
  //     RC_ASSERT(((vel[0]-64)/127) == *motor1_spd);
  //     });
