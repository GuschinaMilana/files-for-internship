#ifndef __MAZE_HPP__
#define __MAZE_HPP__
#include <utility>
#include <set>
#include <cstdint>
#include <cstddef>

typedef std::pair<uint8_t, uint8_t> Cell;
typedef std::pair<int, int> Direction;

class Maze {
private:    
    // Класс-перечисление для возможных типов ячеек лабиринта
    enum class mazeCell : uint8_t 
    {
        MAZE_PASS = 0, // проход
        MAZE_WALL = 1  // стенка
    };
    // Двумерный массив, поле лабиринта 
    mazeCell** maze;
    // Количество строк в лабиринте
    size_t sizeRows;
    // Количество столбцов в лабиринте
    size_t sizeColumns;
    // Множество смещений для определения соседних через одну ячеек (сверху, снизу, слева, спава)
    std::set<Direction> neighbourDirection;

    size_t randomNumber(size_t from, size_t to);
    size_t randomOddNumber(size_t from, size_t to);
    void allocateMaze();
    void eraseMaze();
    void setAllWalls();
    void getSizeUserInput();
    size_t getIntInRange(size_t minValue, size_t maxValue);
    bool checkCellType(Cell cell, mazeCell cellType); 

public:
    Maze(size_t rows, size_t columns);
    Maze();
    ~Maze();
    void printMaze();
    void generateMaze();
    void writeToFile(const char* fileName);
    void readFromFile(const char* fileName);
};

#endif
