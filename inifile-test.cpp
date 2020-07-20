#include <stdlib.h>
#include "inifile.h"

// Ожидаемое содержание ini файла для теста
const char* INI_FILE_CONTENT =
    "[Section1]\n"
    "LINEONE\n"
    "; comment <paramname> = <param value>\n"
    "Param1 = line123\n"
    "Param2 = 1 2 3\n"
    "Param3 = 1.05   2  3.3\n"
    "Param4 = 5 6 78 890\n"
    "\n"
    "[Section2]\n"
    "ParamA = 4\n"
    "ParamB =\n"
    "\n"
    "[Section3]\n"
    "ParamC = 5\n"
    "ParamD = END\n";


int main(int argc, char *argv[])
{
    if (argc != 4)
        return EXIT_FAILURE;
    IniFile ini;
    bool rc0 = ini.load(argv[1]);
    bool rc1 = ini.store(argv[2]);
    int c0 = ini.get_section_count();
    bool rc2 = ini.delete_section("Section2");
    int c1 = ini.get_section_count();
    int c2 = ini.get_params_count("Section1");
    int c3 = ini.get_params_count("Section2");
    bool rc3 = ini.delete_param("Section1", "ParamA");
    bool rc4 = ini.delete_param("Section1", "Param2");
    bool rc5 = ini.set_param("Section1", "set_new_param1", "new_param1_val");
    bool rc6 = ini.set_param("Section1", "Param1", "Param1_val");
    bool rc7 = ini.set_param("Section2", "Param5", "Param5_val");
    bool rc8 = ini.store(argv[3]);

    char char_buf[INI_LINELEN_MAX];
    int ls0 = 0;
    bool rc9 = ini.get_param("Section1", "Param1", sizeof(char_buf) / sizeof(char_buf[0]), &ls0, char_buf);
    
    int ls1 = 0;
    bool rc10 = ini.get_param("Section1", "Param_NOT_EXISTS", sizeof(char_buf) / sizeof(char_buf[0]), &ls1, char_buf);
    
    int ls2 = 0;
    bool rc11 = ini.get_param("Section1_NOT_EXISTS", "Param1", sizeof(char_buf) / sizeof(char_buf[0]), &ls2, char_buf);
    
    int int_buf[32];
    int ls3 = 0;
    bool rc12 = ini.get_param("Section1", "Param4", 4, &ls3, int_buf);
    
    double double_buf[32];
    int ls4 = 0;
    bool rc13 = ini.get_param("Section1", "Param3", 3, &ls4, double_buf);
    
    int c4 = ini.get_params_count("Section1");
    bool rc14 = ini.rename_section("Section1", "Section1_RENAMED");
    int c5 = ini.get_params_count("Section1");
    int c6 = ini.get_params_count("Section1_RENAMED");

    bool rc = rc0 && rc1 && (c0 == 3) && rc2 && (c1 == 2) && (c2 == 4) && (c3 == 0)
            && (!rc3) && rc4 && rc5 && rc6 && rc7 && rc8 && rc9 && (!rc10) && (!rc11)
            && rc12 && (ls3 == 4) && rc13 && (ls4 == 3) && rc14 && (c5 == 0) && (c4 == c6);

    return rc ? 0 : 1;
}
