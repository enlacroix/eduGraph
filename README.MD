# Лабораторная работа №3. Создание ориентированного графа. Реализация алгоритма Дейкстры.

## Как пользоваться?
1. Сформировать (скачать) .exe файл. 
2. cd [путь к директории, где лежит исполняемый файл]
3. DijkstraImpl.exe --file "graph1.txt" --from 3 --to 6
(все демонстрационные файлы я назвал graph{i}.txt, где i принимает натуральные значения. Это важно для питоновского скрипта.)
4. [Необязательно] *python c:/[путь до .py скрипта] 1 [номер файла] 3 5 4 2 [путь, скопируй его из консоли]*

## Визуализация
alg::visualize(filepath, route) - позволяет считать результаты алгоритма и запустить питоновский скрипт на визуализацию графа. 
Путь отмечается красными рёбрами, но, к сожалению, происходит наслоение весов при неудачной генерации картинки. 
Поддерживаются петли!
visualize() продемонстрирует обычный граф, если route будет пустым. 

## Исключения
- GraphKeyError - переданный ключ не существует в графе. 
- InvalidAdjMatrix - условия на матрицу смежности не соблюдены.  
- InvalidPath - по указанному пути не был найден файл.
- ExpectedAnotherSymbol - некорректный символ при считывании файла.
- AllElementsMustBePositive - условие алгоритма Дейкстры.
- InvalidInputArgs - проблемы при считывании аргументов из комадной строки. 
- NoRoute - пути между указанными вершинами не существует. 
- NotSquareMatrix - матрица смежности в файле не квадратная. Отлаливается через std::invalid_argument. 

## Примечания по алгоритму и программе
- Обрабатывает ситуацию, если начальная вершина равна конечной, вам выведется, что вес ноль, а путь пустой.
- Поддерживает ввод аргументов в любом порядке, используется ввод с помощью unordered_map c texteditor-a. 
- Для метки бесконечности используется limits::inf().
- Веса могут быть не только double.
- Для русской локализации необходимо использовать запятые в качестве разделителей целой и дробной части. Соответствующую поправку нужно сделать и в .py файле, нужный фрагмент закомментирован. 
- Не использую value_type для того чтобы хранить метки для алгоритма, для этого завёл атрибут label. 

## Расширенные возможности (вне задания)
- Поддерживает deleteNode и deleteEdge. 
- show() - текстовое представление графа в консоли. Можно задать имя графу с помощью метода .setName(). 
- showLabels() - метод, позволяющий поэтапно отследить выполнение алгоритма Дейкстры. Применяется для отлова ошибок. 
## "Матричное DLC"
- (* c проверкой на то, что матрица квадратная *)Добавил новый конструктор для графа через матрицу весов (матрица смежности не совсем корректное название для неё). Пример: (значение для вершин - по умолчанию)
```
linalg::Matrix adj = { {0, 1, 0}, {1, 1, 2}, {4, 5, 0} };
Graph<std::size_t, std::string, double> mySummerGraph(adj);
```
- Получить матрицу смежности (параметр по умолчанию - true, т.е. получить матрицу смежности из нулей и единиц.) Если false, то вызовется матричное представление графа. 
```
std::cout << mySummerGraph.getAdjacencyMatrix(false);
```
- Добавил к уже существующему модулю метод .power(), который возводит в натуральную степень матрицы. (если нулевая степень вернет единичную матрицу)
- Введено понятие "матрицы достижимости" - в качестве примера взята матрица из Википедии с соотв. статьи. 
```
linalg::Matrix adj = { {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 1, 0} };
std::cout << mySummerGraph.getReachabilityMatrix() << "\n";
```
|1 2 2 1|
|0 1 0 0|
|0 2 2 2|
|0 1 2 2|
Смысл: Reach[i][j] - сколько путей существует из вершины i + 1 в вершину j + 1(учитывается, что мы можем прийти из вершины 1 в вершину 1, даже если нет петли, это достигается добавлением единичной матрицы). 
```
linalg::Matrix wht = { {0, 5, 1, 0}, {0, 0, 0, 0}, {0, 6, 0, 1}, {0, 0, 5, 0} };
```
Можно передать матрицу весов wht, и применить тот же метод и это сработает! Поскольку ранее я определил метод, который вернёт матрицу смежности. 
- C помощью invertedGraph(graph.getReachabilityMatrix(false).transpose()) можно перенаправить (т.е. изменить направление каждому маршруту) все дороги в нашем графе. 
## Пример графа (для быстрого старта)
Граф социальных связей. Ключ - имя пользователя, значение - возраст, веса - взаимоотношения (количество лайков/дизлайков).
```
std::string NAME_1 = "ilusha88";
std::string NAME_2 = "Valerie";
std::string NAME_3 = "Innokentiy";
std::string NAME_4 = "Admin";

Graph<std::string, int, int> OKSocialNetwork; 
using default_node = typename Graph<std::string, int, int>::Node;
OKSocialNetwork.setName("Best friends forever?");
	
OKSocialNetwork.insert_node(NAME_1, 15);
OKSocialNetwork.insert_node(NAME_2, 40);
OKSocialNetwork.insert_node(NAME_3, 28);
OKSocialNetwork.insert_node(NAME_4, 0);

OKSocialNetwork.insert_edge(std::make_pair(NAME_1, NAME_2), -2);
OKSocialNetwork.insert_edge(std::make_pair(NAME_1, NAME_3), -10);
OKSocialNetwork.insert_edge(std::make_pair(NAME_1, NAME_4), 1);
OKSocialNetwork.insert_edge(std::make_pair(NAME_2, NAME_2), 1);
OKSocialNetwork.show();
```
Борьба с самолайками:
```
OKSocialNetwork.deleteEdge(std::make_pair(NAME_2, NAME_2)); 
```
