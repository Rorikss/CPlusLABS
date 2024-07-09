//
// Created by Roriks on 5/3/2024.
//
#pragma once
#include <stack>
#include <string>
#include <vector>
#include <sstream>

namespace SearchEngine {
    class SyntaxParser {
    public:
        struct Node {
            Node* right_ = nullptr;
            Node* left_ = nullptr;

            virtual bool IsPrior() { return false; }
            virtual void MakePrior() {}

            virtual ~Node() {
                delete right_;
                delete left_;
                right_ = left_ = nullptr;
            }
        };

        struct WordNode : public Node {
            WordNode(std::string data) : data_(data) {}

            std::string data_;
        };

        struct LogicNode : public Node {
            bool prioritized_ = false;

            bool IsPrior() override {
                return prioritized_;
            }

            void MakePrior() override {
                prioritized_ = true;
            }
        };

        struct OrNode : public LogicNode {};
        struct AndNode : public LogicNode {};
        struct OpenBkt : public Node {};

        SyntaxParser();
        ~SyntaxParser();
        void MakeQuery(const std::string& query);
        auto GetRoot();
        std::vector<std::string> MakeParseOrder();

    private:
        void CheckTokens(const std::vector<std::string>& tokens);
        std::vector<std::string> ClearFromBrk(std::string& token);
        std::vector<std::string> ParseToTokens(const std::string& query);
        void StackEmptyError(const std::stack<Node*>& st);
        void BuildQueryTree(const std::vector<std::string>& tokens);
        void PostOrder(std::vector<std::string>& priority, Node* node);
        std::string NodeToText(Node* node);
        void CloseBrk(std::stack<Node*>& st);
        void HangNode(std::stack<Node*>& st, Node* rhs);

        Node* root_;
    };
} // end of namespace