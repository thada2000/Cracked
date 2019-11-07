#include "provided.h"
#include <string>
#include <vector>
#include "MyHash.h"
#include <cctype>
#include<iostream>
#include<fstream>
using namespace std;


class WordListImpl
{
public:
	
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> m_words; //my hash that contains the list of words in a hash table
	string getLetterPattern(string word) const; // private function that gets a string letter pattern
};


bool WordListImpl::loadWordList(string filename)
{
	ifstream infile(filename);  
	if (!infile)		        //check if file does not load
	{
		return false;
	}
	while ((!infile)==false) { // while there are words in file
		string k;
		infile >> k; 
		if (m_words.find(getLetterPattern(k)) != nullptr) {//check if there is a letter pattern instance in hash table
			m_words.find(getLetterPattern(k))->push_back(k); //push word onto the vector
		}
		
		else {
			vector<string> add; //else associate new vector to hash table
			add.push_back(k);
			m_words.associate(getLetterPattern(k), add); }
    }
	
	return true;

	// This compiles, but may not be correct
}

bool WordListImpl::contains(string word) const
{
	string s = getLetterPattern(word); 
	if (m_words.find(s) != nullptr) {  // if word letter pattern vector has wrods
		vector<string> temp = *(m_words.find(s)); //retrieve that vector
		for (int i = 0; i < m_words.find(s)->size(); i++) {//iterate through vector
			string p = ""; // two new string that will contain lowercase letters
			string q = "";
				if (temp[i].size() != word.size()) // if the two words dont have same length continue looping
					continue;
				for (int j = 0; j < temp[i].size(); j++) {//lowercase the 2 words
					p += tolower(temp[i][j]);
					q += tolower(word[j]);
				}
			if (p == q)
				return true; // return true if they are equal
		}
	}
	return false;// if word not found return
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	vector<string> candidates;
	if (cipherWord.size() != currTranslation.size()) //if the 2 strings are unequal return an empty vector
		return candidates;
	for (int i = 0; i < cipherWord.size(); i++) //check if 2 strings are valid and their individual characters are paired correctly
	{
		if (!isalpha(cipherWord[i]) && cipherWord[i] != '\'')
			return candidates;
		if (!isalpha(currTranslation[i]) && currTranslation[i] != '\''&&currTranslation[i] != '?')
			return candidates;
		if (isalpha(currTranslation[i]) && !isalpha( cipherWord[i]))
			return candidates;
		if (currTranslation[i] == '?' && !isalpha(cipherWord[i]))
			return candidates;
		if (currTranslation[i] == '\'' && cipherWord[i] != '\'')
			return candidates;
	}
	// find vector of candidates with same letter patter
	const vector<string> possible_candi = *( m_words.find(getLetterPattern(cipherWord)));
	if (possible_candi.empty()) // if there are no words with that letter pattern
		return candidates;//return empty vector
	for (int j = 0; j < possible_candi.size(); j++) {//iterate though vector and check for candidates
		bool cont = false;
		if (possible_candi[j].size() != cipherWord.size())
			continue;
			for (int i = 0; i < currTranslation.size(); i++) // check each character in order to approve the possible candidate as a candidate
			{
				if (isalpha(currTranslation[i]) && (tolower(currTranslation[i]) != tolower(possible_candi[j][i]))) {
					cont = true;
					break;
				}
				if (currTranslation[i] == '\''&&possible_candi[j][i] != '\'') {
					cont = true;
					break;
				}
				if (currTranslation[i] == '?' && !isalpha(possible_candi[j][i])) {
					cont = true;
					break;
				}
				
				
			}
			
			if(!cont)
			candidates.push_back(possible_candi[j]); // push the string onto the candidate vector

		
	}
	return candidates; //return the candidate vector
}

//create a letter patter according th the string
string WordListImpl::getLetterPattern(string word) const
{
	string s;
	s += 'a';
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	int currAlph = 1;
	for (int i = 1; i < word.size(); i++)  
	{
		bool found = false;
		for (int j = 0; j < i; j++)  {
			
			if (toupper(word[i]) == toupper(word[j])) {
				s += s[j];
				found = true;
				break;
			}
			
		}
		if(!found) {
			char a = static_cast<char>(alphabet[currAlph]);
			s += a;
			currAlph++;
		}
    }
	return s;
}


//***** hash functions for string, int, and char *****

unsigned int hasher(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hasher(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hasher(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
