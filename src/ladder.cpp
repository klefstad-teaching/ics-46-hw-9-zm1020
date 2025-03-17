#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << " to " << word2 << ")" << endl;
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

bool edit_distance_within(const string& str1, const string& str2, int d) {
    if (str1 == str2) {
        return true;
    }
    
    size_t len1 = str1.length(), len2 = str2.length();
    
    if (abs(static_cast<int>(len1 - len2)) > d) {
        return false;
    }
    
    if (d == 1) {
        if (len1 == len2) {
            int diffs = 0;
            for (size_t i = 0; i < len1; i++) {
                if (str1[i] != str2[i]) diffs++;
                if (diffs > 1) return false;
            }
            return diffs <= 1;
        } else {
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
                    i++; j++;
                }
            }
            return (j - i) <= 1;
        }
    }
    
    // Only use full matrix for d > 1
    vector<int> prev(len2 + 1), curr(len2 + 1);
    
    for (size_t j = 0; j <= len2; j++) prev[j] = j;
    
    for (size_t i = 1; i <= len1; i++) {
        curr[0] = i;
        for (size_t j = 1; j <= len2; j++) {
            if (str1[i-1] == str2[j-1]) {
                curr[j] = prev[j-1];
            } else {
                curr[j] = 1 + min({prev[j], curr[j-1], prev[j-1]});
            }
        }
        prev = curr;
    }
    
    return prev[len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) {
        return true;
    }
    
    size_t len1 = word1.length(), len2 = word2.length();
    int len_diff = abs(static_cast<int>(len1 - len2));
    
    if (len_diff > 1) {
        return false;
    }
    
    if (len1 == len2) {
        int diff_count = 0;
        for (size_t i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                diff_count++;
                if (diff_count > 1) return false;
            }
        }
        return diff_count == 1;
    }
    
    const string& shorter = (len1 < len2) ? word1 : word2;
    const string& longer = (len1 < len2) ? word2 : word1;
    
    size_t i = 0, j = 0;
    bool skipped = false;
    
    while (i < shorter.length() && j < longer.length()) {
        if (shorter[i] != longer[j]) {
            if (skipped) return false;
            skipped = true;
            j++;
        } else {
            i++; j++;
        }
    }
    
    return (j - i) <= 1;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, 
                                  const set<string>& word_list) {
    string begin = begin_word, end = end_word;
    for (char& c : begin) c = tolower(c);
    for (char& c : end) c = tolower(c);
    
    if (begin == end) {
        return {};
    }
    
    if (word_list.count(end) == 0) {
        return {};
    }
    
    // Filter the dictionary to include only relevant words
    set<string> filtered_words;
    size_t begin_len = begin.length();
    
    for (const string& word : word_list) {
        size_t len = word.length();
        if (abs(static_cast<int>(len - begin_len)) <= 1) {
            filtered_words.insert(word);
        }
    }
    
    queue<vector<string>> ladder_queue;
    set<string> visited;
    
    visited.insert(begin);
    ladder_queue.push({begin});
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        for (const string& word : filtered_words) {
            if (visited.count(word) > 0) {
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

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << " ";
    }
    cout << endl;
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

