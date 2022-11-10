#include "sabertooth.hpp"
#include <gtest/gtest.h>

TEST(SabertoothTest, EndpointSpeeds)
{
  auto s = rml::Sabertooth<rml::SimplifiedSerialConfig,
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
TEST(SabertoothTest, BoundaryInvariants)
{
  auto s = rml::Sabertooth<rml::SimplifiedSerialConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  for (int i = -100; i <= 100; i++) {
    auto vel = s.command(0, 0, (float)i / 100);
    vel = s.command(0, 1, (float)i / 100);
    EXPECT_LT(vel[0], 128);
    EXPECT_GT(vel[0], 0);
    EXPECT_LT(vel[1], 256);
    EXPECT_GT(vel[1], 127);
  }
}
TEST(SabertoothTest, NegativeCorrelation)
{
  auto s = rml::Sabertooth<rml::SimplifiedSerialConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  for (int i = -100; i < 0; i++) {
    auto vel = s.command(0, 0, (float)i / 100);
    vel = s.command(0, 1, (float)i / 100);
    EXPECT_LT(vel[0], 64);
    EXPECT_GT(vel[0], 0);
    EXPECT_LT(vel[1], 192);
    EXPECT_GT(vel[1], 127);
  }
}
TEST(SabertoothTest, PositiveCorrelation)
{
  auto s = rml::Sabertooth<rml::SimplifiedSerialConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  for (int i = 1; i < 100; i++) {
    auto vel = s.command(0, 0, (float)i / 100);
    vel = s.command(0, 1, (float)i / 100);
    EXPECT_LT(vel[0], 128);
    EXPECT_GT(vel[0], 63);
    EXPECT_LT(vel[1], 256);
    EXPECT_GT(vel[1], 191);
  }
}
TEST(LinearInterpolationTest, IncrementalCorrelation)
{
  auto s = rml::Sabertooth<rml::SimplifiedSerialConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  auto vel = s.command(0, 0, 0);
  vel = s.command(0, 0, 0);
  for (int i = 1; i < 100; i++) {
    vel = s.command(0, 0, (float)i / 100);
    EXPECT_GT(vel[0]-63, vel[1]-192);
    vel = s.command(0, 1, (float)i / 100);
  }
}
TEST(LinearInterpolationTest, DecrementalCorrelation)
{
  auto s = rml::Sabertooth<rml::SimplifiedSerialConfig,
                           rml::LinearInterpolation,
                           rml::ReturnOutput>();
  auto vel = s.command(0, 0, 0);
  vel = s.command(0, 1, 0);
  for (int i = 1; i < 100; i++) {
    vel = s.command(0, 0, (float)i / 100);
    EXPECT_LT(vel[0]-64, vel[1]-190);
    vel = s.command(0, 1, (float)i / 100);
  }
}
