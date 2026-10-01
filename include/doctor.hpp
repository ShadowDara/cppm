#pragma once

// same like flutter doctor

#include "colors.hpp"
#include "ingitignconf.hpp"

#include <unordered_map>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <iostream>


bool is_in_path(const std::string& program)
{
    const char* path_env = std::getenv("PATH");

    if (!path_env)
        return false;

#ifdef _WIN32
    constexpr char separator = ';';
#else
    constexpr char separator = ':';
#endif

    std::string path(path_env);
    std::size_t start = 0;

    while (start <= path.size())
    {
        std::size_t end = path.find(separator, start);

        std::string dir = path.substr(
            start,
            end == std::string::npos ? std::string::npos : end - start
        );

        if (!dir.empty())
        {
            std::filesystem::path candidate =
                std::filesystem::path(dir) / program;

#ifdef _WIN32
            // Windows sucht auch .exe, .cmd, .bat usw.
            if (std::filesystem::exists(candidate))
                return true;

            if (std::filesystem::exists(candidate.string() + ".exe"))
                return true;

            if (std::filesystem::exists(candidate.string() + ".cmd"))
                return true;

            if (std::filesystem::exists(candidate.string() + ".bat"))
                return true;
#else
            if (std::filesystem::exists(candidate) &&
                !std::filesystem::is_directory(candidate))
            {
                return true;
            }
#endif
        }

        if (end == std::string::npos)
            break;

        start = end + 1;
    }

    return false;
}

void doctor_print(std::string msg, bool success)
{
    if (success)
    {
        std::cout << ANSI_GREEN "[OK]" ANSI_END " " << msg << '\n';
    }
    else
    {
        std::cout << ANSI_GREEN "[FAIL]"  ANSI_END " " << msg << '\n';
    }
}

void doctor()
{
    if (is_in_path("cmake"))
    {
		doctor_print("cmake is installed and in PATH!", true);
    }
    else
    {
        doctor_print("cmake was not found in PATH!", true);
    }

	/*std::unordered_map<std::string, std::string> config = readConfig();
	if (config.empty())
	{
		std::cout << "No configuration found. Please run 'cppm init' to initialize the configuration.\n";
		return;
	}
	std::cout << "Configuration:\n";
	for (const auto& [key, value] : config)
	{
		std::cout << key << ": " << value << '\n';
	}*/
}
