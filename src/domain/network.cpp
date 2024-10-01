#include "network.h"

namespace domain {

Network::Network(IpRange range) : ip_start_(std::move(range.begin)), ip_finish_(std::move(range.end)) {
}

NetworkParams Network::GetNetworkParams() const {
  return NetworkParams();
}

void Network::SetRange(IpRange range) {
}

std::string Network::GetMacAddress() const {
  return std::string();
}

std::string Network::GetSubnetMask() const {
  return std::string();
}

std::string Network::GetBroadcastAddress() const {
  return std::string();
}

std::string Network::GetNetworkAddress() const {
  return std::string();
}

}  // namespace domain
