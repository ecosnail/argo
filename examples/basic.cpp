#include <ecosnail/argo.hpp>

#include <iostream>
#include <string>

namespace argo = ecosnail::argo;

int main(int argc, char* argv[])
{
    auto message = argo::option<std::string>("-m", "--message")
        .required();
    auto times = argo::option<int>("-t", "--times")
        .defaultValue(3)
        .help("number of times to print the message");
    auto offset = argo::option<int>("--offset")
        .help("size of fancy offset");
    auto decoration = argo::option<std::string>("-d", "--decoration")
        .defaultValue("***")
        .help("characters to decorate text with");
    argo::parse(argc, argv);

    for (int i = 0; i < times; i++) {
        std::cout << std::string(offset, ' ') << decoration << message <<
            std::endl;
    }
}

