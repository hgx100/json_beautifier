#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

int main(int argc, char** argv) {
    std::string input_file;
    std::string output_file;
    int indent = 4;

    CLI::App app{"JSON Beautifier CLI Tool"};
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

    // parse file
    std::ifstream ifs(input_file);
    if (!ifs) {
        std::cerr << "Error: Could not open input file: " << input_file << std::endl;
        return 1;
    }

    json j;
    try {
        ifs >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return 1;
    }

    // write output
    std::ofstream ofs(output_file);
    if (!ofs) {
        std::cerr << "Error: Could not open output file: " << output_file << std::endl;
        return 1;
    }
    
    ofs << j.dump(indent) << std::endl;

    std::cout << "Beautified JSON written to " << output_file << std::endl;
    return 0;
}
