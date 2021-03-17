// Copyright (c) 2012, NetEase Inc. All rights reserved.
//
// wrt(guangguang)
// 2012/5/31
//
// utilities for security associated operations

#include "base/win32/security_util.h"
#if defined(OS_WIN)
#include <aclapi.h> // ConvertStringSecurityDescriptorToSecurityDescriptorW
#include <sddl.h>	// SetSecurityInfo
#include "base/win32/win_util.h"
#include "base/win32/scoped_win_handle.h"

namespace nbase
{
namespace win32
{

static const wchar_t kLowIntegeritySddlSaclW[] = L"S:(ML;;NW;;;LW)";

bool SetObjectToLowIntegrity(HANDLE hObject, SE_OBJECT_TYPE type /*= SE_KERNEL_OBJECT*/)
{
	if(!IsRunningOnVistaOrHigher())
		return true;

	bool bRet = false;
	DWORD dwErr = ERROR_SUCCESS;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pSacl = NULL;
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;

	if (::ConvertStringSecurityDescriptorToSecurityDescriptorW(kLowIntegeritySddlSaclW, SDDL_REVISION_1, &pSD, NULL))
	{
		if (::GetSecurityDescriptorSacl(pSD, &fSaclPresent, &pSacl, &fSaclDefaulted))
		{
			dwErr = SetSecurityInfo(hObject, type, LABEL_SECURITY_INFORMATION, NULL, NULL, NULL, pSacl);
			bRet = (ERROR_SUCCESS == dwErr);
		}
		::LocalFree(pSD);
	}
	return bRet;
}

bool IsUserAdmin()
{
	SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
	PSID administrators_group = NULL;
	BOOL result = ::AllocateAndInitializeSid(&nt_authority,
											 2,
											 SECURITY_BUILTIN_DOMAIN_RID,
											 DOMAIN_ALIAS_RID_ADMINS,
											 0, 0, 0, 0, 0, 0,
											 &administrators_group);
	if (result)
	{
		if (!::CheckTokenMembership(NULL, administrators_group, &result))
			result = false;
		::FreeSid(administrators_group);
	}
	return !!result;
}

bool IsUserNonElevatedAdmin()
{
	// If pre-Vista return false;
	if (!IsRunningOnVistaOrHigher())
		return false;

	bool non_elevated_admin = false;
	ScopedWinHandle token;
	if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_READ, &token))
	{
		TOKEN_ELEVATION_TYPE elevation_type = TokenElevationTypeDefault;
		DWORD infoLen = 0;
		if (::GetTokenInformation(token.Get(),
								  TokenElevationType,
								  reinterpret_cast<void*>(&elevation_type),
								  sizeof(elevation_type),
								  &infoLen))
		{
			if (elevation_type == TokenElevationTypeLimited)
				non_elevated_admin = true;
		}
	}

	return non_elevated_admin;
}

HRESULT GetSidIntegrityLevel(PSID sid, MANDATORY_LEVEL* level)
{
	static SID_IDENTIFIER_AUTHORITY mandatory_label_auth =
		SECURITY_MANDATORY_LABEL_AUTHORITY;

	if (!IsValidSid(sid))
		return E_FAIL;

	SID_IDENTIFIER_AUTHORITY* authority = ::GetSidIdentifierAuthority(sid);
	if (!authority)
		return E_FAIL;

	if (memcmp(authority, &mandatory_label_auth, sizeof(SID_IDENTIFIER_AUTHORITY)))
		return E_FAIL;

	PUCHAR count = ::GetSidSubAuthorityCount(sid);
	if (!count || *count != 1)
		return E_FAIL;

	DWORD* rid = ::GetSidSubAuthority(sid, 0);
	if (!rid)
		return E_FAIL;

	if ((*rid & 0xFFF) != 0 || *rid > SECURITY_MANDATORY_PROTECTED_PROCESS_RID)
		return E_FAIL;

	*level = static_cast<MANDATORY_LEVEL>(*rid >> 12);
	return S_OK;
}

HRESULT GetProcessIntegrityLevel(DWORD process_id, MANDATORY_LEVEL* level)
{
	if (!IsRunningOnVistaOrHigher())
		return E_NOTIMPL;

	if (process_id == 0)
		process_id = ::GetCurrentProcessId();

	HRESULT result = E_FAIL;
	HANDLE process = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, process_id);
	if (process != NULL)
	{
		HANDLE current_token;
		if (::OpenProcessToken(process,
							   TOKEN_QUERY | TOKEN_QUERY_SOURCE,
							   &current_token))
		{
				DWORD label_size = 0;
				TOKEN_MANDATORY_LABEL* label;
				::GetTokenInformation(current_token,
									  TokenIntegrityLevel,
									  NULL,
									  0,
									  &label_size);
				if (label_size && (label = reinterpret_cast<TOKEN_MANDATORY_LABEL*>
					(::LocalAlloc(LPTR, label_size))) != NULL)
				{
						if (::GetTokenInformation(current_token,
												  TokenIntegrityLevel,
												  label,
												  label_size,
												  &label_size))
						{
							result = GetSidIntegrityLevel(label->Label.Sid, level);
						}
						::LocalFree(label);
				}
				::CloseHandle(current_token);
		}
		::CloseHandle(process);
	}
	return result;
}

} // namespace win32
} // namespace nbase

#endif // OS_WIN
