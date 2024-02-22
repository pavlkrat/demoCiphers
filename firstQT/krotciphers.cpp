#include "krotciphers.h"
#include <cctype>
#include <climits>
#include <unordered_map>
#include <algorithm>
#include <random>

int getcode(int c)
{
    if(c >= a && c <= e)
        return c - a;
    if(c == yo)
        return 6;
    if(c >= zh && c <= ya)
        return c + 1 - a;
    return INT_MIN;
}

int getsymb(int c)
{
    if(c >= 0 && c <= 5)
        return c + a;
    if(c == 6)
        return yo;
    if(c >= 7 && c <= 32)
        return c - 1 + a;
    return INT_MIN;
}

int rustolower(int c)
{
    if(c == YO)
        return yo;
    if(c >= A && c <= YA)
        return c + a - A;
    return c;
}

bool isruslower(int c)
{
    return (c == yo || (c >= a && c <= ya));
}

bool isrusupper(int c)
{
    return (c == YO || (c >= A && c <= YA));
}

bool isrus(int c)
{
    return (isruslower(c) || isrusupper(c));
}

int rustoupper(int c)
{
    if(c == yo)
        return YO;
    if(c >= a && c <= ya)
        return c - a + A;
    return c;
}

std::string engtoobr(const std::string &str, const std::string &obr)
{
    std::string ans = str;
    for(int i = 0;i < obr.size();i++)
        if(isupper(obr[i]))
            ans[i] = toupper(ans[i]);
    return ans;
}

std::u16string rustoobr(const std::u16string &str, const std::u16string &obr)
{
    std::u16string ans = str;
    for(int i = 0;i < obr.size();i++)
        if(isrusupper(obr[i]))
            ans[i] = rustoupper(ans[i]);
    return ans;
}

std::string EnglishTextToDecimation(const std::string &text, int key)
{
    std::string ans = text;
    for(auto &c : ans)
        c = tolower(c);
    int encryptions[ENGLENGTH];
    for(int i = 0; i < ENGLENGTH;i++)
        encryptions[i] = (i * key) % ENGLENGTH;
    for(int i = 0;i < ans.size();i++)
        if(islower(ans[i]))
            ans[i] = 'a' + encryptions[ans[i] - 'a'];
    return engtoobr(ans, text);
}

std::string DecimationToEnglishText(const std::string &text, int key)
{
    std::string ans = text;
    for(auto &c : ans)
        c = tolower(c);
    int decryptions[ENGLENGTH];
    for(int i = 0; i < ENGLENGTH;i++)
        decryptions[(i * key) % ENGLENGTH] = i;
    for(int i = 0;i < ans.size();i++)
        if(islower(ans[i]))
            ans[i] = 'a' + decryptions[ans[i] - 'a'];
    return engtoobr(ans, text);
}

std::u16string RussianTextToDecimation(const std::u16string &text, int key)
{
    std::u16string ans = text;
    for(auto &c : ans)
        c = rustolower(c);
    int encryptions[RUSLENGTH];
    for(int i = 0; i < RUSLENGTH;i++)
        encryptions[i] = (i * key) % RUSLENGTH;
    for(int i = 0;i < ans.size();i++)
        if(isruslower(ans[i]))
            ans[i] = getsymb(encryptions[getcode(ans[i])]);
    return rustoobr(ans, text);
}

std::u16string DecimationToRussianText(const std::u16string &text, int key)
{
    std::u16string ans = text;
    for(auto &c : ans)
        c = rustolower(c);
    static int decryptions[RUSLENGTH];
    for(int i = 0; i < RUSLENGTH;i++)
        decryptions[(i * key) % RUSLENGTH] = i;
    for(int i = 0;i < ans.size();i++)
        if(isruslower(ans[i]))
            ans[i] = getsymb(decryptions[getcode(ans[i])]);
    return rustoobr(ans, text);
}

int VigenerFormula(int x, int y)
{
    int ans = x + y;
    if(ans >= RUSLENGTH)
        return ans - RUSLENGTH;
    return ans;
}

int VigenerReverseFormula(int x, int y)
{
    int ans = x - y;
    if(ans < 0)
        return ans + RUSLENGTH;
    return ans;
}

std::u16string rusfilter(const std::u16string &str)
{
    std::u16string ans = u"";
    for(int i = 0;i < str.size();i++)
        if(isrus(str[i]))
            ans.push_back(rustolower(str[i]));
    return ans;
}

std::string engfilter(const std::string &str)
{
    std::string ans = "";
    for(int i = 0;i < str.size();i++)
        if(isalpha(str[i]))
            ans.push_back(tolower(str[i]));
    return ans;
}

std::u16string RussianTextToVigenere(const std::u16string &text, const std::u16string &key)
{
    std::u16string ans = text, obraz, filtered, filkey = rusfilter(key);
    filtered = rusfilter(text);
    obraz.resize(filtered.size());
    int cnt;
    for(cnt = 0;cnt < std::min(filkey.size(), obraz.size());cnt++)
        obraz[cnt] = filkey[cnt];
    for(int i = 0;cnt + i < obraz.size();i++)
        obraz[cnt + i] = filtered[i];
    for(int i = 0;i < filtered.size();i++)
        filtered[i] = getsymb(VigenerFormula(getcode(filtered[i]), getcode(obraz[i])));
    cnt = 0;
    for(int i = 0; cnt < filtered.size();i++)
        if(isrus(ans[i]))
            ans[i] = filtered[cnt++];
    return rustoobr(ans, text);
}

std::u16string VigenereToRussianText(const std::u16string &text, const std::u16string &key)
{
    std::u16string ans = text, obraz, filtered, filkey = rusfilter(key);
    filtered = rusfilter(text);
    obraz.resize(filtered.size());
    int cnt;
    for(cnt = 0;cnt < filkey.size();cnt++)
        obraz[cnt] = filkey[cnt];
    for(int i = 0;i < filtered.size();i++)
    {
        filtered[i] = getsymb(VigenerReverseFormula(getcode(filtered[i]), getcode(obraz[i])));
        if(cnt < obraz.size())
            obraz[cnt++] = filtered[i];
    }
    cnt = 0;
    for(int i = 0; cnt < filtered.size();i++)
        if(isrus(ans[i]))
            ans[i] = filtered[cnt++];
    return rustoobr(ans, text);
}

std::string EnglishTextToPlayfair(const std::string &text, const std::vector<std::vector<int> > &tables)
{
    std::vector<std::unordered_map<int, std::pair<int, int> > > maps;
    maps.resize(4, {});
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0; j < tables[i].size() - 1;j++)
            maps[i][tables[i][j]] = {j / 5, j % 5};
        maps[i][tables[i].back()] = {4,4};
    }
    std::string ans = text, filt = engfilter(text);
    if(filt.size() % 2 == 1)
    {
        ans.push_back('x');
        filt.push_back('x');
    }
    for(int i = 0; i < filt.size();i+=2)
    {
        int c1 = filt[i] - 'a', c2 = filt[i + 1] - 'a';
        std::pair<int, int> pos1 = maps[0][c1], pos2 = maps[3][c2];
        std::pair<int, int> res1 = {pos1.first, pos2.second}, res2 = {pos2.first, pos1.second};
        int r1 = res1.first * 5 + res1.second, r2 = res2.first * 5 + res2.second;
        filt[i] = 'a' + tables[1][r1];
        filt[i + 1] = 'a' + tables[2][r2];
    }
    int cnt = 0;
    for(int i = 0; cnt < filt.size();i++)
        if(isalpha(ans[i]))
            ans[i] = filt[cnt++];
    return engtoobr(ans, text);
}

std::string PlayfairToEnglishText(const std::string &text, const std::vector<std::vector<int> > &tables)
{
    std::vector<std::unordered_map<int, std::pair<int, int> > > maps;
    maps.resize(4, {});
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0; j < tables[i].size() - 1;j++)
            maps[i][tables[i][j]] = {j / 5, j % 5};
        maps[i][tables[i].back()] = {4,4};
    }
    std::string ans = text, filt = engfilter(text);
    if(filt.size() % 2 == 1)
    {
        ans.push_back('x');
        filt.push_back('x');
    }
    for(int i = 0; i < filt.size();i+=2)
    {
        int c1 = filt[i] - 'a', c2 = filt[i + 1] - 'a';
        std::pair<int, int> pos1 = maps[1][c1], pos2 = maps[2][c2];
        std::pair<int, int> res1 = {pos1.first, pos2.second}, res2 = {pos2.first, pos1.second};
        int r1 = res1.first * 5 + res1.second, r2 = res2.first * 5 + res2.second;
        filt[i] = 'a' + tables[0][r1];
        filt[i + 1] = 'a' + tables[3][r2];
    }
    int cnt = 0;
    for(int i = 0; cnt < filt.size();i++)
        if(isalpha(ans[i]))
            ans[i] = filt[cnt++];
    return engtoobr(ans, text);
}

std::vector<int> shuff(int len)
{
    std::vector<int> ans;
    ans.resize(len);
    for(int i = 0;i < len;i++)
        ans[i] = i;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(ans.begin(), ans.end(), g);
    return ans;
}

int keytoint(const std::string &key)
{
    std::string ans = "0";
    for(int i = 0;i < key.size();i++)
        if(isdigit(key[i]))
            ans.push_back(key[i]);
    return std::stoi(ans);
}
