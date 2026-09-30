#pragma once

#include <cstdio>
#include <string>

inline std::string curlGet(const std::string& url)
{
    std::string result;

    std::string command = "curl -s \"" + url + "\"";

    FILE* pipe = _popen(command.c_str(), "r");

    if (!pipe)
        return {};

    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;

    _pclose(pipe);

    return result;
}
