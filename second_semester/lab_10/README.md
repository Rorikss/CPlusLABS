# Лабораторная работа 10

Шедулер задач

## Задача

Вашей задачей будет разработать класс отвечающий за выполнение связанных по данным между собой задач.
Часто, чтобы решить какую либо задачу, требуется выполнить граф вычислений, где узел графа это задача, ребро связь между результатом выполнения одной задачи и параметром для запуска другой. Вам предстоит разработать класс **TTaskScheduler** , решающий подобную задачу.

Пример такой задачи - нахождение корней квадратного уравнения (предполагаем что коэффициенты гарантирует что корня 2). Решение подобной задачи в лоб - подразумевает вычисление корня из дискриминанта дважды.  Очевидно, что это не оптимально.


Вот так мог бы выглядеть код для решение данной задачи с помощью TTaskScheduler

```cpp
float a = 1;
float b = -2;
float c = 0;

TTaskScheduler scheduler;

auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d)}, b, scheduler.getFutureResult<float>(id2));

auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d)}, b, scheduler.getFutureResult<float>(id2));

auto id5 = scheduler.add([]{float a, float v}{return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));

auto id6 = scheduler.add([]{float a, float v}{return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

scheduler.executeAll();

std::cout << "x1 = " << scheduler.getResult<int>(id5) << std::endl;
std::cout << "x2 = " << scheduler.getResult<int>(id6) << std::endl;
```

Где getFutureResult это результат выполнения задачи в будущем,

### Публичный интефейс TTaskScheduler

 - **add** - принимает в качестве аргумента задание для него. Возвращает объект описывающий добавленную таску.
 - **getFutureResult<T>** - возвращает объект, из которого в будущем можно получить результат задания, переданного в качестве результата типа Т
 - **getResult<T>** - возвращает результат выполнения задания определенного типа. Вычисляет его если оно еще не подсчитано
 - **executeAll** - выполняет все запланированные задания

### Требования и ограничения к заданиям

  - [Callable object](https://en.cppreference.com/w/cpp/named_req/Callable)
  - Аргументы гарантированно [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)
  - Возвращаемое значение произвольное, гарантированно [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)
  - Количество аргументов не больше 2

## Ограничения

 Запрещено использовать стандартную библиотеку, за исключением контейнеров и умных указателей.

## Тесты

Все вышеуказанный класс должен быть покрыты тестами, с помощью фреймворка [Google Test](http://google.github.io/googletest).

Тесты также являются частью задания, поэтому покрытие будет влиять на максимальный балл.

## NB

В данной работе могут быть использованы идеи [Type Erasure](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure) который мы разбирали на лекции

## Допустимые упрощения

Задание может быть упрощено, с уменьшением максимального балла

   1. Тип возвращаемого значения одинаковый но является частью шаблона класса - 2 штрафных балла
   2. Сигнатура всех заданий одинаково - 2 штрафных балла
   3. Порядок добавления заданий важен для корректности вычисления  - 2 штрафных балла


## Deadline

1. 23.04.24. 0.8
2. 30.04.24. 0.65
3. 07.05.24. 0.5

Максимальное количество баллов - 10
