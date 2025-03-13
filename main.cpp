#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <exception>

using json = nlohmann::json;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
    try {
        std::string input_file;
        std::string output_file;
        int indent = 4;

        CLI::App app{"JSON Beautifier"};
        app.add_option("input", input_file, "Input JSON file")->required();
        app.add_option("-i,--indent", indent, "Number of spaces for indentation");
        app.add_option("-o,--output", output_file, "Output file name");
        CLI11_PARSE(app, argc, argv);

        // create a default name if output filename is not provided
        if (output_file.empty()) {
            size_t dot_pos = input_file.rfind('.');
            if (dot_pos != std::string::npos) {
                output_file = input_file.substr(0, dot_pos) + "_beautified" + input_file.substr(dot_pos);
            } else {
                output_file = input_file + "_beautified.json";
            }
        }

        // confirm overwrite
        try {
            if (fs::exists(output_file)) {
                std::cout << std::format("File '{}' already exists. Overwrite? [y/N]: ", output_file);
                char answer{};
                std::cin >> answer;
                if (!(answer == 'y' || answer == 'Y')) {
                    std::cout << std::format("Aborted: Output file '{}' will not be overwritten.\n", output_file);
                    return 0;
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << std::format("Filesystem error while checking output file: {}\n", e.what());
            return 1;
        }

        // parse
        json j;
        try {
            std::ifstream ifs(input_file);
            if (!ifs) {
                throw std::runtime_error(std::format("Could not open input file '{}'", input_file));
            }
            ifs >> j;
        } catch (const std::exception& e) {
            std::cerr << std::format("Error reading/parsing input file '{}': {}\n", input_file, e.what());
            return 1;
        }

        // measure conversion time
        std::string formatted_json;
        auto conversion_start = std::chrono::high_resolution_clock::now();
        try {
            formatted_json = j.dump(indent);
        } catch (const std::exception& e) {
            std::cerr << std::format("Error during JSON conversion: {}\n", e.what());
            return 1;
        }
        auto conversion_end = std::chrono::high_resolution_clock::now();
        auto conversion_duration = conversion_end - conversion_start;

        // choose the best suitable time unit
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(conversion_duration).count();
        std::string time_str;
        if (duration_us < 1000) {
            time_str = std::format("{} microseconds", duration_us);
        } else if (duration_us < 1000000) {
            auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(conversion_duration).count();
            time_str = std::format("{} milliseconds", duration_ms);
        } else {
            auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(conversion_duration).count();
            time_str = std::format("{} seconds", duration_s);
        }

        // write
        try {
            std::ofstream ofs(output_file);
            if (!ofs) {
                throw std::runtime_error(std::format("Could not open output file '{}'", output_file));
            }
            ofs << formatted_json << std::endl;
        } catch (const std::exception& e) {
            std::cerr << std::format("Error writing to output file '{}': {}\n", output_file, e.what());
            return 1;
        }

        std::cout << std::format("Beautified JSON written to '{}'. Conversion took {}.\n", output_file, time_str);
    } catch (const std::exception& e) {
        std::cerr << std::format("Unhandled exception: {}\n", e.what());
        return 1;
    }

    return 0;
}
