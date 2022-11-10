#include "smartelex.hpp"
#include "sabertooth.hpp"
#include <gtest/gtest.h>

TEST(SmartElexTest, MessageStructure) {
  auto s = rml::SmartElex<rml::SerialModeConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  auto vel = s.command(0,0, 1.0f);
  EXPECT_EQ(vel.size(), 5);
  EXPECT_EQ(vel[0], 42);
  EXPECT_EQ(vel.back(), 35);
}
TEST(SmartElexTest, EndpointSpeeds)
{
  auto s = rml::SmartElex<rml::SerialModeConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  auto vel = s.command(0,0, 1.0f);
  EXPECT_EQ((vel[1] >> 2)& 0b11, 3);
  EXPECT_GT(vel[2], 200);
  vel = s.command(0,0, 0.0f);
  EXPECT_EQ((vel[1] >> 2)& 0b11, 0);
  EXPECT_EQ(vel[2], 0);


  vel = s.command(0,1, 0.0f);
  EXPECT_EQ((vel[1])& 0b11, 0);
  EXPECT_EQ(vel[3], 0);
  vel = s.command(0,1, 1.0f);
  EXPECT_EQ((vel[1])& 0b11, 3);
  EXPECT_GT(vel[3], 200);
}
// TEST(SmartElexTest, BoundaryInvariants)
// {
//   auto s = rml::SmartElex<rml::SerialModeConfig,
//                            rml::LinearInterpolation,
//                            rml::ReturnOutput>();
//   for (int i = -100; i <= 100; i++) {
//     auto vel = s.command(0, float(i)/100, (float)i / 100);
//     EXPECT_LT(vel[2], 256);
//     EXPECT_GE(vel[2], 0);
//     EXPECT_LT(vel[3], 256);
//     EXPECT_GE(vel[3], 0);
//   }
// }
