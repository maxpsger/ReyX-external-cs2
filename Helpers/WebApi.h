#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include <regex>
#include <stdexcept>

namespace Web
{
    inline void Get(std::string url, std::string& response)
    {
        response = "";
        // Add timeout, user-agent and disable SSL verification for GitHub raw content
        std::string cmd = "curl -s -k --connect-timeout 10 --max-time 30 -H \"User-Agent: Mozilla/5.0\" -X GET " + url;

        std::array<char, 128> buffer;
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);

        if (!pipe)
           throw std::runtime_error("failed to get curl request");
        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
            response += buffer.data();

        if (response == "")
            throw std::runtime_error("bad internet connection");

        std::regex pattern("\\d{3}:");
        if (std::regex_search(response, pattern))
            throw std::runtime_error(response);
    }
}