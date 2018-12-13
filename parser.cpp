#include <ecosnail/argo/parser.hpp>

#include <cassert>
#include <iomanip>
#include <sstream>
#include <utility>

namespace ecosnail::argo {

Parser globalParser;

bool Parser::parse(int argc, char* argv[])
{
    if (argc > 0) {
        programName(argv[0]);
    }

    std::vector<std::string_view> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    return parse(args);
}

// TODO: check help keys do not overlap with others
// TODO: check all args either have flags or expect an argument
// TODO: warn about multiple multi args with no flags
// TODO: implement '=' notation

void Parser::preParseCheck()
{
}

bool Parser::parse(const std::vector<std::string_view>& args)
{
    preParseCheck();

    bool helpRequested = false;

    auto freeArgIt = _arguments.begin();
    while (freeArgIt != _arguments.end() && !(*freeArgIt)->flags.empty()) {
        ++freeArgIt;
    }

    for (auto arg = args.begin(); arg != args.end(); ++arg) {
        if (_helpKeys.count(*arg)) {
            helpRequested = true;
            continue;
        }

        if (auto it = _argsByFlag.find(*arg); it != _argsByFlag.end()) {
            auto& data = _arguments.at(it->second);
            data->timesUsed++;

            if (data->takesArgument) {
                ++arg;
                if (arg == args.end()) {
                    *_output << "no value for argument" << std::endl;
                    break;
                }
                data->provide(*arg);
            }
        } else {
            if (freeArgIt != _arguments.end()) {
                (*freeArgIt)->provide(*arg);
                if (!(*freeArgIt)->multi) {
                    while (freeArgIt != _arguments.end() &&
                            !(*freeArgIt)->flags.empty()) {
                        ++freeArgIt;
                    }
                }
            } else {
                _freeArgs.push_back(std::string(*arg));
            }
        }
    }

    if (helpRequested) {
        printHelp();
        return false;
    }

    postParseCheck();

    return !helpRequested;
}

void Parser::postParseCheck()
{
    for (const auto& data : _arguments) {
        if (data->required && data->timesUsed == 0) {
            *_output << "required argument not used: ";
            if (auto it = data->flags.begin(); it != data->flags.end()) {
                *_output << *it++;
                for (; it != data->flags.end(); ++it) {
                    *_output << ", " << *it;
                }
            }
            *_output << "\n";
        }

        if (!data->multi && data->timesUsed > 1) {
            assert(!data->flags.empty());
            *_output << "a non-multi argument used multiple times";
        }
    }
}

void Parser::printHelp() const
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

    *_output << "usage: " << _programName << "\n" <<
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
