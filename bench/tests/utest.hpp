#ifndef UTEST_HPP
#define UTEST_HPP
#include <QDebug>
#include <QString>



#define PRINT(MES) {qDebug()<<(MES);}

#define START_TEST do{ \
bool utest_result=true; \
int utest_n=0;\

#define STOP_TEST PRINT("");PRINT("Final RESULT:");\
PRINT(utest_result);\
}while(0);

#define BREAK break;

#define TEST(MESSAGE,VAL_RES,VAL_EXP) do{\
PRINT("");\
PRINT(QString("[Test %1]").arg(utest_n));\
PRINT(MESSAGE);\
    PRINT(QString("result=%1(0x%2) expected=%3(0x%4) - %5")\
    .arg(VAL_RES)\
    .arg(QString::number(VAL_RES,16))\
    .arg(VAL_EXP)\
    .arg(QString::number(VAL_EXP,16))\
    .arg((VAL_RES) == (VAL_EXP)?"OK":"ERROR")\
);\
utest_result &= ( (VAL_RES) == (VAL_EXP));\
utest_n++;\
}while(0);



#endif // UTEST_HPP
