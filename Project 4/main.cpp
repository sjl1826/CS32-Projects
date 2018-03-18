#include "provided.h"
#include "MyHash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cassert>
using namespace std;

// **************************************************************
//
// CHANGE TO false THE VARIABLES CORRESPONDING TO CLASSES YOU ARE *NOT*
// YET READY TO TEST

const bool shouldTestMyHash = true;
const bool shouldTestTokenizer = true;
const bool shouldTestWordList = true;
const bool shouldTestTranslator = true;
const bool shouldTestDecrypter = true;

// **************************************************************

const char* FILENAME = "sanityfile";

void testMyHash();
void testTokenizer();
void testWordList();
void testTranslator();
void testDecrypter();
bool createWordListFile();

int main()
{
    if (shouldTestMyHash    ) { testMyHash();     cout << "Passed MyHash tests" << endl; }
    if (shouldTestTokenizer ) { testTokenizer();  cout << "Passed Tokenizer tests" << endl; }
    if (shouldTestWordList  ) { testWordList();   cout << "Passed WordList tests" << endl; }
    if (shouldTestTranslator) { testTranslator(); cout << "Passed Translator tests" << endl; }
    if (shouldTestDecrypter ) { testDecrypter();  cout << "Passed Decrypter tests" << endl; }
}

void testMyHash()
{
    MyHash<int, string> m;
    m.associate(222, "Ethel");
    m.associate(333, "Lucy");
    m.associate(111, "Fred");
    assert(m.getNumItems() == 3);
    assert(abs(m.getLoadFactor() - 0.03) < 0.00001);
    string* s = m.find(333);
    assert(s != nullptr  &&  *s == "Lucy");
    m.associate(333, "Ricky");
    assert(m.getNumItems() == 3);
    const MyHash<int, string>& cm = m;
    const string* cs = cm.find(333);
    assert(cs != nullptr  &&  *cs == "Ricky");
    int x = 1776;
    for (int k = 0; k < 1000; k++)
    {
        m.associate(x, "Carey");
        x += 1789;
    }
    assert(m.getNumItems() == 1003);
    double lf = m.getLoadFactor();
    assert(lf >= 0.5/2  &&  lf <= 0.5);
    m.reset();
    m.associate(444, "David");
    assert(m.getNumItems() == 1);
    assert(abs(m.getLoadFactor() - 0.01) < 0.00001);
}

void testTokenizer()
{
    Tokenizer t(".#");
    vector<string> v = t.tokenize("this.is##.#a#SILLY test#..");
    const char* expected[4] = { "this", "is", "a", "SILLY test" };
    assert(v.size() == 4  && equal(v.begin(), v.end(), expected));
}

void testWordList()
{
    if ( ! createWordListFile())
    {
        cout << "Cannot complete WordList test" << endl;
        exit(1);
    }
    WordList wl;
    assert(wl.loadWordList(FILENAME));
    assert(wl.contains("expel"));
    assert(!wl.contains("extol"));
    vector<string> v = wl.findCandidates("wpxWv", "?x???");
    const char* expected[] = { "excel", "expel" };
    assert(v.size() == 2);
    for (auto& w : v)
        transform(w.begin(), w.end(), w.begin(), [](char c) { return tolower(c); });
    assert(equal(v.begin(), v.end(), expected) || equal(v.rbegin(), v.rend(), expected));
    v = wl.findCandidates("wpxwv", "?????");
    assert(v.size() == 4);
    v = wl.findCandidates("wpxwv", "?s??y");
    assert(v.size() == 0);
    remove(FILENAME);
}

void testTranslator()
{
    const char* q5bang = "?????" "!";  // Avoid trigraph
    Translator t;
    assert(!t.pushMapping("hqdoplh", "abcdefg"));
    assert(t.getTranslation("Hdqlx!") == q5bang);
    assert( ! t.popMapping());
    assert(t.getTranslation("Hdqlx!") == q5bang);
    assert(t.pushMapping("dhL", "Erd"));
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert( ! t.pushMapping("QX", "RY"));
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert( ! t.pushMapping("H", "S"));
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert(t.pushMapping("QX", "AY"));
    assert(t.getTranslation("Hdqlx!") == "Ready!");
    assert(t.popMapping());
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert(t.popMapping());
    assert(t.getTranslation("Hdqlx!") == q5bang);
    assert( ! t.popMapping());
}

void testDecrypter()
{
    if ( ! createWordListFile())
    {
        cout << "Cannot complete Decrypter test" << endl;
        exit(1);
    }
    Decrypter d;
    assert(d.load(FILENAME));
    vector<string> v = d.crack("smxsdg SGOSDG gfvgx!!");
    const char* expected[] = {
        "ablate AERATE excel!!",
        "ablate AERATE expel!!",
        "purple PEOPLE eater!!",
        "purple PEOPLE ether!!"
    };
    assert(v.size() == 4  &&  equal(v.begin(), v.end(), expected));
    assert((d.crack("smxsdg SGOSDG gfvgy!!")).empty());
    remove(FILENAME);
}

bool createWordListFile()
{
    ofstream ofs(FILENAME);
    if ( ! ofs)
    {
        cout << "Cannot create word list file named " << FILENAME << endl;
        return false;
    }
    const char* words[] = {
        "people", "aerate", "ether", "essay", "eater",
        "excel", "Expel", "ablate", "purple"
    };
    for (auto w : words)
        ofs << w << endl;
    return true;
}






//
//
//#include "provided.h"
//#include "MyHash.h"
//#include <iostream>
//#include <string>
//#include <vector>
//#include <cstring>
//#include <cctype>
//#include <random>
//#include <algorithm>
//#include <numeric>
//using namespace std;
//
//const string WORDLIST_FILE = "/Users/sjl1826/Downloads/wordlist.txt";
//
//string encrypt(string plaintext)
//{
//      // Generate a to z
//    char plaintextAlphabet[26+1];
//    iota(plaintextAlphabet, plaintextAlphabet+26, 'a');
//    plaintextAlphabet[26] = '\0';
//
//      // Generate permutation
//    string ciphertextAlphabet(plaintextAlphabet);
//    default_random_engine e((random_device()()));
//    shuffle(ciphertextAlphabet.begin(), ciphertextAlphabet.end(), e);
//
//      // Run translator (opposite to the intended direction)
//    Translator t;
//    t.pushMapping(plaintextAlphabet, ciphertextAlphabet);
//    return t.getTranslation(plaintext);
//}
//
//bool decrypt(string ciphertext)
//{
//    Decrypter d;
//    if ( ! d.load(WORDLIST_FILE))
//    {
//        cout << "Unable to load word list file " << WORDLIST_FILE << endl;
//        return false;
//    }
//    for (const auto& s : d.crack(ciphertext))
//        cout << s << endl;
//    return true;
//}
//
//
//void f()
//{
//    WordList wl;
//    if ( ! wl.loadWordList(WORDLIST_FILE))
//    {
//        cout << "Unable to load word list" << endl;
//        return;
//    }
//    if (wl.contains("onomatopoeia"))
//            cout << "I found onomatopoeia!" << endl;
//    else
//            cout << "Onomatopoeia is not in the word list!" << endl;
//    string cipher = "xyqbbq";
//    string decodedSoFar = "?r????";
//    vector<string> v = wl.findCandidates(cipher, decodedSoFar);
//    if (v.empty())
//        cout << "No matches found" << endl;
//    cout << "Found these matches:" << endl;
//    for (size_t k = 0; k < v.size(); k++)
//        cout << v[k] << endl; // writes grotto and troppo
//}
//
//
//int main(int argc, char* argv[])
//{
//    if (argc == 3  &&  argv[1][0] == '-')
//    {
//        switch (tolower(argv[1][1]))
//        {
//          case 'e':
//            cout << encrypt(argv[2]) << endl;
//            return 0;
//          case 'd':
//            if (decrypt(argv[2]))
//                return 0;
//            return 1;
//        }
//    }
//    f();
//
//    Decrypter sam;
//    sam.load(WORDLIST_FILE);
//    vector<string> yay = sam.crack("Trcy oyc koon oz rweelycbb vmobcb, wyogrcn oecyb; hjg ozgcy tc moox bo moya wg grc vmobck koon grwg tc ko yog bcc grc oyc trlvr rwb hccy oecyck zon jb. -Rcmcy Xcmmcn");
//    for(int i = 0; i<yay.size(); i++)
//        cout<<yay[i]<<endl;
//
//    yay.clear();
//    yay = sam.crack("Jxwpjq qwrla glcu pcx qcn xkvv dw uclw ekarbbckpjwe dq jzw jzkpta jzrj qcn ekep'j ec jzrp dq jzw cpwa qcn eke ec. -Urls Jxrkp");
//
//    for(int j = 0; j<yay.size(); j++)
//        cout<<yay[j]<<endl;
//
////    yay.clear();
////    yay = sam.crack("Xjzwq gjz cuvq xz huri arwqvudiy fuk ufjrqoq svquxiy. -Lzjk Nqkkqcy");
////
////    for(int b = 0; b<yay.size(); b++)
////        cout<<yay[b]<<endl;
//
//    yay.clear();
//    yay = sam.crack("Axevfvu lvnelvp bxqp mvpprjv rgl bvoop Grnxvgkvuj dqupb jvbp buvrbvl be lqggvu.");
//
//    for(int b = 0; b<yay.size(); b++)
//        cout<<yay[b]<<endl;
//
//    return 1;
//}

