// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/14
//
// Base64 encode/decode

#include "base64.h"
#include "base/third_party/modp_b64/modp_b64.h"

namespace nbase
{
bool Base64Encode(const std::string& input, std::string* output)
{
	std::string temp;
	temp.resize(modp_b64_encode_len(input.size()));  // makes room for null byte

	// null terminates result since result is base64 text!
	int input_size = static_cast<int>(input.size());
	int output_size= modp_b64_encode(&(temp[0]), input.data(), input_size);
	if (output_size < 0)
		return false;

	temp.resize(output_size);  // strips off null byte
	output->swap(temp);
	return true;
}

bool Base64Decode(const std::string& input, std::string* output)
{
	std::string temp;
	temp.resize(modp_b64_decode_len(input.size()));

	// does not null terminate result since result is binary data!
	int input_size = static_cast<int>(input.size());
	int output_size = modp_b64_decode(&(temp[0]), input.data(), input_size);
	if (output_size < 0)
		return false;

	temp.resize(output_size);
	output->swap(temp);
	return true;
}

}  // namespace nbase