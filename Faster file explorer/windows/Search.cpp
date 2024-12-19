#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

std::vector<std::filesystem::path> getAvailableDrives() {
    std::vector<std::filesystem::path> drives;
#ifdef _WIN32
    // Get all drives
    DWORD mask = GetLogicalDrives();
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        if (mask & (1 << (letter - 'A'))) {
            std::string driveRoot = std::string(1, letter) + ":\\";
            drives.push_back(driveRoot);
        }
    }
#endif
    return drives;
}

int main() {
    std::string pattern = "1234";

    auto drives = getAvailableDrives();
    for (auto& drive : drives) {
        std::cout << "Searching in drive: " << drive << "\n";
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(drive, std::filesystem::directory_options::skip_permission_denied)) {
                if (entry.is_regular_file()) {
                    std::ifstream file(entry.path());
                    if (!file) continue;

                    bool found = false;
                    std::string line;
                    while (std::getline(file, line)) {
                        if (line.find(pattern) != std::string::npos) {
                            found = true;
                            break;
                        }
                    }

                    if (found) {
                        std::cout << "Found pattern \"" << pattern << "\" in file: " << entry.path() << "\n";
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            // Handle errors, possibly log them
        }
    }

    return 0;
}
