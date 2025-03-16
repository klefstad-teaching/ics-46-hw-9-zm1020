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
    
    if (is_adjacent(begin, end)) {
        return {begin, end};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
    const int MAX_LADDER_SIZE = 10; 
    int begin_len = begin.length();
    int end_len = end.length();
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        if (current_ladder.size() > MAX_LADDER_SIZE) {
            continue; 
        }
        
        string last_word = current_ladder.back();
        int last_len = last_word.length();
        
        for (const string& word : word_list) { 
            if (visited.find(word) != visited.end()) continue;
            if (abs((int)word.length() - last_len) > 1) continue; 
            if (!is_adjacent(last_word, word)) continue;
            
            vector<string> new_ladder = current_ladder;
            new_ladder.push_back(word);
            visited.insert(word);
            
            if (word == end) {
                return new_ladder;
            }
            
            ladder_queue.push(new_ladder);
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
    word_list.insert("cat");
    word_list.insert("cot");
    word_list.insert("cog");
    word_list.insert("dog");
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
}