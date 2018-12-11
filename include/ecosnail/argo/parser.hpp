#pragma once

#include <ecosnail/argo/argument.hpp>
#include <ecosnail/argo/errors.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace ecosnail::argo {

// TODO: Check flag correctness (e.g. flags start with "--")?

class Parser {
public:
    // TODO: all flags can be converted to strings?
    // TODO: at least one flag present?
    template <class Type, class... Flags>
    Argument<Type> option(Flags&&... flags)
    {
        // TODO: different mappings do not overlap?
        ((_mapping[flags] = _arguments.size()), ...);
        auto data = std::make_shared<TypedArgumentData<Type>>();
        _arguments.push_back(data);
        (data->flags.push_back(flags), ...);
        return Argument<Type>(data);
    }

    template <class Args = std::initializer_list<std::string_view>>
    bool parse(std::string_view programName, const Args& args)
    {
        bool helpRequested = false;

        for (auto arg = args.begin(); arg != args.end(); ++arg) {
            if (_helpKeys.count(*arg)) {
                helpRequested = true;
                continue;
            }

            if (auto it = _mapping.find(*arg); it != _mapping.end()) {
                auto& data = _arguments.at(it->second);

                check(data->multi || data->timesUsed == 0,
                    "a non-multi argument used multiple times");
                data->timesUsed++;

                if (data->takesArgument) {
                    ++arg;
                    check(arg != args.end(), "no value for argument");
                    data->provide(*arg);
                }
            }
        }

        if (helpRequested) {
            printHelp(programName);
            return false;
        }

        // After-parse checks
        std::ostringstream problems;

        for (const auto& data : _arguments) {
            if (data->required && data->timesUsed == 0) {
                problems << "required argument not used: ";
                if (auto it = data->flags.begin(); it != data->flags.end()) {
                    problems << *it++;
                    for (; it != data->flags.end(); ++it) {
                        problems << ", " << *it;
                    }
                }
                problems << "\n";
            }
        }

        auto problemsString = problems.str();
        if (!problemsString.empty()) {
            throw Exception(problemsString);
        }

        return !helpRequested;
    }

    bool parse(int argc, char* argv[]);

    template <class... Keys>
    void helpOption(Keys&&... keys)
    {
        (_helpKeys.insert(keys), ...);
    }

private:
    void printHelp(std::string_view programName) const;

    std::vector<std::shared_ptr<ArgumentData>> _arguments;
    std::map<std::string, size_t, std::less<>> _mapping;
    std::vector<std::string> _freeArgs;
    std::set<std::string, std::less<>> _helpKeys;
};

extern Parser globalParser;

template <class Type, class... Flags>
Argument<Type> option(Flags&&... flags)
{
    return globalParser.option<Type>(std::forward<Flags>(flags)...);
}

template <class Args = std::initializer_list<std::string_view>>
bool parse(const Args& args)
{
    return globalParser.parse(args);
}

bool parse(int argc, char* argv[]);

template <class... Keys>
void helpOption(Keys&&... keys)
{
    globalParser.helpOption(std::forward<Keys>(keys)...);
}

} // namespace ecosnail::argo
