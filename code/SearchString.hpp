#ifndef CODE_SMARTSTRING_H
#define CODE_SMARTSTRING_H

#include <iostream>
#include <vector>

using namespace std;

class SearchString
{
private:
    vector<long> backTableKMP;

public:
    string str;

    explicit SearchString(string st)
    {
        str = std::move(st);
        // KMP String Search Pre-processing
        backTableKMP = *new vector<long>(10);
        backTableKMP[0] = -1;
        long i = 0, j = -1;
        while (i < str.size())
        {
            while (j >= 0 && str[i] != str[j])
                j = backTableKMP[j];
            i++;
            j++;
            backTableKMP[i] = j;
        }
    }

    // Replaces each character of the string shifted by some number (Caeser Cipher)
    string rotate(int rot)
    {
        string result = str;
        for (char &ch : result)
        {
            if ('A' <= ch && ch <= 'Z')
                ch = (char)(((ch - 'A') + rot) % 26) + 'A';
            else if ('a' <= ch && ch <= 'z')
                ch = (char)(((ch - 'a') + rot) % 26) + 'a';
        }
        return result;
    }
    // Implementation of the Knuth-Morris-Pratt String Search algortihm
    vector<long> search(const string &text)
    {
        long i = 0, j = 0;
        vector<long> positions;
        while (i < text.size())
        {
            while (j > 0 && text[i] != str[j])
                j = backTableKMP[j];
            i++;
            j++;
            if (j == str.size())
            {
                positions.push_back(i - j);
                j = backTableKMP[j];
            }
        }
        return positions;
    }
};

#endif //CODE_SMARTSTRING_H
