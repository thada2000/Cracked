#include "provided.h"
#include <string>
#include<map>
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
	vector<map<char, char>> m_stack; //vector of maps manipulated like a stack
	int numPush;
	int numPop;
};

//Translator constructor
TranslatorImpl::TranslatorImpl() {
	map<char, char> init;
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	for(int i = 0; i<26; i++) 
		init[alphabet[i]]='?';
	m_stack.push_back(init); // push initial ? mark map
	numPush = 0; // no pushes
	numPop = 0;// no pops
}
bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	map<char,char> currMap = m_stack[m_stack.size() - 1]; // current mapping
	if (ciphertext.size() != plaintext.size()) // if not same size return false
	    return false;

	for (int i = 0; i < ciphertext.size(); i++) { //if both strings contain non alphabets return false
		if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
			return false;
	}
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < ciphertext.size(); i++) { // check if mappings are valid and do not collide
		map<char, char> ::iterator it;
		it = currMap.find(tolower(ciphertext[i]));
		if (it->second != '?'&& tolower(plaintext[i]) != it->second)
			return false;
		for (int j = 0; j < currMap.size(); j++) {
			it = currMap.find(alphabet[j]);
			if (it->second == tolower(plaintext[i]) && it->first != tolower(ciphertext[i]))
				return false;
		}
	}
	map<char, char> add = currMap; 
	for (int i = 0; i < ciphertext.size(); i++) { // add new mapping
		add[tolower(ciphertext[i])] = tolower(plaintext[i]);
	}
	m_stack.push_back(add); // push onto stack
	numPush++; // increment number pushed
	return true;
	
}

bool TranslatorImpl::popMapping()
{
	if (numPop >= numPush) // do not allow to pop if nothing has been pushed
		return false;
	if (m_stack.empty()) // if nothing in stack return
	return false;
	m_stack.pop_back(); //pop from stack
	numPop++;// increment number popped
	return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string s;
	map<char, char> currMap=m_stack[m_stack.size()-1]; //current map
	
	for (int i = 0; i < ciphertext.size(); i++) {
		if (!isalpha(ciphertext[i])) //if not alphabet leave ciphertext char as is
			s += ciphertext[i];
		else {
			if (isalpha(ciphertext[i])) {//else add current translation to char from map
				if (isupper(ciphertext[i]))
					s += toupper(currMap.find(tolower(ciphertext[i]))->second);
				else s += currMap.find(tolower(ciphertext[i]))->second;
			}
		}

	}
	return s; //return final string
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
