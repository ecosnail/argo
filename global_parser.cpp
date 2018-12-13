#include <ecosnail/argo/global_parser.hpp>

#include <utility>

namespace ecosnail::argo {

Parser globalParser;

bool parse(int argc, char* argv[])
{
    return globalParser.parse(argc, argv);
}

void programName(std::string name)
{
    globalParser.programName(std::move(name));
}

void output(std::ostream& outputStream)
{
    globalParser.output(outputStream);
}

void message(Message id, std::string text)
{
    globalParser.message(id, text);
}

} // namespace ecosnail::argo
