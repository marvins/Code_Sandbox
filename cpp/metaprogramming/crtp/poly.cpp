#include <iostream>
#include <string>

using namespace std;

enum ANIMALS{DOG,CAT,PIG,MOUSE};
string pets[] = {"Dog","Cat","Pig","Mouse"};

class animal{
  public:
    virtual string speak() const{ return "speak not declared";}
};

class dog : public animal{
  public:
    static const int type = DOG;
    virtual string speak() const{
      return "woof!";}
};
class cat : public animal{
  public:
    static const int type = CAT;
    virtual string speak() const{
      return "meow!";}
};
class pig : public animal{
  public:
    static const int type = PIG;
    virtual string speak() const{
      return "oink!";}
};
class mouse : public animal{
  public:
    static const int type = MOUSE;
};

string call(animal pet)
{
  return pet.speak();  
}

int main()
{
   dog alfie;
   cat pooky;
   pig babe;
   mouse itty;
  
  for(int i=0;i<50000;i++){
    for(int j=0;j<50000;j++){
      alfie.speak();
      //call(pooky);
      //call(babe);
    }
  }

  return 0;
}

