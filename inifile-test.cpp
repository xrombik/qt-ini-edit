#include <stdlib.h>
#include "inifile.h"


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

    bool rc = rc0 && rc1 && (c0 == 2) && rc2 && (c1 == 1) && (c2 == 3) && (c3 == 0)
            && (!rc3) && rc4 && rc5 && rc6 && rc7 && rc8;

    return rc ? 0 : 1;
}
