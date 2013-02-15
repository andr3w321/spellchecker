#include <fstream>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Dictionary {
	Dictionary(char* inputFile) {
		// load words
		ifstream input (inputFile);
		for (string line; getline(input, line);) _words.insert(line);
  	}
	bool contains(string const& word) const { return _words.count(word); }

	set<string> _words;
};

vector<int> find_repeat_positions(string str);
void check_remove_variations(string permuted_removals, vector<int> repeat_positions, set<string> &checked_remove_vars, Dictionary &dict, string &found_word);
vector<int> find_vowel_positions(string str);
void check_vowel_variations(string permuted_vowels, int nvowels, int level, string possible_word, vector<int> vowelpos, Dictionary &diction, string &found_word);
bool is_vowel(char ch);
void remove_extra_letters(string &str);

int main(int argc, char* argv[]) 
{
	if(argc != 2) {
		cout << "You must pass a dictionary file as an argument.  For example:\n\nspellcheck /usr/share/dict/words\n\n";
		return 1;
	}

	// read dictionary file
	Dictionary dict(argv[1]);

	// loop until user closes program
	while(true) {

		// input a word
		string word;
		cout << "> ";
		cin >> word;

		// convert to lowercase
		transform(word.begin(), word.end(),word.begin(), ::tolower);

		string output = "";

		// check if exists in dictionary
		if(dict.contains(word)) output = word;

		remove_extra_letters(word);

		if(output == "") {
			vector<int> vowel_positions = find_vowel_positions(word);
			check_vowel_variations("", vowel_positions.size(), 0, word, vowel_positions, dict, output);
		}

		if(output == "") {
			vector<int> repeat_pos = find_repeat_positions(word);
			set<string> checked_remove_variations;
			check_remove_variations(word, repeat_pos, checked_remove_variations, dict, output);
		}

		if(output == "") output = "NO SUGGESTION";
		cout << output << endl;
		
	}

	return 0;
}

vector<int> find_repeat_positions(string str)
{
	vector<int> repeat_positions;
	bool in_repeated_string = false;
	
	char prev_char = str[0];
	for(unsigned int i = 1; i < str.length(); i++) {
		int this_char = str[i];
		if((prev_char == this_char || (is_vowel(prev_char) && is_vowel(this_char))) && !in_repeated_string) {
			in_repeated_string = true;
			repeat_positions.push_back(i-1);
			
		} else {
			in_repeated_string = false;
		}
		prev_char = this_char;
	}
	return repeat_positions;
}

void check_remove_variations(string permuted_removals, vector<int> repeat_positions, set<string> &checked_remove_vars, Dictionary &dict, string &found_word)
{
	if(repeat_positions.size() > 0) {
		for(unsigned int i = 0; i < repeat_positions.size(); i++) {

			// remove a char from string
			string new_string = permuted_removals.substr(0, repeat_positions[i]) + permuted_removals.substr(repeat_positions[i]+1, permuted_removals.length());
			if(dict.contains(new_string)) found_word = new_string;

			//check vowel variations
			if(checked_remove_vars.count(new_string) == 0) {
				if(found_word == "") {
					vector<int> vowel_positions = find_vowel_positions(new_string);
					check_vowel_variations("", vowel_positions.size(), 0, new_string, vowel_positions, dict, found_word);
				}
				checked_remove_vars.insert(new_string);
				vector<int> new_repeat_pos = find_repeat_positions(new_string);
				check_remove_variations(new_string, new_repeat_pos, checked_remove_vars, dict, found_word);
			}
		}
	}
}


vector<int> find_vowel_positions(string str)
{
	vector<int> vowel_pos;
	for(unsigned int i = 0; i < str.length(); i++) {
		if(is_vowel(str[i])) vowel_pos.push_back(i);
	}
	return vowel_pos;

}

void check_vowel_variations(string permuted_vowels, int nvowels, int level, string possible_word, vector<int> vowelpos, Dictionary &diction, string &found_word)
{
	if(level == nvowels) { // found a vowel variation

		// substitute the possible word with vowel variation (eg Hello for Halla)
		for(int i = 0; i < nvowels; i++) {

			// convert permuted vowel char to const char*
			string sym(1, permuted_vowels[i]);

			// substitute new vowel into possible_word
			possible_word.replace(vowelpos[i],1,sym.c_str());	

		}

		// check if possible word in dictionary
		if(diction.contains(possible_word)) found_word = possible_word;

	} else if(found_word == "") { // keep calling until find a match

		string vowels = "aeiou";
		for(int i = 0; i < 5; i++) { 
			check_vowel_variations(permuted_vowels+vowels[i], nvowels, level+1, possible_word, vowelpos, diction, found_word);
		}
	}
}

bool is_vowel(char ch)
{
	return(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

// no english word has more than 5 consecutive vowels
// no english word has more than 2 consecutive letters
void remove_extra_letters(string &str)
{
	// if letter is a consonent, add to shortened string if previous two letters are not the same
	// if letter is a vowel, add to shortened string if four previous letters are not vowels
	string shortened_word = "";
		for(unsigned int i = 0; i < str.length(); i++) {
			bool add_char = true;
			if(is_vowel(str[i])) {
				if(shortened_word.length() > 4) {
					if(	(is_vowel(str[i-1]) == true) && 
						(is_vowel(str[i-2]) == true) && 
						(is_vowel(str[i-3]) == true) &&
						(is_vowel(str[i-4]) == true) &&
						(is_vowel(str[i-5]) == true)) {
						add_char = false;
					}
				} 
			} else {
				if(shortened_word.length() > 1) {
					if(	(str[i] == str[i-1]) && 
						(str[i] == str[i-2])) {
						add_char = false;
					}
				}
			}
			if(add_char) shortened_word += str[i];
		}
	str = shortened_word;
}
