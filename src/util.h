#ifndef UTIL_H
#define UTIL_H
std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
bool ofinterest(char c);
int isapos(char c);

#endif
