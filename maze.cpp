#include "maze.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <stdio.h>
#include <sys/ioctl.h> //ioctl() и TIOCGWINSZ
#include <unistd.h>

using namespace std;

/************************************************************************************/
/* Метод: Maze::Maze(size_t rows, size_t columns)                                   */
/* Цель: конструктор, инициализирует поля sizeRows, sizeColumns и выделяет память   */
/* под двумерный массив лабиринта                                                   */
/* Исходные данные:                                                                 */
/* rows – количество строк в лабиринте                                              */
/* columns – количество столбцов в лабиринте                                        */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: Maze::allocateMaze()                                          */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
Maze::Maze(size_t rows, size_t columns) : sizeRows(rows), sizeColumns(columns)
{
    allocateMaze();
    neighbourDirection = {{-2,0}, {0, -2}, {2, 0}, {0, 2}};
}

/************************************************************************************/
/* Метод: Maze::Maze()                                                              */
/* Цель: конструктор, инициализирует поля sizeRows, sizeColumns, maze значениями по */
/* умолчанию: 0, 0, nullptr соответственно                                          */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
Maze::Maze() : sizeRows(0), sizeColumns(0), maze(nullptr)
{
    neighbourDirection = {{-2,0}, {0, -2}, {2, 0}, {0, 2}};
}

/************************************************************************************/
/* Метод: Maze::~Maze()                                                             */
/* Цель: деструктор, освобождает память, выделенную под массив лабиринта            */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
Maze::~Maze()
{
    if (maze != nullptr) {
        for (size_t i = 0; i < sizeRows; i++) {
            delete[] maze[i];
        }
        delete[] maze;
    }
}

/************************************************************************************/
/* Метод: size_t Maze::random(size_t from, size_t to)                               */
/* Цель: генерация случайного целого числа из заданного диапазона                   */
/* Исходные данные:                                                                 */
/* from - начальное значение диапазона (включительно)                               */
/* to - конечное значение диапазона (включительно)                                  */
/* Возвращаемое значение: целое число из диапазона                                  */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
size_t Maze::randomNumber(size_t from, size_t to)
{
    // random_device формирует случайную последовательность с помощью внешнего устройства
    // Результаты равномерно распределены в замкнутом диапазоне [0, 2^32).
    std::random_device rd;
    // 32-разрядный механизм ГПСЧ типа "Вихрь Мерсенна" с начальным 32-разрядным недетерминистическим значением
    std::mt19937 gen(rd());
    // Формирует равномерное распределение целых чисел в заданном диапазоне.
    std::uniform_int_distribution<> distr(from, to);
    // Возвращаем ПСЧ полученное с помощью ГСПЧ gen
    return distr(gen);
}

size_t Maze::randomOddNumber(size_t from, size_t to)
{
    // С помощью ГПСЧ получам ПСЧ из диапазона 
    size_t result = randomNumber(from, to);
    // Если полученное число четное, то делаем его нечетным
    if (!(result % 2)) {
        if (result == to) {
            result--;
        } else {
            result++;
        }
    }
    return result;
}

/************************************************************************************/
/* Метод: Maze::allocateMaze()                                                      */
/* Цель: выделяет память под массив лабиринта                                       */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::allocateMaze()
{
    maze = new mazeCell *[sizeRows];
    for (int i = 0; i < sizeRows; i++) {
        maze[i] = new mazeCell[sizeColumns];
    }
}

/************************************************************************************/
/* Метод: Maze::eraseMaze()                                                         */
/* Цель: освобождает память, выделенную под массив лабиринта и устанавливает        */
/* размеры лабиринта 0, 0                                                           */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::eraseMaze()
{
    for (size_t i = 0; i < sizeRows; i++) {
        delete[] maze[i];
    }
    delete[] maze;
    maze = nullptr;
    sizeRows = 0;
    sizeColumns = 0;
}

/************************************************************************************/
/* Метод: Maze::printMaze()                                                         */
/* Цель: выводит сгенерированный лабиринт на экран                                  */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::printMaze()
{
    if (!maze)
        return;

    cout << "Лабиринт " << sizeRows << " x " << sizeColumns << endl << endl;
    for (size_t i = 0; i < sizeRows; i++) {
        for (size_t j = 0; j < sizeColumns; j++) {
            if ((i == 0 && j == 1) || ((i == sizeRows - 1) && (sizeRows % 2) && (j == sizeColumns - 1 - (sizeColumns % 2)))) {
                cout << "  "; // в первой и последней строке делаем вход и выход из лабиринта
            } else {
                cout << (maze[i][j] == mazeCell::MAZE_WALL ? " *" : "  ");
            }
        }
        if ((sizeColumns % 2) == 0) { // выводим правую стенку лабиринта
            cout << " *";
        }
        cout << endl;
    }
    // Выводим нижнюю стенку лабиринта
    if ((sizeRows % 2) == 0) {
        for (size_t j = 0; j < sizeColumns + !(sizeColumns % 2); j++) {
            if (j == (sizeColumns - 1 - (sizeColumns % 2))) {
                cout << "  ";
            } else {
                cout << " *";
            }
        }
        cout << endl;
    }
    cout << endl;
}

/************************************************************************************/
/* Метод: Maze::setAllWalls()                                                       */
/* Цель: заполняет весь массив лабиринта элементами типа "стенка"                   */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::setAllWalls()
{
    for (size_t i = 0; i < sizeRows; i++)
        for (size_t j = 0; j < sizeColumns; j++)
            maze[i][j] = mazeCell::MAZE_WALL;
}

/************************************************************************************/
/* Метод: Maze::getSizeUserInput()                                                  */
/* Цель: запрашивает пользователя ввести размеры лабиринта                          */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: Maze::getIntInRange()                                         */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::getSizeUserInput()
{
    struct winsize twSize;
    // Получаем размеры окна терминала и выводим их на экран
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &twSize);
    cout << "Размер окна терминала " << twSize.ws_row << " x " << twSize.ws_col << endl;
    cout << "Если количество столбцов лабиринта превысит половину размера окна терминала, лабиринт будет выведен некорректно." << endl;

    cout << "Введите количество строк лабиринта [3, 500]: ";
    sizeRows = getIntInRange(3, 500);

    cout << "Введите количество столбцов лабиринта [3, 500]: ";
    sizeColumns = getIntInRange(3, 500);
}

/************************************************************************************/
/* Метод: Maze::getIntInRange()                                                     */
/* Цель: ожидает ввод целого числа из заданного диапазона с проверкой типа          */
/* введеного значе и вхождения в диапазон                                           */
/* Исходные данные:                                                                 */
/* minValue - нижняя граница диапазона                                              */
/* maxValue - верхняя граница диапазона                                             */
/* Возвращаемое значение: целое число из заданного диапазона                        */
/* Вызываемые модули: Maze::getIntInRange()                                         */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
size_t Maze::getIntInRange(size_t minValue, size_t maxValue)
{
    int res = 0;
    while (true) {
        if (cin >> res) {
            if (res >= minValue && res <= maxValue) {
                break;
            }
            cout << "Недопустимое значение " << res << ". ";
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Введите целое число [" << minValue << ", " << maxValue << "]: ";
    }
    return res;
}

/************************************************************************************/
/* Метод: Maze::checkCellType()                                                     */
/* Цель: проверяет, является ли ячейка с заданными координатами ячейкой             */
/* проверяемого типа                                                                */
/* Исходные данные:                                                                 */
/*   cell - пара координат ячейки                                                   */
/*   cellType - проверяемый тип                                                     */
/* Возвращаемое значение: true - если ячейка является ячейкой проверяемого типа     */
/*   false - в противном случае или координаты выходят за границы массива           */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
bool Maze::checkCellType(Cell cell, mazeCell cellType)
{
    if ((cell.first >= 0) && (cell.first < sizeRows) 
        && (cell.second >= 0) && (cell.second < sizeColumns) 
        && maze[cell.first][cell.second] == cellType) {
        return true;
    }
    return false;
}

/************************************************************************************/
/* Метод: Maze::generateMaze()                                                      */
/* Цель: генерирует лабиринт заданного размера используя модифицированный алгоритм  */
/* Прима                                                                            */
/* Исходные данные: нет                                                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: Maze::getSizeUserInput(), Maze::allocateMaze(),               */
/* Maze::checkCellType(), Maze::randomOddNumber()                                   */
/* Maze::setAllWalls(), Maze::random()                                              */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::generateMaze()
{
    // Если массив лабиринта не определен, то
    if (!maze) {
        // заапрашиваем размеры лабиринта у пользователя
        getSizeUserInput();
        // и выделяем память под массив
        allocateMaze();
    }
    // Помечаем все ячейки как стенки
    setAllWalls();
    
    // С помощью ГПСЧ получам индекс строки и столбца начальной ячейки 
    size_t startRow = randomOddNumber(0, sizeRows - 1);
    size_t startColumn = randomOddNumber(0, sizeColumns - 1);
    
    // Делаем начальную ячейку "проходом"
    maze[startRow][startColumn] = mazeCell::MAZE_PASS;

    // У начальной ячейки определяем соседние ячейки-стенки, расположенные через одну от начальной 
    // в направлениях сверху, снизу, слева и справа. Найденные ячейки-стенки добавляем в множество 
    // ячеек-стенок wallNeighbours
    set<Cell> wallNeighbours;
    for (auto dir : neighbourDirection) {
        Cell wallNeighbour(startRow + dir.first, startColumn + dir.second);
        if (checkCellType(wallNeighbour, mazeCell::MAZE_WALL)) {
            wallNeighbours.insert(wallNeighbour);
        }
    }
    // Пока множество соседних ячеек-стенок не пусто
    while (!wallNeighbours.empty()) {
        // Случайным образом выбираем одну ячейку-стенку из множества ячеек-стенок wallNeighbours
        size_t index = randomNumber(0, wallNeighbours.size() - 1);
        Cell selectedWallNeighbour = *next(wallNeighbours.begin(), index);
        // Удаляем выбранную ячейку-стенку из множества wallNeighbours
        wallNeighbours.erase(selectedWallNeighbour);

        // В массиве лабиринта преобразуем выбранную ячейку-стенку в проход
        maze[selectedWallNeighbour.first][selectedWallNeighbour.second] = mazeCell::MAZE_PASS;

        // Копируем множество направлений (вверх, вниз, вправо, влево)
        set<Direction> tmpDir = neighbourDirection;
        // В цикле пока множество направлений не пустоб ищем одну из ячеек-проходов, расположенную через одну от выбранной
        while (!tmpDir.empty()) {
            // Случайным образом получаем одно из направлений
            size_t index = randomNumber(0, tmpDir.size() - 1);
            Direction selectedDir = *next(tmpDir.begin(), index);
            // Получаем координаты соседней ячейки
            Cell passNeighbour(selectedWallNeighbour.first + selectedDir.first, selectedWallNeighbour.second + selectedDir.second);
            // Проверяем находятся ли координаты в границах массива и ячейка является проходом, и если это так, то
            if (checkCellType(passNeighbour, mazeCell::MAZE_PASS)) {
                // Соединяем выбранную ячейку и соседнюю (через одну) проходом
                maze[selectedWallNeighbour.first + selectedDir.first / 2][selectedWallNeighbour.second + selectedDir.second / 2] = mazeCell::MAZE_PASS;
                // выходим из цикла
                break;
            }
            // Если же полученные координаты за границами массива или ячейка не является проходом, то удаляем это направление из множества
            // и переходим к следующей итерации
            tmpDir.erase(selectedDir);
        }
        
        // У выбранной ячейки определяем соседние ячейки-стенки, расположенные через одну от нее 
        // в направлениях сверху, снизу, слева и справа. Найденные ячейки-стенки добавляем в множество 
        // ячеек-стенок wallNeighbours
        for (auto dir : neighbourDirection) {
            Cell newWallNeighbour(selectedWallNeighbour.first + dir.first, selectedWallNeighbour.second + dir.second);
            if (checkCellType(newWallNeighbour, mazeCell::MAZE_WALL)) {
                wallNeighbours.insert(newWallNeighbour);
            }
        }
    }
}

/************************************************************************************/
/* Метод: Maze::writeToFile()                                                       */
/* Цель: Сохраняет размеры лабиринта и сам лабиринт в файл                          */
/* Исходные данные: fileName - имя файла для сохранения                             */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: нет                                                           */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::writeToFile(const char *fileName)
{
    std::ofstream outFile(fileName, std::ios::out | std::ios::binary);
    if (!outFile) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    // Сохраняем размеры
    outFile.write((const char *)&sizeRows, sizeof(sizeRows));
    outFile.write((const char *)&sizeColumns, sizeof(sizeColumns));
    // Сохраняем лабиринт
    for (size_t i = 0; i < sizeRows; i++) {
        outFile.write((const char *)&maze[i][0], sizeof(maze[0][0]) * sizeColumns);
    }
    outFile.close();
}

/************************************************************************************/
/* Метод: Maze::readFromFile()                                                      */
/* Цель: Считывает размеры лабиринта и сам лабиринт из файла                        */
/* Исходные данные: fileName - имя файла для чтения                                 */
/* Возвращаемое значение: нет                                                       */
/* Вызываемые модули: Maze::eraseMaze(), Maze::allocateMaze()                       */
/* Дата: 2025 / 04 / 26 Версия 1.01                                                 */
/* Автор: Гущина М.О.                                                               */
/************************************************************************************/
void Maze::readFromFile(const char *fileName)
{
    std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
    if (!inFile) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }
    // Если лабиринт уже есть, то удаляем его
    if (maze != nullptr) {
        eraseMaze();
    }
    // Считываем размеры лабиринта
    inFile.read((char *)&sizeRows, sizeof(sizeRows));
    inFile.read((char *)&sizeColumns, sizeof(sizeColumns));
    // Выделяем память
    allocateMaze();
    // Считываем лабиринт
    for (size_t i = 0; i < sizeRows; i++) {
        inFile.read((char *)&maze[i][0], sizeof(maze[0][0]) * sizeColumns);
    }
    inFile.close();
}
