#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl(): m_tokenizer(" ,;:.!()[]{}-\"#$%^&")
    {
    }
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList m_wordlist; //wordlist member
    Translator m_translator; //translator member
    Tokenizer m_tokenizer; //tokenizer member
    vector<string> finalwords; //store my solutions in this vector to return it
    
    string returnLower(string makemeLower) const
    {
        string holder = makemeLower;
        for(int i = 0; i<makemeLower.size(); i++)
        {
            holder[i] = tolower(makemeLower[i]);
        }
        return holder;
    }
    
    string returnUnsolved(vector<string>& tokenizedwords) //returns the most unsolved word
    {
        int count = 0; //counter to keep track of the max length of unsolved characters
        string mostunsolved; //holds the most unsolved word
        for(int i = 0; i<tokenizedwords.size();i++) //loop through the list of words
        {
            string temp = m_translator.getTranslation(tokenizedwords[i]); //get its translation
            int tempcount = 0; //to count the question marks
            for(int j = 0; j<temp.size(); j++)
            {
                if(temp[j] == '?')
                    tempcount++; //increment number of question marks
            }
            if(tempcount>count) //if more question marks exist in this word than any other word then change the max
            {
                count = tempcount;
                mostunsolved = tokenizedwords[i];
            }
        }
        return mostunsolved; //return the most unsolved word
    }
    
    void translate(vector<string>& words, vector<string> tokenwords) //to translate each word and put it into a vector
    {
        for(int i = 0; i<tokenwords.size(); i++)
        {
            string temp = returnLower(tokenwords[i]);
            words.push_back(m_translator.getTranslation(temp));
        }
    }
    
    bool noquestionMark(string word) //check if there are question marks in the word
    {
        for(int i = 0; i<word.size(); i++)
        {
            if(word[i] == '?')
                return false;
        }
        return true;
    }
    
    
    void cracker(string ciphertext_message)
    {
        vector<string> tokenizedwords = m_tokenizer.tokenize(ciphertext_message); //tokenize words
        string mostunsolved = returnUnsolved(tokenizedwords); //find the most unsolved word ciphered
        string translated = m_translator.getTranslation(mostunsolved); //get the translation from the current mapping table
        vector<string> candidates = m_wordlist.findCandidates(mostunsolved, translated); //find candidates that can possibly match
        
        if(candidates.empty()) //if no candidates, then throw the mapping table away and return to previous recursive call
        {
            m_translator.popMapping();
            return;
        }

        for(int i = 0; i<candidates.size();i++) //loop through the possible candidates
        {

            if(!m_translator.pushMapping(mostunsolved, candidates[i])) //try to map the ciphertext to the possible candidates
            {continue;} //if it fails then skip to the next candidate
            vector<string> words; //vector of strings to hold the translated words
            translate(words, tokenizedwords); //attempt to translate all the words and store it in vector
            bool sam = false; //boolean to check if I need to skip this candidate
            int count = 0; //count that keeps track of how many words were solved and are found in wordlist
    
            for(int j = 0; j<words.size(); j++) //loop through the translated words
            {
                if(noquestionMark(words[j])) //if they have a question mark, don't do anything
                {
                    if(!m_wordlist.contains(words[j])) //if the word is fully translated but is not in wordlist
                    {
                        m_translator.popMapping(); //throw away the current mapping table
                        sam = true; //set skip to true
                        break; //break out of the loop
                    }
                    else //if it is fully translated and is in wordlist
                    {
                        count++; //increment the amount of fully translated words
                    }
                }
            }
            if(sam) //if it is set to true then skip to the next candidate
                continue;
            string rawtranslate = m_translator.getTranslation(ciphertext_message);
            vector<string> temp;
            translate(temp, tokenizedwords);
            if(count<words.size()) //if there are words that are fully translated and words that are not
            {
                cracker(ciphertext_message); //recursively call to the second step to keep on cracking
            }
            else if(count == words.size()) //if all the words are fully translated and they are in wordlist
            {
                bool tell = false;
                for(int l = 0; l<temp.size(); l++)
                {
                    if(!m_wordlist.contains(temp[l])) //if the word is fully translated but is not in wordlist
                    {
                        m_translator.popMapping(); //throw away the current mapping table
                        tell = true;
                        break;
                    }
                }
                if(tell)
                    continue;
    
                finalwords.push_back(rawtranslate); //push the string on to the final vector
                m_translator.popMapping(); //throw away the map
                continue; //go to the next candidate
            }
        }
        m_translator.popMapping(); //if it has gone through all the candidates then throw away the map and return to the previous recursive call
        return;
    }
    
    
};

bool DecrypterImpl::load(string filename)
{
    if(m_wordlist.loadWordList(filename))
        return true;
    return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    finalwords.clear();
    cracker(ciphertext);
    sort(finalwords.begin(), finalwords.end(), less<string>());
    return finalwords;
}

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
