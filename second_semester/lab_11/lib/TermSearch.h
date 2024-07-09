//
// Created by Roriks on 5/4/2024.
//
#pragma once
#include <memory>
#include <string>
#include <iterator>
#include <algorithm>
#include "InfoStructures.h"

namespace SearchEngine {
struct TermSearch {
    virtual ~TermSearch() = default;
    void Next();
    FullInfo Get();
    bool IsEnd();

    size_t cur_pointer_ = 0;
    std::vector<std::string> terms_;
    std::vector<FullInfo> insertions_;
};

struct WordTerm : public TermSearch {
    WordTerm(const std::string& term, const std::vector<FullInfo>& data);
};

struct LogicOperand : public TermSearch {
    LogicOperand(std::shared_ptr<TermSearch>& lhs, std::shared_ptr<TermSearch>& rhs);
    void MakeTerms();

    std::shared_ptr<TermSearch> lhs_;
    std::shared_ptr<TermSearch> rhs_;
};

struct AND : public LogicOperand {
    AND(std::shared_ptr<TermSearch> lhs, std::shared_ptr<TermSearch> rhs);
    void MakeInsertions();
};

struct OR : public LogicOperand {
    OR(std::shared_ptr<TermSearch> lhs, std::shared_ptr<TermSearch> rhs);
    void MakeInsertions();
    void WriteTillEnd(std::shared_ptr<TermSearch>& operand);
};

} // end of namespace
