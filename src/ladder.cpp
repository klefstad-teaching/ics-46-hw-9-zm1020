#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << endl;
    cerr << "Cannot create word ladder from " << word1 << " to " << word2 << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int len1 = str1.length();
    int len2 = str2.length();
    
    if (abs(len1 - len2) > d) {
        return false;
    }
    
    if (len1 == len2) {
        int differences = 0;
        for (int i = 0; i < len1; i++) {
            if (str1[i] != str2[i]) {
                differences++;
                if (differences > d) return false;
            }
        }
        return true;
    }
    
    if (d == 1 && abs(len1 - len2) == 1) {
        const string& shorter = (len1 < len2) ? str1 : str2;
        const string& longer = (len1 < len2) ? str2 : str1;
        
        int i = 0, j = 0;
        bool skipped = false;
        
        while (i < shorter.length() && j < longer.length()) {
            if (shorter[i] == longer[j]) {
                i++;
                j++;
            } else {
                if (skipped) return false;
                skipped = true;
                j++;
            }
        }
        
        return (j == longer.length() || !skipped);
    }
    
    return false;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream fin(file_name);
    if (!fin) {
        cerr << "Error: Could not open file " << file_name << endl;
        exit(1);
    }
    
    string word;
    while (fin >> word) {
        for (char& c : word) {
            c = tolower(c);
        }
        word_list.insert(word);
    }
    
    fin.close();
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    string begin = begin_word;
    string end = end_word;
    for (char& c : begin) c = tolower(c);
    for (char& c : end) c = tolower(c);
    
    if (begin == end) {
        error(begin, end, "Start and end words are the same");
        return {};
    }
    
    if (word_list.find(end) == word_list.end()) {
        error(begin, end, "End word not found in dictionary");
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
    // For performance, filter the dictionary first by length
    set<string> filtered_dict;
    int begin_len = begin.length();
    int end_len = end.length();
    
    for (const auto& word : word_list) {
        int len = word.length();
        if (len == begin_len || len == begin_len + 1 || len == begin_len - 1 ||
            len == end_len || len == end_len + 1 || len == end_len - 1) {
            filtered_dict.insert(word);
        }
    }
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        int last_len = last_word.length();
        
        // Only check words with similar length
        for (const string& word : filtered_dict) {
            if (visited.find(word) != visited.end()) continue;
            
            int word_len = word.length();
            if (abs(word_len - last_len) > 1) continue;
            
            if (is_adjacent(last_word, word)) {
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(word);
                
                if (word == end) {
                    return new_ladder;
                }
                
                visited.insert(word);
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    error(begin, end, "No word ladder exists");
    return {};
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " ";
        }
    }
    cout << " " << endl;
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}