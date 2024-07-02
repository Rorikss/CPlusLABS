[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=14075992&assignment_repo_type=AssignmentRepo)
# Лабораторная работа 8

Бинарное дерево поиска. Шаблоны. STL.

## Задача

Реализовать три STL-совместимых контейнера для [BinarySearchTree](https://en.wikipedia.org/wiki/Binary_search_tree), реализующие различные [способы обхода дерева (in-, pre-, post-order)](https://en.wikipedia.org/wiki/Tree_traversal) через итератор.

## Требования

Контейнер должен предоставлять из себя шаблон, праметрезируемый типом хранимых объетов, оператором сравнения и аллокатором, а так же удовлетворять следующим требованиям к stl - совместимым контейнерам:

  - [контейнера](https://en.cppreference.com/w/cpp/named_req/Container)
  - [контейнера с обратным итератором](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer)
  - [контейнера поддерживающие аллокатор](https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)
  - [oбладать двунаправленным итератом](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)

Способ обхода дерева должен быть реализован через итератор, те оператор "++" должен приводить к перемещению итератора к следующему элементу в дереве, согласно правилу обхода.

## Тесты

Все вышеуказанные требования должны быть покрыты тестами, с помощью фреймворка [Google Test](http://google.github.io/googletest).

Тесты также являются частью задания, поэтому покрытие будет влиять на максимальный балл.

## Ограничения

- Запрещено использовать стандартные контейнеры

## NB

Подумайте над тем как не делать 3 разных контейнера, а воспользоваться [Tag Dispatch Idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)

## Deadline

1. 12.03.24. 0.8
2. 19.03.24. 0.65
3. 26.03.24. 0.5

Максимальное количество баллов - 15
