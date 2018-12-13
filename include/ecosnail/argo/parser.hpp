#pragma once

#include <ecosnail/argo/argument.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace ecosnail::argo {

// TODO: check default value not present for required arguments

class Parser {
public:
    // TODO: all flags can be converted to strings?
    // TODO: at least one flag present?
    template <class Type = std::string, class... Flags>
    Argument<Type> option(Flags&&... flags)
    {
        return option<Type>({flags...});
    }

    bool parse(int argc, char* argv[]);

    template <class... Args>
    bool parse(Args&&... args)
    {
        return parse({std::forward<Args>(args)...});
    }

    template <class... Keys>
    void helpOption(Keys&&... keys)
    {
        (_helpKeys.insert(keys), ...);
    }

    void programName(std::string name)
    {
        _programName = std::move(name);
    }

    void output(std::ostream& outputStream)
    {
        _output = &outputStream;
    }

private:
    bool parse(const std::vector<std::string_view>& args);
    void printHelp() const;
    void preParseCheck();
    void postParseCheck();

    template <class Type>
    Argument<Type> option(const std::vector<std::string_view>& flags)
    {
        // TODO: different mappings do not overlap?

        for (const auto& flag : flags) {
            _argsByFlag[std::string(flag)] = _arguments.size();
        }
        auto data = std::make_shared<TypedArgumentData<Type>>();
        _arguments.push_back(data);

        for (const auto& flag : flags) {
            data->flags.push_back(std::string(flag));
        }

        return Argument<Type>(data);
    }

    std::string _programName = "<program>";
    std::ostream* _output = &std::cout;
    std::vector<std::shared_ptr<ArgumentData>> _arguments;
    std::map<std::string, size_t, std::less<>> _argsByFlag;
    std::vector<std::string> _freeArgs;
    std::set<std::string, std::less<>> _helpKeys;
};

extern Parser globalParser;

template <class Type = std::string, class... Flags>
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
