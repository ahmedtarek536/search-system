#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>        // For std::tolower
#include <unordered_map> // For std::tolower
using namespace std;

vector<string> products = {
    "Apple iPhone 13",
    "Apple iPhne 13",
    "Apple iPhone 12",
    "Samsung Galaxy S21",
    "Samsung Galaxy S20",
    "Samsung Galxy S21",
    "Google Pixel 6",
    "Google Pxel 6",
    "Google Pixel 5",
    "OnePlus 9 Pro",
    "OnePlus 9",
    "OnePls 9 Pro",
    "Sony WH-1000XM4 Headphones",
    "Sony WH-1000XM3 Headphones",
    "Sony WH1000XM4 Headphones",
    "Apple MacBook Pro",
    "Apple MacBook Air",
    "Apple MacBookPro",
    "Dell XPS 13",
    "Dell XPS 15",
    "Dell XPS13",
    "HP Spectre x360",
    "HP Spectre x350",
    "HP Spectre x36",
    "Asus ROG Strix G15",
    "Asus ROG Strix G14",
    "Asus ROG Strik G15",
    "Lenovo ThinkPad X1 Carbon",
    "Lenovo ThinkPad X1",
    "Lenovo ThinkPad X1 Carbn",
    "Samsung QLED TV",
    "Samsung QLED",
    "Samsung QLDE TV",
    "LG OLED TV",
    "LG OELD TV",
    "LG OLED",
    "Sony Bravia TV",
    "Sony Bravia",
    "Sony Brvia TV",
    "Amazon Echo Dot",
    "Amazon Echo",
    "Amazon Echo Dt",
    "Google Nest Hub",
    "Google Nest",
    "Google Nst Hub"};

//  apple

// Helper Funcation
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
              [](unsigned char c)
              { return tolower(c); });
    return lowerStr;
}
vector<string> get_spelling_words(string word)
{
    vector<string> words;
    string text = "";
    for (int i = 0; i < word.size(); i++)
    {
        if (word[i] == ' ')
        {
            if (text.size() > 0)
            {
                words.push_back(text);
                text = "";
            }
        }
        else
            text += word[i];
    }
    if (text.size() > 0)
        words.push_back(text);

    return words;
}
bool compareByValue(const pair<string, int> &a, const pair<string, int> &b)
{
    return a.second > b.second;
}

// Algorithm
class Node
{
public:
    char data;
    unordered_map<char, Node *> children;
    bool isWord;
    bool isLeaf;
    Node(char data, bool isWord) : data(data), isWord(isWord), isLeaf(true) {}
    Node() : isWord(false), isLeaf(true) {}
};

class Trie
{
private:
    Node *root = new Node(' ', false);
    unordered_map<string, vector<string>> products;
    unordered_map<string, int> result;
    void insert(string word, Node *cur)
    {
        for (int i = 0; i < word.size(); i++)
        {
            if (cur->children[word[i]])
            {
                cur = cur->children[word[i]];
                if (i == word.size() - 1)
                    cur->isWord = true;
            }
            else
            {
                if (i == word.size() - 1)
                    cur->children[word[i]] = new Node(word[i], true);
                else
                    cur->children[word[i]] = new Node(word[i], false);
                cur = cur->children[word[i]];
            }
        }
    }
    bool search(string word, Node *cur)
    {
        for (int i = 0; i < word.size(); i++)
        {
            if (!cur->children[word[i]])
                return false;

            cur = cur->children[word[i]];
        }
        return cur->isWord;
    }
    void suggest(string word, Node *cur)
    {
        string text = "";
        for (int i = 0; i < word.size(); i++)
        {
            if (!cur->children[word[i]])
            {
                for (auto val : cur->children)
                {
                    get_suggestion(text, val.second);
                }
                return;
            }

            cur = cur->children[word[i]];
            if (i != word.size() - 1)
                text += cur->data;
        }
        get_suggestion(text, cur);
    }
    void get_suggestion(string text, Node *cur)
    {
        if (!cur)
            return;
        text += cur->data;
        if (cur->isWord)
        {
            vector<string> items = products[text];
            for (auto val : items)
            {
                result[val]++;
            }
            // cout << text << " \n";
        }
        for (auto val : cur->children)
        {
            get_suggestion(text, val.second);
        }
    }

public:
    void insert(string word)
    {
        string item = toLowerCase(word);
        vector<string> sub_items = get_spelling_words(item);
        insert(item, root);
        products[item].push_back(item);
        for (string val : sub_items)
        {
            insert(val, root);
            products[val].push_back(item);
        }
    }
    void search(string word)
    {
        bool isHere = search(word, root);
        cout << isHere << '\n';
    }
    void suggest(string word)
    {
        suggest(word, root);
    }
    void search_deep(string word)
    {
        string item = toLowerCase(word);
        vector<string> sub_items = get_spelling_words(item);
        suggest(word);
        for (auto val : sub_items)
        {
            suggest(val);
        }
        vector<pair<string, int>> vec(result.begin(), result.end());
        sort(vec.begin(), vec.end(), compareByValue);
        for (auto &p : vec)
        {
            cout << p.first << "\n";
        }
        result.clear();
    }
};

int main()
{

    Trie list;
    for (int i = 0; i < products.size(); i++)
    {
        list.insert(products[i]);
    }
    cout << "Result : \n";
    list.search_deep("apple pro");

    cout << "\n";
    cout << "Result : \n";
    list.search_deep("gog");

    cout << "\n";
    cout << "Result : \n";
    list.search_deep("headnone");

    return 0;
}

/*
time complexity : O(k⋅m+rlogr)
k -> number letter
m -> number substring
r -> number result
k*m <= 200

*/
