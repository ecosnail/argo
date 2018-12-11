#include <ecosnail/argo/parser.hpp>

#include <sstream>
#include <utility>
#include <iomanip>

namespace ecosnail::argo {

Parser globalParser;

bool Parser::parse(int argc, char* argv[])
{
    std::vector<std::string_view> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
    std::string programName = "program";
    if (argc > 0) {
        programName = argv[0];
    }
    return parse(programName, args);
}

void Parser::printHelp(std::string_view programName) const
{
    struct ArgumentInfo {
        std::string keys;
        std::string help;
    };

    size_t maxKeyStringWidth = 0;
    std::vector<ArgumentInfo> infos;
    for (const auto& argument : _arguments) {
        std::ostringstream keyStream;
        const auto& flags = argument->flags;
        if (auto it = flags.begin(); it != flags.end()) {
            keyStream << *it++;
            for (; it != flags.end(); ++it) {
                keyStream << ", " << *it;
            }
        }

        auto keyString = keyStream.str();
        auto width = keyString.length();
        if (width > maxKeyStringWidth) {
            maxKeyStringWidth = width;
        }

        infos.push_back({std::move(keyString), argument->helpText});
    }

    std::cout << "usage: " << programName << "\n" <<
        "\n" <<
        "Options:\n";
    for (const auto& info : infos) {
        std::cout << "  " << std::setw(maxKeyStringWidth) << std::left <<
            info.keys << " " << info.help << "\n";
    }

    std::cout.flush();
}

bool parse(int argc, char* argv[])
{
    return globalParser.parse(argc, argv);
}

} // namespace ecosnail::argo
