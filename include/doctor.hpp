#pragma once

// same like flutter doctor

#include "colors.hpp"
#include "ingitignconf.hpp"
#include "config.hpp"
#include "../generated_version.hpp"

#include <unordered_map>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <iostream>
#include <filesystem>


inline bool is_in_path(const std::string& program)
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

inline void doctor_print(std::string msg, bool success, int& suc)
{
    if (success)
    {
        std::cout << ANSI_GREEN "[OK]" ANSI_END " " << msg << '\n';
    }
    else
    {
        std::cout << ANSI_RED "[FAIL]"  ANSI_END " " << msg << '\n';
        suc++;
    }
}

inline bool exists(std::string path)
{
    return std::filesystem::is_regular_file(path);
}

#define s no_success_count

inline void doctor()
{
	int no_success_count = 0;

	std::cout << "Running cppm doctor...\n";

	doctor_print("cppm" BUILD_MESSAGE, true, s);

    // CMAKE
    if (is_in_path("cmake"))
    {
		doctor_print("cmake is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("cmake was not found in PATH!", true, s);
    }

	// CMAKE_SETTINGS_FILE
    if (exists(CMAKE_SETTINGS_FILE))
    {
        doctor_print("CMakeSettings.json exists!", true, s);
    }
    else
    {
        doctor_print("CMakeSettings.json does not exist!", false,s);
	}

    // GIT
    if (is_in_path("git"))
    {
        doctor_print("git is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("git was not found in PATH!", false, s);
	}

    // CL EXE MSVC COMPILER
#if WIN32
    // CL
    if (is_in_path("cl"))
    {
        doctor_print("cl is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("cl was not found in PATH!", false, s);
	}
#endif

    if (is_in_path("gcc"))
    {
        doctor_print("gcc is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("gcc was not found in PATH!", false, s);
    }

    if (is_in_path("g++"))
    {
        doctor_print("g++ is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("g++ was not found in PATH!", false, s);
    }

    if (is_in_path("clang"))
    {
        doctor_print("clang is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("clang was not found in PATH!", false, s);
    }

    if (is_in_path("clang++"))
    {
        doctor_print("clang++ is installed and in PATH!", true, s);
    }
    else
    {
        doctor_print("clang++ was not found in PATH!", false, s);
    }

	std::cout << "\nDoctor finished with " << s << " errors!\n";
}
