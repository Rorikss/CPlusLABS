#include <iostream>
#include <fstream>
#include <string>

struct FileData {
    std::string filename;
    long long n = 0;
    char delimiter = '\n';
    bool is_lines = false;
    bool is_tail = false;
    bool is_delimiter = false;
    bool delimiter_was_changed = false;
    bool number_was_given = false;
};

const int kMinLength = 3;
const int kEscapeLength = 4;
enum FinishCode {
    kSuccess, kError
};

int PrintFirstLines(FileData &file_data) {
    std::ifstream infile(file_data.filename);
    if (!infile.is_open()) {
        std::cerr << "cannot open the file, please, check the path";
        return kError;
    }
    if ((file_data.n == 0) && (!file_data.number_was_given)) file_data.n = std::numeric_limits<long long>::max();
    char tmp;
    long long cnt = 0;
    while ((infile.get(tmp)) && (cnt < file_data.n)) {
        if (tmp == file_data.delimiter) {
            std::cout << '\n';
            ++cnt;
        } else if (tmp != '\n') {
            std::cout << tmp;
        }
    }
    if ((cnt < file_data.n) && file_data.is_lines && !file_data.is_tail) {
        std::cerr << "\nNOTE: there were not as many lines as you wanted in file :(\n"
                     "so I printed all, what I could\n";
    }
    return kSuccess;
}

int PrintLastLines(FileData &file_data) {
    std::ifstream infile(file_data.filename);
    if (!infile.is_open()) {
        std::cerr << "cannot open the file, please, check the path";
        return kError;
    }
    char tmp;
    long long cnt = 0;
    while (infile.get(tmp)) {
        if (tmp == file_data.delimiter) cnt++;
    }
    if (file_data.n > cnt) {
        std::cerr << "\nthere are no as many lines, as you want, I will print you whole file" << std::endl;
        return PrintFirstLines(file_data);
    }
    long long cur_lines = 0;
    infile.clear();
    infile.seekg(0);
    bool started = false;
    while (infile.get(tmp)) {
        if (cur_lines > cnt - file_data.n) started = true;
        if (started) {
            if (tmp == file_data.delimiter) std::cout << '\n';
            else if (tmp != '\n') std::cout << tmp;
        }
        if (tmp == file_data.delimiter) ++cur_lines;
    }
    return kSuccess;
}

int PrintFile(FileData &file_data) {
    if (!file_data.number_was_given && (file_data.is_lines || file_data.is_tail)) {
        std::cerr << "you haven't entered number of lines, so I'll print you all file" << std::endl;
        return PrintFirstLines(file_data);
    }
    if (file_data.is_tail || (file_data.is_tail && file_data.is_lines)) {
        return PrintLastLines(file_data);
    }
    if (file_data.is_lines) {
        return PrintFirstLines(file_data);
    }
    return PrintFirstLines(file_data);
}

char Convert(FileData &file_data, const char *line) {
    file_data.delimiter_was_changed = true;
    char del;
    if (!strcmp(line, "'")) {
        del = ' ';
    } else if (!strcmp(line, "''")) {
        del = '\0';
    } else if (line[1] == '\\' && strlen(line) == kEscapeLength) {
        switch (line[2]) {
            case 'n':
                del = '\n';
                break;
            case 't':
                del = '\t';
                break;
            case 'a':
                del = '\a';
                break;
            case 'b':
                del = '\b';
                break;
            case 'v':
                del = '\v';
                break;
            case 'f':
                del = '\f';
                break;
            case 'r':
                del = '\r';
                break;
            case '0':
                del = '\0';
                break;
        }
    } else {
        if (strlen(line) > kMinLength) {
            std::cerr << "unfortunately, delimiter can be only escape symbol or symbol with length 1" << std::endl;
            std::cerr << "now delimiter equals to first char of entered string" << std::endl;
        }
        del = line[1];
    }
    return del;
}

bool IsNumber(const char *arg) {
    for (long long i = 0; i < strlen(arg); ++i) {
        if (!isdigit(arg[i]) && !(i == 0 && arg[i] == '-')) return false;
    }
    return true;
}

int Enumerate(FileData &file_data, const char *arg) {
    file_data.number_was_given = true;
    if (arg[0] != '-') {
        char *end;
        file_data.n = std::strtoll(arg, &end, 10);
        return kSuccess;
    }
    std::cerr << "number of lines must be >= 0";
    return kError;
}

int GetLongArgument(FileData &file_data, const char *arg) {
    auto pos = strchr(arg, '=');
    long long length = (pos - arg);
    if (!strncmp(arg, "--lines", length)) file_data.is_lines = true;
    else if (!strncmp(arg, "--delimiter", length)) file_data.is_delimiter = true;
    else {
        std::cerr << "some unknown argument entered :((";
        return kError;
    }
    const char *param = arg;
    for (int i = 0; i <= length; ++i) {
        ++param;
    }
    if (strlen(param) == 0) {
        std::cerr << "you have not entered any parameter";
        return kError;
    }
    if (IsNumber(param)) {
        return Enumerate(file_data, param);
    }
    if (arg[0] == '\'') {
        file_data.delimiter = Convert(file_data, param);
        return kSuccess;
    }
    std::cerr << "some undefined parameter was entered";
    return kError;
}

int ParseArgument(FileData &file_data, const char *arg) {
    if (!strcmp(arg, "-l")) file_data.is_lines = true;
    else if (!strcmp(arg, "-t") || !strcmp(arg, "--tail")) file_data.is_tail = true;
    else if (!strcmp(arg, "-d")) file_data.is_delimiter = true;
    else if (arg[0] == '\'') file_data.delimiter = Convert(file_data, arg);
    else if (IsNumber(arg)) return Enumerate(file_data, arg);
    else if (strchr(arg, '.') != nullptr) file_data.filename = arg;
    else if (strchr(arg, '=') != nullptr) return GetLongArgument(file_data, arg);
    else {
        std::cerr << "some unknown argument was entered :((" << std::endl;
        return kError;
    }
    return kSuccess;
}

FinishCode Parse(FileData& data, int argc, char *argv[]){
    for (int i = 1; i < argc; ++i) {
        char *arg = argv[i];
        int failed = ParseArgument(data, arg);
        if (failed == 1) return kError;
    }

    return kSuccess;
}

FinishCode Run(FileData& file_data){
    if (!file_data.is_lines && file_data.is_tail && file_data.n > 0) {
        std::cerr << "-t or --tail doesn't take argument, use -l or --lines for it";
        return kError;
    }
    if (!file_data.is_lines && !file_data.is_tail && file_data.n > 0) {
        std::cerr << "you have entered number, but didn't enter -l --lines parameters, please, specify your order";
        return kError;
    }
    if (file_data.delimiter_was_changed && !file_data.is_delimiter) {
        std::cerr << "you can give delimiter only with --delimiter or -d argument" << std::endl;
        return kError;
    }
    PrintFile(file_data);
    return kSuccess;
}

int main(int argc, char *argv[]) {
    FileData file_data;
    FinishCode code = Parse(file_data, argc, argv);
    if (code != kSuccess) {
        std::cerr << "unfortunately, parsing failed";
        return kError;
    }
    return Run(file_data);
}
