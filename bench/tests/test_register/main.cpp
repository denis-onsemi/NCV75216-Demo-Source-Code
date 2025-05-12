#include <QCoreApplication>
#include "utest.hpp"
#include "register.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    START_TEST
    {

//        // TEST constructors
//        {
//            Register REG1("@register=REG1  @readonly  ;"
//                         "@bit=A1_aasdsadaasdada12132 @descr=bitA1 description; "
//                         "@bit=A2_GHJGHJGHGGHJGGGJGJGghjghjgjg;"
//                         "@field=A3_[5];"
//                         "@field=A4_[0:1];"
//                         "@bit=A5"
//                         );
//            TEST("REG1 Register size", REG1.size(),2+5+2+1);
//            TEST("REG1 Register readonly", REG1.readOnly(),1);
//            qDebug()<<REG1.name()<<REG1.bitAt(0)->description();
//        }

//        {
//            Register REG2("@register=REG2  ;"
//                         "@bit=A1_aasdsadaasdada12132;"
//                         "@bit=A2_GHJGHJGHGGHJGGGJGJGghjghjgjg;"
//                         "@field=A3_[5];"
//                         "@field=A4_[0:1];"
//                         "@bit=A5"
//                         );
//            TEST("REG2 Register size", REG2.size(), 2+5+2+1);
//            TEST("REG2 Register readonly", REG2.readOnly(), 0);
//        }

//        {
//            Register REG3(123,"REG3");
//            TEST("REG3 Register size", REG3.size(), 123);
//            TEST("REG3 Register readonly", REG3.readOnly(), 0);
//        }

//        //
//        Register REG("@register=REG;"
//                     "@bit=Bit1 @descr=description BIT1;"
//                     "@bit=Bit2 @descr=description BIT2;"
//                     "@field=field[0:2] @descr=description field 3 bits;"
//                     );



//        TEST("New register REG",REG.size(),5);

//        REG.setValue("REG[0:1]",0);
//        TEST("Value set REG[0:1]=0",REG.value("[0:1]"),0);

//        REG.setValue("REG[2:4]",7);
//        TEST("Value set REG[2:4]=7",REG.value("[2:4]"),7);

//        REG.setValue("REG[0:4]",0xA);
//        TEST("Value set REG[0:4]=0x0A",REG.value("[0:4]"),0xA);

//        REG.setValue("REG[Bit1]",1);
//        TEST("SET REG[Bit1]=1", REG.value("REG[Bit1]"),1);

//        REG.setValue("REG[Bit2]",0);
//        TEST("SET REG[Bit2]=0", REG.value("REG[Bit2]"),0);

//        REG.setValue("REG[field]",7);
//        TEST("SET REG[field]=7", REG.value("REG[field]"),7);

//        //
//        REG.setValue(0, 999, 0xff);
//        TEST("SET from 0 to 999(exceeding) = ff", REG.value(0,999), 0x1f);

//        REG.setValue(0, 999, 0x00);
//        TEST("SET from 0 to 999(exceeding) = 0", REG.value(0,999), 0x00);

//        TEST("COMPARE REG[0:4] == REG[]",REG.value("REG[0:4]"),REG.value("REG[]"));

//        // from <-> to swapped
//        REG.setValue(4,0,0xff);
//        TEST("SET from 4 to 0 = ff",REG.value(4,0), 0x1f);

//        // try false register set to one
//        TEST("SET bit[7]=1", REG.setValue("REG[7]",0), 0);

//        //
//        REG.setValue("REG[]",0x1f);
//        TEST("SET bit[]=ff",REG.value("REG[]"),0x1f);

//        //
//        REG.setValue("REG[]",0x00);
//        TEST("SET REG[]=00",REG.value("REG[]"),0x00);

//        // test equal
//        REG = 0x12;
//        TEST("REG = 0x12",REG.value("[]"),0x12);

//        // test OR function
//        REG |= 0x1;
//        TEST("REG |= 0x1",REG.value("[]"),0x13);

//        Bit *pbit = new Bit();
//        pbit->setName("added_bit");
//        REG.addBit(pbit);
//        TEST("Add bit",REG.size(),6);



//        qDebug()<<REG.setValue("[]",0);

//        qDebug()<<">>>"<<REG.toString();

//        qDebug()<<REG.setValue("",99);

//        qDebug()<<">>>"<<REG.toString();

        Register REG_SEQ("@register=REG;"
                         "a[0:10];"
                         "b[0:5];"
                         "a[11:12]"
                     );

        qDebug()<<REG_SEQ.toString("\n@bitn::@name");



//        qDebug()<<sc.toString();

//        sc.setValue("bit[]",0xA5);
//        TEST("SET bit[]=A5",sc.value("[]"),0xA5);
//        qDebug()<<sc.toString();

//        TEST("READ bit[]=",sc.value("jopa[]"),0xA5);
//        qDebug()<<sc.toString();

//        TEST("READ bit[3:0]=",sc.value("jopa[3:0]"),0x5);
//        qDebug()<<sc.toString();

//        TEST("READ bit[4:7]=",sc.value("jopa[4:7]"),0xa);
//        qDebug()<<sc.toString();


//        QByteArray test1;
//        sc.toBitString(test1);
//        qDebug()<<test1;

//        QByteArray test2;
//        sc.toByteArray(test2);
//        TEST("To bytearray:",test2.size(),sc.size()/8);
//        qDebug()<<"HEX:"<<test2.toHex();

//        sc.fromByteArray(test2,10);
//        TEST("FrOM bytearray",sc.size(),8);

//        QByteArray test3;
//        sc.toBitString(test3);
//        TEST("to bit string len",test3.size(),8);
//        TEST("to bit string text",test3== QByteArray("10100101"),true);

//        qDebug()<<test3;
    }
    STOP_TEST;



    return a.exec();
}

