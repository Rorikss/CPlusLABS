//
// Created by avant on 5/4/2024.
//

#include "TermSearch.h"

void SearchEngine::TermSearch::Next() {
    ++cur_pointer_;
}

bool SearchEngine::TermSearch::IsEnd() {
    return cur_pointer_ == insertions_.size();
}

SearchEngine::FullInfo SearchEngine::TermSearch::Get() {
    return insertions_[cur_pointer_];
}

SearchEngine::WordTerm::WordTerm(const std::string& term, const std::vector<FullInfo>& data) {
    insertions_ = data;
    terms_ = {term};
}

SearchEngine::LogicOperand::LogicOperand(std::shared_ptr<TermSearch>& lhs, std::shared_ptr<TermSearch>& rhs)
        : lhs_(lhs)
        , rhs_(rhs)
        {}

void SearchEngine::LogicOperand::MakeTerms() {
    for (const auto& term : lhs_->terms_) {
        terms_.push_back(term);
    }
    for (const auto& term : rhs_->terms_) {
        terms_.push_back(term);
    }
}

SearchEngine::AND::AND(std::shared_ptr<TermSearch> lhs, std::shared_ptr<TermSearch> rhs)
        : LogicOperand(lhs, rhs) {
            MakeInsertions();
            MakeTerms();
        }

void SearchEngine::AND::MakeInsertions() {
    while (!lhs_->IsEnd() && !rhs_->IsEnd()) {
        auto first_element = lhs_->Get();
        auto second_element = rhs_->Get();
        if (first_element.file_index_ == second_element.file_index_) {
            std::vector<size_t> tmp;
            std::set_intersection(first_element.line_numbers_.begin(), first_element.line_numbers_.end(),
                                  second_element.line_numbers_.begin(), second_element.line_numbers_.end(),
                                  std::back_inserter(tmp));
            if (!tmp.empty()) {
                insertions_.push_back(FullInfo(first_element.file_index_, tmp));
            }
            lhs_->Next();
            rhs_->Next();
            continue;
        }
        if (first_element.file_index_ < second_element.file_index_) {
            lhs_->Next();
        } else if (first_element.file_index_ > second_element.file_index_) {
            rhs_->Next();
        }
    }
}

SearchEngine::OR::OR(std::shared_ptr<TermSearch> lhs, std::shared_ptr<TermSearch> rhs)
        : LogicOperand(lhs, rhs) {
            MakeInsertions();
            MakeTerms();
        }

void SearchEngine::OR::MakeInsertions() {
    while (!lhs_->IsEnd() && !rhs_->IsEnd()) {
        auto first_element = lhs_->Get();
        auto second_element = rhs_->Get();
        if (first_element.file_index_ == second_element.file_index_) {
            std::vector<size_t> tmp;
            std::set_union(first_element.line_numbers_.begin(), first_element.line_numbers_.end(),
                           second_element.line_numbers_.begin(), second_element.line_numbers_.end(),
                           std::back_inserter(tmp));
            insertions_.push_back(FullInfo(first_element.file_index_, tmp));
            lhs_->Next();
            rhs_->Next();
            continue;
        }
        if (first_element.file_index_ < second_element.file_index_) {
            insertions_.push_back(first_element);
            lhs_->Next();
        } else {
            insertions_.push_back(second_element);
            rhs_->Next();
        }
    }
    WriteTillEnd(lhs_);
    WriteTillEnd(rhs_);
}

void SearchEngine::OR::WriteTillEnd(std::shared_ptr<TermSearch>& operand) { // так можно епта?
    while (!operand->IsEnd()) {
        insertions_.push_back(operand->Get());
        operand->Next();
    }
}
