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

/*
Гуи приложуха следит за самой игрой

в либу отправляются следующие запросы:
0) Инизиализация -> доска
1) Клетка -> инфо о фигуре, массив клеток с возможными ходами
2) Доска, ход, цвет походившего игрока -> Доска(новая), цвет следующего игрока, состояние игры, массив убитых фигур

*/