#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <algorithm>
using namespace std;

#define MAX_LINE 40960
void print_help() {
    cerr << "-f the file need to choose data" << endl;
    cerr << "-n the count of lines to choose" << endl;
    cerr << endl;
}

class FileLine {
    public:
        FileLine(const char* file) : file_name(file) {
            ifs.open(file_name);
            line_nums.clear();
        }
        ~FileLine() {
            ifs.close();
            line_nums.clear();
        }
        bool out_lines(ostream& out, int n) {
            if (!get_line_num(n)) {
                return false;
            }
            int lc = 0;
            char line[MAX_LINE];
            cerr << "choose line num: " << line_nums.size() << endl;
            for (uint32_t i = 0; i < line_nums.size(); ++i) {
                while (lc < line_nums[i]) {
                    int ret = get_line(line, MAX_LINE);
                    if (ret == -1) {
                        cerr << "read line:" << lc << "failed" << endl;
                        return false;
                    }
                    lc++;
                }
                cerr << "lc:" << lc << " obj line:" << line_nums[i] << endl;
                int ret = get_line(line, MAX_LINE);
                ++lc;
                if (ret == -1) {
                    return false;
                } else if (ret == 1) {
                    cerr << i << " of " << n << "(" << (1 + line_nums[i]) << "):out_of_memory" << endl;
                    continue;
                } else {
                    cerr << i << " of " << n << "(" << (1 + line_nums[i]) << "):" << line << endl;
                    out << line << endl;
                }
            }
            return true;
        }

    private:
        /* return 1 表示超过存储空间，可以丢弃
         * return 0 表示正常读取数据，可以处理
         * return -1 表示文件读取结束，可能是正常结束，也可能是异常结束
         * */
        int get_line(char* line, int size) {
            int n = 0;
            char c;
            while (ifs.get(c)) {
                if (c == '\n') {
                    if (n == size) {
                        line[n - 1] = 0;
                        return 1;
                    } else {
                        line[n] = 0;
                        return 0;
                    }
                }
                if (n < size) {
                    line[n] = c;
                    ++n;
                }
            }
            if (n == size) {
                line[n - 1] = 0;
                return 1;
            } else {
                line[n] = 0;
            }
            if (n == 0) {
                return -1;  //无数据
            } else {
                return 0;
            }
        }

        bool get_line_num(int n) {
            line_nums.clear();
            char c;
            int ch = 0;
            int total_line_count = 0;
            while (ifs.get(c)) {
                if (c == '\n') {
                    if (ch > 0) {
                        ++total_line_count;
                    }
                    ch = 0;
                } else {
                    ++ch;
                }
            }
            if (ch > 0) {
                ++total_line_count;
            }
            if (total_line_count <= 0) {
                cerr << "fuck, total line is 0?" << endl;
                return false;
            }
            cerr << "the file total line count is: " << total_line_count << endl;
            srand(time(NULL));
            for (int i = 0; i < n; ++i) {
                int idx = rand() % total_line_count;
                line_nums.emplace_back(idx);
            }
            sort(line_nums.begin(), line_nums.end());
            ifs.close();
            ifs.open(file_name);
            return true;
        }
    private:
        const char* file_name;
        vector<int> line_nums;
        ifstream ifs;
};

int main(int argc, char** argv) {
    int c = 0;
    char* fname = nullptr;
    int count = 0;
    while ((c = getopt(argc, argv, "hf:n:")) != -1) {
        switch (c) {
            case 'h':
                print_help();
                return -1;
            case 'f':
                fname = optarg;
                break;
            case 'n':
                count = atoi(optarg);
                break;
            default:
                print_help();
                return -1;
        }
    }
    if (!fname || count == 0) {
        print_help();
        return -1;
    }
    FileLine fl(fname);
    fl.out_lines(cout, count);
    return 0;
}   
    
