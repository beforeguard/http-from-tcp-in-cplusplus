#pragma once

#include <string>
#include <unordered_map>

struct HttpRequest
{
	std::string method;
	std::string target;
	std::string version;
};