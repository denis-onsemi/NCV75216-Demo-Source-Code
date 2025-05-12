#ifndef BOARD_DEF_H
#define BOARD_DEF_H
#include <QTypeInfo>

#define DIO_INPUT (1)
#define DIO_OUTPUT (0)

#define CLK "=2"
#define PULSE "=3"
#define HIGH "=1"
#define LOW "=0"

/* [ PINS ]*/
#define PIN_LIN1_EN         "DIO50"
#define PIN_IO_IN           "DIO30"
#define PIN_IO_OUT          "DIO31"

#define PIN_LIN2_EN         "DIO32"
#define PIN_IO_IN_DUT2      "DIO45"
#define PIN_IO_OUT_DUT2     "DIO44"

#define PIN_LIN3_EN         "DIO33"
#define PIN_IO_IN_DUT3      "DIO20"
#define PIN_IO_OUT_DUT3     "DIO21"

#define PIN_LIN4_EN         "DIO55"
#define PIN_IO_IN_DUT4      "DIO46"
#define PIN_IO_OUT_DUT4     "DIO47"


#endif // BOARD_DEF_H
