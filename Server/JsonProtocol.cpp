#include "JsonProtocol.h"

std::optional<json::object> JsonProtocol::parseLine(const std::string& line, std::string& errorMessage) {
    boost::system::error_code ec;
    json::value parsed = json::parse(line, ec);

    if (ec) {
        errorMessage = "Invalid JSON message";
        return std::nullopt;
    }

    if (!parsed.is_object()) {
        errorMessage = "JSON message must be an object";
        return std::nullopt;
    }

    return parsed.as_object();
}

std::string JsonProtocol::serializeMessage(const json::object& message) {
    return json::serialize(message) + "\n";
}
