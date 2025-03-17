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
        int diff = 0;
        for (int i = 0; i < len1; i++) {
            if (str1[i] != str2[i]) {
                diff++;
                if (diff > d) return false;
            }
        }
        return true;
    }
    
    if (abs(len1 - len2) == 1 && d == 1) {
        const string& shorter = (len1 < len2) ? str1 : str2;
        const string& longer = (len1 < len2) ? str2 : str1;
        
        for (int i = 0; i <= (int)longer.length() - (int)shorter.length(); i++) {
            bool match = true;
            for (int j = 0, k = 0; j < (int)shorter.length(); j++, k++) {
                if (k == i) k++;
                if (shorter[j] != longer[k]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
    }
    
    return false;
}

bool is_adjacent(const string& word1, const string& word2) {
    int len1 = word1.length();
    int len2 = word2.length();
    
    if (len1 != len2) {
        if (abs(len1 - len2) != 1) return false;
        
        const string& shorter = (len1 < len2) ? word1 : word2;
        const string& longer = (len1 < len2) ? word2 : word1;
        
        for (int i = 0; i < (int)longer.length(); i++) {
            string temp = longer;
            temp.erase(i, 1);
            if (temp == shorter) return true;
        }
        return false;
    }
    
    int diff = 0;
    for (int i = 0; i < len1; i++) {
        if (word1[i] != word2[i]) {
            diff++;
            if (diff > 1) return false;
        }
    }
    return diff == 1;
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