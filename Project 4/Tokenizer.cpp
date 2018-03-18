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
    vector<char> m_separators;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for(int i = 0; i<separators.size();i++)
    {
        m_separators.push_back(separators[i]);
    }
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> words;
    string tempword;
    for(int i = 0; i<s.size();i++)
    {
        bool validchar = true;
        vector<char>:: const_iterator itr = m_separators.begin();
        while(itr!= m_separators.end())
        {
            if(s[i] == *itr)
            {
                validchar = false;
                if(tempword!="")
                {
                    words.push_back(tempword);
                }
                break;
            }
            itr++;
        }
        if(i == s.size()-1)
        {
            if(tempword!="")
            {
                if(isalpha(s[i]))
                    tempword+=s[i];
                words.push_back(tempword);
            }
        }
        if(validchar)
            tempword+=s[i];
        else
            tempword = "";
    }
    
    return words;  // This compiles, but may not be correct
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
