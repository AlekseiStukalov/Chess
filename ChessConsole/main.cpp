#include "stdafx.h"
#include "ConsoleChessUI.h"

//pattern Wrapper
class IAnimal
{
public:
    virtual void MakeSound() = 0;
};
class Cat : public IAnimal
{
    void MakeSound()
    {
        std::cout << "Meow" << std::endl;
    }
};
class Tiger : public Cat
{
public:
    Tiger(IAnimal *pCat)
    {
        pAnimal = pCat;
    }

    void MakeSound()
    {
        std::cout << "Rrrrr ";
        pAnimal->MakeSound();
    }

private:
    IAnimal *pAnimal;
};

int main(int argc, char *argv[])
{
    //IAnimal *pAnimal = new Cat();
    //pAnimal = new Tiger(pAnimal);
    //pAnimal->MakeSound();


    ConsoleChessUI consoleUI;
    consoleUI.Start();

    system("pause");

}