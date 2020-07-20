#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "strs.h"


#define INI_LINELEN_MAX 1024

enum IniRS {NONE, BEGIN, FOUND, PARAM, END};

class IniFile
{
    StrList lines;
    void rewind_section(StrNode* cur, const char* section, int* found, char* buf) const;

    template <typename T>
    void format_params(char* buf, const char* fmt, size_t count, T* vals);

    template <typename T>
    void process_str_buf(char* buf, const char* fmt, int need_count, int* fact_count, T* vals);

public:
    IniFile () {}

    virtual ~IniFile() { }
    
    void append(const char* s);

    /// очистка КФ
    void clear();

    /// загрузка КФ (file_name - имя файла)
    /// возвращает true в случае успеха и false если произошла ошибка;
    bool load(const char* file_name);

    /// сохранение КФ (file_name - имя файла)
    /// возвращает true в случае успеха и false если произошла ошибка;
    bool store(const char* file_name);

    /// возвращает число секций
    int get_section_count(StrList* sections = nullptr) const;

    /// возвращает число параметров в секции
    int get_params_count(const char* section, StrList* params = nullptr) const;

    /// удаляет секцию с именем section и все ее содержимое
    bool delete_section(const char* section);

    // Установить/добавить параметр
    // 1. если существует, то изменить значение
    // 2. если не существует, но секция найдена, то добавить в конец секции
    // 3. если не существует, и секция не найдена, то добавить в конец секцию с этим параметром

    /// пишет строку buf в параметр param секции section
    bool set_param(const char* section, const char* param, const char* buf);

    /// пишет массив целых чисел buf[0..count-1] в параметр param секции section
    bool set_param(const char* section, const char* param, int count, const int* buf);

    /// пишет массив чисел c плавающей точкой buf[0..count-1] в параметр param секции section
    bool set_param(const char* section, const char* param, int count, const double* buf);

    /// Переименовывает название параметра param1 на param2 в секции section
    bool rename_param(const char* section, const char* param1, const char* param2);

    /// Переименовывает название секции section1 на section2
    bool rename_section(const char* section1, const char* section2);

    /// читает в buf строку (массив чисел) из параметра param секции section
    /// max_buf_size : размер буфера для чтения строки
    /// need_count: кол-во элементов для чтения массива чисел
    /// fact_count : размер прочитанной строки или прочитанных чисел
    /// возвращает true в случае успеха и false если произошла ошибка;
    bool get_param(const char* section, const char* param, int max_buf_size, int* fact_count, char* buf);
    bool get_param(const char* section, const char* param, int need_count, int* fact_count, int* buf);
    bool get_param(const char* section, const char* param, int need_count, int* fact_count, double* buf);

    /// удаляет параметр param из секции section
    bool delete_param(const char* section, const char* param);
};
