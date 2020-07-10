#include "strs.h"


int main(int argc, char** argv)
{
    StrList sl;
    bool rc;
    char buf[1024];
    StrNode* sn1 = sl.append("1-abcd");
    StrNode* sn2 = sl.append("2-fghv");
    StrNode* sn3 = sl.append("3-vbnm");
    StrNode* sn4 = sl.append("4-uiop");

    StrNode* sn5 = sl.find("1-abcd");
    StrNode* sn6 = sl.find("2-fghv");
    StrNode* sn7 = sl.find("3-vbnm");
    StrNode* sn8 = sl.find("4-uiop");
    StrNode* sn9 = sl.find("notfound");

    bool rc1 = sl.remove("notfound");
    bool rc2 = sl.remove("2-fghv");
    bool rc3 = sl.remove("3-vbnm");
    bool rc4 = sl.remove("4-uiop");
    bool rc5 = sl.remove("1-abcd");
    bool rc6 = sl.remove("notfound");

    StrNode* sn10 = sl.append("1-abcd");
    StrNode* sn11 = sl.append("2-fghv");
    StrNode* sn12 = sl.append("3-vbnm");
    StrNode* sn13 = sl.append("4-uiop");

    bool rc7 = sl.pop(buf);
    bool rc8 = sl.pop(buf);
    bool rc9 = sl.pop(buf);
    bool rc10 = sl.pop(buf);
    bool rc11 = sl.pop(buf);

    rc = (sn1 == sn5) && (sn2 == sn6) && (sn3 == sn7) && (sn4 == sn8) && (sn9 == nullptr);
    rc &= (rc1 == false) && (rc2 == true) && (rc3 == true) && (rc4 == true) && (rc5 == true) && (rc6 == false);
    rc &= (sn10 != nullptr) && (sn11 != nullptr) && (sn12 != nullptr) && (sn13 != nullptr);
    rc &= (rc7 == true) && (rc8 == true) && (rc9 == true) && (rc10 == true) && (rc11 == false);
    return rc ? 0 : 1;
}
