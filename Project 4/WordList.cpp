#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
    WordListImpl()
    {}
    ~WordListImpl()
    {}
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> m_hashtable; //pattern to vector of words
    MyHash<string, string> m_hashtable2; //holds the words to make it easily accessible
    
    bool isValid(string checkword) const
    {
        for(int i = 0; i<checkword.size(); i++)
        {
            if(isalpha(checkword[i]) || checkword[i] == '\'')
                continue;
            else
                return false;
        }
        return true;
    }
    
    bool isValidTrans(string checkword) const
    {
        for(int i = 0; i<checkword.size(); i++)
        {
            if(isalpha(checkword[i]) || checkword[i] == '\'' || checkword[i] == '?')
                continue;
            else
                return false;
        }
        return true;
    }
    
    string returnLower(string makemeLower) const
    {
        string holder = makemeLower;
        for(int i = 0; i<makemeLower.size(); i++)
        {
                holder[i] = tolower(makemeLower[i]);
        }
        return holder;
    }
    
    
    string findPattern(string word1) const
    {
        string word = returnLower(word1);
        char patternNum[27] = {0}; //Initialize character array that holds a dummy value
        bool tracker[27] = {false}; //initializes a boolean array that tracks if a character has been encountered
        string pattern; //holds the pattern to return
        int countNumOn = 0; //keeps track of dsitinct numbers
        char alphabet[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', '\''};
        //made an alphabet value holder for the pattern
        for(int i = 0; i<word.size(); i++)
        {
            if(word[i] == '\'') //check for special case of apostrophe
            {
                if(tracker[26]) //if it has been encountered
                    pattern+= patternNum[26]; //add the character to pattern
                else
                {
                    tracker[26] = true; //if it has not been encountered then show that it has been encountered
                    patternNum[26] = alphabet[countNumOn]; //set that character array position to representation character
                    pattern+= patternNum[26]; //add the character to the string
                    countNumOn++; //increment the distinct number count
                }
                continue;
            }
            
            if(tracker[word[i]-'a']) //check if it has been encountered
                pattern+= patternNum[word[i]-'a']; //add it to string pattern if it has
            else
            {
                tracker[word[i]-'a'] = true; //if it has not been encountered, set it to encountered
                patternNum[word[i]-'a'] = alphabet[countNumOn]; //set that character to the character representation
                pattern+= patternNum[word[i]-'a']; //add it to string
                countNumOn++; //increment the distinct number count
            }
        }
        return pattern; //return the final pattern
    }
    
    
};

bool WordListImpl::loadWordList(string filename)
{
    m_hashtable.reset();
    m_hashtable2.reset();
    ifstream infile(filename);    // infile is a name of our choosing
    if ( ! infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    std::string s;
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, s))
    {
        string madelower = returnLower(s);
        if(!isValid(madelower))
            continue;
        m_hashtable2.associate(madelower, madelower);
        string pattern = findPattern(madelower);
        vector<string>* temp = m_hashtable.find(pattern);
        if(temp == nullptr)
        {
            vector<string> tempword;
            tempword.push_back(madelower);
            m_hashtable.associate(pattern, tempword);
        }
        else
        {
            temp->push_back(madelower);
        }
    }
    
    return true;
}

bool WordListImpl::contains(string word) const
{
    string pattern = findPattern(word);
    if(m_hashtable2.find(word) !=  nullptr)
        return true;
    return false; // This compiles, but may not be correct
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    if(cipherWord.size() != currTranslation.size() || !isValid(cipherWord) || !isValidTrans(currTranslation))
        return vector<string>(); //check for mismatched sizes, invalid characters
    
    for(int j = 0; j<currTranslation.size(); j++) //check that the cipherword and currtranslation are valid
    {
        if(isalpha(currTranslation[j]))
        {
            if(!isalpha(cipherWord[j]))
                return vector<string>();
        }
        else if(currTranslation[j] == '?')
        {
            if(!isalpha(cipherWord[j]))
                return vector<string>();
        }
        else if(currTranslation[j] == '\'')
        {
            if(cipherWord[j] != '\'')
                return vector<string>();
        }
        
    }
    
    vector<string> holder;
    const vector<string>* output; // get the pointer to the vector with words of the same pattern
    output = m_hashtable.find(findPattern(cipherWord));
    if(output == nullptr)
        return vector<string>();
    vector<string>::const_iterator itr = output->begin();
    bool teller = true;
    string curr = returnLower(currTranslation);
    while(itr!=output->end()) //loop through the vector and find the matching words
    {
        teller = true;
        if(findPattern(*itr) == findPattern(cipherWord))
        {
            for(int i = 0; i<curr.size(); i++)
            {
                if(isalpha(curr[i]))
                {
                    if(curr[i] != (*itr)[i])
                    {teller = false; break;}
                }
                else if(curr[i] == '?')
                {
                    if(!isalpha((*itr)[i]))
                    {teller = false; break;}
                }
                else if(curr[i] == '\'')
                {
                    if((*itr)[i]!= '\'')
                    {teller= false;break;}
                }
            }
            if(teller)
                holder.push_back(*itr);
            
        }
        itr++;
    }
    return holder;  // This compiles, but may not be correct
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
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
