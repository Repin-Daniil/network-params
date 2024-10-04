#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <array>
#include <winsock2.h>
#include <iphlpapi.h>
#include <memory>
#include <functional>
#include <stdexcept>

#pragma comment(lib, "IPHLPAPI.lib")

namespace functions {

std::array<uint8_t, 4> ConvertIpToBytes(const std::string& ip) {
    std::stringstream ss(ip);

    std::array<uint8_t, 4> bytes{};

    for (size_t i = 0; i < 4; ++i) {
        std::string part;

        std::getline(ss, part, '.');

        bytes[i] = static_cast<uint8_t>(std::stoi(part));
    }

    return bytes;
}


std::array<uint8_t, 4> CalculateNetworkMask(const std::array<uint8_t, 4>& begin, const std::array<uint8_t, 4>& end) {
    std::array<uint8_t, 4> mask = { 0, 0, 0, 0 };

    bool has_difference = false;

    for (size_t i = 0; i < 4; ++i) {
        for (uint8_t b = 128; b >= 1; b /= 2) {
            if (!has_difference && ((begin[i] & b) == (end[i] & b))) {
                mask[i] |= b;
            } else {
                has_difference = true;
                mask[i] &= ~b;
            }
        }
    }

    return mask;
}

std::array<uint8_t, 4> CalculateNetworkAddress(const std::array<uint8_t, 4>& beginIP, const std::array<uint8_t, 4>& networkMask) {
    std::array<uint8_t, 4> network_address{};

    for (size_t i = 0; i < 4; ++i) {
        network_address[i] = beginIP[i] & networkMask[i];
    }

    return network_address;
}

std::array<uint8_t, 4> CalculateBroadcastAddress(const std::array<uint8_t, 4>& networkAddress, const std::array<uint8_t, 4>& networkMask) {
    std::array<uint8_t, 4> broadcast_address{};

    for (size_t i = 0; i < 4; ++i) {
        broadcast_address[i] = networkAddress[i] | (~networkMask[i]);
    }

    return broadcast_address;
}

std::unordered_map<std::wstring, std::string> GetMacAddresses() {
    ULONG buf_len = 15000;

    std::unique_ptr<IP_ADAPTER_ADDRESSES, std::function<void(IP_ADAPTER_ADDRESSES *)>> addresses(
            nullptr, [](IP_ADAPTER_ADDRESSES *ptr) { HeapFree(GetProcessHeap(), 0, ptr); }
    );

    addresses.reset(reinterpret_cast<IP_ADAPTER_ADDRESSES *>(HeapAlloc(GetProcessHeap(), 0, buf_len)));

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
}