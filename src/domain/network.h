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

struct NetworkParams {
  IpRange ip_range;

  std::string network_address;
  std::string broadcast_address;
  std::string mac_address;
  std::string subnet_mask;
};

class Network {
 public:
  explicit Network(IpRange range);

  void SetRange(IpRange range);
  NetworkParams GetNetworkParams() const;

  std::string GetNetworkAddress() const;
  std::string GetBroadcastAddress() const;
  std::string GetMacAddress() const;
  std::string GetSubnetMask() const;

 private:
  IP ip_start_;
  IP ip_finish_;
};
}  // namespace domain
