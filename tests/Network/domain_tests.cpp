#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "src/domain/network.h"

using namespace network;
namespace network {
class NetworkTester {
 public:
  std::unordered_map<std::wstring, std::string> GetMacAddresses() const {
    return net.GetMacAddresses();
  }
  std::array<uint8_t, 4> ConvertIpToBytes(const std::string& ip) const {
    return net.ConvertIpToBytes(ip);
  }

 private:
  Network net;
};
}  // namespace network

TEST_CASE("IP conversion works correctly", "[network]") {
  NetworkTester net{};
  std::array<uint8_t, 4> expected_ip{192, 168, 1, 1};
  std::array<uint8_t, 4> result_ip = net.ConvertIpToBytes("192.168.1.1");

  REQUIRE(result_ip == expected_ip);
}

TEST_CASE("Network settings are generated correctly", "[network]") {
  Network net;
  IpRange range{IP{"192.168.1.0"}, IP{"192.168.1.255"}};

  NetworkSettings settings = net.GetNetworkSettings(range);

  REQUIRE(settings.network_address == "192.168.1.0");
  REQUIRE(settings.broadcast_address == "192.168.1.255");
  // Добавьте больше проверок для subnet_mask и mac_addresses
}


/*Test cases
 * 192.168.1.1
192.168.3.3
144.0.0.1
192.168.2.3
129.0.0.1
134.1.2.3
 *
 */