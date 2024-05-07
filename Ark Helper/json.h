#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

    class Node;

    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node{
    public:
        using Value = std::variant<std::nullptr_t, bool, int, double, std::string, Dict, Array>;

        friend bool operator==(const Node& left, const Node& right);
        friend bool operator!=(const Node& left, const Node& right);

        Node() = default ;
        Node(const Node& value) = default;
        Node(nullptr_t value);
        Node(Dict value);
        Node(std::string value);
        Node(double value);
        Node(int value);
        Node(Array value);
        Node(bool value);

        bool IsNull() const;
        bool IsBool() const;
        bool IsInt() const;
        bool IsDouble() const;
        bool IsPureDouble() const;
        bool IsString() const;
        bool IsArray() const;
        bool IsMap() const;

        const Value& GetValue() const;
        bool AsBool() const;
        int AsInt() const;
        double AsDouble() const;
        const std::string& AsString() const;
        const Array& AsArray() const;
        const Dict& AsMap() const;

    private:
        Value value_;
    };

    class Document {
    public: 
        explicit Document(Node root);
        const Node& GetRoot() const;
        friend bool operator==(const Document& left, const Document& right);
        friend bool operator!=(const Document& left, const Document& right);

    private:
        Node root_;
    };

    Document Load(std::istream& input);

    void Print(const Document& doc, std::ostream& output);

}  // namespace json