#ifndef JSON_PROTOCOL_H
#define JSON_PROTOCOL_H

#include <boost/json.hpp>
#include <optional>
#include <string>

namespace json = boost::json;

class JsonProtocol {
public:
    static std::optional<json::object> parseLine(const std::string& line, std::string& errorMessage);
    static std::string serializeMessage(const json::object& message);
};

#endif
