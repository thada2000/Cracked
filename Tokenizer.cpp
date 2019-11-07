#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
	string m_separators; //string containing separators
}; 

TokenizerImpl::TokenizerImpl(string separators)
{
	m_separators = separators;
}


vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
	vector<string> choppedStr;
	int lastSepIndex=-1; //index of last separator starts at -1
	for (int i = 0; i < s.size(); i++) {
		bool is_sep = false; // bool to check if char is a separator
   	  for (int j = 0; j < m_separators.size(); j++) {
			
			if (s[i] == m_separators[j]) {// if char is a separator
				if (s.substr(lastSepIndex+1, i - lastSepIndex-1) != "") //if substring is not empty
					choppedStr.push_back(s.substr(lastSepIndex+1, i - lastSepIndex-1)); //push substr onto vector
					lastSepIndex = i; //set index of last separator
					is_sep = true;
					break;
			}
	}
	  if (i == s.size() - 1) {
		  if (is_sep) // is a separator do nothing
			  continue;
		  else {
			  //push substring onto vector
			  choppedStr.push_back(s.substr(lastSepIndex + 1, i - lastSepIndex ));
			  break;
		  }
	  }
	}
	return choppedStr;// This compiles, but may not be correct
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
