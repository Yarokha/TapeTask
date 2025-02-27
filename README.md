# Class Taped
Класс Taped наследник интерфейса ITaped, который релизует эмуляцию работы с лентой с помощью текстового файла.
В нем реализованы следующие функции:

Private members:

    void Next();
        Переход к следующей ячейке без учета задержки из файла конфигурации.
    
    void Previous();
        Переход к предыдущей ячейке без учета задержки из файла конфигурации.

    void CreateTempFile(fs::path tmp, unsigned int i);
        Вспомогательная функция для создания временного файла. 
        На вход функции идет путь к временной папке, если ее нет, то создается автоматически.
        А также подается на вход переменная i и создается файл Temp_i.dat во временной папке.
        В частности, применяется для создания временного файла для записи в строку.

Public members:

    Taped(std::string file_path, std::ios_base::openmode openmode, std::map<std::string, int> sett);
        Конструктор класса. Передается путь к текстовому файлу, как открыть файл, и map параметров задержек. 

    int32_t ReadCell() const override; 
        Чтение значения в ячейки ленты (возвращает значение из текущего положения в текстовом файле [ячейка ленты = строка файла] )

    long long int NumCell() const override; 
        Возвращает номер текущей ячейки(отсчет с нуля).
    
    void NextCell() override;
        Переход к следующей ячейке с учетом задержки.
    
    void PrevCell() override;
        Переход к предыдущей ячейке с учетом задержки.
    
    void Forward(unsigned long long n) override; 
        Перемотка на n ячеек вперед с учетом задержки.
    
    void Backward(unsigned long long n) override; 
        Перемотка на n ячеек назад с учетом задержки.
    
    void MoveTo(unsigned long long n) override;
        Перемотка на n-ую ячейку с учетом задержки.
    
    void WriteCell(int32_t val) override; 
        Запись в текущую ячейку с учетом задержки.
    
    bool IsLast() const override; 
        Проверка последняя ячейка в ленте или нет.
    
    void Erase();
        Вспомогательная функция для очистки текстового файла (для временных файлов)
    
    ~Taped();
        Деструктор класс. При выходе закрывает файл. 
        
# Class SortTape
Класс создан для сортировки входной ленты в выходную при условии недостаточного размера оперативной памяти (в файле Setings.conf задается количество элементов массива).
Алгоритм реализован в следующем порядке:
1) В "оперативную память" загружаем кусочек файла (n-ячеек)
2) Сортируем с помощью быстрой сортировки
3) Вспомогательно для сортировки слиянием создаются два текстовых файла в папке tmp, имитирующие временные ленты (Лента 1 и Лента 2).
4) С помощью сортировки слиянием массив оперативной памяти сливается с лентой 1 и записывается в ленту 2. Обе ленты затем перематываются на начало. 
5) Далее пункты 1-4 повторяются (п.3 заменяется на: лента 1 и лента 2 меняются местами) и повторяется до тех пор, пока в "оперативную память" не будет загружен последний участок входной ленты. Тогда временная выходная лента, куда происходит слияние, заменяется на выходную ленту, куда нужно записать отсортированные данные по возрастанию.
6) Удаляется папка tmp

В классе реализованы следующие функции:
private members:

     std::vector<int32_t>::iterator LoadInMem(Taped& in);
         Вспомогательная функция для чтения значений ячеек ленты в "оперативную память"
   
    template <typename ForwardIter>  
    void Merge(ForwardIter begin, ForwardIter end, Taped& in, Taped& out);
        Вспомогательная функция для слияния "оперативной памяти" и одной временной ленты в другую.

public members:
   
    SortTape(Taped& temp1, Taped& temp2, int memory_size);
        Конструктор класса на вход, принимает две временные ленты (типа Taped), а также размер массива "оперативной памяти".
   
    void Sort(Taped& input_tape, Taped& output_tape);
        Функция, выполняющая непосредственную сортировку. На вход поступает входная и выходная ленты.

non-class members:
Для удобства не членом класса была объявлена функция sort в своем пространстве имен tape(отделить от std::sort) в вызываемых функций.

     namespace tape{
         void sort(Taped& input, Taped& output, std::map<std::string, int>settings);
             Функция принимает входную и выходную ленты (типа Taped), а также настройки из конфиг. файла (типа std::map).
             Функция инициализирует временные ленты, класса Taped, вызывает сортировку, в конце стирает временные ленты и саму временную папку.
    }

Для создания временных файлов реализована функция CreateTempFile создающая временные текстовые файлы:

    void CreateTempFile(fs::path tmp, unsigned int i);
        Вспомогательная функция для создания временного файла. 
        На вход функции идет путь к временной папке, если ее нет, то создается автоматически.
        А также подается на вход переменная i и создается файл Temp_i.dat во временной папке.
        А также подаётся i и создается файл Temp_i.dat во временной папке.  


# Файл конфигурации Settings.conf
Файл конфигурации представляет собой текстовый файл с рядом параметров для имитации задержек по чтению и записи в ячейку ленты, сдвигу и перемотке, а также размер "оперативной памяти".

    delay_reading=1000 -- задержка чтения ячейки
    delay_writing=1 -- задержка записи в ячейку
    delay_sweep=1 -- задержка на одной ячейке при перемотке
    delay_shift=1 -- задержка при сдвиге на одну ячейку
    number_memory=1000 -- количество элементов массива "оперативной памяти"

Все времена указываются в наносекундах целым (int) числом.

В main.cpp реализованная функция парсинга конфиг. файла: 

    std::map<std::string, int> LoadConfig();
        Функция парсинга конфиг. файла. Внутри функции происходит чтение, парсинг и возвращает std::map, где ключ это имя параметра.

