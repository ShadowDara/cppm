#include <iostream>

#include <string>
#include <vector>
#include <sstream>

#include <yaml-cpp/yaml.h>

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
    YAML::Node config = YAML::LoadFile(".cpp-registry.yaml");

    for (const auto& package : config["packages"])
    {
        result.push_back(parsePackage(package.as<std::string>()));
    }

    return result;
}

int main(int argc, char **argv)
{
	std::cout << "Hello";
}
