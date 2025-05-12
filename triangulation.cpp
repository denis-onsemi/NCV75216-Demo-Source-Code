/*!
Class Triangulation (Trilateration)
-------------------
.Triangulation
******************************
This class analyzes measured times from multiple ultrasonic sensors and converts them
to distances. Converted distances are checked by two different algorithms, which can
detect the presence of a real obstacle and calculate its coordinates. Calculated
coordinates are respresented in graphical manner in associated GUI.
******************************
*/

#include "triangulation.h"
#include "ui_triangulation.h"
#include "mainapp.h"
#include <QDebug>
#include "qmath.h"
#include "tmccontroller.h"
#include "measurement.h"

#define MEASUREMENTS 3          //Number of measurements
#define BEAM_PATTERN 80         //Horizontal beam pattern of used piezotransducer [degrees]
#define DIRECT(direct)              (SpeedOfSound*100*direct*1e-6*0.5)               //[cm]
#define INDIRECT(direct, indirect)  ((indirect-(direct*0.5))*1e-6*SpeedOfSound*100)  //[cm]

Triangulation * gp_Triangulation;

/****************************************************************************
 *                              [1] REGISTERS
 ****************************************************************************/
double Distances[MEASUREMENTS][MEASUREMENTS][4];        //Table with Distances - format: [Number of measurement][DUT][0,1=Direct1; 2,3=Direct2]
double Obstacles[8][6] = {0};               //Table with confirmed [Both DUTs !=0] and unconfirmed [First DUT != 0] distances to detected obstacles
                                            //[Number of obstacle(max.=8)][/Sens x/Distance x/Sens y/Distance y/(Sens z/Distance z - only in triple conf.)]
int obs_cnt = 0;                            //Obstacle count
int uncf_cnt = 0;                           //Count of unconfirmed obstacles
QVector<double> ObsX, ObsY;                 //Vector with coordinates of detected obstacles
QVector<double> ObsXtemp, ObsYtemp;                 //Vector with coordinates of detected obstacles
bool Unconfirmed[MEASUREMENTS][MEASUREMENTS][4] = {1};  //Table with flags, which are set to '1' if this distance is NOT confirmed - same structure as the Distances array
int TOLERANCE = 5;                          //Tolerance during comparasion of equality between Direct/Indirect measurements [cm]
int TOL_PRUSEC = 5;                         //Tolerance during calculation of the point of three circle intersection
double Positions[64][3] = {0};              //Table containing the positions determined by algorithm which have to be checked for geometrical solvability
int pos_cnt = 0;                            //Count of positions confirmed by algorithm
quint8 AlgChoice = 0;                       //Selection of used confirmation algorithm
bool ProjSel = 1;                           //Projection selection - observing from the front (1) of the bumper or from behind (0) the bumper
double SpeedOfSound = 343;                  //Speed of sound in the environment
bool AdAxisRng = 0;                         //Adaptive axis range, disabled by default

//Saving previous results - QVector concept
typedef QVector<double> t_coords;           //Columns in "array" - X coordinate, Y coordinate
typedef QVector<t_coords> t_obs;            //Lines in "array" - obstacle1,2,3....8
typedef QVector<t_obs> t_measurements;      //Measurements - third dimension of "array" - 0 = newest, 9 = oldest (10 saved previous measurements)
t_coords v_coords;
t_obs v_obs;
t_measurements v_measurements;
int mem_cnt = 0;                            //Number of saved measurements
int TOL_MEM = 10;                           //Tolerance pri posuzovani shody s prekazkou v pameti
int mem_match_thr = 2;                      //Po kolika shodach s pameti ma byt prekazka vykreslena

//Coordinates of piezotransdurers (X,Y)
double Coordinates[MEASUREMENTS][2] =   {0, 0,      //Sensor 1
                                        -20, 0,      //Sensor 2
                                        20, 0};     //Sensor 3

/****************************************************************************
 *                              [2] APPLICATION
 ****************************************************************************/
Triangulation::Triangulation(QWidget *parent) :
    Applet(parent),
    ui(new Ui::Triangulation)
{
    ui->setupUi(this);
    setWindowTitle("Triangulation");    //Trilateration
    ui->tw_results->setCurrentIndex(1);
}

void Triangulation::setupCustomUI()
{
    ui->ptTriang->addGraph();   //piezo
    ui->ptTriang->addGraph();   //bumper
    ui->ptTriang->xAxis->setLabel("x [cm]");
    ui->ptTriang->yAxis->setLabel("y [cm]");
    QPen GraphPen;              //piezo pen
    GraphPen.setColor(Qt::black);
    GraphPen.setWidth(14);
    ui->ptTriang->graph(0)->setPen(GraphPen);
    QPen GraphPenLine;          //line pen
    GraphPenLine.setColor(Qt::black);
    GraphPenLine.setWidth(5);
    ui->ptTriang->graph(1)->setPen(GraphPenLine);
}


bool Triangulation::initialize()
{
    return true;
}

Triangulation::~Triangulation()
{
    delete ui;
}


/****************************************************************************
 *                              [3] FUNCTIONALITY
*****************************************************************************/
QVariant Triangulation::FUNCTION_LIST()
{
    //UNREFERENCED_PARAMETER(arg_data);                   // to avoid warning, parameter is not used

    FUNCTION("ReadTriData","Read all triangulation data and plot the result")
    {
        //qDebug() << "|||||||||||---------------NEXT MEASUREMENT---------------|||||||||||";
        //TOL_PRUSEC = ui->sbTriangulation_Tol_prusec->value();   //read both TOLERANCE settings from GUI
        //TOLERANCE = ui->sbTriangulation_Tolerance->value();
        AlgChoice = ui->cbAlgChoice->currentIndex();            //Read Algorithm choice

/**************************************************/
        //Populate Distances table - conversion from measured ToFs
        double calc_distance;
        for(int meas=0; meas<MEASUREMENTS; meas++)              //Measurement 1-4
        {
            //Direct DUT has to be found first, because it is used to calculate indirect distances
            for(int j=0; j<4; j++)
            {
                if(j<2)
                {
                    calc_distance = DIRECT(received_data[meas][meas][0]);         //Direct1
                    if (calc_distance > 0)
                        Distances[meas][meas][j] = calc_distance;
                    else
                        Distances[meas][meas][j] = 0;
                }
                else
                {
                    calc_distance = DIRECT(received_data[meas][meas][1]);         //Direct2
                    if (calc_distance > 0)
                        Distances[meas][meas][j] = calc_distance;
                    else
                        Distances[meas][meas][j] = 0;
                }
            }
            //Indirect distances
            for(int i=0; i<MEASUREMENTS; i++)                  //DUT 1-4
            {
                if(i != meas)   //DUTs in indirect mode
                {
                    for(int j=0; j<2; j++)                                            //Switching between Direct1/Direct2
                    {
                        calc_distance = INDIRECT(received_data[meas][meas][j],received_data[meas][i][0]);   //Indirect1
                        if (calc_distance > 0)
                             Distances[meas][i][j*2] = calc_distance;
                        else
                             Distances[meas][i][j*2] = 0;

                        calc_distance = INDIRECT(received_data[meas][meas][j],received_data[meas][i][1]);   //Indirect2
                        if (calc_distance > 0)
                             Distances[meas][i][1+(j*2)] = calc_distance;
                        else
                             Distances[meas][i][1+(j*2)] = 0;
                    }
                }
            }
        }


        /*qDebug() << "________________Distances_________________";
        for(int i=0; i<MEASUREMENTS; i++)       //meas
        {
            for(int m=0; m<MEASUREMENTS; m++)   //duts
            {
                qDebug() << "DUT" << m+1 << ": col0" << Distances[i][m][0] << "col1" << Distances[i][m][1] << "col2" << Distances[i][m][2] << "col3" << Distances[i][m][3]; //values
            }
            qDebug() << "--------------------------";
        }*/


/**************************************************/
        //Look for Confirmed positions in 3D Distances array
        ui->ptTriang->clearGraphs();
        Triangulation::setupCustomUI();
        obs_cnt = 0;                                            //Total count of obstacles (both conf. and unconf.)
        uncf_cnt = 0;                                           //Count of unconfirmed obstacles
        pos_cnt = 0;                                            //Count of determined positions
        ObsX.clear();                                           //Clear previous results
        ObsY.clear();
        ObsXtemp.clear();                                           //Clear previous results
        ObsYtemp.clear();

        for(int i=0; i<3; i++)                                  //Unconfirmed Flags are set as a default value
        {
            for(int j=0; j<3; j++)
            {
                for(int k=0; k<4; k++)
                {
                    Unconfirmed[i][j][k] = 1;
                }
            }
        }
        for(int j=0; j<8; j++)                                  //Initialize Obstacles array
        {
            for(int k=0; k<6; k++)
            {
               Obstacles[j][k] = 0;
            }
        }
        for(int j=0; j<64; j++)                                 //Initialize Positions array
        {
            for(int k=0; k<3; k++)
            {
               Positions[j][k] = 0;
            }
        }
        /*CONFIRMATION ALGORITHMS*/
        if(AlgChoice==0)
        {
        /*********************************************************/
        //Algorithm utilizing all 3 sensors
            /******** Obstacle confirmation algorithm ********/
            int meas = 1;
            int sensor = 1;
            for(int curr_dir=0; curr_dir<4; curr_dir=curr_dir+2)            //Step1 - get one of the direct distances measured in meas.2 by sensor 2
            {
                double M2_dir = Distances[meas][sensor][curr_dir];          //Save it as current direct
                //qDebug() << "__Triple conf. algorithm__" << M2_dir;
                //qDebug() << "M2_dir" << M2_dir;

                for(int curr_ind_swp1=0; curr_ind_swp1<2; curr_ind_swp1++)  //Step2 - compare indirect meas. measured by sensor 1 in meas.2 with direct meas. in meas. 1 (by the same sensor)
                {                                                           //[only indirects calculated by subtracting M2_dir]
                    for(int prev_dir=0; prev_dir<4; prev_dir=prev_dir+2)
                    {
                        if((qAbs(Distances[meas][sensor-1][curr_dir+curr_ind_swp1]-Distances[meas-1][sensor-1][prev_dir]) <= TOLERANCE))
                       {
                            //qDebug()<< "*first confirmation";
                            /*qDebug()<< "indirect meas. measured by sensor 1 in meas.2 = direct meas. in meas. 1" << "- Indirect" << curr_dir+curr_ind_swp1 << " = "<< Distances[meas][sensor-1][curr_dir+curr_ind_swp1]
                                    << ", Direct" << prev_dir << "= "<< Distances[meas-1][sensor-1][prev_dir]
                                    << ", Delta:" << Distances[meas][sensor-1][curr_dir+curr_ind_swp1]-Distances[meas-1][sensor-1][prev_dir];*/
                           //Step3 - Now compare indirect meas. measured by sensor 3 in meas. 2 with direct meas. in meas. 3
                           for(int curr_ind_swp3=0; curr_ind_swp3<2; curr_ind_swp3++)   //[only indirects calculated by subtracting M2_dir]
                           {
                               for(int next_dir=0; next_dir<4; next_dir=next_dir+2)
                               {
                                   if((qAbs(Distances[meas][sensor+1][curr_dir+curr_ind_swp3]-Distances[meas+1][sensor+1][next_dir]) <= TOLERANCE))
                                   {
                                       //qDebug()<< "*second confirmation";
                                       /*qDebug()<< "indirect meas. measured by sensor 3 in meas. 2 = direct meas. in meas. 3" << "- Indirect" << curr_dir+curr_ind_swp3 << " = "<< Distances[meas][sensor+1][curr_dir+curr_ind_swp3]
                                               << ", Direct" << next_dir << "= "<< Distances[meas+1][sensor+1][next_dir]
                                               << ", Delta:" << Distances[meas][sensor+1][curr_dir+curr_ind_swp3]-Distances[meas+1][sensor+1][next_dir];*/
                                       //Step4 - compare indirects measured by sensor 2 in meas. 1 with M2_dir
                                       for(int prev_ind_swp=0; prev_ind_swp<2; prev_ind_swp++)
                                       {
                                           if((qAbs(Distances[meas-1][sensor][prev_dir+prev_ind_swp]-M2_dir) <= TOLERANCE))
                                           {
                                               //qDebug()<< "*third confirmation";
                                               /*qDebug()<< "indirect measured by sensor 2 in meas. 1 = M2_dir" << "- Indirect" << prev_dir+prev_ind_swp << " = "<< Distances[meas-1][sensor][prev_dir+prev_ind_swp]
                                                       << ", Direct" << M2_dir
                                                       << ", Delta:" << Distances[meas-1][sensor][prev_dir+prev_ind_swp]-M2_dir;*/
                                               //Step5 - compare indirects measured by sensor 2 in meas. 3 with M2_dir
                                               for(int next_ind_swp=0; next_ind_swp<2; next_ind_swp++)
                                               {
                                                  if((qAbs(Distances[meas+1][sensor][next_dir+next_ind_swp]-M2_dir) <= TOLERANCE))
                                                  {
                                                      //qDebug()<< "*fourth confirmation";
                                                      /*qDebug()<< "indirect measured by sensor 2 in meas. 3 = M2_dir" << "- Indirect" << next_dir+next_ind_swp << " = "<< Distances[meas+1][sensor][next_dir+next_ind_swp]
                                                              << ", Direct" << M2_dir
                                                              << ", Delta:" << Distances[meas+1][sensor][next_dir+next_ind_swp]-M2_dir;*/
                                                      //Obstacle position is now confirmed by several measurements from 3 sensors [direct measurements tend to be more precise]
                                                      Positions[pos_cnt][0] = Distances[meas-1][sensor-1][prev_dir];    //Distance from DUT1
                                                      Positions[pos_cnt][1] = M2_dir;                                   //Distance from DUT2
                                                      Positions[pos_cnt][2] = Distances[meas+1][sensor+1][next_dir];    //Distance from DUT3
                                                      pos_cnt++;
                                                      //Clear unconfirmed flags of direct measurements - only for plotting purposes
                                                      Unconfirmed[meas-1][sensor-1][prev_dir] = 0;
                                                      Unconfirmed[meas][sensor][curr_dir]   = 0;
                                                      Unconfirmed[meas+1][meas+1][next_dir] = 0;
                                                      //qDebug() << "confirmation finished";
                                                      //qDebug() << "-%-%-%-%-%-%-%-%-%-%-";
                                                      break;
                                                  }
                                               }
                                           }
                                       }
                                   }
                               }
                           }
                       }
                    }
                }
            }
        }
        else
        {
        /*********************************************************/
        //Algorithm utilizing only 2 sensors
            /******** Obstacle confirmation algorithm ********/
            for(int meas = 0; meas<MEASUREMENTS-1; meas++)                      //Measurement 1-3
            {
                for(int dir = 0; dir < 4; dir = dir+2 )                         //Direct 1 / Direct 2
                {
                    double prev_dir = Distances[meas][meas][dir];               //Save direct to be compared from sensor X

                    for(int indir_swp = 0; indir_swp <2; indir_swp++)           //Compare indirect measured by sensor Y in the current measurement
                    {                                                               //[only indirects calculated by subtraction of saved direct]
                        for(int dir_swp = 0; dir_swp <4; dir_swp = dir_swp+2)   //With direct measured by sensor Y in the next measurement
                        {                                                           //[both directs allowed]
                            if((qAbs(Distances[meas][meas+1][dir+indir_swp] - Distances[meas+1][meas+1][dir_swp])<= TOLERANCE) && (Unconfirmed[meas][meas+1][dir+indir_swp]==1))
                            {
                                //Theoreticaly one of the distances to obstacle has been confirmed
                                /*qDebug()<< "______Double conf. algorithm_______ ";
                                qDebug()<< "Possible first confirmation: Dir:" << Distances[meas+1][meas+1][dir_swp] << " Indir:" << Distances[meas][meas+1][dir+indir_swp] << " Delta:"<< (qAbs(Distances[meas][meas+1][dir+indir_swp] - Distances[meas+1][meas+1][dir_swp]));
                                qDebug()<< "Indexes: Dir >> Meas:" << meas+1+1 << " DUT:" << meas+1+1 << " TOF:" << dir_swp;
                                qDebug()<< "Indexes: Indir >> Meas:" << meas+1 << " DUT:" << meas+1+1 << " TOF:" << dir+indir_swp;
                                qDebug()<< "Continue...";*/
                                //Try to confirm second distance to the same obstacle
                                for(int next_indir_swp = 0; next_indir_swp < 2; next_indir_swp++)                       //Compare indirect measured by sensor X in the next measurement
                                {                                                                                           //[only indirects calculated by subtraction of previously confirmed direct with dir_swp]
                                                                                                                        //With the saved direct measured by sensor X
                                    if(qAbs(Distances[meas+1][meas][dir_swp+next_indir_swp] - prev_dir)<= TOLERANCE && (Unconfirmed[meas+1][meas][dir_swp+next_indir_swp]==1))
                                    {
                                        //It is now confirmed, that both direct measurements aim at the same obstacle
                                            //Positions[count][0=S1, 1=S2, 2=S3]
                                        Positions[pos_cnt][meas]    = prev_dir;                             //Distance from Sensor<1-3>
                                        Positions[pos_cnt][meas+1]  = Distances[meas+1][meas+1][dir_swp];   //Distance from Sensor<2-3>
                                        pos_cnt++;
                                        //Clear unconfirmed flags of direct measurements - only for plotting purposes
                                        Unconfirmed[meas][meas][dir] = 0;
                                        Unconfirmed[meas+1][meas+1][dir_swp] = 0;
                                        //Clear unconfirmed flags of indirect measurements - exclusion from further comparasions
                                        Unconfirmed[meas+1][meas][dir_swp+next_indir_swp] = 0;
                                        Unconfirmed[meas][meas+1][dir+indir_swp] = 0;
                                        /*qDebug()<< "Obstacle # confirmed:" << pos_cnt;
                                        qDebug()<< "Second confirmed distance: Dir:" << prev_dir << " Indir:" << Distances[meas+1][meas][dir_swp+next_indir_swp]<< " Delta:"<< (qAbs(Distances[meas+1][meas][dir_swp+next_indir_swp] - prev_dir));
                                        qDebug()<< "Indexes: Dir >> Meas:" << meas+1 << " DUT:" << meas+1 << " TOF:" << dir;
                                        qDebug()<< "Indexes: Indir >> Meas:" << meas+1+1 << " DUT:" << meas+1 << " TOF:" << dir_swp+next_indir_swp;
                                        qDebug()<< "Determined distances from object";
                                        qDebug()<< "DUT x" << Positions[pos_cnt-1][0] << "Dist x" << Positions[pos_cnt-1][1];
                                        qDebug()<< "DUT y" << Positions[pos_cnt-1][2] << "Dist y" << Positions[pos_cnt-1][3];
                                        qDebug()<< "************End of object confirmation************";*/
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /*************************************************/
        /*qDebug() << "position count" << pos_cnt;
        for(int m=0; m<pos_cnt; m++)
        {
            qDebug() << "_______________________";
            qDebug() << "Sensor 1 " << Positions[m][0];
            qDebug() << "Sensor 2 " << Positions[m][1];
            qDebug() << "Sensor 3 " << Positions[m][2];
        }*/
/**************************************************/
        //Plot obstacles and save results
        Triangulation::plotObstacles();
        return true;
    }
    return false;
}

/**************************************************
 *IsConstructable()
 *  Public function
 *  Determines if circles with given radials (input parameters r0,r1,r2)
 *  and known centres intersect in one point, which signalizes
 *  the presence of the obstacle.
 *  If there is no intersection, transfer distances to the Unconfirmed
 *  obstacles array.
 *  Resulting coordinates of the obstacle are added to vector (ObsX, ObsY)
 *  for easy plotting.
 */
void Triangulation::IsConstructable(double r0, double r1, double r2)
{
    //if(obs_cnt < 2) {                           //TEST PODMINKA OMEZUJICI MAX POCET OBJEKTU NA 2------------------------------------------------------------------------------------------
        double x0, y0;
        double r_p, r_n;
        double rx, ry;
        double pointP_x, pointP_y;
        double int_pos[3][2] = {0};                     //Positions of intersections between circles

        //Three triangulations
        for(int tri=0; tri<3; tri++) {
            double dx = 0;
            double dy = 0;
            switch(tri) {
                //Cycle 1 - intersection between S1 - S2
                case 0: x0 = Coordinates[0][0];
                        y0 = Coordinates[0][1];
                        dx = Coordinates[1][0] - x0;    //Distance on the X axis
                        dy = Coordinates[1][1] - y0;    //Distance on the Y axis
                        r_p = r0;
                        r_n = r1;
                        break;
                //Cycle 2 - intersection between S1 - S3
                case 1: x0 = Coordinates[0][0];
                        y0 = Coordinates[0][1];
                        dx = Coordinates[2][0] - x0;
                        dy = Coordinates[2][1] - y0;
                        r_p = r0;
                        r_n = r2;
                        break;
                //Cycle 3 - intersection between S2 - S3
                case 2: x0 = Coordinates[1][0];
                        y0 = Coordinates[1][1];
                        dx = Coordinates[2][0] - x0;
                        dy = Coordinates[2][1] - y0;
                        r_p = r1;
                        r_n = r2;
                        break;
                default: break;
            }
            //If one of the distances r0/r1/r2 is zero...
            if(r_p == 0 || r_n == 0) {
                //qDebug() << "Distance is zero!";
                int_pos[tri][0] = 0;
                int_pos[tri][1] = 0;
                continue;                                   //Go to the next iteration of the cycle
            }

            //Determine the distance between two sensors (centers)
            double l = qSqrt(qPow(dy, 2) + qPow(dx, 2));    //Straight line distance between the two sensors

            //Check for geometrical solvability
            if (l > (r_p + r_n) || l < qAbs(r_p - r_n)) {
                //|no intersection| OR |circles are inside each other| -> add to unconfirmed obstacles array
                //qDebug()<<"|no intersection| OR |circles are inside each other|";
                for(int i=0; i<3; i=i+2) {
                    if(Distances[0][0][i]==r0) {
                        Unconfirmed[0][0][i] = 1;
                    }
                    if(Distances[1][1][i]==r1) {
                        Unconfirmed[1][1][i] = 1;
                    }
                    if(Distances[2][2][i]==r2) {
                        Unconfirmed[2][2][i] = 1;
                    }
                }
                return;
            }
            //Intersection between the circles exists

            //Calculate the distance from Sensor1 to point P (foot of the altitude)
            double l1 = ((qPow(r_p, 2) - qPow(r_n, 2)) + qPow(l, 2)) / (2.0 * l);

            //Calculate the coordinates of point P
            pointP_x = x0 + (dx * l1/l);        //in third cycle (S2-S3) -> converted to offset from S1
            pointP_y = y0 + (dy * l1/l);        //in third cycle (S2-S3) -> converted to offset from S1

            //Calculate the distance from point P to intersection point
            double h = qSqrt(qPow(r_p, 2) - qPow(l1, 2));

            //Calculate the altitude of the intersection point from point P
            rx = -dy * (h/l);
            ry = dx * (h/l);

            //Determine the coordinates of intersection points
            double IP_x = pointP_x + rx;
            double IP_y = pointP_y + ry;
            double IP2_x = pointP_x - rx;
            double IP2_y = pointP_y - ry;

            if(IP_y >= 0 && IP_y < 999999 && qAbs(IP_x) < 999999)
            {
                int_pos[tri][0] = IP_x;
                int_pos[tri][1] = IP_y;
            }
            else if(IP2_y >= 0 && IP2_y < 999999 && qAbs(IP2_x) < 999999)
            {
                int_pos[tri][0] = IP2_x;
                int_pos[tri][1] = IP2_y;
            }
        }

        //After 'int_pos' has been filled - calculate centroid                          //ToDo - pokud je zvolen Double alg., budou dve z techto pozic nulove, nepocitat centroid.. lepsi by bylo nepocitat vubec tu trojitou triangulaci
        double cent_x = 0, cent_y = 0;
        int n = 0;
        for(int i=0; i<3; i++) {
            //qDebug() << "int_pos["<< i <<"][0]: " << int_pos[i][0] << " int_pos[" << i << "][1]: " << int_pos[i][1];
            if(int_pos[i][0] != 0 && int_pos[i][1] != 0) {
                cent_x = cent_x + int_pos[i][0];
                cent_y = cent_y + int_pos[i][1];
                n++;
            }
        }
        //Calculate the approximate coordinates of the obstacle
        cent_x = cent_x/n;
        cent_y = cent_y/n;
        //qDebug() << "Centroid n: " << n;
        //qDebug() << "Centroid X: " << cent_x << "Centroid Y: " << cent_y;

        //Evaluate if the coordinates of the newly calculated centroid are within tolerance band from the original distances r0, r1, r2
        for(int i=0; i<3; i++) {
            double r = 0;
            switch(i) {
                case 0: r = r0; break;
                case 1: r = r1; break;
                case 2: r = r2; break;
                default: break;
            }
            if(r > 0) {
                double dx = cent_x - Coordinates[i][0];
                double dy = cent_y - Coordinates[i][1];
                double d1 = qSqrt(qPow(dy, 2) + qPow(dx, 2));       //Vzdalenost mezi bodem a senzorem
                if(!(qAbs(d1 - r) < TOL_PRUSEC && cent_y >= 0 && cent_y < 999999 && qAbs(cent_x) < 999999)) {   //Pokud neni v toleranci
                    //qDebug() << "Neni v toleranci, d1=" << d1 << ", r=" << r << ", abs(d1-r)=" << qAbs(d1 - r) << ", TOL_PRUSEC=" << TOL_PRUSEC;
                    break;
                }
            }
            if(i==2) {                          //Cely cyklus overujici tolerance prosel uspesne
                ObsXtemp.append(cent_x);
                ObsYtemp.append(cent_y);

                if(AlgChoice==1)                //Double confirmation algorithm is selected
                {
                    if(r0==0 && r1!=0 && r2!=0) {
                        Obstacles[obs_cnt][0] = 2;      //DUT2
                        Obstacles[obs_cnt][1] = r1;     //Distance from DUT2
                        Obstacles[obs_cnt][2] = 3;      //DUT3
                        Obstacles[obs_cnt][3] = r2;     //Distance from DUT3
                    }
                    else {
                        Obstacles[obs_cnt][0] = 1;      //DUT1
                        Obstacles[obs_cnt][1] = r0;     //Distance from DUT1
                        Obstacles[obs_cnt][2] = 2;      //DUT2
                        Obstacles[obs_cnt][3] = r1;     //Distance from DUT2
                    }
                }
                else {
                    Obstacles[obs_cnt][0] = 1;      //DUT1
                    Obstacles[obs_cnt][1] = r0;     //Distance from DUT1
                    Obstacles[obs_cnt][2] = 2;      //DUT2
                    Obstacles[obs_cnt][3] = r1;     //Distance from DUT2
                    Obstacles[obs_cnt][4] = 3;      //DUT3
                    Obstacles[obs_cnt][5] = r2;     //Distance from DUT3
                }
                obs_cnt++;
            }
        }
    /*}
    else {                                                      //TEST PODMINKA OMEZUJICI MAX POCET OBJEKTU NA 2------------------------------------------------------------------------------------------
        for(int i=0; i<3; i=i+2) {
            if(Distances[0][0][i]==r0) {
                Unconfirmed[0][0][i] = 1;
            }
            if(Distances[1][1][i]==r1) {
                Unconfirmed[1][1][i] = 1;
            }
            if(Distances[2][2][i]==r2) {
                Unconfirmed[2][2][i] = 1;
            }
        }
    }*/
}

/**************************************************
 *plotObstacles()
 *  Public function
 *  Extracts data needed for graphical representation of obstacles from
 *  Positions array. Data is transformed to qVector and plotted -
 *  confirmed obstacles are represented as red dots while unconfirmed ones
 *  are represented as blue arc (size of which is also determined
 *  by beam pattern of used piezoelectric transducer)
 */
void Triangulation::plotObstacles()
{
    //Check if positions determined by algorithm are geometrically solvable (intersection of three circles)
    //result: qVector of coordinates
    for(int obs=0; obs<pos_cnt; obs++)
    {
        IsConstructable(Positions[obs][0],Positions[obs][1],Positions[obs][2]);
    }
    //qDebug() << "Confirmed object position - X:" << ObsXtemp << "Y:" << ObsYtemp;
    //qDebug() << "____";


    //Hleadni podobnych prekazek v historii
    if(mem_cnt == 9) {                                          //Az je naplnena pamet prekazek...
        for(int cf_obs=0; cf_obs<ObsXtemp.size(); cf_obs++) {   //Pro kazdou potvrzenou prekazku...
            int match_cnt = 0;                                  //Pocitadlo shod
            bool obs_written = 0;
            double Xavg = 0;
            double Yavg = 0;
            for(int mem_i=0; mem_i<5; mem_i++) {                //...se divej do pameti starsich prekazek zda tam byla podobna [pouze 5 starsich mereni]
                for(int mem_obs=0; mem_obs<v_measurements.value(mem_i).size(); mem_obs++) {
                    double tempX = v_measurements.value(mem_i).value(mem_obs).value(0);
                    double tempY = v_measurements.value(mem_i).value(mem_obs).value(1);
                    double currX = ObsXtemp.value(cf_obs);
                    double currY = ObsYtemp.value(cf_obs);
                    //Vypocet vzdalenosti mezi temito body (soucasne zmereny bod a bod z pameti)
                    double diff = qSqrt(qPow(qAbs(currX-tempX),2)+qPow(qAbs(currY-tempY),2));
                    if(diff < TOL_MEM) {                        //Pokud je rozdil v tolerancnim pasmu
                        match_cnt++;                            //Inkrementuj pocitadlo shod
                        Xavg = Xavg + tempX;
                        Yavg = Yavg + tempY;
                        if(match_cnt >= mem_match_thr && obs_written == 0) {     //Po tomto poctu shod ma byt prekazka vykreslena
                            Xavg = (Xavg + currX)/(match_cnt+1);
                            Yavg = (Yavg + currY)/(match_cnt+1);
                            //zapis souradnice teto prekazky do arraye urcene pro prekazky jenz budou vykreslovany
                            obs_written = 1;
//                            ObsX.append(ObsXtemp.value(cf_obs));
//                            ObsY.append(ObsYtemp.value(cf_obs));
                            ObsX.append(Xavg);
                            ObsY.append(Yavg);
                        }
                    }
                }
            }
        }
    }
    else {
        for(int i=0; i<ObsXtemp.size(); i++) {
            ObsX.append(ObsXtemp.value(i));
            ObsY.append(ObsYtemp.value(i));
        }
    }

    //qDebug() << "ObsX size (proslo shodou s pameti):" << ObsX.size();

    Triangulation::SaveObstacles();                                                         //save the confirmed obstacles to the history array (from ObsX/Ytemp)

    //Add Unconfirmed direct measurements to the Obstacles array
    for(int meas=0; meas<MEASUREMENTS; meas++)
    {
        for(int mode=0; mode<4; mode = mode+2)
        {
            if(Unconfirmed[meas][meas][mode] == 1)                                          //direct measurement was not confirmed
            {
                //meas = measurement when this DUT was in direct mode (also its row in distances table), mode =  0 (direct1 [0] or [1] in distances) or 1 (direct2 [2] or [3] in distances)
                Obstacles[obs_cnt][0] = meas+1;                                             //DUT
                Obstacles[obs_cnt][1] = Distances[meas][meas][mode == 0 ? 0 : 2 ];          //Distance from given DUT
                obs_cnt++;
                uncf_cnt++;
            }
        }
    }
    //qDebug() << "-- After unconfirmed obj. addition- obs_cnt:" << obs_cnt << ", uncf_cnt:" << uncf_cnt;

    while(qAbs(obs_cnt-uncf_cnt) > ObsX.size()) {
        obs_cnt--;
    }

    //qDebug() << "-- After memory compesantion - " << obs_cnt << ", uncf_cnt:" << uncf_cnt;

    ui->lb_obj1CF->setText("");
    ui->lb_obj2CF->setText("");
    ui->lb_obj1->setText("");
    ui->lb_obj2->setText("");
    ui->lb_obj3->setText("");
    ui->lb_obj4->setText("");
    ui->lb_obj5->setText("");
    ui->lb_obj6->setText("");
    ui->lb_obj1CF_2->setText("");
    ui->lb_obj2CF_2->setText("");
    ui->lb_obj3CF_2->setText("");
    ui->lb_obj4CF_2->setText("");
    ui->lb_obj1_2->setText("");
    ui->lb_obj2_2->setText("");
    ui->lb_obj3_2->setText("");
    ui->lb_obj4_2->setText("");
    ui->lb_obj5_2->setText("");
    ui->lb_obj6_2->setText("");

    if(AlgChoice == 0)          //Triple confirmation
    {
        //qDebug() << "________________Confirmed Obstacles_________________";
        for(int i=0; i<ObsY.size(); i++)        //confirmed obstacles
        {
            //qDebug() << "DUTx" << Obstacles[i][0] << "; Dist x:" <<  Obstacles[i][1] << "DUTy" <<  Obstacles[i][2] << "Dist y" <<  Obstacles[i][3] ;
            switch (i)
            {
                case 0: ui->lb_obj1CF->setText(QString("S%1: %2 cm\nS%3: %4 cm\nS%5: %6 cm\nv: %7 cm").arg(Obstacles[i][0]).arg(Obstacles[i][1],8,'f',1).arg(Obstacles[i][2]).arg(Obstacles[i][3],8,'f',1).arg(Obstacles[i][4]).arg(Obstacles[i][5],8,'f',1).arg(ObsY.at(i),10,'f',1)); break;
                case 1: ui->lb_obj2CF->setText(QString("S%1: %2 cm\nS%3: %4 cm\nS%5: %6 cm\nv: %7 cm").arg(Obstacles[i][0]).arg(Obstacles[i][1],8,'f',1).arg(Obstacles[i][2]).arg(Obstacles[i][3],8,'f',1).arg(Obstacles[i][4]).arg(Obstacles[i][5],8,'f',1).arg(ObsY.at(i),10,'f',1)); break;
                default: break;
            }
        }
        //qDebug() << "________________Unconfirmed Obstacles_________________";
        for(int i=0; i<uncf_cnt; i++)           //unconfirmed obstacles
        {
            //qDebug() << "DUTx" << Obstacles[ObsY.size()+i][0] << "; Dist x:" <<  Obstacles[ObsY.size()+i][1];
            switch(i)
            {
                case 0: ui->lb_obj1->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 1: ui->lb_obj2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 2: ui->lb_obj3->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 3: ui->lb_obj4->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 4: ui->lb_obj5->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 5: ui->lb_obj6->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                default: break;
            }
        }
        //qDebug() << "------------------------------------------";
    }
    else                        //Double confirmation
    {
        //qDebug() << "________________Confirmed Obstacles_________________";
        for(int i=0; i<ObsY.size(); i++)        //confirmed obstacles
        {
            //qDebug() << "DUTx" << Obstacles[i][0] << "; Dist x:" <<  Obstacles[i][1] << "DUTy" <<  Obstacles[i][2] << "Dist y" <<  Obstacles[i][3] ;
            switch (i)
            {
                case 0: ui->lb_obj1CF_2->setText(QString("S%1: %2 cm\nS%3: %4 cm\nv: %5 cm").arg(Obstacles[i][0]).arg(Obstacles[i][1],6,'f',1).arg(Obstacles[i][2]).arg(Obstacles[i][3],6,'f',1).arg(ObsY.at(i),8,'f',1)); break;
                case 1: ui->lb_obj2CF_2->setText(QString("S%1: %2 cm\nS%3: %4 cm\nv: %5 cm").arg(Obstacles[i][0]).arg(Obstacles[i][1],6,'f',1).arg(Obstacles[i][2]).arg(Obstacles[i][3],6,'f',1).arg(ObsY.at(i),8,'f',1)); break;
                case 2: ui->lb_obj3CF_2->setText(QString("S%1: %2 cm\nS%3: %4 cm\nv: %5 cm").arg(Obstacles[i][0]).arg(Obstacles[i][1],6,'f',1).arg(Obstacles[i][2]).arg(Obstacles[i][3],6,'f',1).arg(ObsY.at(i),8,'f',1)); break;
                case 3: ui->lb_obj4CF_2->setText(QString("S%1: %2 cm\nS%3: %4 cm\nv: %5 cm").arg(Obstacles[i][0]).arg(Obstacles[i][1],6,'f',1).arg(Obstacles[i][2]).arg(Obstacles[i][3],6,'f',1).arg(ObsY.at(i),8,'f',1)); break;
                default: break;
            }
        }
        //qDebug() << "________________Unconfirmed Obstacles_________________";
        for(int i=0; i<uncf_cnt; i++)           //unconfirmed obstacles
        {
            //qDebug() << "DUTx" << Obstacles[ObsY.size()+i][0] << "; Dist x:" <<  Obstacles[ObsY.size()+i][1];
            switch(i)
            {
                case 0: ui->lb_obj1_2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 1: ui->lb_obj2_2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 2: ui->lb_obj3_2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 3: ui->lb_obj4_2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 4: ui->lb_obj5_2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                case 5: ui->lb_obj6_2->setText(QString("S%1:%2 cm").arg(Obstacles[ObsY.size()+i][0]).arg(Obstacles[ObsY.size()+i][1],9,'f',1)); break;
                default: break;
            }
        }
        //qDebug() << "------------------------------------------";
    }
    //Graphical representation of sensor 1, 2 and 3
    QVector<double> k,l;

    for(int i=0; i<3; i++)
    {
        k.append(Coordinates[i][0]);
        l.append(Coordinates[i][1]);
    }
    ui->ptTriang->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->ptTriang->graph(0)->setScatterStyle(QCPGraph::ssSquare);    //only points
    ui->ptTriang->graph(0)->setData(k,l);
    ui->ptTriang->graph(1)->setLineStyle(QCPGraph::lsLine);         //only connecting line (thinner)
    ui->ptTriang->graph(1)->setScatterStyle(QCPGraph::ssNone);
    ui->ptTriang->graph(1)->setData(k,l);

    ui->ptTriang->yAxis->setRange(0, 400);
    ui->ptTriang->xAxis->setRange(-70, 150);
    //Representation of confirmed obstacles
    QPen GraphPen2;                                                 //obstacle pen
    GraphPen2.setColor(Qt::red);                                    //first detected obstacle is red
    GraphPen2.setWidth(12);
    QColor green_Col(0, 170, 0);                                    //custom green color
    QColor brown_Col(170, 85, 0);                                   //custom brown color
    QColor purple_Col(170, 85, 225);                                //custom purple color
    for(int i=2; i<obs_cnt-uncf_cnt+2; i++)                         //graphs 0-1 already exist (sensors)
    {
        ui->ptTriang->addGraph();                                   //add additional graph
        ui->ptTriang->graph(i)->setPen(GraphPen2);                  //set graph style
        ui->ptTriang->graph(i)->setScatterStyle(QCPGraph::ssDisc);
        ui->ptTriang->graph(i)->addData(ObsX.at(i-2),ObsY.at(i-2)); //add one obstacle as a point
        switch(i)
        {
            case 2: GraphPen2.setColor(green_Col); break;   //second detected obstacle will be green
            case 3: GraphPen2.setColor(brown_Col); break;   //third detected obstacle will be brown
            case 4: GraphPen2.setColor(purple_Col); break;  //fourth detected obstacle will be purple
            default: break;
        }
        if(ObsY.at(i-2)>=400 && AdAxisRng == 1)
            ui->ptTriang->yAxis->setRange(0, 600);                  //Confirmed object farther away than 4 meters
        if((ObsX.at(i-2)<=-70 && AdAxisRng == 1) || (ObsX.at(i-2)>=150 && AdAxisRng == 1))
            ui->ptTriang->xAxis->setRange(-140, 220);
    }

    /*qDebug() << "______________UNCONFIRMED______________";
    for(int i=0; i<MEASUREMENTS; i++)  //meas
    {
        qDebug() << "DUT" << i+1 << ": ToF1:" << Unconfirmed[i][i][0] << "ToF2:" << Unconfirmed[i][i][2];
    }
    qDebug() << "--------------------------";*/
    //Representation of unconfirmed distances
    for(int uncf=0; uncf<uncf_cnt; uncf++)
    {
        double DUTx, DUTy;
        int DUTnumber = Obstacles[obs_cnt-uncf_cnt+uncf][0];
        DUTx = Coordinates[DUTnumber-1][0];
        DUTy = Coordinates[DUTnumber-1][1];

        double semi_radius = Obstacles[obs_cnt-uncf_cnt+uncf][1];
        double x0, y0;                                                              //used to draw only part of the circle, limited by beam pattern of transducer
        y0 = qSin(((180-BEAM_PATTERN)/2)*M_PI/180)*semi_radius;
        x0 = qCos(((180-BEAM_PATTERN)/2)*M_PI/180)*semi_radius;

        QVector<double> x(50), y(50);
        for (int i=0; i<50; i++)
        {
           x[i] = i*(x0*2/50)+(DUTx-x0);                                           //x range <DUTx-x0, DUTx+x0>
           y[i] = qSqrt((semi_radius*semi_radius)-((x[i]-DUTx)*(x[i]-DUTx)))+DUTy; //y = semicircle
        }
        ui->ptTriang->addGraph();
        ui->ptTriang->graph(obs_cnt-uncf_cnt+uncf+2)->setScatterStyle(QCPGraph::ssNone);
        ui->ptTriang->graph(obs_cnt-uncf_cnt+uncf+2)->setData(x,y);
    }
    ui->ptTriang->xAxis->setSubGrid(true);
    ui->ptTriang->yAxis->setSubGrid(true);

    ui->ptTriang->xAxis->setRangeReversed(ProjSel);
    ui->ptTriang->yAxis->setRangeReversed(ProjSel);

    ui->ptTriang->replot();
    //qDebug() << "--MEASUREMENT EVALUATION DONE--";
}

/**************************************************
 *SaveObstacles()
 *  Public function
 *  Saves confirmed obstacles from the previous measurements
 *  to the list of last 10 measurements
 */
void Triangulation::SaveObstacles()
{
    //Confirmed obstacles are located in the ObsX and ObsY QVectors
    for(int lines=0; lines<ObsXtemp.size(); lines++) {
        v_coords.append(ObsXtemp.at(lines));
        v_coords.append(ObsYtemp.at(lines));
        v_obs.append(v_coords);
        v_coords.clear();
    }
    //Saving to the v_measurements vector
    if(mem_cnt>=0 && mem_cnt<9) {
        //Nemusi se posouvat, pouze se nasune dopredu
        v_measurements.prepend(v_obs);
        v_obs.clear();
        mem_cnt++;
        //qDebug() << "+1" << mem_cnt;
    }
    else                                    //10th meas. - delete last, shift to the right, insert first
    {
        //qDebug() << "MEM Overload" << v_measurements.size();
        v_measurements.resize(8);           //remove the oldest measurement from the end of array
        v_measurements.prepend(v_obs);
        v_obs.clear();
    }
    //qDebug() << v_measurements;

    //Testovaci debug vystup - vypsani pouze poslednich ulozenych udaju
    //qDebug() << "Number of obstacles detected in the last measurement:" << v_measurements.at(0).size();
    for(int i=0; i< v_measurements.at(0).size(); i++) {
      //qDebug() << "Last memory entry: X:" << v_measurements.value(0).value(i).value(0) << "; Y:" << v_measurements.value(0).value(i).value(1);
    }
}

/**************************************************
*on_cbAlgChoice_currentIndexChanged(int index)
*   Private slot
*   Makes sure that the Tab Widget containing measurement results is adequate to algorithm choice
*/
void Triangulation::on_cbAlgChoice_currentIndexChanged(int index)
{
    ui->tw_results->setCurrentIndex(index);
}

/**************************************************
 *saveSettings(double S2X, double S2Y, double S3X, double S3Y, double SOS, quint8 proj_sel)
 *  Public function
 *  Saves Advanced setup setting from Measurement window
 *  parameters are in [cm]
 */
void Triangulation::saveSettings(double S2X, double S2Y, double S3X, double S3Y, double SOS, quint8 proj_sel, bool AdAxRg, int MemTol, int MM_thr, int alg_tol, int prus_tol)
{
    //Sensor 2
    Coordinates[1][0] = S2X;
    Coordinates[1][1] = S2Y;
    //Sensor 3
    Coordinates[2][0] = S3X;
    Coordinates[2][1] = S3Y;
    //Speed of sound
    SpeedOfSound = SOS;
    //Projection settings
    ProjSel = proj_sel;
    AdAxisRng = AdAxRg;
    //Tolerances and algorithm parameters
    TOL_MEM = MemTol;
    TOL_PRUSEC = prus_tol;
    TOLERANCE = alg_tol;
    mem_match_thr = MM_thr; 
}

