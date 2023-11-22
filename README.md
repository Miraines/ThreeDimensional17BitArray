[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=12997238&assignment_repo_type=AssignmentRepo)
# Лабораторная работа 5

## Задача

Спроектировать и реализовать класс для хранения трехмерного массива 17-битных положительных целых чисел.

## Требования

Ваша задача состоит в том, чтобы разработать класс, реализующий трехмерных массив и обладающий дополнительным набором свойств.

Пример использования:

```cpp
YourClassName arr = YourClassName::make_array(10, 2, 30);

arr[4][3][2] = 100500;
std::cout << arr[4][3][2];
```

В классе должены быть определены следующие операторы и методы:

- обращение к элементу массива за O(1) по времени и по памяти (реализация может использовать константный объем доп памяти, не зависящий от размера массива)
- операторы ввода и вывода
- умножение на число, где результат это новый массив
- сложение и вычитание двух массивов, где результат также является новым массивом (размеры по всем трем осям должны совпадать, в противном случае UB)
- [статический метод](https://en.cppreference.com/w/cpp/language/static) для создания массива, принимающий три положительных числа в качестве его размеров

## Реализация

1. Результат должен представлять из себя библиотеку
2. Ваш класс должен быть покрыт тестами, при этом в качестве фреймворка для написания тестов используется [GoogleTest](https://google.github.io/googletest/). Тесты также являются частью задания. Их полнота и покрытие - также оцениваются
3. Требуется внимательно следить за утечкам памяти

## Ограничения

1. Использование стандартной библиотеки помимо операторов ввода и вывода - запрещено
2. Ограничение по памяти на класс - (x * y * z * 17)/8 + const, где x, y, z - размер массива, const - некоторая константа

## Deadline

1. 5.12.23 24:00 - 0.8
2. 12.12.23 24:00 - 0.65
3. 19.12.23 24:00 - 0.5
