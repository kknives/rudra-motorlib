#pragma once

namespace rml {
  class SimplifiedSerialConfig {
    public:
      void construct_command();
    protected:
      ~SimplifiedSerialConfig() {}
  };
  class AffineInterpolation {
    public:
      void transform();
    protected:
      ~AffineInterpolation() {}
  };
  class SerialOutput {
    public:
      void send();
    protected:
      ~SerialOutput() {}
  };
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
class Sabertooth
  : public MotorConfig
  , public Interpolation
  , public OutputInterface
{
  public:
    Sabertooth();
    void command();
};
}
