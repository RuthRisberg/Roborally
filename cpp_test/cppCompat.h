#ifndef cppCompat_h
#define cppCompat_h 

// comment out the #include of this file in Utility.h when running as Arduino C

#include <iostream>
#include <string>

typedef std::string String;

#define String( arg ) std::to_string(arg)
#define substring substr
#define charAt at
#define stringToInt std::stoi
#define toInt length // just to stop compile errors


class SerialClass {
  public:
    SerialClass() {}
    void print(String string) 
    {
      std::cout << string;
    }
    void print(int i) 
    {
      std::cout << i;
    }
    void println(String string) 
    {
      std::cout << string << '\n';
    }
    void println(int i) 
    {
      std::cout << i << '\n';
    }
    int available() {
      int i = 1;
      return i;
    }
    String readStringUntil(char c)
    {
      String string;
      std::getline(std::cin, string);
      return string;
    }
  
  private:
};

extern SerialClass Serial;

#endif