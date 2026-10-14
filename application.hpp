#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__
class Application
{
private:
    // Количество аргументов командной строки при запуске программы
    int argc;
    // Массив с-строк - параметры командной строки
    char** argv;

    void generateMaze();
    void readMaze();
public:
    Application(int _argc, char** _argv);
    void Run();
    void printHelp(const char* msg);
};

#endif