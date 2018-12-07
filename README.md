This document, as the library itself, is under development. It probably does not reflect the current state of events.

argo
====

**argo** is an experimental C++ library for parsing command line arguments. For a quick start, see the [canonical example](#example) of describing a simple command line interface. Also, consider the [limitations](#limitations): some are by design, some are meant to be fixed in the future.

Contents
--------

* [Overall description](#overall-description)
* [Basic features](#basic-features)
* [Limitations](#limitations)
* [Example](#example)

Overall description
-------------------

Parser? Options?

Multi / required

Default, if `defaultValue` is not called, is the default-constructed object.

Basic features
--------------

#### Arguments are separated from parser

The parser object and options created with it exist independently of each other. The options are not owned by the parser.

You can use an option even if the parser object no longer exists:

```c++
auto parser = std::make_unique<argo::Parser>(parser);
auto option = parser->option("--something")
    .help("an option that does nothing");
parser->parse(argc, argv);
parser.reset();

// Parser no longer exists, but you can still use the option
std::cout << option << std::endl;
```

It also works the other way around: the parser will recognize the option even if you don't hold the option object. Suppose, we have a throwaway option that does not actually affect the program:

```c++
argo::Parser parser;
parser->option("--legacy-option")
    .help("left for compatibility, does nothing");

// The parser will still recognize this option, although you now cannot access its value if it has one
parser->parse(argc, argv);
```

#### Option properties are set at runtime

Any time, in fact. You can set various option properties at the moment of option object creation, or later, before the `parse` function is called (properties set after the `parse` call will have no effect).

The most obvious way is to set all the properties at once:

```c++
auto option = parser.option<int>("-n", "--number")
    .defaultValue(3)
    .help("this is some number used in the program");
```

But you can also do it later:

```c++
auto option = parser.option<int>("-n", "--number");
option.defaultValue(3);
option.help("this is some number used in the program");
```

#### No artificial limitations on option format

There are no specific limitations on how your options should look like. You may use the POSIX/GNU approach and use single dashes for short options (`-n`) and double dashes for long ones (`--number`), or prepend them with a slash (`/number`). In fact, you can use any string (without whitespace characters) as a key.

For example, you could define your interface as follows:

```c++
auto number = argo::option<int>("number");
auto string = argo::option<std::string>("string");
```

This is how you could call such a program:

```
./program number 12 string "some message"
```

#### Options are implicitly converted to their underlying types

For each singular `Option<T>` object, an implicit conversion to `T` is defined. In some cases, you can just pass the `Option` object to where you expect the underlying value to be:

```c++
void f(int x) { /*...*/ }

auto number = argo::option<int>("-n");
argo::parse(argc, argv);

f(number);
```

It does not work in all cases.

**Examples?**

#### Global parser object

For simple cases, you don't have to explicitly create a `Parser` object. You can use the global one, managed by the library. You can use it by calling free functions where you would otherwise call member functions of a `Parser` object:

```c++
// No need to work with your own parser:
// argo::Parser parser;
// auto argument = parser.option<int>("--some-argument");

// Use the free functions:
auto argument = argo::option<int>("--some-argument");
```

Limitations
-----------

### Meant by design

* C++17 required (exact features?)
* No exception usage. All errors go to stderr, status reported as return code.
* **Not** designed with efficiency in mind. If you, for some reason, need to parse your arguments really fast, **argo** probably does not suit you.

### Present in current implementation

* No free argument handling
* No subparsers
* No multi options
* All options are case-sensitive
* Options and their values must be separated with spaces. No `=` notation (`--some-key=some-value`).

Example
-------

```c++
#include <ecosnail/argo.hpp>

#include <string>

namespace argo = ecosnail::argo;

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
