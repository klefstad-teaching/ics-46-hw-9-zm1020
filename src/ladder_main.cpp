#include "ladder.h"

int main() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    string begin_word, end_word;
    
    cout << "Enter the starting word: ";
    cin >> begin_word;
    
    cout << "Enter the ending word: ";
    cin >> end_word;
    
    if (begin_word == end_word) {
        cout << "Error: Start and end words are the same" << endl;
        return 1;
    }
    
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    print_word_ladder(ladder);
    
    return 0;
}