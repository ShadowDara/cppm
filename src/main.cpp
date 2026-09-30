#include <iostream>

#include <string>
#include <vector>
#include <sstream>

#include <yaml-cpp/yaml.h>

#include "argparser.h"
#include "curl.hpp"

#include "../version.hpp"
#include "../json.hpp"

using namespace nlohmann;

struct Package
{
	std::string pkgname;
	std::string owner;
};

Package parsePackage(std::string input)
{
    bool first = true;
    std::stringstream ss(input);
    std::string item;
    Package p;

    while (std::getline(ss, item, '/')) {
        if (first)
        {
            p.owner = item;
            first = false;
        }
        else
        {
            p.pkgname = item;
            return p;
        }
    }

    return p;
}

std::vector<Package> parseConf()
{
    std::vector<Package> result;

    try
    {
        YAML::Node config = YAML::LoadFile(".cpp-registry.yaml");

        for (const auto& package : config["packages"])
        {
            result.push_back(
                parsePackage(package.as<std::string>())
            );
        }
    }
    catch (const YAML::Exception&)
    {
        return {};
    }

    return result;
}

int main(int argc, char **argv)
{
    auto conf = parseConf();

    ArgCommand* root = arg_command_new(NAME, "", "", false, NULL, 0);

    const char* name_aliases[] = { "n" };
    ArgCommand* search = arg_command_new("search", "search for libarys.", "", false, NULL, 0);
    arg_command_string(search, "name", "", "File name or pattern to search for.", true, name_aliases, 1);
    
    const char* aliases[] = { "v", "-v", "--v", "--version" };
    ArgCommand* version = arg_command_new("version", "Displays the version", "", false, aliases, 4);

    arg_command_add_subcommand(root, search);
    arg_command_add_subcommand(root, version);

    // Parsed command
    ArgCommand* cmd = arg_command_parse(root, argc - 1, argv + 1);

    if (cmd == search)
    {
        const char* name = arg_command_get_string(cmd, "name");
        
        if (name == nullptr || name[0] == '\0')
        {
            return 1;
        }

        auto data = curlGet(REGISTRY);

        if (data.empty())
        {
            return 1;
        }

        json j = json::parse(data);

        json package = j[name];

        if (!package)
        {
            std::cout << "Package not found: " << name << '\n';
            return 1;
        }
        
        std::cout << "Package: " << name << '\n';
        
        for (const auto& [key, value] : package.items())
        {
            std::cout << key << ": ";
            
            if (value.is_string()) std::cout << value.get<std::string>();
            else std::cout << value.dump(); std::cout << '\n';
        }
    }
    else if (cmd == version)
    {
        std::cout << VERSION "\n";
    }
    else
    {
        std::cout << NAME " - package viewer for the cpp registry: https://cpp-registry.github.io\n";
        if (conf.size() > 0)
        {
            std::cout << "\nPackages:\n";
            for (const auto& e : conf)
            {
                std::cout << e.owner << "/" << e.pkgname << "\n";
            }
        }
    }
    
    arg_command_free(root);

    return 0;
}
