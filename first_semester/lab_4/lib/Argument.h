#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace ArgumentParser {
    struct Key {
        std::string short_key;
        std::string long_key;
    };

    template<typename ArgValueType>
    class Argument {

    public:
        Argument(const std::string& key, const std::string& key_long) : key_{key, key_long} {}

        ~Argument() {};

        void SetHelpInfo(const std::string& info_about_argument) {
            help_info_ = info_about_argument;
        }

        Argument& Default(const ArgValueType& value) {
            is_default_ = true;
            is_evaluated_ = true;
            values_.push_back(value);
            if (is_multi_value_) {
                throw std::runtime_error("Invalid parameters settled");
            }
            if (is_stored_ && storage_pointer_ != nullptr) {
                *storage_pointer_ = value;
            }
            return *this;
        }

        Argument& Default(const std::vector<ArgValueType>& values) {
            is_default_ = true;
            is_evaluated_ = true;
            for (int i = 0; i < values.size(); ++i) {
                values_.push_back(values[i]);
            }
            if (is_stored_ && storage_pointers_ != nullptr) {
                *storage_pointers_ = values;
            }
            return *this;
        }

        Argument& StoreValue(ArgValueType& value_location) {
            is_stored_ = true;
            storage_pointer_ = &value_location;
            return *this;
        }

        Argument& StoreValues(std::vector<ArgValueType>& values_location) {
            is_stored_ = true;
            storage_pointers_ = &values_location;
            return *this;
        }

        Argument& MultiValue(const size_t& min_val_cnt=1) {
            is_multi_value_ = true;
            minimal_value_count_ = min_val_cnt;
            return *this;
        }

        Argument& Positional() {
            is_positional_ = true;
            return *this;
        }

        ArgValueType GetArgumentValue(const int& index=0) const {
            static const ArgValueType kDefaultValue{};
            if (values_.empty()) {
                return kDefaultValue;
            }
            return values_[index];
        }

        void SetValue(const ArgValueType& value) {
            is_evaluated_ = true;
            if (is_default_) {
                values_.clear();
            }

            if (is_stored_) {
                if (is_multi_value_ && storage_pointers_ != nullptr) {
                    (*storage_pointers_).push_back(value);
                } else if (storage_pointer_ != nullptr) {
                    *storage_pointer_ = value;
                }
            }
            if (values_.empty() || is_multi_value_) {
                values_.push_back(value);
            } else {
                values_[0] = value;
            }
        }

        // Get smth part
        std::string_view GetLongName() const { return static_cast<std::string_view>(key_.long_key); }

        std::string_view GetShortName() const { return static_cast<std::string_view>(key_.short_key); }

        std::string GetHelpInfo() const { return help_info_; }

        bool IsEvaluated() const { return is_evaluated_; }

        bool IsMultiValue() const { return is_multi_value_; }

        bool IsPositional() const { return is_positional_; }

        int GetActualValueCount() const { return values_.size(); }

        int GetMinValueCount() const { return minimal_value_count_; }

    private:
        Key key_;
        std::string help_info_;

        //storage of values
        std::vector<ArgValueType> values_;
        ArgValueType* storage_pointer_ = nullptr;
        std::vector<ArgValueType> *storage_pointers_ = nullptr;

        // bools of help
        bool is_multi_value_ = false;
        bool is_default_ = false;
        bool is_positional_ = false;
        bool is_stored_ = false;
        bool is_evaluated_ = false;
        //some other helper variables
        int minimal_value_count_ = 1;
    };
}
