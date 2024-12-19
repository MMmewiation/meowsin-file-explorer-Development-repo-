#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
    std::filesystem::path searchDir = "/";  // Start from the root of the system
    std::string pattern = "1234";

    for (const auto& entry : std::filesystem::recursive_directory_iterator(searchDir, std::filesystem::directory_options::skip_permission_denied)) {
        try {
            if (entry.is_regular_file()) {
                std::ifstream file(entry.path());
                if (!file) {
                    // Can't open the file (permissions, etc.)
                    continue;
                }

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
        } catch (const std::filesystem::filesystem_error& e) {
            // This will catch errors like permission issues or broken symlinks.
            // Just ignore or log them as needed.
        }
    }

    return 0;
}
