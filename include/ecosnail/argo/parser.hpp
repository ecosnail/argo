#pragma once

#include <ecosnail/argo/argument.hpp>
#include <ecosnail/argo/messages.hpp>
#include <ecosnail/argo/shared_data.hpp>

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

class Parser {
public:
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

    void programName(std::string name);
    void output(std::ostream& outputStream);
    void message(Message id, std::string text);

private:
    bool parse(const std::vector<std::string_view>& args);
    void printHelp() const;
    void preParseCheck();
    void postParseCheck();
    const std::string& message(Message id) const;

    template <class Type>
    Argument<Type> option(const std::vector<std::string_view>& flags)
    {
        for (const auto& flag : flags) {
            if (_argsByFlag.count(flag)) {
                *_output << message(Message::OptionFlagOverlaps) << ": " <<
                    flag << std::endl;
            }
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
    MessageTexts _messageTexts;
};

} // namespace ecosnail::argo
