#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <stack>
#include <cmath>

// Деком (англ. deque – аббревиатура от double-ended queue, двухсторонняя очередь) называется структура данных,
// в которую можно удалять и добавлять элементы как в начало, так и в конец.
// Дек хранится в памяти так же, как и очередь.
//
// ЗАДАЧА 1
//
// Проверить, является ли введенная строка палиндромом
// (читается одинаково слева направо и справа налево). Использовать дек.
//
// Алгоритм решения:
// Считываем строку посимвольно. каждый символ добавляем в хвост дека.
// Далее сравниваем первый и последний символ дека.
//
//
//
// ЗАДАЧА 2
//
// Реализовать поиск выпуклой оболочки точек.
//
// Алгоритм Грэхема.
//
// Этот алгоритм является трехшаговым. На первом шаге ищется любая точка в A, гарантированно входящая в выпуклую оболочку.
// В качестве точки можно взять точку с наименьшей x-координатой (самая левая точка).
// Эту точку (будем называть ее стартовой) перемещаем в начало дека, вся дальнейшая работа будет производиться с
// оставшимися точками. Заведем дек P, в котором будут хранится номера точек (их позиции в массиве A).
// Итак, первый шаг алгоритма заключается в том, чтобы первой точкой в P оказалась точка с наименьшей x-координатой.
//
// Второй шаг в алгоритме Грэхема — сортировка всех точек (кроме P[0]-ой), по степени их левизны относительно
// стартовой точки R. Будем говорить, что B<C, если точка С находится по левую сторону от вектора RB.
// Если теперь соединить точки в полученном порядке, то получится многоугольник, который, однако, не является выпуклым.
//
// Третье действие. Нужно пройтись по всем вершинам и удалить те из них, в которых выполняется правый
// поворот (угол в такой вершине оказывается больше развернутого). Заводится дек S (реально список) и помещаем в него
// первые две вершины (они, опять же, гарантированно входят в ВО).
// Затем просматриваем все остальные вершины, и отслеживаем направление поворота в них с точки зрения последних двух вершин
// в стеке S: если это направление отрицательно, то можно срезать угол удалением из стека последней вершины.
// Как только поворот оказывается положительным, срезание углов завершается, текущая вершина заносится в стек.
//
// В итоге в деке S (который теперь можно рассматривать, как список) оказывается искомая последовательность вершин,
// причем в нужной нам ориентации, определяющая МВО заданного множества точек A.
//
// Для определения  с какой стороны от вектора AB находится точка C (положительное возвращаемое значение соответствует
// левой стороне, отрицательное — правой). использовать формулу (B_x-A_x)*(C_y-B_y)-(B_y-A_y)*(C_x-B_x)  - значение
// координаты z векторного произведения.

double isPalindrome(std::string line) {

    std::deque<char> deq;

    if (line.size() == 0) return false;

    for (char ch : line) {
       if (std::isalpha(ch)) deq.push_back(ch);
    }

    while (deq.size() > 1) {

        if (deq.front() != deq.back()) return false;

        deq.pop_front();
        deq.pop_back();
    }

    return true;
}


struct Point {
    int x, y;

    // Оператор сравнения для поиска самой левой точки
    bool operator < (const Point &p) const {
        return (x < p.x) || (x == p.x && y < p.y);
    }
};

// Вычисляет ориентацию поворота для трех точек (отрицательное - правый поворот, положительное - левый)
int orientation(const Point &a, const Point &b, const Point &c) {
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

// Возвращает квадрат расстояния между двумя точками (используется для сортировки)
int distanceSq(const Point &a, const Point &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// Функция для сортировки точек по углу относительно стартовой точки
bool compare(const Point &p1, const Point &p2, const Point &start) {
    int orient = orientation(start, p1, p2);
    if (orient == 0) // Коллинеарные точки - ближняя точка идет первой
        return distanceSq(start, p1) < distanceSq(start, p2);
    return orient > 0; // Левосторонние точки идут первыми
}

// Реализация алгоритма Грэхема для нахождения выпуклой оболочки
std::vector<Point> convexHull(std::vector<Point> &points) {
    int n = points.size();
    if (n < 3) return {}; // Выпуклая оболочка невозможна

    // Находим самую левую точку
    std::swap(points[0], *std::min_element(points.begin(), points.end()));

    Point start = points[0];

    // Сортировка остальных точек по углу относительно стартовой
    std::sort(points.begin() + 1, points.end(), [start](const Point &p1, const Point &p2) {
        return compare(p1, p2, start);
    });

    // Инициализация дека для хранения точек выпуклой оболочки
    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    // Основной цикл для построения выпуклой оболочки
    for (int i = 2; i < n; i++) {
        while (hull.size() > 1 && orientation(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0)
            hull.pop_back(); // Удаляем вершину с правым поворотом
        hull.push_back(points[i]);
    }

    return hull;
}



int main() {
    std::string input;
    std::cout << "Введите строку: ";
    std::getline(std::cin, input);

    if (isPalindrome(input)) {
        std::cout << "Строка является палиндромом\n";
    } else {
        std::cout << "Строка не является палиндромом\n";
    }
}
