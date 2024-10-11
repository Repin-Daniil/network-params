#pragma once

#include <winsock2.h>
#include <iphlpapi.h>
#include <iptypes.h>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#pragma comment(lib, "IPHLPAPI.lib")

namespace network {

struct IP {
  std::string address;
};

struct IpRange {
  IP begin;
  IP end;
};

struct NetworkSettings {
  std::string network_address;
  std::string broadcast_address;
  std::unordered_map<std::wstring, std::string> mac_addresses;
  std::string subnet_mask;
};

class Network {
 public:
  NetworkSettings GetNetworkSettings(IpRange range) const;
  friend class NetworkTester;

 private:
  std::array<uint8_t, 4> GetNetworkAddress(const std::array<uint8_t, 4>& begin_ip,
                                           const std::array<uint8_t, 4>& network_mask) const;
  std::array<uint8_t, 4> GetBroadcastAddress(const std::array<uint8_t, 4>& network_address,
                                             const std::array<uint8_t, 4>& network_mask) const;
  std::array<uint8_t, 4> GetNetworkMask(const std::array<uint8_t, 4>& begin, const std::array<uint8_t, 4>& end) const;
  std::unordered_map<std::wstring, std::string> GetMacAddresses() const;

  std::array<uint8_t, 4> ConvertIpToBytes(const std::string& ip) const;
};
}  // namespace network
