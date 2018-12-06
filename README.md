argo
====

Features
--------

* Arguments are separated from parser
* Runtime tuning, no template overuse (.help(), .required())
* No artificial limitations on option format

Design limitations
------------------

* C++17 required (exact features?)
* No exception usage. All errors go to stderr, status reported as return code.

Current limitations
-------------------

* No free argument handling
* No subparsers
* No multi options

Example
-------

```c++
#include <ecosnail/argo.hpp>

#include <string>

int main(int argc, char* argv[])
{
    argo::helpOption("-h", "--help");   // option to show help
    auto message = argo::option<std::string>("-m", "--message")
        .required()
        .help("message to print");
    auto times = argo::option<int>("-t", "--times")
        .defaultValue(3)
        .help("number of times to print the message");
    auto offset = argo::option<int>("--offset")
        .help("size of fancy offset");
    auto decoration = argo::option<std::string>("-d", "--decoration")
        .defaultValue("***")
        .help("characters to decorate text with");
    
    if (argo::parse(argc, argv)) {
        return 1;
    }
    
    for (int i = 0; i < times; i++) {
        std::cout <<
            std::string(offset, ' ') << decoration << message << std::endl;
    }
}
```

