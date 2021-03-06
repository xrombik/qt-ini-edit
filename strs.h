#pragma once

#include <malloc.h>
#include <string.h>


struct StrNode
{
    char* data;
    StrNode* next;
    StrNode() : data(nullptr), next(nullptr) {}
};


class StrList
{
    StrNode* root;
    void remove_next(StrNode* sn_prev, StrNode* sn);
public:
    /// Находит узел с заданной строкой
    StrNode* find(const char* s);
    /// Возвращает строку из первого узла и удаляет его
    bool pop(char* s);
    /// Удаляет узел с заданной строкой
    bool remove(const char* s);
    /// Удаляет узел по указателю
    bool remove(StrNode* sn);
    /// Дабавляет строку в конец
    StrNode* append(const char* s);
    /// Дабавляет строку в начало
    bool push(const char* s);
    /// Возвращает указатель на начало списка
    StrNode* top(void) const { return root; }
    StrList(void) : root(nullptr) { }
    void clear(void);
    virtual ~StrList() { clear(); }
};
