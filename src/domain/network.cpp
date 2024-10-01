#include "network.h"

namespace domain {

NetworkSettings Network::GetNetworkSettings(IpRange range) const {
  return NetworkSettings({"network", "broadcast", "mac\n     mac2\nmac3", "subnet"});
}

std::string Network::GetMacAddress() const {
  return std::string();
}

std::string Network::GetSubnetMask(IpRange range) const {
  return std::string();
}

std::string Network::GetBroadcastAddress(IpRange range) const {
  return std::string();
}

std::string Network::GetNetworkAddress(IpRange range) const {
  return std::string();
}

}  // namespace domain
