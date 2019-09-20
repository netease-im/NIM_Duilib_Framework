// Network utility function

#ifndef BASE_NETWORK_NETWORK_UTIL_H_
#define BASE_NETWORK_NETWORK_UTIL_H_

#include "nio_base.h"
#include <vector>
#include "base/base_export.h"

namespace nbase 
{
// 获取本机IP地址
BASE_EXPORT bool GetIpAddressList(std::vector<uint32_t> &ip_addresses);
BASE_EXPORT bool GetIpAddressList(std::vector<std::string> &ip_addresses);
    
// 获取本机MAC地址
BASE_EXPORT bool GetMacAddress(std::string &mac_address);
    
// 是否内网IP
BASE_EXPORT bool IsInternalIP(const uint32_t ip);
BASE_EXPORT bool IsInternalIP(const std::string &ip);
    
// 字符串IP地址转换成Number地址
BASE_EXPORT uint32_t InetStringToNumber(const char *ip);
BASE_EXPORT uint32_t InetStringToNumber(const std::string &ip);
    
// 字符串IP地址转换成Number地址
BASE_EXPORT std::string InetNumberToString(uint32_t ip);
    
// 把IP地址和端口拼成字符串
BASE_EXPORT std::string MakeNetAddress(const uint32_t ip, const uint16_t port);
BASE_EXPORT std::string MakeNetAddress(const std::string &ip, const uint16_t port);
    
// 从地址字符串中获取IP地址和端口    
BASE_EXPORT bool AnalyzeNetAddress(const std::string &address, uint32_t &out_ip, uint16_t &out_port);
}  // namespace nbase

#endif  // BASE_NETWORK_NETWORK_UTIL_H_
