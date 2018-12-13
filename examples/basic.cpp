#include <ecosnail/argo.hpp>

#include <iostream>
#include <string>

namespace argo = ecosnail::argo;

int main(int argc, char* argv[])
{
    // TODO: fix strings (' ++ ' string is read as just '++')
    argo::helpOption("-h", "--help");
    auto message = argo::option<std::string>("-m", "--message")
        .required()
        .help("message to print");
    auto times = argo::option<int>("-t", "--times")
        .defaultValue(3)
        .help("number of times to print the message");
    auto offset = argo::option<int>("--offset")
        .help("size of fancy offset");
    auto decoration = argo::option("-d", "--decoration")
        .defaultValue("***")
        .help("characters to decorate text with");

    if (!argo::parse(argc, argv)) {
        return 1;
    }

    for (int i = 0; i < times; i++) {
        std::cout <<
            std::string(offset, ' ') << decoration << message << std::endl;
    }
}

