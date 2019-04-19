// Copyright (c) 2013, NetEase Inc. All rights reserved.
//
// wrt(guangguang)
// 2013/8/28
//
// Xml document utilities

#ifndef SHARED_XML_UTIL_H_
#define SHARED_XML_UTIL_H_

#include <string>
#include "third_party/tinyxml/tinyxml.h"

namespace shared
{

bool LoadXmlFromFile(TiXmlDocument &xml, const std::wstring &file_path);

}


#endif // SHARED_XML_UTIL_H_
