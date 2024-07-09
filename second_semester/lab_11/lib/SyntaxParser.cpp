//
// Created by Roriks on 5/3/2024.
//
#include "SyntaxParser.h"

SearchEngine::SyntaxParser::SyntaxParser() : root_(nullptr) {}

SearchEngine::SyntaxParser::~SyntaxParser() {
    delete root_;
    root_ = nullptr;
}

void SearchEngine::SyntaxParser::MakeQuery(const std::string& query)  {
    auto tokens = ParseToTokens(query);
    CheckTokens(tokens);
    BuildQueryTree(tokens);
}

auto SearchEngine::SyntaxParser::GetRoot()  {
    return root_;
}

std::vector<std::string> SearchEngine::SyntaxParser::MakeParseOrder()  {
    std::vector<std::string> order;
    PostOrder(order, root_);
    return order;
}

void SearchEngine::SyntaxParser::CheckTokens(const std::vector<std::string>& tokens)  {
    if (tokens.size() == 1) {
        return;
    }
    for (size_t i = 0; i < tokens.size(); ++i) {
        if ((tokens[i] == ")" || tokens[i] == "OR" || tokens[i] == "AND") &&
            (!i || i && (tokens[i - 1] == "OR" || tokens[i - 1] == "AND"))) {
            throw std::runtime_error("Invalid query");
        }
    }
}

std::vector<std::string> SearchEngine::SyntaxParser::ClearFromBrk(std::string& token) {
    std::vector<std::string> result;
    bool front = token[0] == '(';
    bool back = token.back() == ')';
    if (front) {
        result.push_back("(");
        token = token.substr(1, token.size());
    }
    if (back) {
        token = token.substr(0, token.size() - 1);
    }
    if (token[0] == '(' || token.back() == ')') { // for recursive cleaning
        auto tmp = ClearFromBrk(token);
        for (const auto& tk: tmp) {
            result.push_back(tk);
        }
    } else {
        result.push_back(token);
    }
    if (back) {
        result.push_back(")");
    }
    return result;
}

std::vector<std::string> SearchEngine::SyntaxParser::ParseToTokens(const std::string& query) {
    std::vector<std::string> result;
    std::istringstream iss(query);
    std::string token;
    while (iss >> token) {
        auto tmp = ClearFromBrk(token);
        for (const auto& item: tmp) {
            result.push_back(item);
        }
    }
    return result;
}

void SearchEngine::SyntaxParser::StackEmptyError(const std::stack<Node*>& st) {
    if (st.empty()) {
        throw std::runtime_error("Invalid query");
    }
}

void SearchEngine::SyntaxParser::BuildQueryTree(const std::vector<std::string>& tokens) {
    std::stack<Node*> st;
    for (const auto& item: tokens) {
        if (item == "AND") {
            auto node = new AndNode();
            st.push(node);
        } else if (item == "OR") {
            auto node = new OrNode();
            st.push(node);
        } else if (item == "(") {
            auto node = new OpenBkt();
            st.push(node);
        } else if (item == ")") {
            CloseBrk(st);
        } else { // is a word
            if (!st.empty() && dynamic_cast<WordNode*>(st.top())) { // if two words lay together and chill without connector
                throw std::runtime_error("Invalid query");
            }
            if (st.empty() || !st.empty() && dynamic_cast<OpenBkt*>(st.top())) {
                auto node = new WordNode(item);
                st.push(node);
                continue;
            }
            auto rhs = new WordNode(item);
            HangNode(st, rhs);
        }
    }
    if (st.empty() || st.size() > 1) {
        throw std::runtime_error("Invalid query");
    }
    root_ = st.top();
    st.pop();
}

void SearchEngine::SyntaxParser::PostOrder(std::vector<std::string>& priority, SearchEngine::SyntaxParser::Node* node) {
    if (!node) {
        return;
    }
    PostOrder(priority, node->left_);
    PostOrder(priority, node->right_);
    priority.push_back(NodeToText(node));
}

std::string SearchEngine::SyntaxParser::NodeToText(SearchEngine::SyntaxParser::Node* node) {
    if (dynamic_cast<WordNode*>(node)) {
        return dynamic_cast<WordNode*>(node)->data_;
    }
    if (dynamic_cast<AndNode*>(node)) {
        return "AND";
    }
    return "OR";
}

void SearchEngine::SyntaxParser::CloseBrk(std::stack<Node*>& st) {
    std::stack<Node*> tmp;
    while (!st.empty() && !dynamic_cast<OpenBkt*>(st.top())) {
        tmp.push(st.top());
        st.pop();
    }
    StackEmptyError(st);
    st.pop();
    if (tmp.size() > 1) {
        throw std::runtime_error("Invalid query");
    }
    auto node = tmp.top();
    tmp.pop();
    if (dynamic_cast<LogicNode*>(node)) {
        node->MakePrior();
    }
    if (!st.empty() && dynamic_cast<LogicNode*>(st.top())) {
        HangNode(st, node);
        return;
    }
    st.push(node);
}

void SearchEngine::SyntaxParser::HangNode(std::stack<Node*>& st, SearchEngine::SyntaxParser::Node* rhs) {
    auto connector = st.top();
    st.pop();
    StackEmptyError(st); // оставь
    auto lhs = st.top();
    st.pop();

    // переподвязка
    if (dynamic_cast<OrNode*>(lhs) && dynamic_cast<AndNode*>(connector) && !lhs->IsPrior()) {
        connector->right_ = rhs;
        connector->left_ = lhs->right_;
        lhs->right_ = connector;
        st.push(lhs);
        return;
    }
    connector->right_ = rhs;
    connector->left_ = lhs;
    st.push(connector);
}
