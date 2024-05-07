#include "json.h"

#include <cctype>
#include <cmath>
#include <unordered_map>

using namespace std::string_literals;

namespace json {

    namespace {

        struct NodeContainerPrinter {
            std::ostream& out;

            void operator()(std::nullptr_t) const {
                out << "null"s;
            }
            void operator()(bool value) const {
                out << (value ? "true"s : "false"s);
            }
            void operator()(int value) const {
                out << value;
            }
            void operator()(double value) const {
                out << value;
            }
            void operator()(const std::string& value) const {
                out << '"';
                for (const char symbol : value) {
                    switch (symbol) {
                    case '\r':
                        out << "\\r"s;
                        break;
                    case '\n':
                        out << "\\n"s;
                        break;
                    case '\t':
                        out << "\\t"s;
                        break;
                    case '"':
                        out << '\\';
                        out << symbol;
                        break;
                    case '\\':
                        out << '\\';
                        out << symbol;
                        break;
                    default:
                        out << symbol;
                        break;
                    }
                }
                out << '"';
            }

            void operator()(const Dict& map) const {
                out << '{';
                int id{ 0 };
                for (const auto& [key, value] : map) {
                    if (id++ != 0)
                        out << ", "s;
                    // Print "key" in this way to take into account escape symbols
                    std::visit(NodeContainerPrinter{ out }, Node{ key }.GetValue());
                    out << ':';
                    std::visit(NodeContainerPrinter{ out }, value.GetValue());
                }
                out << '}';
            }

            void operator()(const Array& array) const {
                out << '[';

                int id{ 0 };
                for (const auto& value : array) {
                    if (id++ != 0)
                        out << ", "s;
                    std::visit(NodeContainerPrinter{ out }, value.GetValue());
                }

                out << ']';
            }
        };

        std::string LoadLetters(std::istream& input) {
            std::string result;
            while (std::isalpha(static_cast<unsigned char>(input.peek()))) {
                result.push_back(static_cast<char>(input.get()));
            }
            return result;
        }

        Node LoadNull(std::istream& input) {
            const auto value = LoadLetters(input);
            if (value == "null"s) {
                return Node{ nullptr };
            }

            throw ParsingError("parsing error");
        }

        Node LoadBool(std::istream& input) {
            std::string value = LoadLetters(input);
            if (value == "true"s) {
                return Node{ true };
            }
            if (value == "false"s) {
                return Node{ false };
            }

            throw ParsingError("parsing error");
        }

        Node LoadNode(std::istream& input);

        Node LoadArray(std::istream& input) {
            Array result;

            for (char symbol; input >> symbol && symbol != ']';) {
                if (symbol != ',')
                    input.putback(symbol);

                result.emplace_back(LoadNode(input));

                if (input >> symbol) {
                    if (symbol == ']') {
                        break;
                    }
                    if (symbol != ',') {
                        throw ParsingError("parsing error");
                    }
                }
                else {
                    throw ParsingError("parsing error");
                }
            }

            if (!input) {
                throw ParsingError("parsing error");
            }
            return Node{ std::move(result) };
        }

        Node LoadNumber(std::istream& input) {
            std::string parsed_num;
            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("parsing error");
                }
                };

            auto read_digits = [&input, read_char] {
                if (!std::isdigit(input.peek())) {
                    throw ParsingError("parsing error");
                }
                while (std::isdigit(input.peek())) {
                    read_char();
                }
                };

            if (input.peek() == '-') {
                read_char();
            }

            if (input.peek() == '0') {
                read_char();
            }
            else {
                read_digits();
            }

            bool is_int = true;
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {

                    try {
                        return Node{ std::stoi(parsed_num) };
                    }
                    catch (...) {}
                }
                return Node{ std::stod(parsed_num) };

            }
            catch (...) {
                throw ParsingError("parsing error"s);
            }
        }

        Node LoadString(std::istream& input) {
            auto position = std::istreambuf_iterator<char>(input);
            auto end = std::istreambuf_iterator<char>();

            char current;
            char escape_symbol;

            std::string result;
            while (true) {
                if (position == end) {
                    throw ParsingError("parsing error"s);
                }

                current = *position;

                if (current == '"') {
                    ++position;
                    break;
                }
                else if (current == '\\') {

                    if (++position == end) {
                        throw ParsingError("parsing error"s);
                    }

                    escape_symbol = *position;
                    switch (escape_symbol) {
                    case 'n':
                        result.push_back('\n');
                        break;
                    case 't':
                        result.push_back('\t');
                        break;
                    case 'r':
                        result.push_back('\r');
                        break;
                    case '"':
                        result.push_back('"');
                        break;
                    case '\\':
                        result.push_back('\\');
                        break;
                    default:
                        throw ParsingError("parsing error"s);
                    }
                }
                else if (current == '\n' || current == '\r') {
                    throw ParsingError("parsing error"s);
                }
                else {
                    result.push_back(current);
                }

                ++position;
            }
            return Node{ std::move(result) };
        }

        Node LoadDict(std::istream& input) {
            Dict result;

            for (char symbol; input >> symbol && symbol != '}';) {
                if (symbol == '"') {
                    std::string key = LoadString(input).AsString();
                    if (result.count(key) > 0)
                        throw ParsingError("parsing error"s);

                    if (input >> symbol && symbol != ':')
                        throw ParsingError("parsing error"s);

                    result.emplace(std::move(key), LoadNode(input));
                }
                else if (symbol != ',') {
                    throw ParsingError("parsing error"s);
                }
            }

            if (!input) {
                throw ParsingError("parsing error"s);
            }
            return Node{ std::move(result) };
        }

        Node LoadNode(std::istream& input) {
            char symbol;
            if (!(input >> symbol))
                throw ParsingError("parsing error"s);

            if (symbol == 'n') {
                input.putback(symbol);
                return LoadNull(input);
            }
            else if (symbol == 't' || symbol == 'f') {
                input.putback(symbol);
                return LoadBool(input);
            }
            else if (symbol == '[') {
                return LoadArray(input);
            }
            else if (symbol == '{') {
                return LoadDict(input);
            }
            else if (symbol == '"') {
                return LoadString(input);
            }
            else {
                input.putback(symbol);
                return LoadNumber(input);
            }
        }

    }  // namespace


    Node::Node(nullptr_t value) : value_(value){}

    Node::Node(Dict value) : value_(value) {}

    Node::Node(std::string value) : value_(value) {}

    Node::Node(int value) : value_(value) {}

    Node::Node(Array value) : value_(value) {}

    Node::Node(double value) : value_(value) {}

    Node::Node(bool value) : value_(value) {}

    bool Node::IsNull() const {
        return std::holds_alternative<std::nullptr_t>(value_);
    }
    bool Node::IsBool() const {
        return std::holds_alternative<bool>(value_);
    }
    bool Node::IsInt() const {
        return std::holds_alternative<int>(value_);
    }
    bool Node::IsDouble() const {
        return std::holds_alternative<double>(value_) || static_cast<double>(std::holds_alternative<int>(value_));
    }
    bool Node::IsPureDouble() const {
        return std::holds_alternative<double>(value_);
    }
    bool Node::IsString() const {
        return std::holds_alternative<std::string>(value_);
    }
    bool Node::IsArray() const {
        return std::holds_alternative<Array>(value_);
    }
    bool Node::IsMap() const {
        return std::holds_alternative<Dict>(value_);
    }

    const Node::Value& Node::GetValue() const {
        return value_;
    }

    bool Node::AsBool() const {
        if (value_.index() == 1) {
            return std::get<bool>(value_);
        }
        throw std::logic_error("error"s);
    }

    int Node::AsInt() const {
        if (value_.index() == 2) {
            return std::get<int>(value_);
        }
        throw std::logic_error("error"s);
    }

    double Node::AsDouble() const {

        if (value_.index() == 3) {
            return std::get<double>(value_);
        }

        if (value_.index() == 2) {
            return static_cast<double>(std::get<int>(value_));
        }

        throw std::logic_error("error"s);
    }

    const std::string& Node::AsString() const {

        if (value_.index() == 4) {
            return std::get<std::string>(value_);
        }

        throw std::logic_error("error"s);
    }

    const Array& Node::AsArray() const {

        if (value_.index() == 6) {
            return std::get<Array>(value_);
        }

        throw std::logic_error("error"s);
    }

    const Dict& Node::AsMap() const {

        if (value_.index() == 5) {
            return std::get<Dict>(value_);
        }

        throw std::logic_error("error"s);
    }


    bool operator==(const Node& left, const Node& right) {
        return left.GetValue() == right.GetValue();
    }

    bool operator!=(const Node& left, const Node& right) {
        return !(left == right);
    }


    Document::Document(Node root) : root_(std::move(root)) {}

    const Node& Document::GetRoot() const {
        return root_;
    }

    bool operator==(const Document& left, const Document& right) {
        return left.GetRoot() == right.GetRoot();
    }

    bool operator!=(const Document& left, const Document& right) {
        return !(left == right);
    }

    Document Load(std::istream& input) {
        return Document{ LoadNode(input) };
    }

    void Print(const Document& doc, std::ostream& output) {
        std::visit(NodeContainerPrinter{ output }, doc.GetRoot().GetValue());
    }

}  // namespace json