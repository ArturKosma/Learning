#include <filesystem>
#include <fstream>
#include <string>
#include "json.hpp"

#include "AFCooking.h"

int main(int argc, char* argv[])
{
    // No args.
    if (argc == 1)
    {
        return 1;
    }

    // Read every 3 passed args.
    for (int i = 1; i < argc; i += 4)
    {
        const std::string type = argv[i];
        const std::string source = argv[i + 1];
        const std::string target = argv[i + 2];
        const std::string additionalArgs = argv[i + 3];

        // Create json array manifest.
        nlohmann::json manifest = nlohmann::json::array();

        // Go through every file in source directory.
        for (const auto& file : std::filesystem::directory_iterator(source))
        {
            // We only care about certain file formats (.gltf or .glb).
            if (file.is_regular_file() && (file.path().extension() == ".gltf" || file.path().extension() == ".glb"))
            {
                // CookFile this file.
                const std::string& cookedName = AFCooking::CookFile(type, file.path().string(), target, additionalArgs);

                // Add it to the current manifest.
                if (!cookedName.empty())
                {
                    nlohmann::json entry;
                    entry["name"] = cookedName;
                    manifest.push_back(entry);
                }
            }
        }

        // Create json manifest at the target directory.
        const std::string& manifestPath = (std::filesystem::path(target) / "manifest.json").string();
        std::ofstream outJson(manifestPath);
        outJson << manifest.dump(2);
        printf("json manifest -> %s\n", manifestPath.c_str());
        outJson.close();
    }
}

