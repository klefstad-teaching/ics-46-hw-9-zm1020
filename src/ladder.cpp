#include "ladder.h"
bool is_adjacent(const string& word1, const string& word2) {
    int len1 = word1.length();
    int len2 = word2.length();
    
    if (len1 == len2) {
        int diff = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                diff++;
                if (diff > 1) return false;
            }
        }
        return diff == 1;
    }
    
    if (abs(len1 - len2) == 1) {
        const string& shorter = (len1 < len2) ? word1 : word2;
        const string& longer = (len1 < len2) ? word2 : word1;
        
        int j = 0;
        for (int i = 0; i < shorter.length() && j < longer.length(); ) {
            if (shorter[i] == longer[j]) {
                i++;
                j++;
            } else {
                j++;
                if (j - i > 1) return false;
            }
        }
        return true;
    }
    
    return false;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    string begin = begin_word;
    string end = end_word;
    for (char& c : begin) c = tolower(c);
    for (char& c : end) c = tolower(c);
    
    if (begin == end) {
        return {};
    }
    
    if (word_list.find(end) == word_list.end()) {
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        for (const string& word : word_list) {
            if (visited.find(word) != visited.end()) {
                continue;
            }
            
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
    
    return {};
}

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << endl;
    cerr << "Cannot create word ladder from " << word1 << " to " << word2 << endl;
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
    
    vector<string> ladder = generate_word_ladder("cat", "dog", word_list);
    my_assert(ladder.size() == 4);
}