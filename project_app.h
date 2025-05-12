#ifndef HEADER_H
#define HEADER_H




/************************************************
 * [1]
 * Add here header file
 *
 ************************************************/

/* custom */
#include "tmccontroller.h"
#include "measurement.h"
#include "triangulation.h"

/************************************************
 * [2]
 * register module here
 *
 ************************************************/
#define SPECIFIC_INIT \
  REGISTER_APPLET(TmcController);\
  REGISTER_APPLET(Measurement);\
  REGISTER_APPLET(Triangulation);

#include "mainapp.h"
#include <customdialogs.h>


//#define PROJECT_TYPES    "NAA","OAA"



static bool  ProjectApp_Load_Data()
{
    bool result = false;
    return result;
}



#endif // HEADER_H
