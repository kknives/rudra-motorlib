#include "smartelex.hpp"
#include "sabertooth.hpp"
#include <gtest/gtest.h>

TEST(SmartElexTest, EndpointSpeeds)
{
  auto s = rml::SmartElex<rml::SerialModeConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  auto vel = s.command(0, 0, 1.0f);
  vel = s.command(0, 1, 1.0f);
  EXPECT_EQ(vel[0], 127);
  EXPECT_EQ(vel[1], 255);
  vel = s.command(0, 0, 0.0f);
  vel = s.command(0, 1, 0.0f);
  EXPECT_EQ(vel[0], 64);
  EXPECT_EQ(vel[1], 192);
}
TEST(SmartElexTest, BoundaryInvariants)
{
  auto s = rml::SmartElex<rml::SerialModeConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  for (int i = -100; i <= 100; i++) {
    auto vel = s.command(0, float(i)/100, (float)i / 100);
    EXPECT_LT(vel[2], 256);
    EXPECT_GE(vel[2], 0);
    EXPECT_LT(vel[3], 256);
    EXPECT_GE(vel[3], 0);
  }
}
