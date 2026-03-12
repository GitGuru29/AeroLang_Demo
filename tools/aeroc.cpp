#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "CodeGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printUsage() {
    std::cout << "AeroLang Compiler v0.2.0\n\n";
    std::cout << "Usage: aeroc <input.aero> [-o <output.cpp>] [--source-map]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o <file>       Specify output file (default: output.cpp)\n";
    std::cout << "  --source-map    Generate a .aero.srcmap source-map JSON file alongside the .cpp\n";
    std::cout << "  -h, --help      Show this help message\n";
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not write to file " << filename << std::endl;
        exit(1);
    }
    file << content;
}

// Serialize SourceMapEntries to a JSON string
std::string buildSourceMapJSON(const std::string& aeroFile,
                               const std::string& cppFile,
                               const std::vector<aero::SourceMapEntry>& entries) {
    std::ostringstream json;
    json << "{\n";
    json << "  \"version\": 1,\n";
    json << "  \"aeroFile\": \"" << aeroFile << "\",\n";
    json << "  \"cppFile\": \"" << cppFile << "\",\n";
    json << "  \"mappings\": [\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        json << "    { \"aeroLine\": " << entries[i].aeroLine
             << ", \"cppLine\": " << entries[i].cppLine << " }";
        if (i + 1 < entries.size()) json << ",";
        json << "\n";
    }
    json << "  ]\n";
    json << "}\n";
    return json.str();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string inputFile;
    std::string outputFile = "output.cpp";
    bool generateSourceMap = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printUsage();
            return 0;
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "Error: -o requires an argument\n";
                return 1;
            }
        } else if (arg == "--source-map") {
            generateSourceMap = true;
        } else {
            inputFile = arg;
        }
    }

    if (inputFile.empty()) {
        std::cerr << "Error: No input file specified\n";
        printUsage();
        return 1;
    }

    try {
        std::cout << "Reading " << inputFile << "...\n";
        std::string source = readFile(inputFile);

        std::cout << "Tokenizing...\n";
        aero::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        std::cout << "Found " << tokens.size() << " tokens\n";

        std::cout << "Parsing...\n";
        aero::Parser parser(tokens);
        auto program = parser.parse();
        if (!program) {
            std::cerr << "Parse failed\n";
            return 1;
        }
        std::cout << "Parse successful\n";

        std::cout << "Running semantic analysis...\n";
        aero::SemanticAnalyzer analyzer;
        if (!analyzer.analyze(*program)) {
            std::cerr << "Semantic errors found:\n";
            for (const auto& error : analyzer.getErrors()) {
                std::cerr << "  - " << error << "\n";
            }
            return 1;
        }
        std::cout << "Semantic analysis passed\n";

        std::cout << "Generating C++ code...\n";
        aero::CodeGenerator generator;
        std::string cppCode = generator.generate(*program);

        std::cout << "Writing to " << outputFile << "...\n";
        writeFile(outputFile, cppCode);

        // Optionally write the source map
        if (generateSourceMap) {
            std::string srcMapFile = inputFile + ".srcmap";
            std::string srcMapJSON = buildSourceMapJSON(inputFile, outputFile,
                                                        generator.getSourceMap());
            writeFile(srcMapFile, srcMapJSON);
            std::cout << "Source map written to " << srcMapFile << "\n";
        }

        std::cout << "Compilation successful!\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
