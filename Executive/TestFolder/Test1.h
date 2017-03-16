#pragma once
// Test1.h

#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

void global_function() { std::cout << "Test gFunction!";}
namespace Test
{
    using namespace Test_namespace;
    Test::Test1::Test1() {}
    int global_data = 1;
  class Test1
  {
  public:
      
      using testUsing = std::string;
      typedef unsigned long ul;
      enum testEnum { enum1, enum2 };
      
      Test1();
      Test1(const Test1&) = delete;
      Test1& operator=(const Test1&) = default;
      ~Test1();
      void doThing1() { /* don't do anything */ }
  private:
      Scanner::SemiExp se;
  };
}