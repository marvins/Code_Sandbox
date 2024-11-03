#include <iostream>
#include <string>

using namespace std;

enum ANIMALS{DOG,CAT,PIG,MOUSE};
string pets[] = {"Dog","Cat","Pig","Mouse"};

template<class DerivedT>
class animal{
  public:
    DerivedT& impl(){ return static_cast<DerivedT&>(*this); }
    string speak() const{ return "speak not declared for "+pets[DerivedT::type];}
};

class dog : public animal<dog>{
  public:
    static const int type = DOG;
    string speak() const{
      return "woof!";}
};
class cat : public animal<cat>{
  public:
    static const int type = CAT;
    string speak() const{
      return "meow!";}
};
class pig : public animal<pig>{
  public:
    static const int type = PIG;
    string speak() const{
      return "oink!";}
};
class mouse : public animal<mouse>
{
  public:
    static const int type = MOUSE;
};

template<class DerivedT>
string call(animal<DerivedT> pet)
{
  return pet.impl().speak();  
}

int main()
{
   dog alfie;
   cat pooky;
   pig babe;
   mouse itty;
  
   std::cout << call(alfie) << std::endl;
   std::cout << call(pooky) << std::endl;
   std::cout << call(babe) << std::endl;
   std::cout << call(itty) << std::endl;

  return 0;
}

