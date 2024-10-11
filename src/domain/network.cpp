#include "network.h"

namespace network {

NetworkSettings Network::GetNetworkSettings(IpRange range) const {
  NetworkSettings result{};

  std::array<uint8_t, 4> begin_bytes = ConvertIpToBytes(range.begin.address);
  std::array<uint8_t, 4> end_bytes = ConvertIpToBytes(range.end.address);

  for (size_t i = 0; i < 4; ++i) {
    if (begin_bytes[i] > end_bytes[i]) {
      //      throw std::runtime_error("Wrong order!");
    } else if (begin_bytes[i] == end_bytes[i]) {
      continue;
    } else {
      break;
    }
  }

  std::array<uint8_t, 4> network_mask_bytes = GetNetworkMask(begin_bytes, end_bytes);

  std::stringstream network_mask;
  for (size_t i = 0; i < 4; ++i) {
    network_mask << static_cast<int>(network_mask_bytes[i]) << (i < 3 ? "." : "");
  }

  result.subnet_mask = network_mask.str();

  std::array<uint8_t, 4> network_address_bytes = GetNetworkAddress(begin_bytes, network_mask_bytes);

  std::stringstream network_address;
  for (size_t i = 0; i < 4; ++i) {
    network_address << static_cast<int>(network_address_bytes[i]) << (i < 3 ? "." : "");
  }

  result.network_address = network_address.str();

  std::array<uint8_t, 4> broadcast_address_bytes = GetBroadcastAddress(network_address_bytes, network_mask_bytes);

  std::stringstream broadcast_address;
  for (size_t i = 0; i < 4; ++i) {
    broadcast_address << static_cast<int>(broadcast_address_bytes[i]) << (i < 3 ? "." : "");
  }

  result.broadcast_address = broadcast_address.str();

  auto mac_addresses = GetMacAddresses();

  result.mac_addresses = mac_addresses;

  return result;
}

std::unordered_map<std::wstring, std::string> Network::GetMacAddresses() const {
  ULONG buf_len = 15000;

  std::unique_ptr<IP_ADAPTER_ADDRESSES, std::function<void(IP_ADAPTER_ADDRESSES*)>> addresses(
      nullptr, [](IP_ADAPTER_ADDRESSES* ptr) { HeapFree(GetProcessHeap(), 0, ptr); });

  addresses.reset(reinterpret_cast<IP_ADAPTER_ADDRESSES*>(HeapAlloc(GetProcessHeap(), 0, buf_len)));

  DWORD adapters = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, addresses.get(), &buf_len);

  if (adapters != NO_ERROR) {
    throw std::runtime_error("GetAdaptersAddresses failed with error: " + std::to_string(adapters));
  }

  std::unordered_map<std::wstring, std::string> mac_addresses;

  auto current_address = addresses.get();

  while (current_address) {
    if (current_address->PhysicalAddressLength != 0) {
      std::string mac_address;

      for (unsigned int i = 0; i < current_address->PhysicalAddressLength; i++) {
        mac_address += std::to_string(static_cast<int>(current_address->PhysicalAddress[i])) +
                       (i == current_address->PhysicalAddressLength - 1 ? "" : "-");
      }

      mac_addresses[current_address->Description] = mac_address;
    }

    current_address = current_address->Next;
  }

  return mac_addresses;
}

std::array<uint8_t, 4> Network::GetNetworkMask(const std::array<uint8_t, 4>& begin,
                                               const std::array<uint8_t, 4>& end) const {
  std::array<uint8_t, 4> mask = {0, 0, 0, 0};

  bool edge = false;

  for (size_t i = 0; i < 4; ++i) {
    for (uint8_t b = 128; b >= 1; b /= 2) {
      if (!edge && ((begin[i] & b) == (end[i] & b))) {
        mask[i] |= b;
      } else {
        edge = true;
        mask[i] &= ~b;
      }
    }
  }

  return mask;
}

std::array<uint8_t, 4> Network::GetBroadcastAddress(const std::array<uint8_t, 4>& network_address,
                                                    const std::array<uint8_t, 4>& network_mask) const {
  std::array<uint8_t, 4> broadcast_address{};

  for (size_t i = 0; i < 4; ++i) {
    broadcast_address[i] = network_address[i] | (~network_mask[i]);
  }

  return broadcast_address;
}

std::array<uint8_t, 4> Network::GetNetworkAddress(const std::array<uint8_t, 4>& begin_ip,
                                                  const std::array<uint8_t, 4>& network_mask) const {
  std::array<uint8_t, 4> network_address{};

  for (size_t i = 0; i < 4; ++i) {
    network_address[i] = begin_ip[i] & network_mask[i];
  }

  return network_address;
}

std::array<uint8_t, 4> Network::ConvertIpToBytes(const std::string& ip) const {
  std::stringstream ss(ip);

  std::array<uint8_t, 4> bytes{};

  for (size_t i = 0; i < 4; ++i) {
    std::string part;

    std::getline(ss, part, '.');

    int value;
    try {
      value = std::stoi(part);
    } catch (const std::exception& e) {
      throw std::runtime_error("Wrong format!");
    }

    if (value < 0 || value > 255) {
      throw std::out_of_range("IP octet out of range");
    }

    bytes[i] = static_cast<uint8_t>(value);
  }

  if (ss.rdbuf()->in_avail() != 0) {
    throw std::invalid_argument("Invalid IP address format");
  }

  return bytes;
}

}  // namespace network
