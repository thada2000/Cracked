#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
	DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
	WordList m_list; 
	Translator m_translator;
	Tokenizer m_token;
};

bool DecrypterImpl::load(string filename)
{
   return m_list.loadWordList(filename);  //use the wordlist function to load the list of words
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	vector<string> solnVector; // vector that contains the final solutions
	string translate = m_translator.getTranslation(ciphertext); // translate the ciphertext string
	vector<string> tokenizedtrans = m_token.tokenize(translate);//tokenize the translated and ciphertext strings
	vector<string> tokenized = m_token.tokenize(ciphertext);
	string currLongest="";
	int maxNumUntranslated=0;
	for (int i = 0; i < tokenized.size(); i++) {//find the word with the most untranslated letters
		int count = 0;
		for (int j = 0; j < tokenized[i].size(); j++)
			if (tokenizedtrans[i][j] == '?')
				count++;
		if (count > maxNumUntranslated) {
			currLongest = tokenized[i];
			maxNumUntranslated = count;
		}
		}
	string translation = m_translator.getTranslation(currLongest);//get translation of most untranslated word
	vector<string>candidates = m_list.findCandidates(currLongest, translation); // find a vector of candidates
	if (candidates.empty())
	   return candidates;//return an empty vector if there are no candidates
	for (int i = 0; i < candidates.size(); i++) {//iterate thorugh candidates vector
		if (!m_translator.pushMapping(currLongest, candidates[i])) //push mapping if possibe if not continue the loop
			continue;
		bool is_invalid = false;
		bool is_text_translated = true;
		string currTranslation = m_translator.getTranslation(ciphertext); // get new translation
		vector<string> tokenize2 = m_token.tokenize(currTranslation);//tokenize the new translation
		for (int j = 0; j < tokenize2.size(); j++) { // check to see if the string is fully tanslated and if all of the words are valid
			bool isUntranslated = false;
			for (int z = 0; z < tokenize2[j].size(); z++) {
				if (tokenize2[j][z] == '?') {
					isUntranslated = true;
					break;
				}
				  
			}
			if (!isUntranslated) {//check to see if word is in word list
				string temp = "";
				for (int k = 0; k < tokenize2[j].size(); k++)
					if (isupper(tokenize2[j][k]))
						temp += tolower(tokenize2[j][k]);
					else temp += tokenize2[j][k];
				if (!m_list.contains(temp)) {
					is_invalid = true;
					break;
				}
			}
			else { is_text_translated = false; }
     	}
		if (is_invalid) {
			m_translator.popMapping(); //remove current mapping if string is invalid
			continue;
		}
		if (is_text_translated) { //remove mapping and push the translation onto the solution vector if the string is fully translated and valid
			m_translator.popMapping();
			solnVector.push_back(currTranslation);
			
		}
		else { vector<string> soln = crack(ciphertext);//if not fully translated recursively call crack using the current mapping
		for (int k = 0; k < soln.size(); k++) {
			solnVector.push_back(soln[k]); //push onto the solution vector the values returned by crack
		}
		m_translator.popMapping();//pop the mapping once done
		
		}
	}
	sort(solnVector.begin(), solnVector.end());//sort the vector alphabetically
	return solnVector; //return the solution vector
}

DecrypterImpl::DecrypterImpl() :m_token(" ,;:.!()[]{}-\"#$%^&1234567890") {}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
