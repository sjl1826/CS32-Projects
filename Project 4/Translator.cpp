#include "provided.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl():m_vectorpos(0), numMaps(1)
    {
        unordered_map<char, char> temp;
        for(char i = 'a'; i != 'z'; i++)
        {
            temp.emplace( i, '?');
        }
        temp.emplace('z', '?');
        m_maps.push_back(temp);
        m_maps1.push_back(temp);
    }
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    vector<unordered_map<char, char>> m_maps; //map cipher to plain
    vector<unordered_map<char, char>> m_maps1; //map plain to cipher
    int m_vectorpos; //keeps track of what position I will use in the vector to access map
    int numMaps; //keeps track of the number of maps


    bool validAlpha(string word) const //checks every character in a word is a letter
    {
        for(int i = 0; i<word.size(); i++)
        {
            if(!isalpha(word[i]))
                return false;
        }
        return true;
    }


    string returnLower(string makemeLower) const //returns the word in lowercase
    {
        string holder = makemeLower;
        for(int i = 0; i<makemeLower.size(); i++)
        {
            holder[i] = tolower(makemeLower[i]);
        }
        return holder;
    }
};

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    string cipher = returnLower(ciphertext); //change to lower case
    string plain = returnLower(plaintext);
    if(ciphertext.size()!= plaintext.size() || !validAlpha(cipher) || !validAlpha(plain)) //mismatch size, invalid characters cause false
        return false;
    unordered_map<char, char> temp; //make temporary map for cipher to plain to pushback on vector
    unordered_map<char, char> temp1; //make temporary map for plain to cipher to pushback on vector

    for(int i = 0; i<ciphertext.size();i++) //loop through each character of cipher
    {
        if(m_maps[numMaps-1][cipher[i]] != '?') //if the character already exists in a previous map and
            if(m_maps[numMaps-1][cipher[i]] != plain[i]) //and it does not equal that character, then return false
                return false;
        if(m_maps1[numMaps-1][plain[i]] != '?') //same the other way around
            if(m_maps1[numMaps-1][plain[i]] != cipher[i])
                return false;
        if(i>0) //check for repetition within ciphertext that does not correspond with other mappings
        {
            if(temp[cipher[i]] != '?') //if the character already exists in a current map and
                if(temp[cipher[i]] != plain[i]) //and it does not equal that character, then return false
                    return false;
            if(temp1[plain[i]] != '?') //same the other way around
                if(temp1[plain[i]] != cipher[i])
                    return false;
        }
        
        if(i == 0) //set the temporary maps to previous map only once
        {
            temp = m_maps[numMaps-1];
            temp1 = m_maps1[numMaps-1];
        }
        
        temp[cipher[i]] = plain[i]; //map the characters
        temp1[plain[i]]= cipher[i];

        if(i == ciphertext.size()-1)
        {
            m_maps.push_back(temp); //on the last iteration push the maps on to the vector
            m_maps1.push_back(temp1);
        }

    }
    if(numMaps>0) //to create the proportionate numbers
        m_vectorpos++;
    numMaps++;
    return true;
}

bool TranslatorImpl::popMapping()
{
    if(numMaps == 1) //if no maps have been added return false
        return false;
    vector<unordered_map<char,char>>::iterator itr = m_maps.end(); //set iterators to the end
    vector<unordered_map<char,char>>::iterator itr1 = m_maps1.end();
    itr--; //decrement to have it point to the last element
    itr1--;
    m_maps.erase(itr); //erase the last element
    m_maps1.erase(itr1);
    numMaps--; //decrement the numbers
    m_vectorpos--;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string result = returnLower(ciphertext); //make the cipher lowercase
    for(int i = 0; i<ciphertext.size(); i++) //loop through each letter
    {
        if(!isalpha(result[i])) //if it is not a letter, leave it alone
            continue;
        if(isupper(ciphertext[i])) //if it is uppercase, then change the result to uppercase before adding the character
        {
            result[i] = toupper(m_maps[m_vectorpos].find(result[i])->second);
        }
        else
            result[i] = m_maps[m_vectorpos].find(result[i])->second; //add the character to result
    }
    return result; //return the result
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

