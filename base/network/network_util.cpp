// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/11/6
//
// Network utility function

#include "network_util.h"
#include "base/util/string_util.h"
#include <sstream>
#include <stdlib.h>

#if defined (OS_WIN)
#include <nb30.h>
#include <snmp.h>
#include <iphlpapi.h>
#else
#include <net/if_dl.h>
#if defined (OS_IOS) && !defined (IFT_ETHER)
#define IFT_ETHER   0x06 /* Ethernet CSMACD */ 
#else
#include <net/if_types.h>
#endif
#include <ifaddrs.h>
#endif

namespace nbase 
{

bool GetIpAddressList(std::vector<uint32_t> &ip_addresses)
{
    ip_addresses.clear();
    
    char host_name[128];
    if (::gethostname(host_name, sizeof(host_name)) == 0)
    {
        struct hostent *host;
        host = ::gethostbyname(host_name);
        for (int i = 0; host != NULL && host->h_addr_list[i] != NULL; ++i) 
        {
            char *ip = ::inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);
            if (ip)
            {
                ip_addresses.push_back(InetStringToNumber(ip));
            }
        }
    }
    return ip_addresses.size() > 0; 
}

bool GetIpAddressList(std::vector<std::string> &ip_addresses)
{
    ip_addresses.clear();
    
    char host_name[128];
    std::string ip_address;
    if (::gethostname(host_name, 128) == 0)
    {
        struct hostent *host;
        host = ::gethostbyname(host_name);
        for (int i = 0; host != NULL && host->h_addr_list[i] != NULL; ++i) 
        {
            char *ip = ::inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);
            if (ip)
            {
                ip_address.assign(ip, strlen(ip));
                ip_addresses.push_back(ip_address);
            }
        }
    }
    return ip_addresses.size() > 0;        
}

#if defined(OS_WIN)
typedef struct _ASTAT  
{  
	ADAPTER_STATUS adapt;  
	NAME_BUFFER NameBuffer[30];  
}ASTAT, *PASTAT;
//通过NetBIOS获取MAC地址
bool GetMacAddressByNetBIOS(std::string &mac_address)
{
	ASTAT     Adapter;  
	NCB       Ncb;  
	UCHAR     uRetCode;  
	LANA_ENUM lenum;  
	int       i;  
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBENUM;  
	Ncb.ncb_buffer  = (UCHAR *)&lenum;  
	Ncb.ncb_length  = sizeof(lenum);  
	uRetCode        = Netbios(&Ncb);  
	for (i=0; i < lenum.length; ++i)  
	{  
		memset(&Ncb, 0, sizeof(Ncb));  
		Ncb.ncb_command  = NCBRESET;  
		Ncb.ncb_lana_num = lenum.lana[i];  
		uRetCode         = Netbios(&Ncb);                                       
		memset(&Ncb, 0, sizeof(Ncb));  
		Ncb.ncb_command  = NCBASTAT;  
		Ncb.ncb_lana_num = lenum.lana[i];  
		strcpy((char *)Ncb.ncb_callname, "*                               ");  
		Ncb.ncb_buffer   = (unsigned char *)&Adapter;  
		Ncb.ncb_length   = sizeof(Adapter);  
		uRetCode         = Netbios(&Ncb);  
		if (uRetCode == 0)  
		{  
			if (Adapter.adapt.adapter_address[0]+  
				Adapter.adapt.adapter_address[1]+  
				Adapter.adapt.adapter_address[2]+  
				Adapter.adapt.adapter_address[3]+  
				Adapter.adapt.adapter_address[4]+  
				Adapter.adapt.adapter_address[5]!=0)  
			{
				StringPrintf(mac_address, "%02x-%02x-%02x-%02x-%02x-%02x",  
					Adapter.adapt.adapter_address[0],  
					Adapter.adapt.adapter_address[1],  
					Adapter.adapt.adapter_address[2],  
					Adapter.adapt.adapter_address[3],  
					Adapter.adapt.adapter_address[4],  
					Adapter.adapt.adapter_address[5]); 

				return true;  
			}  
		}
	}
	return false;
}
/*
bool ParseMac(const std::string &input, std::string &mac_address)
{
	//using boost
	const static boost::regex expression(
		"([0-9a-fA-F]{2})-([0-9a-fA-F]{2})-([0-9a-fA-F]{2})-([0-9a-fA-F]{2})-([0-9a-fA-F]{2})-([0-9a-fA-F]{2})",
		boost::regex::perl | boost::regex::icase);
	boost::cmatch what; 
	if(boost::regex_search(input.c_str(), what, expression)) 
	{
		mac_address = what[1] + "-" + what[2] + "-" + what[3] + "-" + what[4] + "-" + what[5] + "-" + what[6];
		return true;
	}
	return false;
}
//通过对控制台ipconfig /all命令重定向
bool GetMacAddressByCmd(std::string &mac_address)
{
	bool ret = false;
	//初始化返回MAC地址缓冲区
	SECURITY_ATTRIBUTES sa; 
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL; 
	sa.bInheritHandle = TRUE; 
	//创建管道
	HANDLE read_pipe = NULL;
	HANDLE write_pipe = NULL;
	if(CreatePipe(&read_pipe, &write_pipe, &sa, 0) == TRUE)
	{
		//控制命令行窗口信息
		STARTUPINFO si; 
		memset(&si, 0, sizeof(si));
		//返回进程信息
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO); 
		GetStartupInfo(&si); 
		si.hStdError = write_pipe; 
		si.hStdOutput = write_pipe; 
		si.wShowWindow = SW_HIDE; //隐藏命令行窗口
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//创建获取命令行进程
		if (::CreateProcessW(NULL, L"ipconfig /all", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == TRUE) 
		{ 
			WaitForSingleObject(pi.hProcess, 3000); // 设置超时时间，防止Vista、Win7等操作系统卡死
			unsigned long count;
			CloseHandle(write_pipe);
			std::string buffer(1024 * 10, '\0'); // 准备足够大的缓冲区
			if(ReadFile(read_pipe, const_cast<char*>(buffer.data()), buffer.size() - 1, &count, 0) == TRUE)
			{
				buffer.resize(buffer.find_first_of('\0')); // 截掉缓冲区后面多余的'\0'
				ret = ParseMac(buffer, mac_address);//提取MAC地址串
			}
			CloseHandle(pi.hThread); 
			CloseHandle(pi.hProcess); 
		}
		CloseHandle(write_pipe); // VS2010下调试，此处会有“An invalid handle was specified”的中断，直接运行正常，原因未知。VS2008上正常。
		CloseHandle(read_pipe);
	}
	return ret;
}*/

//通过SNMP(简单网络访问协议)
bool GetMacAddressBySNMP(std::string &mac_address)
{
	bool ret = false;
	WSADATA winsock_data;
	if (WSAStartup(MAKEWORD(2, 0), &winsock_data) != 0) 
		return false;

	// Load the SNMP dll and get the addresses of the functions necessary
	const HINSTANCE m_dll = LoadLibrary(L"inetmib1.dll");
	if (m_dll < (HINSTANCE) HINSTANCE_ERROR)
		return false;

	const PFNSNMPEXTENSIONINIT f_SnmpExtensionInit = (PFNSNMPEXTENSIONINIT) GetProcAddress(m_dll, "SnmpExtensionInit");
//	const PFNSNMPEXTENSIONINITEX f_SnmpExtensionInitEx = (PFNSNMPEXTENSIONINITEX) GetProcAddress(m_dll, "SnmpExtensionInitEx");
 	const PFNSNMPEXTENSIONQUERY f_SnmpExtensionQuery = (PFNSNMPEXTENSIONQUERY) GetProcAddress(m_dll, "SnmpExtensionQuery");
// 	const PFNSNMPEXTENSIONTRAP f_SnmpExtensionTrap = (PFNSNMPEXTENSIONTRAP) GetProcAddress(m_dll, "SnmpExtensionTrap");
	HANDLE poll_for_trap_event;
	AsnObjectIdentifier supported_view;
	f_SnmpExtensionInit(GetTickCount(), &poll_for_trap_event, &supported_view);

	// Initialize the variable list to be retrieved by f_SnmpExtensionQuery
	const AsnObjectIdentifier MIB_NULL = { 0, 0 };

	RFC1157VarBind var_bind[2];
	var_bind[0].name = MIB_NULL;
	var_bind[1].name = MIB_NULL;

	RFC1157VarBindList var_bind_list;
	var_bind_list.list = var_bind;

	UINT OID_ifEntryType[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 3 };
	UINT OID_ifEntryNum[] = { 1, 3, 6, 1, 2, 1, 2, 1 };
	UINT OID_ipMACEntAddr[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 6 };
	AsnObjectIdentifier MIB_ifMACEntAddr = { sizeof(OID_ipMACEntAddr) / sizeof(UINT), OID_ipMACEntAddr };
	AsnObjectIdentifier MIB_ifEntryType = { sizeof(OID_ifEntryType) / sizeof(UINT), OID_ifEntryType };
	AsnObjectIdentifier MIB_ifEntryNum = { sizeof(OID_ifEntryNum) / sizeof(UINT), OID_ifEntryNum };

	// Copy in the OID to find the number of entries in the Inteface table
	var_bind_list.len = 1;        // Only retrieving one item
	SnmpUtilOidCpy(&var_bind[0].name, &MIB_ifEntryNum);
	AsnInteger errorStatus;
	AsnInteger errorIndex;
	f_SnmpExtensionQuery(ASN_RFC1157_GETNEXTREQUEST, &var_bind_list, &errorStatus, &errorIndex);
	var_bind_list.len = 2;

	// Copy in the OID of ifType, the type of interface
	SnmpUtilOidCpy(&var_bind[0].name, &MIB_ifEntryType);

	// Copy in the OID of ifPhysAddress, the address
	SnmpUtilOidCpy(&var_bind[1].name, &MIB_ifMACEntAddr);

	for(int j = 0; j < var_bind[0].value.asnValue.number; j++)
	{
		// Submit the query.  Responses will be loaded into var_bind_list.
		// We can expect this call to succeed a # of times corresponding to the # of adapters reported to be in the system
		if(f_SnmpExtensionQuery(ASN_RFC1157_GETNEXTREQUEST, &var_bind_list, &errorStatus, &errorIndex) == FALSE)
			continue;
		// Confirm that the proper type has been returned
		if(SnmpUtilOidNCmp(&var_bind[0].name, &MIB_ifEntryType, MIB_ifEntryType.idLength) != 0)
			continue;
		// Type 6 describes ethernet interfaces
		if(var_bind[0].value.asnValue.number != 6) 
			continue;
		// Confirm that we have an address here
		if(SnmpUtilOidNCmp(&var_bind[1].name, &MIB_ifMACEntAddr, MIB_ifMACEntAddr.idLength) != 0)
			continue;
		if(var_bind[1].value.asnValue.address.stream == NULL)
			continue;
		// Ignore all dial-up networking adapters
		if ((var_bind[1].value.asnValue.address.stream[0] == 0x44)
			&& (var_bind[1].value.asnValue.address.stream[1] == 0x45)
			&& (var_bind[1].value.asnValue.address.stream[2] == 0x53)
			&& (var_bind[1].value.asnValue.address.stream[3] == 0x54)
			&& (var_bind[1].value.asnValue.address.stream[4] == 0x00)) 
			continue;
		// Ignore NULL addresses returned by other network interfaces
		if ((var_bind[1].value.asnValue.address.stream[0] == 0x00)
			&& (var_bind[1].value.asnValue.address.stream[1] == 0x00)
			&& (var_bind[1].value.asnValue.address.stream[2] == 0x00)
			&& (var_bind[1].value.asnValue.address.stream[3] == 0x00)
			&& (var_bind[1].value.asnValue.address.stream[4] == 0x00)
			&& (var_bind[1].value.asnValue.address.stream[5] == 0x00)) 
			continue;

		StringPrintf(mac_address, "%02x-%02x-%02x-%02x-%02x-%02x",
			var_bind[1].value.asnValue.address.stream[0],
			var_bind[1].value.asnValue.address.stream[1],
			var_bind[1].value.asnValue.address.stream[2],
			var_bind[1].value.asnValue.address.stream[3],
			var_bind[1].value.asnValue.address.stream[4],
			var_bind[1].value.asnValue.address.stream[5]);

		ret = true;
		break;
	}

	// Free the bindings
	SnmpUtilVarBindFree(&var_bind[0]);
	SnmpUtilVarBindFree(&var_bind[1]);
	return ret;
}
//通过GetAdaptersInfo函数（适用于Windows 2000及以上版本）
bool GetMacAddressByAdaptersInfo(std::string &mac_address)
{
	bool ret = false;

	ULONG out_buf_len = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO adapter_info = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if(adapter_info == NULL)
		return false;
	// Make an initial call to GetAdaptersInfo to get the necessary size into the out_buf_len variable
	if(GetAdaptersInfo(adapter_info, &out_buf_len) == ERROR_BUFFER_OVERFLOW) 
	{
		free(adapter_info);
		adapter_info = (IP_ADAPTER_INFO *)malloc(out_buf_len);
		if (adapter_info == NULL) 
			return false;
	}

	if(GetAdaptersInfo(adapter_info, &out_buf_len) == NO_ERROR)
	{
		PIP_ADAPTER_INFO adapter = adapter_info; 
		for(; adapter != NULL; adapter = adapter->Next)
		{
			// 确保是以太网
			if(adapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(adapter->AddressLength != 6)
				continue;

			StringPrintf(mac_address, "%02x-%02x-%02x-%02x-%02x-%02x",
				int (adapter->Address[0]),
				int (adapter->Address[1]),
				int (adapter->Address[2]),
				int (adapter->Address[3]),
				int (adapter->Address[4]),
				int (adapter->Address[5]));

			ret = true;
			break;
		}
	}

	free(adapter_info);
	return ret;
}
//通过GetAdaptersAddresses函数（适用于Windows XP及以上版本）
bool GetMacAddressByAdaptersAddresses(std::string &mac_address)
{
	bool ret = false;

	ULONG out_buf_len = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len);
	if (addresses == NULL) 
		return false;
	// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, addresses, &out_buf_len) == ERROR_BUFFER_OVERFLOW)
	{
		free(addresses);
		addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len);
		if (addresses == NULL) 
			return false;
	}

	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, addresses, &out_buf_len) == NO_ERROR)
	{
		// If successful, output some information from the data we received
		PIP_ADAPTER_ADDRESSES curr_addresses = addresses;
		for(; curr_addresses != NULL; curr_addresses = curr_addresses->Next)
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(curr_addresses->PhysicalAddressLength != 6)
				continue;

			StringPrintf(mac_address, "%02x-%02x-%02x-%02x-%02x-%02x",
				int (curr_addresses->PhysicalAddress[0]),
				int (curr_addresses->PhysicalAddress[1]),
				int (curr_addresses->PhysicalAddress[2]),
				int (curr_addresses->PhysicalAddress[3]),
				int (curr_addresses->PhysicalAddress[4]),
				int (curr_addresses->PhysicalAddress[5]));

			ret = true;
			break;
		}
	} 

	free(addresses);
	return ret;
}
#endif

bool GetMacAddress(std::string &mac_address)
{
#if defined (OS_WIN)
	//目前为止，尚未发现有任何一个通用的100%的适用于所有Windows平台的方法可以稳定的取得MAC地址。
	//优化后的解决方案：通过多种方法依次使用来提高成功率。modified by HarrisonFeng, 2014.5.23
	bool  success;
	if(true == (success = GetMacAddressByNetBIOS(mac_address)))
	{
		//LOG_PRO("---------------- GetMacAddressByNetBIOS: %s ----------------", mac_address.c_str());
		return true;
	}
	else if(true == (success = GetMacAddressBySNMP(mac_address)))
	{
		//LOG_PRO("---------------- GetMacAddressBySNMP: %s ----------------", mac_address.c_str());
		return true;
	}
	else if(true == (success = GetMacAddressByAdaptersInfo(mac_address)))
	{
		//LOG_PRO("---------------- GetMacAddressByAdaptersInfo: %s ----------------", mac_address.c_str());
		return true;
	}
	else if(true == (success = GetMacAddressByAdaptersAddresses(mac_address)))
	{
		//LOG_PRO("---------------- GetMacAddressByAdaptersAddresses: %s ----------------", mac_address.c_str());
		return true;
	}
	else
	{
		mac_address = "unknowuser00";  
	}
	return success;
#else    
    bool  success;
    struct ifaddrs *addrs;
    struct ifaddrs *cursor;
    const struct sockaddr_dl *dl_addr;
    success = (getifaddrs(&addrs) == 0);
    
    if (success) 
    {
        cursor = addrs;
        while (cursor != 0) 
        {
            if ((cursor->ifa_addr->sa_family == AF_LINK)
                && (((const struct sockaddr_dl *)cursor->ifa_addr)->sdl_type == IFT_ETHER))
            {
                dl_addr = (const struct sockaddr_dl *)cursor->ifa_addr;         
                const unsigned char* base = (const unsigned char *)&dl_addr->sdl_data[dl_addr->sdl_nlen];
                
                mac_address.clear();
                for (int i = 0; i < dl_addr->sdl_alen; i++)
                {
                    if (i != 0) 
                    {
                        mac_address.append(":");
                    }
                    char partial_addr[3];    
                    sprintf(partial_addr, "%02X", base[i]);
                    mac_address.append(partial_addr, 2);   
                }
                break;
            }
            cursor = cursor->ifa_next;
        }
        freeifaddrs(addrs);  
    } 
    return success;
#endif
}

/*
 * 检查是否为以下三类内网ip
 * A类: 10.0.0.0 ~ 10.255.255.255
 * B类: 172.16.0.0 ~ 172.31.255.255
 * C类: 192.168.0.0 ~ 192.168.255.255
 */
bool IsInternalIP(const uint32_t ip)
{
    if ((ip >= 0x0A000000 && ip <= 0x0AFFFFFF) ||
        (ip >= 0xAC100000 && ip <= 0xAC1FFFFF) ||
        (ip >= 0xC0A80000 && ip <= 0xC0A8FFFF))
        return true;
    else
        return false;    
}
    
bool IsInternalIP(const std::string &ip)
{
    return IsInternalIP(InetStringToNumber(ip));        
}

uint32_t InetStringToNumber(const char *ip)
{
    if (NULL == ip)
        return 0;
    return ntohl(::inet_addr(ip));
}
    
uint32_t InetStringToNumber(const std::string &ip)
{
    return InetStringToNumber(ip.c_str());
}
    
std::string InetNumberToString(uint32_t ip)
{
    struct in_addr in;
    in.s_addr = htonl(ip);
    return std::string(::inet_ntoa(in));
}
    
std::string MakeNetAddress(const uint32_t ip, const uint16_t port)
{
    std::stringstream buf;
    buf << InetNumberToString(ip) << ":" << port ;
    return buf.str();
}
    
std::string MakeNetAddress(const std::string &ip, const uint16_t port)
{
    std::stringstream buf;
    buf << ip << ":" << port ;
    return buf.str();      
}

bool AnalyzeNetAddress(const std::string &address, uint32_t &out_ip, uint16_t &out_port)
{
    std::list<std::string> ip_and_port;
    StringTokenize(address, ":", ip_and_port);
    if (ip_and_port.size() != 2)
        return false;
    
    out_ip = InetStringToNumber(*(ip_and_port.begin()));
    ip_and_port.pop_front();
    out_port = (uint16_t)atoi((*(ip_and_port.begin())).c_str());
    return true ;
}
    
}  // namespace nbase
