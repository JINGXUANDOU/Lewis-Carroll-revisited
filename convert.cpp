#include "convert.hpp"
#include "Wordset.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <map>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_set>

// You should not need to change this function.
void loadWordsIntoTable(WordSet & words, std::istream & in)
{
	std::string line, word;
	std::stringstream ss;

	while(	getline(in, line) )
	{
		ss.clear();
		ss << line;
		while( ss >> word )
		{
			words.insert( word );
		}
	}

}


// You probably want to change this function.
std::vector< std::string > convert(const std::string & s1, const std::string & s2, const WordSet & words)
{

 	std::queue<std::string> word_queue;
 	std::unordered_set<std::string, std::string> mapping;
	word_queue.push(s1);
 
	while(!word_queue.empty()) {
    	std::string word = word_queue.front();
    	word_queue.pop();
 
    	if (word == s2)
			break;
 
    	for (int i = 0; i < word.size(); i++) {
			std::string conv_word = word;

			for (int j = 'a'; j <= 'z'; j++) {
				if (j == word[i])
					continue;

				char bak = conv_word[i];
				conv_word[i] = j;

				if (words.contains(conv_word) && mapping.find(conv_word) == mapping.end()) {
					word_queue.push(conv_word);
					mapping.insert({conv_word, word});
				}

				conv_word[i] = bak;
			}
    	}
	}

	size_t count = 0;
	std::string key = s2;

	while (key != s1) {  
		++count;
    	key = mapping[key];
	}

 	std::vector<std::string> result(count);
	key = s2;

	while (count > 0) {  
    	result[--count] = key;
    	key = mapping[key];
	}

	return result;
}
