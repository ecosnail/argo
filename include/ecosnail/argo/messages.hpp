#pragma once

#include <cassert>
#include <map>

namespace ecosnail::argo {

enum class Message {
#   define MESSAGE(ID, TEXT) ID,
#   include <ecosnail/argo/messages.inc>
#   undef MESSAGE
};

class MessageTexts {
public:
    const std::string& operator[](Message message) const
    {
        assert(_texts.count(message));
        return _texts.at(message);
    }

    void update(Message message, std::string&& text)
    {
        _texts[message] = std::move(text);
    }

private:
    std::map<Message, std::string> _texts {
#       define MESSAGE(ID, TEXT) {Message::ID, TEXT},
#       include <ecosnail/argo/messages.inc>
#       undef MESSAGE
    };
};

} // namespace ecosnail::argo
