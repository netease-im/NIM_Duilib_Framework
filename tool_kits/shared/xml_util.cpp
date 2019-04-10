// Copyright (c) 2013, NetEase Inc. All rights reserved.
//
// wrt(guangguang)
// 2013/8/28
//
// Xml document utilities
#include "stdafx.h"
#include "shared/xml_util.h"

#include <cstdio>
#include <memory>
#include "base/memory/deleter.h"

namespace shared
{

bool LoadXmlFromFile(TiXmlDocument &xml, const std::wstring &file_path)
{
	std::unique_ptr<FILE, nbase::DeleterFileClose> fp;
	FILE* fp_file = nullptr;
	if (_wfopen_s(&fp_file, file_path.c_str(), L"rb") != 0) {
		return false;
	}
	fp.reset(fp_file);
	if (!fp) {
		return false;
	}
	if (!xml.LoadFile(fp.get())) {
		return false;
	}
	return true;
}

}