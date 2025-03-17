#include "ladder.h"

void error(string word1, string word2, string msg) {
    cout << "Error: " << word1 << " " << word2 << " - " << msg << endl;
    exit(1);
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = str1.length(), len2 = str2.length();
    if (abs(len1 - len2) > d) return false;
    
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));
    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;
    
    for (int i = 1; i <= len1; i++)
        for (int j = 1; j <= len2; j++)
            dp[i][j] = min({dp[i-1][j] + 1, dp[i][j-1] + 1, 
                           dp[i-1][j-1] + (str1[i-1] != str2[j-1])});
    
    return dp[len1][len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, 
                                  const set<string>& word_list) {
    if (begin_word == end_word) 
        error(begin_word, end_word, "Start and end words must differ");
    
    if (word_list.find(end_word) == word_list.end())
        return vector<string>();
        
    queue<vector<string>> ladder_queue;
    set<string> visited;
    
    ladder_queue.push({begin_word});
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = ladder.back();
        
        for (const string& word : word_list) {
            if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                visited.insert(word);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                
                if (word == end_word)
                    return new_ladder;
                    
                ladder_queue.push(new_ladder);
            }
        }
    }
    return vector<string>();
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    string word;
    while (file >> word) {
        for (char& c : word) c = tolower(c);
        word_list.insert(word);
    }
    file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No ladder found" << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) cout << " -> ";
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    #define my_assert(e) {cout << #e << ((e) ? " passed" : " failed") << endl;}
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}