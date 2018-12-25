#pragma once

#include <ecosnail/argo/parser.hpp>

namespace ecosnail::argo {

extern Parser globalParser;

template <class Type = std::string, class... Flags>
Argument<Type> option(Flags&&... flags)
{
    return globalParser.option<Type>(std::forward<Flags>(flags)...);
}

bool parse(int argc, char* argv[]);

template <class Args = std::initializer_list<std::string>>
bool parse(const Args& args)
{
    return globalParser.parse(args);
}

template <class... Keys>
void helpOption(Keys&&... keys)
{
    globalParser.helpOption(std::forward<Keys>(keys)...);
}

void programName(std::string name);
void output(std::ostream& outputStream);
void message(Message id, std::string text);

} // namespace ecosnail::argo
