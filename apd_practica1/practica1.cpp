#include <algorithm>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// Predeclarations
template <typename T> std::ostream& operator<< (std::ostream& os, std::vector<T> v);
std::vector<int> generate_sorted_vector(int n, int max);
std::vector<int> generate_random_vector(int n, int max);
std::vector<int> generate_anti_sorted_vector(int n, int max);
void test_sorts(std::vector<int>& v);
void test_sorts_showing(std::vector<int>& v);

// Main function
int main(int argc, char* argv[]){
    // Argument check
    if ((argc != 3) && (argc != 2)){
        std::cout << "Error: Incorrect call, expected:" << argv[0] << 
                " <N_numbers>  <max_value>" << std::endl;
        std::cout << "Or: " << argv[0] << "<file.txt>" << std::endl;
        exit(1);
    }
    // Reading extern data
    if (argc == 2){
        std::string row;
        std::ifstream in(argv[1]);
        std::vector<int> v;
        while (getline(in, row)){
            int n;
            std::stringstream(row) >> n;
            v.push_back(n);
        }
        std::cout << "== REAL DATA =========================" << std::endl;
        test_sorts(v);
        std::cout << "======================================" << std::endl;
    }else{ // Generate artificial data
        int n = std::atoi(argv[1]);
        if (n < 0){
            std::cout << "Error: Incorrect N, expected bigger than 0, not:" << 
                    argv[1] << std::endl;
            exit(1);
        }

        int max = std::atoi(argv[2]);
        if (max < 0){
            std::cout << "Error: Incorrect max, expected bigger than 0, not:" << 
                    argv[2] << std::endl;
            exit(1);
        }
        std::cout << std::endl;

        //-- Hand-made functional experiment
        std::cout << "== HANDMADE FUNCTIONAL EXPERIMENT ====" << std::endl;
        std::vector<int> aux{1, 4, 59, 9, 40, 18, 38, 1, 39, 2, 14, 30, 0, 1};
        test_sorts_showing(aux);
        std::cout << "======================================" << std::endl;

        //-- Fully-sorted experiment
        std::cout << "== FULLY-SORTED EXPERIMENT ===========" << std::endl;
        aux = generate_sorted_vector(n, max);
        test_sorts(aux);
        std::cout << "======================================" << std::endl;

        //-- Shuffled experiment
        std::cout << "== SHUFFLED EXPERIMENT ===============" << std::endl;
        aux = generate_random_vector(n, max);
        test_sorts(aux);
        std::cout << "======================================" << std::endl;

        //-- Anti-sorted experiment
        std::cout << "== ANTISORTED EXPERIMENT =============" << std::endl;
        aux = generate_anti_sorted_vector(n, max);
        test_sorts(aux);
        std::cout << "======================================" << std::endl;
    }
}

/*=============================*/
// UTILS
/*=============================*/
/**
 * @brief Operator << overload for template vector.
 * 
 * @tparam T int, double, float.
 * @param os The ostream.
 * @param v The vector.
 * @return std::ostream& 
 */
template <typename T> std::ostream& operator<< (std::ostream& os, 
        std::vector<T> v){
    os << "Vector size: "  << v.size() << std::endl;
    for (auto& i : v) os << i << " ";
    return os;
}

/**
 * @brief Returns the digit of the number in the given position.
 * 
 * @param number The number where to get the digit from.
 * @param pos The position of the digit.
 * @return The digit.
 */
int num_digit(int number, int pos){
    int i, aux;
    for (i = 0; number && i < pos; i++){
        aux = number % 10;
        number = (number - (number % 10)) / 10;
    }
    return (i < pos) ? 0 : aux;
}

/**
 * @brief Returns the number of digits of a given number.
 * 
 * @param x Number to get the digits number.
 * @return Digits number of the given number.
 */
int num_lenght(int x){
    return 
        (x < 10 ? 1 :   
        (x < 100 ? 2 :   
        (x < 1000 ? 3 :   
        (x < 10000 ? 4 :   
        (x < 100000 ? 5 :   
        (x < 1000000 ? 6 :   
        (x < 10000000 ? 7 :  
        (x < 100000000 ? 8 :  
        (x < 1000000000 ? 9 :
        10)))))))));  
}

/*=============================*/
// QUICK SORT
/*=============================*/
/**
 * @brief Sorts the vector in the given partition 
 * defined by the begin and end pivots.
 * 
 * @param v The vector to sort.
 * @param begin Partition begin.
 * @param end Partition end.
 * @return Returns a new pivot.
 */
int partition(std::vector<int>& v, int begin, int end){
    int pivot = v[(begin + end) / 2];
    int i = begin - 1;
    int j = end + 1;
    for (; ; ){
        do{
            i++;
        } while (v[i] < pivot);
        
        do{
            j--;
        } while (v[j] > pivot);
        
        if (i >= j){
            return j;
        }

        int aux = v[i];
        v[i] = v[j];
        v[j] = aux;
    }
}

/**
 * @brief Recursive quick sort algorythm for each
 * partition.
 * 
 * @param v The vector to sort.
 * @param begin Partition begin.
 * @param end Partition end.
 */
void quick_sort_aux(std::vector<int>& v, int begin, int end){
    if (begin < end){
        int p = partition(v, begin, end);
        quick_sort_aux(v, begin, p);
        quick_sort_aux(v, p + 1, end);
    }
}

/**
 * @brief Quick sort algorythm. Calls the recursive 
 * one.
 * 
 * @param v The vector to sort.
 */
void quick_sort(std::vector<int>& v){
    quick_sort_aux(v, 0, v.size() - 1);
}

/*=============================*/
// RADIX SORT 
/*=============================*/
/* OLD VERSION: Radix + Quicksort, doesn't work :(
struct radix_number{
    int cypher;
    int original_number;

    void operator= (int b){
        cypher = b;
    }
};

bool operator< (radix_number r1, radix_number r2){
    return r1.cypher < r2.cypher;
}

bool operator<= (radix_number r1, radix_number r2){
    return r1.cypher <= r2.cypher;
}

bool operator> (radix_number r1, radix_number r2){
    return r2 < r1;
}

bool operator>= (radix_number r1, radix_number r2){
    return r2 <= r1;
}

std::ostream& operator<< (std::ostream& os, radix_number r){
    return os << r.original_number << ":" << r.cypher;
}

void radixsort(std::vector<int>& to_sort){
    int max = num_lenght(*(std::max_element(std::begin(to_sort), std::end(to_sort))));
    for (int i = 0; i < max; i++) {
        std::vector<radix_number> radix_numbers;
        for (auto& j : to_sort)
            radix_numbers.push_back(radix_number{num_digit(j, i+1), j});
        quick_sort(radix_numbers, 0, radix_numbers.size()-1);
        for (int j = 0; j < to_sort.size(); j++)
            to_sort[j] = radix_numbers[j].original_number;
        std::cout << "ROUND " << i << ": " << radix_numbers << std::endl;
    }
}
*/

/**
 * @brief Radix sort algorythm usign counting sort.
 * 
 * @param to_sort The vector to sort.
 */
void radix_sort(std::vector<int>& to_sort){
    int max = num_lenght(*(std::max_element(std::begin(to_sort), 
            std::end(to_sort))));

    // Count sort
    for (int i = 0; i < max; i++){
        int aux = std::pow(10, i);
        std::vector<int> to_work(to_sort.size()), count(10, 0);

        for (auto& j : to_sort) {
            count[(j / aux) % 10]++;
        }

        for (int j = 1; j < 10; j++) {
            count[j] += count[j - 1];
        }

        for (int j = to_sort.size() - 1; j >= 0; j--){
            to_work[count[(to_sort[j] / aux) % 10] - 1] = to_sort[j];
            count[(to_sort[j] / aux) % 10]--;
        }

        for (int j = 0; j < to_sort.size(); j++) {
            to_sort[j] = to_work[j];
        }

    }
}

/*=============================*/
// MERGE SORT
/*=============================*/

/**
 * @brief Vector sort and merge of merge sort algorythm.
 * 
 * @param to_sort The vector to sort.
 * @param begin Partition begin.
 * @param middle Partition center.
 * @param end Partition end.
 */
void merge_sort_merge(std::vector<int>&to_sort, int begin, int middle, int end){
    auto const subArrayOne = middle - begin + 1;
    auto const subArrayTwo = end - middle;

    // Create temp arrays
    auto* leftArray = new int[subArrayOne];
    auto* rightArray = new int[subArrayTwo];

    // Copy data to temp to_sorts leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++){
        leftArray[i] = to_sort[begin + i];
    }
    for (auto j = 0; j < subArrayTwo; j++){
        rightArray[j] = to_sort[middle + 1 + j];
    }
    int indexOfSubArrayOne = 0; // Initial index of first sub-to_sort
    int indexOfSubArrayTwo = 0; // Initial index of second sub-to_sort
    int indexOfMergedArray = begin; // Initial index of merged to_sort

    // Merge the temp to_sorts back into to_sort[left..right]
    while ((indexOfSubArrayOne < subArrayOne) && 
            (indexOfSubArrayTwo < subArrayTwo)){
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]){
            to_sort[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }else{
            to_sort[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of left[], if there are any
    while (indexOfSubArrayOne < subArrayOne){
        to_sort[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo){
        to_sort[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

/**
 * @brief Recursive merge sort algorythm.
 * 
 * @param to_sort The vector to sort.
 * @param begin Partition begin.
 * @param end Partition end.
 */
void merge_sort_split(std::vector<int>& to_sort, int begin, int end){
    if (begin >= end){
        return;
    }
    int middle = begin + (end - begin) / 2;
    merge_sort_split(to_sort, begin, middle);
    merge_sort_split(to_sort, middle + 1, end);
    merge_sort_merge(to_sort, begin, middle, end);
}

/**
 * @brief Merge sort algorythm that calls the recursive one.
 * 
 * @param to_sort The vector to sort.
 */
void merge_sort_topdown(std::vector<int>& to_sort){
    merge_sort_split(to_sort, 0, to_sort.size() - 1);
}


/*=============================*/
// GENERATE RANDOM VECTOR
/*=============================*/

/**
 * @brief Generates a sorted vector.
 * 
 * @param n Size of the new vector.
 * @param max Vector max value.
 * @return std::vector<int> 
 */
std::vector<int> generate_sorted_vector(int n, int max){
    std::vector<int> result;
    for (int i = 0; i < n; i++){
        if (i < max){
            result.push_back(i);
        } else {
            result.push_back(max);
        }
    }
    return result;
}

/**
 * @brief Generates a random vector.
 * 
 * @param n Size of the new vector
 * @param max Vector max value.
 * @return std::vector<int> 
 */
std::vector<int> generate_random_vector(int n, int max){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distN(0, max);

    std::vector<int> result;
    for (int i = 0; i < n; i++){
        result.push_back(distN(rng));
    }
    return result;
}

/**
 * @brief Generates an anti-sorted vector.
 * 
 * @param n Size of the new vector.
 * @param max Vector max value.
 * @return std::vector<int> 
 */
std::vector<int> generate_anti_sorted_vector(int n, int max){
  std::vector<int> result;
  int aux;
  for (int i = 0; i < n; i++){
    aux = max - i;
    if (aux > 0){
      result.push_back(aux);
    }else{
      result.push_back(0);
    }
  }
  return result;
}

/*=============================*/
// MAIN
/*=============================*/

/**
 * @brief Tests for each sorting algorythm.
 * 
 * @param v The initial vector to sort.
 */
void test_sorts(std::vector<int>& v){
    std::vector<int> to_quick_order = v, to_radix_order = v, to_merge_order = v;    
    
    std::chrono::steady_clock::time_point begin, end;
    
    //std::cout << " > Original: " << v << std::endl;
    std::cout << "== Quicksort =========================" << std::endl;
    begin = std::chrono::steady_clock::now();
    quick_sort(to_quick_order);
    end = std::chrono::steady_clock::now();
    std::cout << "Time cost = " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << "[us]" << std::endl;

    std::cout << "== Radixsort =========================" << std::endl;
    begin = std::chrono::steady_clock::now();
    radix_sort(to_radix_order);
    end = std::chrono::steady_clock::now();
    std::cout << "Time cost = " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << "[us]" << std::endl;
    
    std::cout << "== Mergesort =========================" << std::endl;
    begin = std::chrono::steady_clock::now();
    merge_sort_topdown(to_merge_order);
    end = std::chrono::steady_clock::now();
    std::cout << "Time cost = " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << "[us]" << std::endl;
}

/**
 * @brief Tests for each sorting algorythm printing vectors.
 * 
 * @param v The initial vector to sort.
 */
void test_sorts_showing(std::vector<int>& v){
    std::vector<int> to_quick_order = v, to_radix_order = v, to_merge_order = v;    
    std::chrono::steady_clock::time_point begin, end;
    
    std::cout << " > Original: " << v << std::endl;
    std::cout << "== Quicksort =========================" << std::endl;
    begin = std::chrono::steady_clock::now();
    quick_sort(to_quick_order);
    end = std::chrono::steady_clock::now();
    std::cout << "Time cost = " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << "[us]" << std::endl;
    std::cout << " > Sorted:   " << to_quick_order << std::endl; 

    std::cout << "== Radixsort =========================" << std::endl;
    begin = std::chrono::steady_clock::now();
    radix_sort(to_radix_order);
    end = std::chrono::steady_clock::now();
    std::cout << "Time cost = " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << "[us]" << std::endl;
    std::cout << " > Sorted:   " << to_radix_order << std::endl;

    
    std::cout << "== Mergesort =========================" << std::endl;
    begin = std::chrono::steady_clock::now();
    merge_sort_topdown(to_merge_order);
    end = std::chrono::steady_clock::now();
    std::cout << "Time cost = " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << "[us]" << std::endl;
    std::cout << " > Sorted:   " << to_merge_order << std::endl;
    
}
