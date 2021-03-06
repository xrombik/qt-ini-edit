#include "inifile.h"


bool IniFile :: rename_section(const char* section1, const char* section2)
{   
    char buf[INI_LINELEN_MAX];
    sprintf(buf, "[%s]\n", section1);
    StrNode* cur = lines.find(buf);
    if (!cur)
        return false;
    sprintf(buf, "[%s]\n", section2);
    cur->data = (char*) realloc(cur->data, strlen(buf) + sizeof('\0'));
    strcpy(cur->data, buf);
    return true;
}


void IniFile :: append(const char* s)
{
    lines.append(s);
}


template <typename T>
void IniFile :: format_params(char* buf, const char* fmt, size_t count, T* vals)
{
    char *cur = buf;
    char s[32];
    for (size_t i = 0; i < count; i ++)
    {
        sprintf(s, fmt, vals[i]);
        cur = strcat(cur, s);
    }
}


template <typename T>
void IniFile :: process_str_buf(char* buf, const char* fmt, int need_count, int* fact_count, T* vals)
{
    *fact_count = 0;
    char* cur = strtok(buf, " ");
    while (*fact_count < need_count)
    {
        if (cur)
        {
            *fact_count += 1 == sscanf(cur, fmt, &vals[*fact_count]);
        }
        else break;
        cur = strtok(nullptr, " ");
    }
}


bool IniFile :: get_param(const char* section, const char* param, int need_count, int* fact_count, int* vals)
{
    char buf[INI_LINELEN_MAX];
    int sl = 0;
    if (!get_param(section, param, sizeof(buf) / sizeof(buf[0]), &sl, buf))
        return false;
    process_str_buf(buf, "%d", need_count, fact_count, vals);
    return *fact_count > 0;
}


bool IniFile :: get_param(const char* section, const char* param, int need_count, int* fact_count, double* vals)
{
    char buf[INI_LINELEN_MAX];
    int sl = 0;
    if (!get_param(section, param, sizeof(buf) / sizeof(buf[0]), &sl, buf))
        return false;
    process_str_buf(buf, "%lf", need_count, fact_count, vals);
    return *fact_count > 0;
}


bool IniFile :: get_param(const char* section, const char* param, int max_buf_size, int* fact_count, char* str)
{
    StrNode* cur = lines.top();
    int found = 0;
    char buf[INI_LINELEN_MAX];
    while (cur)
    {
        rewind_section(cur, section, &found, buf);
        if (found == IniRS::BEGIN)
        {
            continue;
        }
        else if (found == IniRS::FOUND)
        {
            cur = cur->next;
            continue;
        }
        else if (found == IniRS::PARAM)
        {
            if (strchr(cur->data, '=') && (cur->data[0] != ';'))
            {
                sprintf(buf, "%s =", param);
                size_t ls = strlen(buf);
                if (0 == strncmp(buf, cur->data, ls))
                {
                    char* p = strchr(cur->data, '=');
                    if (p)
                    {
                        strncpy(str, &p[1], max_buf_size);
                        *fact_count = strlen(str);
                        return 0 == strcmp(str, &p[1]);
                    }
                }
            }
        }
        cur = cur->next;
    }
    return false;
}


int IniFile :: get_params_count(const char* section, StrList* params) const
{
    int count = 0;
    StrNode* cur = lines.top();
    int found = 0;
    char buf[INI_LINELEN_MAX];
    while (cur)
    {
        rewind_section(cur, section, &found, buf);
        if (found == IniRS::BEGIN)
        {
            continue;
        }
        else if (found == IniRS::FOUND)
        {
            cur = cur->next;
            continue;
        }
        else if (found == IniRS::PARAM)
        {
            if (strchr(cur->data, '=') && (cur->data[0] != ';'))
            {
                if (params)
                    params->append(cur->data);
                count ++;
            }
        }
        else if (found == IniRS::END)
        {
            break;
        }
        cur = cur->next;
    }
    return count;
}


bool IniFile :: set_param(const char* section, const char* param, int count, const int* vals)
{
    char buf[INI_LINELEN_MAX];
    format_params(buf, "%d ", count, vals);
    return set_param(section, param, buf);
}


bool IniFile :: set_param(const char* section, const char* param, int count, const double* vals)
{
    char buf[INI_LINELEN_MAX];
    format_params(buf, "%lf ", count, vals);
    return set_param(section, param, buf);
}


bool IniFile :: set_param(const char* section, const char* param, const char* str)
{
    StrNode* cur = lines.top();
    int found = 0;
    char buf[INI_LINELEN_MAX];
    bool rc = false;

    sprintf(buf, "[%s]\n", section);
    StrNode* sec = lines.find(buf);
    if (!sec)
    {
        lines.append(buf);
        sprintf(buf, "%s = %s\n", param, str);
        lines.append(buf);
        return true;
    }

    while (cur)
    {
        rewind_section(cur, section, &found, buf);
        if (found == IniRS::BEGIN)
        {
            continue;
        }
        else if (found == IniRS::FOUND)
        {
            if (cur->next)
            {
                cur = cur->next;
            }
            continue;
        }
        else if ((found == IniRS::PARAM) && cur->next)
        {
            sprintf(buf, "%s =", param);
            size_t ls = strlen(buf);
            if (0 == strncmp(buf, cur->data, ls))  // такой параметр есть, просто заменить
            {
                size_t slv = strlen(str);
                cur->data = (char*) realloc(cur->data, ls + slv + sizeof(' ') + sizeof('\n') + sizeof('\0'));
                sprintf(cur->data, "%s = %s\n", param, str);
                rc = true;
                break;
            }
            cur = cur->next;
            continue;
        }
        
        // начинается следующая секция или конец
        if ((found == IniRS::END) ||  ((found == IniRS::PARAM) && (!cur->next)))
        {
            StrNode* sn = (StrNode*) malloc(sizeof(StrNode));
            sprintf(buf, "%s = %s\n", param, str);
            sn->data = (char*) malloc(strlen(buf) + sizeof('\0'));
            strcpy(sn->data, buf);
            sn->next = nullptr;
            rc = true;
            if (!sec->next) // если секция в конце
            {
                sec->next = sn;
            }
            else // если секция в середине
            {
                StrNode* tmp = sec->next;
                sec->next = sn;
                sn->next = tmp;
            }
            break;
        }
        cur = cur->next;
    }
    return rc;
}


void IniFile :: rewind_section(StrNode* cur, const char* section, int* found, char* buf) const
{
    if (*found == IniRS::NONE)
    {
        if (1 == sscanf(cur->data, " [%[^]]] ", buf))
        {
            *found = IniRS::BEGIN;
        }
    }
    else if (*found == IniRS::BEGIN)
    {
        sprintf(buf, "[%s]\n", section);
        if (0 == strcmp(buf, cur->data))
        {
            *found = IniRS::FOUND;
        }
        else
        {
            *found = IniRS::NONE;   
        }
    }
    else if (*found == IniRS::FOUND)
    {
        if (1 == sscanf(cur->data, " [%[^]]] ", buf))
        {
            *found = IniRS::END;
        }
        else
        {
            *found = IniRS::PARAM;
        }
    }
    else if (*found == IniRS::PARAM)
    {
        if (1 == sscanf(cur->data, " [%[^]]] ", buf))
        {
            *found = IniRS::END;
        }
    }
}


bool IniFile :: delete_param(const char* section, const char* param)
{
    StrNode* cur = lines.top();
    int found = 0;
    char buf[INI_LINELEN_MAX];
    bool rc = false;
    while (cur)
    {
        rewind_section(cur, section, &found, buf);
        if (found == IniRS::BEGIN)
        {
            continue;
        }
        else if (found == IniRS::FOUND)
        {
            cur = cur->next;
            continue;
        }
        else if (found == IniRS::PARAM)
        {
            sprintf(buf, "%s =", param);
            size_t ls = strlen(buf);
            if (0 == strncmp(buf, cur->data, ls))
            {
                rc = lines.remove(cur->data);
                break;
            }
        }
        else if (found == IniRS::END)
        {
            break;
        }
        cur = cur->next;
    }
    return rc;
}


void IniFile :: clear(void)
{
    lines.clear();
}


bool IniFile :: delete_section(const char* section)
{
    StrNode* cur = lines.top();
    int found = IniRS::NONE;
    char buf[INI_LINELEN_MAX];
    while (cur)
    {
        rewind_section(cur, section, &found, buf);
        if (found == IniRS::BEGIN) 
        {
            continue;
        }
        else if (found == IniRS::FOUND) 
        {
            cur = cur->next;
            continue;
        }
        else if (found == IniRS::PARAM)
        {
            if (lines.remove(cur->data))
            {
                cur = lines.top();
                found = IniRS::NONE;
                continue;
            }
        }
        else if (found == IniRS::END)
        {
            break;
        }
        cur = cur->next;
    }
    sprintf(buf, "[%s]\n", section);
    return lines.remove(buf);
}


int  IniFile :: get_section_count(StrList* sections) const
{
    int sc = 0;
    StrNode* cur = lines.top();
    char buf[INI_LINELEN_MAX];
    while (cur)
    {
        if (1 == sscanf(cur->data, " [%[^]]] ", buf))
        {
            sc += 1;
            if (sections)
            {
                sections->append(cur->data);
            }
        }
        cur = cur->next;
    }
    return sc;
}


bool IniFile :: load(const char* file_name)
{
    FILE* f = fopen(file_name, "r");
    if (f == nullptr)
        return false;
    char buf [INI_LINELEN_MAX];
    while (fgets(buf, sizeof(buf), f))
    {
        size_t sl = strlen(buf);
        buf[sl] = '\0';
        lines.append(buf);
    }
    fclose(f);
    return true;
}


bool IniFile :: store(const char* file_name)
{
    FILE* f = fopen(file_name, "w");
    if (!f)
        return false;
    StrNode* cur = lines.top();
    while(cur)
    {
        fprintf(f, "%s", cur->data);
        cur = cur->next;
    }
    fclose(f);
    return true;
}
