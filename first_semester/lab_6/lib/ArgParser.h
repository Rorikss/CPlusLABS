#pragma once
#include "Argument.h"

namespace ArgumentParser {

    class ArgParser {
    public:
        ArgParser(const std::string& name);
        ~ArgParser() {};

        // add arguments and methods
        Argument<std::string>& AddStringArgument(char key);
        Argument<std::string>& AddStringArgument(const std::string& long_key);
        Argument<std::string>& AddStringArgument(char key, const std::string& long_key, const std::string& info_parser="");

        Argument<int>& AddIntArgument(char key);
        Argument<int>& AddIntArgument(const std::string& long_key, const std::string& info_parser="");
        Argument<int>& AddIntArgument(char key, const std::string& long_key, const std::string& info_parser="");

        Argument<bool>& AddFlag(char key);
        Argument<bool>& AddFlag(const std::string& long_key, const std::string& info_parser="");
        Argument<bool>& AddFlag(char key, const std::string& long_key, const std::string& info_parser="");

        void AddHelp(char key='\0', const std::string& long_key="", const std::string& info_parser="");

        // public argument gets
        int GetIntValue(const std::string& arg, const int& index=0);
        std::string GetStringValue(const std::string& arg, const int& index=0);
        std::vector<std::string> GetStringValues(const std::string& arg);
        bool GetFlag(const std::string& arg, const int& index=0);

        // public parse
        bool Parse(int argc, char** argv);
        bool Parse(std::vector<std::string> args);

        // the help section
        bool Help() const;
        void HelpDescription();
    private:
        std::string name_of_parser_ = "MyParser";
        bool is_help_given_ = false;
        bool is_help_evaluated_ = false;
        std::string parser_help_info_;
        Key help_key_;

        // storage of arguments. mb smth like these
        std::vector<Argument<std::string>> string_arguments_;
        std::vector<Argument<int>> int_arguments_;
        std::vector<Argument<bool>> bool_arguments_;

        // the template functions
        template <typename T>
        Argument<T>* FindArgument(const std::string& arg_name, std::vector<Argument<T>>& arguments) const {
            for (auto& arg : arguments) {
                if (arg.GetLongName() == arg_name || arg.GetShortName() == arg_name) {
                    return &arg;
                }
            }
            return nullptr;
        }

        template <typename T>
        Argument<T>* FindPosArgument(std::vector<Argument<T>>& arguments) const {
            for (auto& arg : arguments) {
                if (arg.IsPositional()) {
                    return &arg;
                }
            }
            return nullptr;
        }

        template <typename T>
        Argument<T>& AddArgument(char key, const std::string& long_key, const std::string& info_parser, std::vector<Argument<T>>& arguments_) {
            std::string short_key(1, key);
            Argument new_argument = Argument<T>(short_key, long_key);
            new_argument.SetHelpInfo(info_parser);
            arguments_.emplace_back(new_argument);
            return arguments_.back();
        }

        template <typename T>
        bool Check(std::vector <T> args) {
            for (auto arg : args) {
                if (!arg.IsEvaluated() || (arg.IsMultiValue() && (arg.GetActualValueCount() < arg.GetMinValueCount()))) {
                    return false;
                }
            }
            return true;
        }

        template <typename T>
        void ArgumentsHelpDescription(const std::string& title, const std::vector<Argument<T>>& arguments) {
            std::cout << title << '\n';
            for (const auto& arg : arguments) {
                std::cout << arg.GetShortName() << ", " << arg.GetLongName() << ", "
                          << arg.GetHelpInfo() << "\n";
            }
        }

        // find arguments
        Argument<int>* FindIntArgument(const std::string& arg_name);
        Argument<std::string>* FindStringArgument(const std::string& arg_name);
        Argument<bool>* FindBoolArgument(const std::string& arg_name);
        // find positionals
        Argument<int>* FindPosInt();
        Argument<std::string>* FindPosString();

        // parsing functions
        bool ParseShortBool(const std::string& line);
        int ParseInt(const std::string& arg);
        void ParseArgument(std::string& arg, Argument<std::string>* &positional_string);
        bool CheckAll();

    };

} // namespace ArgumentParser
