#include "strs.h"

void StrList :: clear(void)
{
    StrNode* prev = nullptr;
    StrNode* cur = root;
    if (!cur)
        return;
    while(cur->next)
    {
       prev = cur;
       cur = cur->next;
       free(prev->data);
       free(prev);
    }
    free(cur->data);
    free(cur);
    root = nullptr;
}


StrNode* StrList :: find(const char* s)
{
    StrNode* sn = root;
    while (sn)
    {
        if (0 == strcmp(sn->data, s))
            break;
        sn = sn->next;
    }
    return sn;
}


bool StrList :: pop(char *s)
{
    if (!root)
        return false;
    StrNode* prev = root;
    strcpy(s, prev->data);
    root = root->next;
    free(prev->data);
    free(prev);
    return true;
}


bool StrList::remove(const char* s)
{
    StrNode* sn = root;
    StrNode* sn_prev = nullptr;
    bool rc = false;
    while (sn)
    {
        if (0 == strcmp(sn->data, s))
        {
            if (sn_prev)
            {
                sn_prev->next = sn->next;
            }
            else
            {
                root = sn->next;
            }
            free(sn->data);
            free(sn);
            rc = true;
            break;
        }
        sn_prev = sn;
        sn = sn->next;
    }
    return rc;
}


StrNode* StrList :: append(const char* s)
{
    StrNode* sn_new = (StrNode*) malloc(sizeof(StrNode));
    sn_new->data = (char*) malloc(strlen(s) + sizeof('\0'));
    sn_new->next = nullptr;
    strcpy(sn_new->data, s);
    StrNode* sn = nullptr;
    if (root)
    {
        sn = root;
        while (sn->next)
        {
            sn = sn->next;
        }
        sn->next = sn_new;
    }
    else
    {
        root = sn_new;
    }
    return sn_new;
}


bool StrList :: push(const char* s)
{
    StrNode* sn = (StrNode*)malloc(sizeof(StrNode));
    sn->data = (char*)malloc(strlen(s) + sizeof('\0'));
    sn->next = root;
    strcpy(sn->data, s);
    root = sn;
    return true;
}
