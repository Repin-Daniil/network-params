#pragma once

#include <string>
#include <utility>

namespace domain {

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
  std::string mac_address;
  std::string subnet_mask;
};

class Network {
 public:
  NetworkSettings GetNetworkSettings(IpRange range) const;

 private:
  std::string GetNetworkAddress(IpRange range) const;
  std::string GetBroadcastAddress(IpRange range) const;
  std::string GetSubnetMask(IpRange range) const;
  std::string GetMacAddress() const;
};
}  // namespace domain
