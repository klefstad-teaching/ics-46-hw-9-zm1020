#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << endl;
    cerr << "Cannot create word ladder from " << word1 << " to " << word2 << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    if (abs(static_cast<int>(str1.length()) - static_cast<int>(str2.length())) > d) {
        return false;
    }
    
    int len1 = str1.length();
    int len2 = str2.length();
    
    if (d == 1 && abs(len1 - len2) == 1) {
        const string& shorter = (len1 < len2) ? str1 : str2;
        const string& longer = (len1 < len2) ? str2 : str1;
        
        int i = 0, j = 0;
        int diff = 0;
        
        while (i < shorter.length() && j < longer.length()) {
            if (shorter[i] == longer[j]) {
                i++;
                j++;
            } else {
                diff++;
                if (diff > 1) return false;
                j++;
            }
        }
        
        // If we reach the end with characters left in longer, count as a diff
        if (j < longer.length()) {
            diff++;
        }
        
        return diff <= d;
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
    
    // Direct check if start and end are adjacent
    if (is_adjacent(begin, end)) {
        return {begin, end};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
    // Add a safety counter to prevent infinite loops
    int iterations = 0;
    const int MAX_ITERATIONS = 1000000; // Adjust as needed
    
    while (!ladder_queue.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        for (const string& word : word_list) {
            if (visited.find(word) != visited.end()) continue;
            
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
    
    if (iterations >= MAX_ITERATIONS) {
        error(begin, end, "Reached maximum iterations, possible infinite loop");
    } else {
        error(begin, end, "No word ladder exists");
    }
    return {};
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder exists." << endl;
        return;
    }
    
    cout << "Found ladder with " << ladder.size() << " words: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " → ";
        }
    }
    cout << endl;
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