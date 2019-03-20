#include <ecosnail/argo/parser.hpp>

#include <cassert>
#include <iomanip>
#include <sstream>
#include <utility>

namespace ecosnail::argo {

bool Parser::parse(int argc, char* argv[])
{
    if (argc > 0) {
        programName(argv[0]);
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    return parse(args);
}

void Parser::programName(std::string name)
{
    _programName = std::move(name);
}

void Parser::output(std::ostream& outputStream)
{
    _output = &outputStream;
}

void Parser::message(Message id, std::string text)
{
    _messageTexts.update(id, std::move(text));
}

void Parser::preParseCheck()
{
}

bool Parser::parse(const std::vector<std::string>& args)
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

        auto it = _argsByFlag.find(*arg);
        if (it != _argsByFlag.end()) {
            auto& data = _arguments.at(it->second);
            data->timesUsed++;

            if (data->takesArgument) {
                ++arg;
                if (arg == args.end()) {
                    *_output << message(Message::NoValueForArgument) <<
                        std::endl;
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
                _freeArgs.push_back(*arg);
            }
        }
    }

    if (helpRequested) {
        printHelp();
        return false;
    }

    postParseCheck();

    return !_messagesWritten;
}

void Parser::postParseCheck()
{
    for (const auto& data : _arguments) {
        if (data->required && data->timesUsed == 0) {
            *_output << message(Message::RequiredArgumentNotUsed) << ": ";
            for (auto it = data->flags.begin(); it != data->flags.end(); ) {
                *_output << *it++;
                for (; it != data->flags.end(); ++it) {
                    *_output << ", " << *it;
                }
            }
            *_output << "\n";
        }

        if (!data->multi && data->timesUsed > 1) {
            assert(!data->flags.empty());
            *_output << message(Message::NonMultiUsedMultipleTimes) <<
                std::endl;
        }
    }
}

const std::string& Parser::message(Message id)
{
    _messagesWritten = true;
    return _messageTexts[id];
}

void Parser::helpOption(const std::vector<std::string>& flags)
{
    for (const auto& flag : flags) {
        _helpKeys.insert(flag);
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
        for (auto it = flags.begin(); it != flags.end(); ) {
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

} // namespace ecosnail::argo
