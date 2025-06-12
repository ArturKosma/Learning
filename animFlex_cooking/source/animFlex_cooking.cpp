#include <filesystem>
#include <string>

#include "AFCooking.h"

int main(int argc, char* argv[])
{
    // No args.
    if (argc == 1)
    {
        return 1;
    }

    // Read every 3 passed args.
    for (int i = 1; i < argc; i+=3)
    {
        const std::string type = argv[i];
        const std::string source = argv[i + 1];
        const std::string target = argv[i + 2];

        // Go through every file in source directory.
        for (const auto& file : std::filesystem::directory_iterator(source))
        {
            // We only care about certain file formats (.gltf or .glb).
            if (file.is_regular_file() && (file.path().extension() == ".gltf" || file.path().extension() == ".glb"))
            {
                // CookFile this file.
                AFCooking::CookFile(type, file.path().string(), target);
            }
        }
    }
}

