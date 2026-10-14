#include "application.hpp"
#include <stdexcept>

using namespace std;

/************************************************************/
/* Функция: int main (int argc, char** argv)                */
/* Цель: главная функция программы                          */
/* Исходные данные:                                         */
/* argc – количество аргументов командной строки            */
/* argv – массив c-строк, аргументы командной строки        */
/* Результат: 0                                             */
/* Вызываемые модули: Application::Run()                    */
/* Дата: 2025 / 04 / 26 Версия 1.01                         */
/* Автор: Гущина М.О.                                       */
/************************************************************/

int main(int argc, char **argv)
{
    Application app(argc, argv);

    try {
        app.Run();
    } catch (invalid_argument e) {
        app.printHelp(e.what());
    }

    return 0;
}