#include "ArgParser.h"
#include "Argument.h"
using namespace ArgumentParser;

// privates

// finding things I need
Argument<int>* ArgParser::FindIntArgument(const std::string& arg_name) {
    return FindArgument(arg_name, int_arguments_);
}

Argument<std::string>* ArgParser::FindStringArgument(const std::string& arg_name) {
    return FindArgument(arg_name, string_arguments_);
}

Argument<bool>* ArgParser::FindBoolArgument(const std::string& arg_name) {
    return FindArgument(arg_name, bool_arguments_);
}

Argument<int>* ArgParser::FindPosInt() {
    return FindPosArgument(int_arguments_);
}

Argument<std::string>* ArgParser::FindPosString() {
    return FindPosArgument( string_arguments_);
}

// functions for parsing :)
void ArgParser::ParseArgument(std::string& arg) {
    std::string_view line = arg;
    bool is_long_argument = (line[1] == '-');
    if (is_long_argument) {
        line.remove_prefix(2);
    } else {
        line.remove_prefix(1);
        if (ParseShortBool(static_cast<std::string>(line))) {
            return;
        }
    }
    int pos_of_equal_sign = line.find("=");
    std::string_view clear_name = line.substr(0, pos_of_equal_sign);
    std::string_view value = line.substr(pos_of_equal_sign + 1);
    Argument<int>* int_arg = FindIntArgument(static_cast<std::string>(clear_name));
    Argument<std::string>* string_arg = FindStringArgument(static_cast<std::string>(clear_name));
    Argument<bool>* flag_arg = FindBoolArgument(static_cast<std::string>(clear_name));

    if (int_arg != nullptr) {
        int_arg->SetValue(ParseInt(static_cast<std::string>(value)));
        return;
    }
    if (string_arg != nullptr) {
        string_arg->SetValue(static_cast<std::string>(value));
        return;
    }
    if (flag_arg != nullptr) {
        flag_arg->SetValue(true);
        return;
    }
    throw std::runtime_error("Invalid argument name.");
}

bool ArgParser::ParseShortBool(const std::string& line) {
    bool set_smt = false;
    for (auto symbol : line) {
        std::string name(1, symbol);
        Argument<bool>* flag_arg = FindBoolArgument(name);
        if (flag_arg != nullptr) {
            set_smt = true;
            flag_arg->SetValue(true);
        } else {
            return false;
        }
    }
    if (set_smt) {
        return true;
    }
    return false;
}

int ArgParser::ParseInt(const std::string& arg) {
    try {
        return std::stoi(arg);
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
        throw std::runtime_error("Invalid int value.");
    }
}

bool ArgParser::CheckAll() {
    return Check(int_arguments_) && Check(string_arguments_) && Check(bool_arguments_);
}

// publics
ArgParser::ArgParser(const std::string& name)  {
    name_of_parser_ = name;
}

// argument adds
Argument<std::string>& ArgParser::AddStringArgument(char key) {
    return AddArgument(key, "", "", string_arguments_);
}

Argument<std::string>& ArgParser::AddStringArgument(const std::string& long_key) {
    return AddArgument('\0', long_key, "", string_arguments_);
}

Argument<std::string>& ArgParser::AddStringArgument(char key, const std::string& long_key, const std::string& info_parser) {
    return AddArgument(key, long_key, info_parser, string_arguments_);
}

Argument<int>& ArgParser::AddIntArgument(char key) {
    return AddArgument(key, "", "", int_arguments_);
}

Argument<int>& ArgParser::AddIntArgument(const std::string& long_key, const std::string& info_parser) {
    return AddArgument('\0', long_key, info_parser, int_arguments_);
}

Argument<int>& ArgParser::AddIntArgument(char key, const std::string& long_key, const std::string& info_parser) {
    return AddArgument(key, long_key, info_parser, int_arguments_);
}

Argument<bool>& ArgParser::AddFlag(char key) {
    return AddArgument(key, "", "", bool_arguments_);
}

Argument<bool>& ArgParser::AddFlag(const std::string& long_key, const std::string& info_parser) {
    return AddArgument('\0', long_key, info_parser, bool_arguments_);
}

Argument<bool>& ArgParser::AddFlag(char key, const std::string& long_key, const std::string& info_parser) {
    return AddArgument(key, long_key, info_parser, bool_arguments_);
}

void ArgParser::AddHelp(char key, const std::string& long_key, const std::string& info_parser) {
    std::string short_key(1, key);
    is_help_given_ = true;
    help_key_ = Key{short_key, long_key};
    parser_help_info_ = info_parser;
}

// argument gets
bool ArgParser::GetFlag(const std::string& arg, const int& index) {
    Argument<bool> *argument = FindBoolArgument(arg);
    if (argument != nullptr) {
        return argument->GetArgumentValue(index);
    }
    return false;
}

std::string ArgParser::GetStringValue(const std::string& arg, const int& index) {
    Argument<std::string>* argument = FindStringArgument(arg);
    if (argument != nullptr) {
        return argument->GetArgumentValue(index);
    }
    return "";
}

int ArgParser::GetIntValue(const std::string& arg, const int& index) {
    Argument<int>* argument = FindIntArgument(arg);
    if (argument != nullptr) {
        return argument->GetArgumentValue(index);
    }
    return 0;
}

// public parse
bool ArgParser::Parse(int argc, char **argv) {
    std::vector<std::string> arguments(argv, argv + argc);
    return Parse(arguments);
}

bool ArgParser::Parse(std::vector<std::string> args) {
    Argument<int>* pos_int_argument = FindPosInt();
    Argument<std::string>* pos_string_argument = FindPosString();
    for (int i = 1; i < args.size(); ++i) {
        std::string_view line = args[i];
        if (is_help_given_ && (line.substr(1) == help_key_.short_key || line.substr(2) == help_key_.long_key)) {
            is_help_evaluated_ = true;
            continue;
        }
        if (line[0] == '-') {
            if (line.length() < 2) {
                throw std::runtime_error("No arguments provided.");
            }
            ParseArgument(args[i]);
        } else if (pos_int_argument != nullptr) {
            pos_int_argument->SetValue(ParseInt(args[i]));
            continue;
        } else if (pos_string_argument != nullptr) {
            pos_string_argument->SetValue(args[i]);
            continue;
        } else {
            throw std::runtime_error("Invalid argument entered.");
        }
    }
    return (CheckAll() || is_help_evaluated_);
}

// the help section
bool ArgParser::Help() const {
    return is_help_evaluated_;
}

void ArgParser::HelpDescription() {
    std::cout << name_of_parser_ << "\n";
    std::cout << parser_help_info_ << "\n\n";
    if (!int_arguments_.empty()) {
        ArgumentsHelpDescription("int arguments", int_arguments_);
    }
    if (!string_arguments_.empty()) {
        ArgumentsHelpDescription("string arguments", string_arguments_);
    }
    if (!bool_arguments_.empty()) {
        ArgumentsHelpDescription("bool arguments", bool_arguments_);
    }
    std::cout << "\n" << help_key_.short_key << " " << help_key_.long_key << " " << parser_help_info_ << std::endl;
}
