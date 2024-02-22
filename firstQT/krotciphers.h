#ifndef KROTCIPHERS_H
#define KROTCIPHERS_H

#include <string>
#include <vector>

const int ENGLENGTH = 26, RUSLENGTH = 33;
const std::u16string al = u"аеёжяЁАЯ";
const int a = al[0], e = al[1], yo = al[2], zh = al[3], ya = al[4], YO = al[5], A = al[6], YA = al[7];

std::string EnglishTextToDecimation(const std::string &text, int key);
std::string DecimationToEnglishText(const std::string &text, int key);
std::u16string RussianTextToDecimation(const std::u16string &text, int key);
std::u16string DecimationToRussianText(const std::u16string &text, int key);
std::u16string RussianTextToVigenere(const std::u16string &text, const std::u16string &key);
std::u16string VigenereToRussianText(const std::u16string &text, const std::u16string &key);
std::string EnglishTextToPlayfair(const std::string &text, const std::vector<std::vector<int> > &tables);
std::string PlayfairToEnglishText(const std::string &text, const std::vector<std::vector<int> > &tables);
int rustolower(int c);
int rustoupper(int c);
int keytoint(const std::string &key);
std::vector<int> shuff(int len);
std::u16string rusfilter(const std::u16string &str);

#endif // KROTCIPHERS_H
