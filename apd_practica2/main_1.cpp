#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using Cells = std::vector<std::vector<int>>;

std::chrono::steady_clock::time_point now() {
    return std::chrono::steady_clock::now();
}

std::ostream& operator<<(std::ostream& os, std::vector<std::string> v) {
    for (auto& i : v) {
        os << i + (i[i.size()-1] == ' ' ? "0" : " 0") << std::endl;
    }
    return os;
}

class Sudoku {
private:
    std::string get_line(std::ifstream& in, const std::string r = "\\r") {
        std::string s;
        getline(in, s);
        return std::regex_replace(s, std::regex(r), "");
    }

    std::string clean(std::string s) {
        return std::regex_replace(s, std::regex("\\r"), "");
    }

    std::vector<std::string> tokenize(std::string s, std::string delimiter = " ") {
        size_t pos = 0;
        std::vector<std::string> tokens;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            tokens.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }
        tokens.push_back(s);
        return tokens;
    }

    bool check_row(int i, int j, int x){
        for (int j_i = 0; j_i < N; j_i++) {
            //std::cout << board[i][j_i] << " ";
            if (board[i][j_i] == x) {
                return false;
            }
        }
        //std::cout << std::endl;
        return true;
    }

    bool check_col(int i, int j, int x) {
        for (int i_i = 0; i_i < N; i_i++) {
            //std::cout << board[i_i][j] << " ";
            if (board[i_i][j] == x) {
                return false;
            }
        }
        //std::cout << std::endl;
        return true;
    }

    bool check_ran(int i, int j, int x) {
        int o_i = std::trunc(i/n), o_j = std::trunc(j/n);
        for (int i_i = n*o_i; i_i < n*(o_i+1); i_i++) {
            for (int j_i = n*o_j; j_i < n*(o_j+1); j_i++) {
                //std::cout << board[i_i][j_i] << " ";
                if (board[i_i][j_i] == x) {
                    return false;
                }
            }
            //std::cout << "\n";
        }
        return true;
    }

    bool empty_cell(int& row, int& col) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (!board[row][col]) {
                    return true;
                }
            }
        }
        return false;
    }

    bool valid_play(int row, int col, int num) {
        return check_row(row, col, num)
            && check_col(row, col, num)
            && check_ran(row, col, num);
    }

    bool solve_bt() {
        int row, col;
        // When all places are filled:
        if (!empty_cell(row, col)) {
            return true;
        }
        // Valid numbers are 1 - N*N
        for (int num = 1; num <= N; num++){
            // Check validation, if yes, put the number in the grid
            if (valid_play(row, col, num)){
                board[row][col] = num;
                // Recursively go for other rooms in the grid
                if (solve_bt()) {
                    return true;
                }
                // Turn to unassigned space when conditions are not satisfied
                board[row][col] = 0;
            }
        }
        return false;
    }

public:

    int n;
    int N;
    Cells board;

    Sudoku() {}
    Sudoku (Cells board) : board(board) {
        this->n = std::sqrt(board.size());
        this->N = board.size();
    }
    Sudoku (std::string file) {
        std::string s;
        std::ifstream in(file);
        while(std::getline(in, s)) {
            std::vector<int> row;
            for (auto& c : tokenize(clean(s))) {
                if (c.size()) {
                    row.push_back((c[0] == '*' ? 0 : std::stoi(c)));
                }
            }
            board.push_back(row);
        }
        n = std::sqrt(board.size());
        N = board.size();
    }
    Sudoku (std::string file, int N) {

        std::ifstream in(file);
        int literal;
        board = std::vector<std::vector<int>>(N, std::vector<int>(N));

        std::string trash;
        in >> trash;
        for (auto& i : board) {
            for (auto& j : i) {
                for (int k = 1; k <= N; k++) {
                    in >> literal;
                    if (literal > 0) j = k;
                }
            }
        }
        n = std::sqrt(N);
        this->N = N;
    }

    bool solve_bt(bool verbose_time) {
        
        std::chrono::steady_clock::time_point p;
        if (verbose_time) {
            p = now();
        }
        bool result = solve_bt();
        if (verbose_time) {
            std::cout << "> BACKTRACKING SOLVING: " << 
                std::chrono::duration_cast<std::chrono::microseconds>
                    (now() - p).count() << " us" << std::endl;
        }
        return result;
    }

    void export_clauses(std::string output, bool verbose_time) {

        std::chrono::steady_clock::time_point p;

        int N_2       = N * N;
        int N_VARS    = N * N * N;
        int N_CLAUSES = 0;
        std::string literal;
        std::vector<std::string> r1p(N * N, ""), r1n, 
            r2p(N, ""), r2n, r3(N, ""), r4(N, ""), r5;

        if (verbose_time) { p = now(); }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {

                //std::cout << "ROW: " << i << ". COL: " << j << std::endl;
                int offset = (N_2 * i) + (j * N), offset_nd = (N * i) + j;
                for (int d = 0; d < N; d++) {

                    literal = std::to_string(offset + d+1) + " ";

                    // Restriccion 1:
                    r1p[offset_nd] += literal;
                    for (int d_i = d+1; d_i < N; d_i++) {
                        r1n.push_back("-" + literal + "-"
                            + std::to_string(offset + d_i+1));
                    }

                    // Restriccion 2:
                    r2p[i] += literal;
                    for (int j_i = j+1; j_i < N; j_i++) {
                        r2n.push_back("-" + literal + "-"
                            + std::to_string(offset + ((j_i - j) * N) + d+1));
                    }

                    // Restriccion 3:
                    r3[j] += literal;
                    for (int i_i = i+1; i_i < N; i_i++) {
                        r3.push_back("-" + literal + "-"
                            + std::to_string(offset + ((i_i - i) * N_2) + d+1));
                    }

                    // Restriccion 4:
                    int o_i = std::trunc(i/n), o_j = std::trunc(j/n), guard = j+1;
                    r4[(i/n * n) + j/n] += literal; 
                    for (int i_i = i; i_i < n*(o_i+1); i_i++) {
                        for (int j_i = guard; j_i < n*(o_j+1); j_i++) {
                            //std::cout << board[i_i][j_i] << ", " << (i_i * N_2) + (j_i*N) + d+1 << " | ";
                            r4.push_back("-" + literal + "-" +  
                                std::to_string((i_i * N_2) + (j_i*N) + d+1));
                        }
                        guard = n*o_j;
                    }
                    //std::cout << "\n";
                }

                // Restriccion 5:
                if (board[i][j]) {
                    r5.push_back(std::to_string(offset + board[i][j]));
                }
            }
        }
        if (verbose_time) {
            std::cout << "> REDUCING TIME: " << 
                std::chrono::duration_cast<std::chrono::microseconds>
                    (now() - p).count() << " us" << std::endl;
        }

        if (verbose_time) {
            p = now();
        }
        std::ofstream os(output);
        N_CLAUSES += r1p.size() + r1n.size() + r2p.size() + r2n.size() 
            + r3.size() + r4.size() + r5.size();
        os << "p cnf " << N_VARS << " " << N_CLAUSES << std::endl;
        os << r1p << r1n << r2p << r2n << r3 << r4 << r5;
        os.close();
        if (verbose_time) {
            std::cout << "> EXPORTING TIME: " <<
                std::chrono::duration_cast<std::chrono::microseconds>
                    (now() - p).count() << " us" << std::endl;
        }
    }

    void export_sudoku(std::string output, std::string input = "") {
        std::ofstream os(output);
        if (input.empty()) {
            for (auto& i : board) {
                for (auto& j : i) {
                    os << j << " ";
                }
                os << std::endl;
            }
        } else {
            os << input;
        }
        os.close();
    }

    friend std::ostream& operator<<(std::ostream& os, const Sudoku& s) {
        for (auto& i : s.board) {
            for (auto &j: i) {
                std::cout << (!j ? "*" : std::to_string(j)) << " ";
            }
            std::cout << std::endl;
        }
        return os;
    }
};

int main(int argc, char *argv[]) {

    std::string usage =
        "usage: ./practica2.old -input <input_file> -output <output_file> -outsol <sol_file> [-sat <results> <n>] [-vet]\n";
    usage += "  -input       Sudoku file input.\n";
    usage += "  -output      Sudoku cnf file output.\n";
    usage += "  -outsol      Sudoku solution file output\n";
    usage += "  -sat         Sat mode. Process the files as SAT ones.";
    usage += "    <results>  Sat results file.";
    usage += "    <n>        Sudoku size.";
    usage += "  -vet         Verbose execution times.";
    
    int ret = 0, N = 0;
    bool verbose_time = false, sat_mode = false;
    std::string input_file, output_file, outsol_file, sat_results;
    for (int s = 1; s < argc; s++) {
        if (!strcmp(argv[s], "-input")) input_file = argv[++s];
        else if (!strcmp(argv[s], "-output")) output_file = argv[++s];
        else if (!strcmp(argv[s], "-outsol")) outsol_file = argv[++s];
        else if (!strcmp(argv[s], "-sat")) {
            sat_mode = true;
            sat_results = argv[++s];
            N = std::stoi(argv[++s]);
        }
        else if (!strcmp(argv[s], "-vet")) verbose_time = true;
        else {
            std::cerr << std::endl << "error: unknown option '" << argv[s] << "'\n";
            std::cerr << usage << std::endl;
            exit(1);
        }
    }
    if (!input_file.size() && !output_file.size()) {
        std::cerr << std::endl << "error: no input or output file given.\n";
        std::cerr << usage << std::endl;
        exit(-1);
    }
    if (sat_mode && !N && !sat_results.size()) {
        std::cerr << std::endl << "error: no result file given or your sudoku size is 0.\n";
        std::cerr << usage << std::endl;
        exit(-1);
    }
    if (verbose_time) { std::cout << "======================================\n"; }
    Sudoku s;
    if (!sat_mode) {
        s = Sudoku(input_file);
        s.export_clauses(output_file, verbose_time);
        if (strcmp(outsol_file.c_str(), "pass")) {
            bool sol = s.solve_bt(verbose_time);
            s.export_sudoku(outsol_file, (sol ? "" : "SUDOKU NON RESOLVABLE WITH BACKTRAKING\n"));
        }
        ret = s.N;
    } else {
        s = Sudoku(input_file, N);
        if (verbose_time) {
            std::cout << std::ifstream(sat_results).rdbuf() << std::endl;
        }
        s.export_sudoku(output_file);
    }
    if (verbose_time) { std::cout << "======================================\n\n"; }
    return ret;

}