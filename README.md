# Class Taped
Класс создан для эммуляции работы с лентой в виде передаваемого файла

В нем реализованны следующие функции:

    Taped(std::string file_path, std::ios_base::openmode openmode, std::map<std::string, int> sett);
    
    int32_t ReadCell() const;  Чтение значения в ячейки ленты (возвращает значение из текущего положения в тесктовом файле  [ячейка ленты = стока файла] )
    
    void NextCell(); - Переход к следующей ячейке
    
    void PrevCell(); - Переход к предыдущей ячейке
    
    void Forward(unsigned long long n); - Перемотка на n ячеек вперед
    
    void Backward(unsigned long long n);  - Перемотка на n ячеек назад
    
    void MoveTo(unsigned long n); - Перемотка на n ячейку
    
    void WriteCell(int32_t val); - Запись в текущую ячейку
    
    bool IsLast() const; - Запись в текущую ячейку
    
    void Erase(); - Вспомогательная функция для очистки текстового файла (для временных файлов)

# Class SortTape
Класс создан для сортировки входной ленты в выходную при условии недостатчоного размера оперативной памяти (в файле Setings.conf задается количество элементов массива).
Алгоритм реализован в следующем порядке:
1) в оперативную память загружаем кусочек файла (n-ячеек)
2) сортируем с помощью быстрой сортировки
3) вспомогательно для сортировки создаются два текстовых файла в папке tmp (один файл становится на ввода, другой -- на выворд)
4) С помощью сортировки слияние массив
