#include "sabertooth.hpp"
#include "smartelex.hpp"
int main() {
  auto s = rml::SmartElex<rml::SerialModeConfig,
                           rml::LinearInterpolation,
                           rml::SerialOutput>();
  s.open("/dev/ttyUSB0");
  s.command_multiple(0.0f, 0.0f);
  printf("%d %d %d %d %d\n",s.last_cmd[0],s.last_cmd[1],s.last_cmd[2],s.last_cmd[3],s.last_cmd[4]);
  return 0;
}
