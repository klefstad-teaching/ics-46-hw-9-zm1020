#include "ladder.h"
#include <chrono>

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << " to " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    size_t len1 = str1.length(), len2 = str2.length();
    if (abs(static_cast<int>(len1 - len2)) > d) return false;
    
    if (len1 == len2) {
        int diff = 0;
        for (size_t i = 0; i < len1; i++) {
            if (str1[i] != str2[i]) diff++;
            if (diff > d) return false;
        }
        return true;
    }
    
    if (d == 1) {
        const string& shorter = len1 < len2 ? str1 : str2;
        const string& longer = len1 < len2 ? str2 : str1;
        size_t i = 0, j = 0;
        bool skipped = false;
        
        while (i < shorter.length() && j < longer.length()) {
            if (shorter[i] != longer[j]) {
                if (skipped) return false;
                skipped = true;
                j++;
            } else {
                i++;
                j++;
            }
        }
        return (j - i) <= 1;
    }
    return false;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream fin(file_name);
    if (!fin) {
        cerr << "Error: Could not open " << file_name << endl;
        exit(1);
    }
    string word;
    while (fin >> word) {
        for (char& c : word) c = tolower(c);
        word_list.insert(word);
    }
    fin.close();
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, 
                                  const set<string>& word_list) {
    string begin = begin_word, end = end_word;
    for (char& c : begin) c = tolower(c);
    for (char& c : end) c = tolower(c);
    
    if (begin == end) return {};
    if (word_list.count(end) == 0) return {};
    
    queue<vector<string>> ladder_queue;
    set<string> visited{begin};
    ladder_queue.push({begin});
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last = ladder.back();
        
        for (const string& word : word_list) {
            if (visited.count(word) == 0 && is_adjacent(last, word)) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if (word == end) return new_ladder;
                visited.insert(word);
                ladder_queue.push(new_ladder);
            }
        }
    }
    return {};
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << (i < ladder.size() - 1 ? " " : "\n");
    }
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}

void test_verify_word_ladder_time() {
    auto start = std::chrono::high_resolution_clock::now();
    verify_word_ladder();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "verify_word_ladder took " << duration.count() << " milliseconds" << endl;
}

