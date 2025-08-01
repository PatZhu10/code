#include <iostream>
#include <fstream>
#include <vector>

void saveToTextFile(const std::vector<double>& data, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        // Write each value in the 1D vector to the file
        for (size_t i = 0; i < data.size(); ++i) {
            outputFile << data[i];
            if (i < data.size() - 1) {
                outputFile << "\n"; // Separate values with a tab
            }
        }
        outputFile << std::endl; // New line after all values
        outputFile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}