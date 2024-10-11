# Class Taped
Класс создан для эммуляции работы с лентой с помощью текстового файла.

В нем реализованны следующие функции:

Private members:

    void Next();
        Переход к следующей ячейки, без учета задержки из файла конфигурации.
    
    void Previous();
        Переход к предыдущей ячейки, без учета задержки из файла конфигурации.

    void CreateTempFile(fs::path tmp, unsigned int i);
        Вспомогательная функция для создания временых файлов. На вход функции идет путь к временной папке, если ее нет создает автоматически. А также поадется i и создается файл Temp_i.dat во временной папке.  В частности применяется для создания временного файла для записи в строку.

Public members:

    Taped(std::string file_path, std::ios_base::openmode openmode, std::map<std::string, int> sett);
        Конструтор класса. Передается путь к текстовуму файлу, как открыть файл, и map параметров задержек. 

    int32_t ReadCell() const; 
        Чтение значения в ячейки ленты (возвращает значение из текущего положения в тесктовом файле  [ячейка ленты = строке файла] )
    
    void NextCell();
        Переход к следующей ячейке
    
    void PrevCell();
        Переход к предыдущей ячейке
    
    void Forward(unsigned long long n); 
        Перемотка на n ячеек вперед
    
    void Backward(unsigned long long n); 
        Перемотка на n ячеек назад
    
    void MoveTo(unsigned long long n);
        Перемотка на n-ую ячейку
    
    void WriteCell(int32_t val); 
        Запись в текущую ячейку
    
    bool IsLast() const; 
        Проверка последняя ячейка
    
    void Erase();
        Вспомогательная функция для очистки текстового файла (для временных файлов)
    
# Class SortTape
Класс создан для сортировки входной ленты в выходную при условии недостатчоного размера оперативной памяти (в файле Setings.conf задается количество элементов массива).
Алгоритм реализован в следующем порядке:
1) В оперативную память загружаем кусочек файла (n-ячеек)
2) Сортируем с помощью быстрой сортировки
3) Вспомогательно для сортировки создаются два текстовых файла в папке tmp иммитирующие временные ленты (Лента 1 и Лента 2).
4) С помощью сортировки слияние массив оперативной памяти сливается с лентой 1 и записывается в ленту 2. Обе ленты затем перематываются на начало. 
5) Затем пункт 1-4 повторяются (п.3 заменяется на: лента 1 и лента 2 меняются местами) и повторяется до тех пор пока в "оперативную память" не будет загружен последний участок входной ленты. Тогда временная выходная лента куда происходит слияние заменяется выходную ленту, куда нужно записать отсортированные данные по возрастанию.
6) Удаляется папка tmp

В классе реализованны следующие функции:

private members:

     std::vector<int32_t>::iterator LoadInMem(Taped& in);
         Вспомогательная функция для чтения значение ячеек ленты в "оперативную память"
   
    template <typename ForwardIter>  
    void Merge(ForwardIter begin, ForwardIter end, Taped& in, Taped& out);
        Вспомогательная функция для слияния оператиной памяти и одной ленты в другую.

public members:
   
    SortTape(Taped& temp1, Taped& temp2, int memory_size);
        Конструктор класса на вход принимает две временные ленты (типа Taped), а также размер массива "оперативной памяти".
   
    void Sort(Taped& input_tape, Taped& output_tape);
        Функция выполняющая непосредственную сортировку. на вход поступает входная и выходная ленты.

non-class members:
Для удобства не членом класса была объявленна функция sort в своем пространсве имен tape(отделить от std::sort) в вызываемых функций.

     namespace tape{
     void sort(Taped& input, Taped& output, std::map<std::string, int>settings);
         Функция принимает входную и выходную ленты (типа Taped), а также настройки из конфиг. файла (типа std::map). Функция иницилизирует временные ленты, класс SortTape, вызывает сортировку, в окнце стирает временные ленты и саму временную папку.
    }

Для создания временных файлов реализованна функция CreateTempFile создающая временные тектовые файлы:

        void CreateTempFile(fs::path tmp, unsigned int i);
        Вспомогательная функция для создания временых файлов. На вход функции идет путь к временной папке, если ее нет создает автоматически. А также поадется i и создается файл Temp_i.dat во временной папке.  


# Файл конфигурации Settings.conf
Файл кофигурации представляют собой текстовый файл с рядом параметров для имитации задержек по чтению и записи в ячейку ленты, сдвигу и переметоке, а также размер "оперативной памяти".

    delay_reading=1000 -- задержка чтения ячейки
    delay_writing=1 -- задержка записи в ячейку
    delay_sweep=1 -- задержка на одной ячейке при перемотке
    delay_shift=1 -- задержка при сдвиге на одну ячейку
    number_memory=1000 -- количесво элементов массива "оперативной памяти"

Все времена указываются в наносекундах целым (int) числом.

В main.cpp реализованная функция парсинга конфиг. файла: 

    std::map<std::string, int> LoadConfig();
        Функция парсинга конфиг. файла. Внутри функци происходит чтение, парсинг и возвращает std::map, где ключ это имя параметра.

