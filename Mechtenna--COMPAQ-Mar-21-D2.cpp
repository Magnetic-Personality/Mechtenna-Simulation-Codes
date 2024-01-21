//#include <windows.h>
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <cmath>
//#include <iostream>
//#include <iomanip>
//#include "vector2.h"



//#include "stdafx.h"

//#include "stdafx.h"

#include <windows.h>
//#include "glut.h"
//#include "glext.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <complex>		// the function     printf("   ")     does not work if we don't include complex.h
//#include "vector2.h"
#include "vector2-long-double.h"			//   change on Dec 5, 2018 ....
									//	 use long double-Precision vector header file
#include <iomanip>
#include <fstream>


#define PI 3.1415926535898

//float angle,angle2;
int i,j;
//float circrad=400; int circpts=80;			
//float magnitude,deltatheta, F;
//float beta = 0.3;//0.15;//.05

//int nWinx=500 ; int nWiny=470 ;				// initial window size
//int nWinx=700 ; int nWiny=700; //660 ;
int nWinx=1050; int nWiny=700; //660 ;


float gnx = 40;
float gny = 40;
float gnz = 40;

float col;

void   field_pattern_display(void);
void   field_pattern_display_MAG03(void);
void   field_pattern_display_MAG03_xyz(void);
void   field_pattern_display_pancake (void);		// added on 3 Mar 2021
void   find_maxmin_1coil (void);

Vector v1,v2,v3,v4,v5;
Vector vc;
long double x,y,z;
long double x2,y2,z2;
long double x3,y3,z3;
long double aa=15;
long double bb=25.0;

long double Lx = .0254, Ly = .0254  ;     	// Electret Length, Width in metres
Vector meg_array_pos_UP [105][105] ;
Vector meg_array_pos_DOWN [105][105] ;

Vector obs_point ;   
Vector Calc_B_from_2by2_THICK_UPPER (Vector) ;
Vector Calc_B_from_2by2_THICK_LOWER (Vector) ;
//Vector Calc_B_from_2by2_THIN (Vector)  ;
//Vector Calc_B_from_2by2_THIN2 (Vector) ;
int nx = 5 , ny = 5 ;         		//nx = 100, ny = 100 ;	// number of grid squares x,y directions
	// change on 1 mar 2021
long double magnif;     // to go from metres to OpenGL model dimensionless coordinates
Vector array_velocity_UP [105];
Vector array_velocity_DOWN [105];
int x_row_num;
long double mu_0 = 4.0 * PI * 1.0E-7  ;  //  SI units, Henries per metre

long double Rect_obs_x1 = -.04;
long double Rect_obs_y1 = +.03;
long double Rect_obs_z = 0;
long double Rect_obs_x2 =  -.01;  //+.05;     //-.01;  //+.05;     //-.01;  // +.05;     // -.01;  //
long double Rect_obs_y2 =  -.05; // -.01; // -.03;     /-.01;   //-.03;     //+.01;  // +.05;     //+.01;  //

int nx_obs = 16 ;    //16;  // 48 ;    //16 , 
int	ny_obs = 16; // 8 ; //8 ;  //24 ; //8 ;
//int nx_obs = 16; 
//int ny_obs = 8 ;

long double Q_cell ;
Vector Field_Position [150][150]  ;
Vector Field_Value [150][150]  ;
long double Field_Max, Field_Min  ;
long double Field_xy_value ;
long double Field_xy_Max, Field_xy_Min  ;
long double Field_xz_Max, Field_xz_Min  ;
long double Field_xyz_Max  ;			// global variable added 3 Mar 2021
long double Field_xyz_Min  ;			// global variable added 3 Mar 2021
long double Max_1coil ;
long double Min_1coil  ;

//bool upper_and_lower = true ;
//bool ultrathin = false;	//  true; // false  ;
//bool ultrathin2 = false; //true; // false  ;
bool test_1a = 0;//true; //false;    //true ;  //
bool test_2a = 0;//true;    //true ;  //
bool Multiple_Angles = 0;//  1;// 1; //  0; //   true;// 22nov20 set true .. false; // true;  //  0;// 0;// 2jan2019changed  true;   //    0;		//0;     //   0;    // 1;  // 0;//  true ;
bool MAG03_display = 0;//1;//  true; //    false;   //22nov20 set false    true   true ;    //
bool sensor_textout = 0;   //true;

bool multiple_z1_gap = false; //  true; // false; // true; // false; //true;   //  false;  //  line added Jan 14, 2019   (remember Woodley Auguste)
			// if we have multiple z1 gap, we should ALSO have Multiple Angles = true ...
			// even though only 3 angles are covered.
double Array_z1_gap[40] ;
double X_multiple_z1_gap[40] ;
double Y_multiple_z1_gap[40] ;
double Z_multiple_z1_gap[40] ;
int num_z1_gaps = 11; //  4; //  11 ;

double fixed_angle = PI/5; //  PI/20;//  PI/2;//PI/0; //8; //    0;		//PI/8; //        PI/2   ; //   0;  // only use if we aren't in Multiple_Angle calc mode

long double h = 0.004 ;      //  .000250  ;         // electret height (thickess) in metres
	// long double	    h = .004; //  // h = .008; // .002 // .000250; // 0; //0.02725;// 0.02 ;//  0;   //  0.01;		//     .010  ;


//bool Plot_Fields_Multiple_Angles = 1;//   1;//false;	//	true; //false ;
//bool Plot_x_Multiple_Angles = false ;
//bool Plot_y_Multiple_Angles = false ;
//bool Plot_z_Multiple_Angles = false ;

//Vector a_vec  ;
long double omega ;

Vector Field_Angle_Loop [130] ; 
double Angle_Array[130] ;
double Max_Field_Angle_Loop	= 0	;
double Min_Field_Angle_Loop	= 0	;
int n_angles ;

double z1_gap =  0.005;  //  .125 ; // 5.5cm		//.005 ;		//    0.5cm 'gap' from Draper Presentation slide 20
		// change as we vary the distance to make something like slide 21/37 Draper
double y1_offset =	.01 ;      // 0;//   .01 ;      //    0;// 	// 1cm 'offset' from Draper Presentation slide 20
double narrow_coil_length = .018 ;		// from drawing, assume all 3 coils identical

double DW_y = .01355 ;	// Drawing (Bartington) "y_Bartington" sensor position
double DW_z = .029 ;	// Drawing (Bartington) "z_Bartington" sensor position
double DW_x = .044 ;	// Drawing (Bartington) "x_Bartington" sensor position


Vector Y_sensor_fields [30];
Vector Z_sensor_fields [30];
Vector X_sensor_fields [30];


double Y_sensor_avg_many_angles [30];
double Z_sensor_avg_many_angles [30];
double X_sensor_avg_many_angles [30];


Vector Y_sensor_position [30];
Vector Z_sensor_position [30];
Vector X_sensor_position [30];

Vector i_unit = Vector (1,0,0) ;     //  boldface i, unit vector in x-direction
Vector j_unit = Vector (0,1,0) ;     //  boldface i, unit vector in x-direction
Vector k_unit = Vector (0,0,1) ;     //  boldface i, unit vector in x-direction

int num_sensor_fields_minus_one = 10 ;


long double Y_sensor_avg ;
long double Z_sensor_avg ;
long double X_sensor_avg ;

long double Largest_B_component = 0 ;

void recalc_Y_sensor_avg(void);
void recalc_Z_sensor_avg(void);
void recalc_X_sensor_avg(void);

// ****** put the COIL variables here .......      // 1 Mar 2021
// ****** assume coil is on y-axis  

double turn_y_first = .0400   ;    //        metres!
double turn_y_last = .0800   ;    //  
int n_turns = 11   ;
double turn_rad = .018  ;    //
double turn_y [100] ;	// array for y-centre of the collection of turns

Vector granule [40][40][20]  ;		//  positions of sugar granules dotted upon the pancake
				//  points at which we will calculate the field
double granule_region [40] ; 		//  area of small region in which is located a granule
Vector granule_field [40][40][20] ;	//  field at each granule point

bool display_field_one_turn = true;	//
int turn_for_display = 7; //2; //     10; //3;		//
long double array_Flux [200] ;	// put in Early 6 March 2021
				// unit should be Webers
double array_moments [100] ;	// moments in time

void init(void)
{

	/*
GLfloat mat_specular[] = {1.0,0.0,0.0,1.0};
GLfloat mat_specular2[] = {1.0,0.0,0.0,1.0};
GLfloat mat_shininess[] = {50,0};
//GLfloat light_position[] = {50,50,300,1.0};// {100,100,300,1.0};
GLfloat light_position[] = {-550,550,2000,1.0};// {100,100,300,1.0};
GLfloat white_light[] = {1.0,1.0,1.0,1.0};
GLfloat lmodel_ambient[] = {0.2,0.2,0.2,1.0};
	
	glShadeModel(GL_SMOOTH);
glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS, mat_shininess);
glLightfv(GL_LIGHT0,GL_POSITION,light_position);
glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);
glEnable(GL_COLOR_MATERIAL);
	*/
	//gluLookAt(-500,-500,500,    0, 0, 100,    1,1,1);  change 12 nov 2018
	glClearColor(1,1,1,0);
//gluLookAt(  0,  0,10,    0, 0, 0,    0,1,0);    // /25nov2020  turn ON, line 202
//25nov2020  turn off	
gluLookAt(  -300 , -300 , 270,    0, 0, 0,    0,0,1); 		// line 232 off, 9mar2021
	// gluLookAt(  -300 , 0 , 0,    0, 0, 0,    0,0,1);
//	   gluLookAt(  0 , 0 , 300,    0, 0, 0,    0,1,0);
glEnable(GL_DEPTH_TEST);

int ii,jj,kk   ;
double DL_turns = turn_y_last  -  turn_y_first  ;
double dL_turns = DL_turns  /  (  n_turns - 1  )  ;

for (  ii=1  ;  ii  <=  n_turns  ;  ii++  )
{
	turn_y [ii] =  turn_y_first + dL_turns * ( ii - 1) ;
}


double px, py, pz , ptheta , pr ;
for (  ii=1  ;  ii  <=  20  ;  ii++  )
{
	for (  jj=1  ;  jj  <=  4  ;  jj++  )
	{
	ptheta =  (   2.0 * PI / 20.0   )   *   ( (double(ii) - 1 ) + 0.5 )  ;
	pr = turn_rad / 4.0 *  ( (double(jj) - 1 ) + 0.5 )  ;

		for (  kk=1  ;  kk  <=  (n_turns - 1)   ;   kk++  )
		{
		granule [ii] [jj] [kk].x   =   pr * cos(ptheta) ;
		granule [ii] [jj] [kk].y   =   turn_y [kk] ;
		granule [ii] [jj] [kk].z   =   pr * sin(ptheta) ;
		}

	}

}

double annulus_width = turn_rad  /  4.0  ;
double first_circrad, second_circrad  ;


		for (  jj=1  ;  jj  <=  4  ;  jj++  )
			{
			first_circrad      = (jj-1) * annulus_width ;		
			second_circrad =    jj   * annulus_width ;
			granule_region [jj] =     (	PI * second_circrad * second_circrad 
					        -     PI * first_circrad * first_circrad    )
						  *   1.0  /  20.0 ; 	
			std::cout << "region for radial index  "
			<< jj <<  " = " << granule_region [jj] << "\n" ;
			}


}	// end '   init()   '    function



void display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

int ii, jj, kk ;

if (Multiple_Angles == true)
		{
		std::exit(0) ;
		}


if (Multiple_Angles == false)
			{
//glutSolidSphere(100,10,10);
//glFlush();
// circle of flowing current
glColor3f(0.8,0.3,0.8);
glLineWidth(2);

/*
glBegin(GL_QUADS);
glVertex3f(0,-100/2,0);
glVertex3f(100,-100/2,0);
glVertex3f(100,+100/2,0);
glVertex3f(0,+100/2,0);
//glVertex3f(0,-100/2,0);
glEnd();
*/


//  ........ sensors for MAG03 instrument ........ 


/*	----- take this part out, 8 Mar 2021 -----
double hside = .0004;	//.0004		// half-side   ... not Halfling, sorry Bilbo
// sensor 'y'		... corrected annotation 3 Jan 2019
glColor3f(0.7,1.0,0.7);		// light green
glLineWidth(2);
glBegin(GL_LINE_LOOP);
glVertex3f(  (0 - z1_gap  -DW_y  - hside  ) * magnif,  (y1_offset -  narrow_coil_length/2) * magnif, 0) ;
glVertex3f(  (0 - z1_gap  -DW_y  - hside  ) * magnif,  (y1_offset +  narrow_coil_length/2) * magnif, 0) ;
glVertex3f(  (0 - z1_gap  -DW_y  +   hside  ) * magnif,  (y1_offset +  narrow_coil_length/2) * magnif, 0) ;
glVertex3f(  (0 - z1_gap  -DW_y  +   hside  ) * magnif,  (y1_offset -  narrow_coil_length/2) * magnif, 0) ;
glEnd();

glBegin(GL_LINE_LOOP);
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset -  hside) * magnif, -narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset +  hside) * magnif, -narrow_coil_length/2* magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset +  hside) * magnif, -narrow_coil_length/2* magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset -  hside) * magnif, -narrow_coil_length/2* magnif) ;
glEnd();

glBegin(GL_LINE_LOOP);
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset -  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset +  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset +  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset -  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glEnd();

glBegin(GL_LINES);
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset -  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset -  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset +  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  -  hside) * magnif,  (y1_offset +  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset +  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset +  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset -  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_y  +  hside) * magnif,  (y1_offset -  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glEnd();


// sensor 'z' ... corrected annotation 3 Jan 2019
//glColor3f(0  ,  .4  ,  0);		//  dark green
glColor3f( .7 , .7 , 1 );		// light blue
glBegin(GL_LINE_LOOP);
glVertex3f(  (0 - z1_gap  -DW_z  - narrow_coil_length/2  ) * magnif,  (y1_offset -  hside) * magnif, 0) ;
glVertex3f(  (0 - z1_gap  -DW_z  - narrow_coil_length/2) * magnif,  (y1_offset +  hside) * magnif, 0) ;
glVertex3f(  (0 - z1_gap  -DW_z  +   narrow_coil_length/2) * magnif,  (y1_offset +  hside) * magnif, 0) ;
glVertex3f(  (0 - z1_gap  -DW_z  +   narrow_coil_length/2) * magnif,  (y1_offset -  hside) * magnif, 0) ;
glEnd();

// sensor 'x' 
//hside = 0.002;
glColor3f( 1 , .7 , .7 );		// light red
glBegin(GL_LINE_LOOP);
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset -  hside) * magnif, -narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset +  hside) * magnif, -narrow_coil_length/2* magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset +  hside) * magnif, -narrow_coil_length/2* magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset -  hside) * magnif, -narrow_coil_length/2* magnif) ;
glEnd();

glBegin(GL_LINE_LOOP);
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset -  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset +  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset +  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset -  hside) * magnif,  narrow_coil_length/2 * magnif) ;
glEnd();

glBegin(GL_LINES);
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset -  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset -  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset +  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  -  hside) * magnif,  (y1_offset +  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset +  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset +  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset -  hside) * magnif, - narrow_coil_length/2 * magnif) ;
glVertex3f(  (0 - z1_gap  -DW_x  +  hside) * magnif,  (y1_offset -  hside) * magnif, + narrow_coil_length/2 * magnif) ;
glEnd();

*/

//  ........ observation rectangle ........ 

glColor3f( 1 , .7 , .7 );		// light red  
glBegin(GL_LINE_LOOP);
glVertex3f(  Rect_obs_x1 * magnif, Rect_obs_y1 * magnif, Rect_obs_z * magnif ) ;
glVertex3f(  Rect_obs_x2 * magnif, Rect_obs_y1 * magnif, Rect_obs_z * magnif) ;
glVertex3f(  Rect_obs_x2 * magnif, Rect_obs_y2 * magnif, Rect_obs_z * magnif );
glVertex3f(  Rect_obs_x1 * magnif, Rect_obs_y2 * magnif, Rect_obs_z * magnif );
glEnd();
  
//  ........ Electret in xy plane ........ 

glColor3f(1,0.7,.3);
glBegin(GL_LINE_LOOP);
glVertex2f(  0,  -Ly/2 * magnif) ;
glVertex2f(  Lx * magnif, -Ly/2 * magnif) ;
glVertex2f(  Lx * magnif, +Ly/2 * magnif) ;
glVertex2f(  0,  +Ly/2  * magnif) ;
glEnd();


// ........... lines along x,y,z directions .............

glColor3f(0.8,0.3,0.8);
glBegin(GL_LINES);
	glColor3f(1,0,0);
glVertex3f(0,0,0);
glVertex3f(160,0,0);
	glColor3f(0,1,0);
glVertex3f(0,0,0);
glVertex3f(0,160,0);
	glColor3f(0,0,1);
glVertex3f(0,0,0);
glVertex3f(0,0,160);
glEnd();


// .......... rectangle under (-z) y axis
/*
glBegin(GL_LINE_STRIP);
	glColor3f(1,1,0);
glVertex3f(0,0,0);
glVertex3f(0,60,0);
glVertex3f(0,60,-60);
glVertex3f(0,0,-60);
glVertex3f(0,0,0);
glEnd();
*/


//...........the box     what's that a reference to ? remember Chris Kuhl




glColor3f(.2 , .5 , .2);

glBegin(GL_LINE_LOOP);
		glVertex3f(0,(Ly/2 * cos(fixed_angle)   +   h/2 *cos(fixed_angle+PI/2) )* magnif,  (Ly/2 * sin(fixed_angle) +   h/2 *sin(fixed_angle + PI/2) ) * magnif );
		glVertex3f(0,(Ly/2 * cos(fixed_angle)  -   h/2 *cos(fixed_angle+PI/2) )* magnif,  (Ly/2 * sin(fixed_angle) -   h/2 *sin(fixed_angle + PI/2) )  * magnif );
		glVertex3f(0,(-Ly/2 * cos(fixed_angle)   -   h/2 *cos(fixed_angle+PI/2) )* magnif,(-Ly/2 * sin(fixed_angle) -   h/2 *sin(fixed_angle + PI/2) ) * magnif );
		glVertex3f(0,(-Ly/2 * cos(fixed_angle)   +   h/2 *cos(fixed_angle+PI/2) )* magnif,(-Ly/2 * sin(fixed_angle) +   h/2 *sin(fixed_angle + PI/2))  * magnif  );
glEnd();		  
	glBegin(GL_LINE_LOOP);
		glVertex3f(Lx* magnif,(Ly/2 * cos(fixed_angle)   +   h/2 *cos(fixed_angle+PI/2) )* magnif,  (Ly/2 * sin(fixed_angle) +   h/2 *sin(fixed_angle + PI/2) ) * magnif );
		glVertex3f(Lx* magnif,(Ly/2 * cos(fixed_angle)  -   h/2 *cos(fixed_angle+PI/2) )* magnif,  (Ly/2 * sin(fixed_angle) -   h/2 *sin(fixed_angle + PI/2) )  * magnif );
		glVertex3f(Lx* magnif,(-Ly/2 * cos(fixed_angle)   -   h/2 *cos(fixed_angle+PI/2) )* magnif,(-Ly/2 * sin(fixed_angle) -   h/2 *sin(fixed_angle + PI/2) ) * magnif );
		glVertex3f(Lx* magnif,(-Ly/2 * cos(fixed_angle)   +   h/2 *cos(fixed_angle+PI/2) )* magnif,(-Ly/2 * sin(fixed_angle) +   h/2 *sin(fixed_angle + PI/2))  * magnif  );
glEnd();
			glBegin(GL_LINES);
				glVertex3f(0,Ly/2 * cos(fixed_angle) * magnif  +   h/2 *cos(fixed_angle+PI/2) * magnif,Ly/2 * sin(fixed_angle) * magnif+   h/2 *sin(fixed_angle + PI/2) * magnif);
				glVertex3f(Lx* magnif,Ly/2 * cos(fixed_angle) * magnif  +   h/2 *cos(fixed_angle+PI/2) * magnif,Ly/2 * sin(fixed_angle)* magnif +   h/2 *sin(fixed_angle + PI/2) * magnif);
				glVertex3f(0,Ly/2 * cos(fixed_angle) * magnif -   h/2 *cos(fixed_angle+PI/2)* magnif ,Ly/2 * sin(fixed_angle) * magnif-   h/2 *sin(fixed_angle + PI/2) * magnif);
				glVertex3f(Lx* magnif,Ly/2 * cos(fixed_angle)* magnif  -   h/2 *cos(fixed_angle+PI/2) * magnif,Ly/2 * sin(fixed_angle)* magnif -   h/2 *sin(fixed_angle + PI/2) * magnif);
				
				glVertex3f(0,-Ly/2 * cos(fixed_angle) * magnif  -   h/2 *cos(fixed_angle+PI/2) * magnif,-Ly/2 * sin(fixed_angle)* magnif -   h/2 *sin(fixed_angle + PI/2) * magnif);
				glVertex3f(Lx* magnif,-Ly/2 * cos(fixed_angle)* magnif   -   h/2 *cos(fixed_angle+PI/2) * magnif,-Ly/2 * sin(fixed_angle)* magnif -   h/2 *sin(fixed_angle + PI/2) * magnif);
				
				glVertex3f(0,-Ly/2 * cos(fixed_angle) * magnif  +   h/2 *cos(fixed_angle+PI/2)* magnif ,-Ly/2 * sin(fixed_angle) * magnif+   h/2 *sin(fixed_angle + PI/2) * magnif);
				glVertex3f(Lx* magnif,-Ly/2 * cos(fixed_angle) * magnif  +   h/2 *cos(fixed_angle+PI/2) * magnif,-Ly/2 * sin(fixed_angle)* magnif +   h/2 *sin(fixed_angle + PI/2)* magnif );
			
				glEnd();
		

// -------------------------------------Small Squares for every 10th Cell Centre
// -------------------------------------change on 1 Mar 2021, EVERY Cell Centre

glColor3f(0.5,0.5,0.5);
glPointSize(4) ; 
glBegin(GL_POINTS);

float px,py,pz;

//  for (ii = 5 ; ii <= 100 ; ii+=10 )           //  x direction
    for (ii = 1 ; ii <= nx ; ii+=1 )              //  x direction
	{
		for (jj = 1; jj <= ny ; jj+=1 )	//  y direction
		{
		

		px = magnif  *  meg_array_pos_DOWN[ii][jj]  .x ;
		py = magnif  *  meg_array_pos_DOWN[ii][jj]  .y ;
		pz = magnif  *  meg_array_pos_DOWN[ii][jj]  .z ;
		glColor3f(0,0,1);			// ... BLUE for lower face of electret
			glVertex3f(px,py,pz);	

		px = magnif  *  meg_array_pos_UP[ii][jj]  .x ;
		py = magnif  *  meg_array_pos_UP[ii][jj]  .y ;
		pz = magnif  *  meg_array_pos_UP[ii][jj]  .z ;
		glColor3f(0.5,0.5,0.5);			// ... "50 shades of grey" for the upper face of electret
			glVertex3f(px,py,pz);		
		}
		
	}
glEnd();


// -----------------points for granules on the pancake which is the 1st turn of the coil 

glBegin(GL_POINTS);

for (kk = 1   ;   kk <= n_turns   ;   kk++ )              //  
	{
	if (      display_field_one_turn == true     &&     kk   !=   turn_for_display      )  continue;
		//  this means skip the following lines (within loop) for 
		//  the ik-loop and "continue" the ik loop using the 
		//  NEXT index value  
  for (ii = 1 ; ii <= 20 ; ii++ )              // 
	{
		for (jj = 1; jj <= 4 ; jj++ )	// 
		{
		
		px = magnif  *  granule [ii] [jj] [kk]  .x ;
		py = magnif  *  granule [ii] [jj] [kk]  .y ;
		pz = magnif  *  granule [ii] [jj] [kk]  .z ;
		glColor3f(0,0,0);			// ... BLUE for lower face of electret
			glVertex3f(px,py,pz);	
		}
		
	}
	}
glEnd();





//   std::cout << "meg_array_pos_UP[5][5]  " <<  
//				meg_array_pos_UP[5][5].x << " " << meg_array_pos_UP[5][5].y << " " << 
//				meg_array_pos_UP[5][5].z << " "  <<  '\n'  ;

std::cout << "meg_array_pos_UP .x  " <<  "\n"  ;


  	for (ii = 1 ; ii <= 7 ; ii++ )             
	{
		for (jj = 1; jj <= 7 ; jj++ )	
		{
				std::cout << meg_array_pos_UP[ii][jj].x << "  "  ;
		}		
		std::cout << ":n" << "\n" ;
	}



// --------------------------------velocity vectors-----------------

glBegin(GL_LINES)  ;

// for (jj = 5 ; jj <= 100 ; jj+= 10 )   
    for (jj = 1 ; jj <= ny ; jj+= 1 )   

	{
//	std::cout << "array_velocity_UP " << jj << " " 
//		<< array_velocity_UP [jj].x << " " 
//		<< array_velocity_UP [jj].y << " " 
//		<< array_velocity_UP [jj].z << " " << '\n' ;
	
	px = magnif  *  meg_array_pos_UP[1][jj]  .x ;   // was [x_row_num][jj]
	py = magnif  *  meg_array_pos_UP[1][jj]  .y ;   // was [x_row_num][jj]
	pz = magnif  *  meg_array_pos_UP[1][jj]  .z ;   // was [x_row_num][jj]

	glColor3f(.7,.7,0) ;		// yellow ... no,  .7 .7 0 is murky green
			glVertex3f( magnif  *  meg_array_pos_UP[1][jj]  .x , 0 , 0 );   // changed 14-dec-2018
			glVertex3f(px,py,pz);
	glColor3f( 0.8 , 0.8 , 0.8) ;	// a shade of grey DARKER for velocity vector
			glVertex3f(px,py,pz);	
			glVertex3f(
	     px + array_velocity_UP [jj]  . x       * 10.0,  
	     py + array_velocity_UP [jj]  . y       * 10.0,
	     pz + array_velocity_UP [jj]  . z       * 10.0
		)   ;
	//  done with upper face yz-position and velocity vectors

	glColor3f(0,0,.37) ;		// shade of blue DARKER for velocity vector for lower face of electret

	px = magnif  *  meg_array_pos_DOWN[1][jj]  .x ;		// was [x_row_num][jj]
	py = magnif  *  meg_array_pos_DOWN[1][jj]  .y ;		// was [x_row_num][jj]
	pz = magnif  *  meg_array_pos_DOWN[1][jj]  .z ;		// was [x_row_num][jj]
			glVertex3f(px,py,pz);	
			glVertex3f(
	     px + array_velocity_DOWN [jj]  . x       * 10.0,
	     py + array_velocity_DOWN [jj]  . y       * 10.0,
	     pz + array_velocity_DOWN [jj]  . z       * 10.0
		)   ;
	}
glEnd()  ;



// -------------------------------------Small Squares Field Display Points
 
 
glColor3f(0.5,0.5,0.5);
glPointSize(4) ; 
glColor3f(0,0,1);
glBegin(GL_POINTS);


for (ii = 1 ; ii <= nx_obs ; ii++ )        //  x direction
	{
		for (jj = 1 ; jj <= ny_obs ; jj++ )	//  y direction
		{
		px = magnif  *  Field_Position[ii][jj]  .x ;
		py = magnif  *  Field_Position[ii][jj]  .y ;
		pz = magnif  *  Field_Position[ii][jj]  .z ;
		glVertex3f(px,py,pz);		
		}
		
	}
glEnd();


// -----------------------Display the TURNS of the COIL  

int qqe = 1; 
glColor3f(0.5,0.5,0);
glPointSize(4) ; 

glLineWidth(3);

int nsincos = 40;


for (qqe = 1   ;   qqe   <=   n_turns   ;   qqe++ )       // no semicolon after FOR loop!
{

glBegin(GL_LINE_LOOP);
for (ii = 0 ; ii < nsincos ; ii++ )       
	{
		px = magnif  * std::cos ( 2.0*PI / nsincos * double(ii) ) * turn_rad  ; 
		py = magnif  * turn_y[ qqe ];
		pz = magnif  * std::sin ( 2.0*PI / nsincos * double(ii) ) * turn_rad ;
		glVertex3f(px,py,pz);			
if (ii==0) {std::cout << " px py pz " << px << " " << py << " " << pz << "\n" ;}
	}
glEnd();
glFlush();
glFinish();
}

glColor3f(0,0,0);
glBegin(GL_LINES);
	glVertex3f(0,  .0254*magnif, 0);
	glVertex3f(.0254*magnif   ,   .0254*magnif, 0);
glEnd();


// -----------------Display B-field vectors----------------

if  ( MAG03_display == false )   
			{	
				field_pattern_display() ;	
				field_pattern_display_pancake () ;
			}
if  ( MAG03_display == true )   
	{
	//field_pattern_display_MAG03() ;	
		field_pattern_display_MAG03_xyz() ;	

	glPointSize(3) ; 
	glColor3f(  .70  ,  .078  , 0 );
	glBegin(GL_POINTS);


	for (ii = 0 ; ii <= num_sensor_fields_minus_one ; ii++ )        
		{
			px = magnif  *  Y_sensor_position[ii]  .x ;
			py = magnif  *  Y_sensor_position[ii]  .y ;
			pz = magnif  *  Y_sensor_position[ii]  .z ;	
				glVertex3f(px,py,pz);	
					std::cout << " ***** " << ii << " " 
		<< px << " " 
		<< py << " " 
		<< pz << " " << '\n' ;
			
			px = magnif  *  Z_sensor_position[ii]  .x ;
			py = magnif  *  Z_sensor_position[ii]  .y ;
			pz = magnif  *  Z_sensor_position[ii]  .z ;
				glVertex3f(px,py,pz);		
			px = magnif  *  X_sensor_position[ii]  .x ;
			py = magnif  *  X_sensor_position[ii]  .y ;
			pz = magnif  *  X_sensor_position[ii]  .z ;
				glVertex3f(px,py,pz);		
		}
	glEnd();



	} // end if MAG03_display == true  braces

//field_pattern_display_MAG03() ;
	 

/*
std::cout << "Field Max : " << Field_Max << '\n' ;
std::cout << "Field Min : " << Field_Min << '\n' ;
std::cout << "Field xy Max : " << Field_xy_Max << '\n' ;
std::cout << "Field xy Min : " << Field_xy_Min << '\n' ;
*/

// ----------------------------------------

//glColor3f(1.0,0.0,1.0);
//glEnable(GL_COLOR_MATERIAL);


std::cout << "\n" ; 
std::cout << "Some Field Values::  33,32,31   x y z components : " << '\n' ; 
std::cout  <<  Field_Value[3][3].x  << " " <<  Field_Value[3][3].y  <<  " "  <<  Field_Value[3][3].z << '\n' ;
std::cout  <<  Field_Value[3][2].x  << " " <<  Field_Value[3][2].y  <<  " "  <<  Field_Value[3][3].z << '\n' ;
std::cout  <<  Field_Value[3][1].x  << " " <<  Field_Value[3][1].y  <<  " "  <<  Field_Value[3][3].z << '\n' ;
			

	}			//  end bool Multiple_Angles IF == false braces
				//  ................ ................ ................ 
				//  ................ ................ ................ 
			 
	if (Multiple_Angles == true)  
		{
			glColor3f(0,0,0);
	//		glLineWidth(1);			// added 9 Jan 2019
			glBegin(GL_LINES);
				glVertex3f(-15,0,0);
				glVertex3f(130,0,0);
					glVertex3f(0,-15,0);
					glVertex3f(0,140,0);
						glVertex3f(0,100,0);		// line to mark y=100 
						glVertex3f(-15,100,0);
			glEnd();
				
				glBegin(GL_LINES);
				for (ii = 0 ; ii < n_angles ; ii++ )	//  y direction
					{		
						glVertex3f(  ii * 100 / (n_angles-1) , -5 , 0);		
						glVertex3f(  ii * 100 / (n_angles-1) , +5 , 0);		
					}
				glEnd();

long double Field_Component_Max = 0 ;  // made it "long" on 22nov2022 ... gave it VIAGRA!
				for (ii = 0 ; ii < n_angles ; ii++ )	//  y direction
					{
std::cout << fabs(Y_sensor_avg_many_angles[ii] )   << "\n" ;
					if ( fabs(Y_sensor_avg_many_angles[ii] ) > Field_Component_Max) Field_Component_Max = fabs(Y_sensor_avg_many_angles[ii] )  ;
					if ( fabs(Z_sensor_avg_many_angles[ii] ) > Field_Component_Max) Field_Component_Max = fabs(Z_sensor_avg_many_angles[ii] )  ;
					if ( fabs(X_sensor_avg_many_angles[ii] ) > Field_Component_Max) Field_Component_Max = fabs(X_sensor_avg_many_angles[ii] )  ;
					}

//  *****  	22 Nov 2020 change it to FABS .... seems necessary in Borland on Compaq **** 
//  *****   Fabulous !


		if (Field_Component_Max == 0) 
		{
			std::cout << "MAX FIELD COMPONENT IS ZERO" << '\n' ;
			std::cin >> ii ;
			std::cout << abs(ii) ;    // added 22 nov 2020
			std::exit(0) ;
		}
				glLineWidth(2);
				glColor3f(0,0,1);   // blue for Z_sensor_field_component
				glBegin(GL_LINE_STRIP);
				for (ii = 0 ; ii < n_angles ; ii++ )	
					{		
						glVertex2f(  ii * 100 / (n_angles-1) , Z_sensor_avg_many_angles[ii] / Field_Component_Max * 100);				
					}
				glEnd();

			glColor3f(0,1,0);   // green for Y_sensor_field_component
				glBegin(GL_LINE_STRIP);
				for (ii = 0 ; ii < n_angles ; ii++ )	
					{		
						glVertex2f(  ii * 100 / (n_angles-1) , Y_sensor_avg_many_angles[ii]  / Field_Component_Max * 100);				
					}
				glEnd();

			glColor3f(1,0,0);   // red for X_sensor_field_component 
				glBegin(GL_LINE_STRIP);
				for (ii = 0 ; ii < n_angles ; ii++ )	
					{		
						glVertex2f(  ii * 100 / (n_angles-1) , X_sensor_avg_many_angles[ii]   / Field_Component_Max * 100);				
					}
				glEnd();
					
		std::cout << "FIELD COMPONENTS PLOTTED" << '\n' ;
		std::cout << "MAX FIELD COMPONENT IS " << '\n' ;
		std::cout << Field_Component_Max << " tesla " << '\n' ;
	//	std::cout << "for observation point (xyz) = " <<  '\n' ; 
	//	std::cout << Field_Position[1][1].x << " " << Field_Position[1][1].y << " " << Field_Position[1][1].z << '\n' ; 
		
		}		
		// .............. end if "for multiple angles"

		std::cout<<  "Largest_B_component (without averaging at each sensor) "  << Largest_B_component << '\n' ;

glFinish();

}
 

void reshape(int w, int h)
{
glViewport(0,0,(GLsizei) w,(GLsizei) h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
//glOrtho(0,w,h,0,-1.0,1.0);	//left right bottom top near far
//glFrustum(-50,50,-40,40, 50, 150);//left right bottom top near far
//glOrtho(-150,150,-150,150, 0, 3000);

//glOrtho(-180,180,-180,180, 0, 3000);		// change 12 nov 2018
//glOrtho(-280,280,-280,280, 0, 300);
//glOrtho(-110,110,-110,110, 0, 650);

//    was -170 170 -170 170
//glOrtho(-210,210,-210,210, 0, 1250);   // far was 650
  glOrtho(-315,315,-210,210, 0, 1250);   // far was 650

//glFrustum(-300,300,-300,300, 800,1200);
glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
}


int main(int argc, char** argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(nWinx,nWiny);
glutInitWindowPosition(100,100);
glutCreateWindow(argv[0]);
init();

// ******* code to calculate Mechtenna field goes here *******


// note 2.54 cm = 1 inch



magnif = 100 / Lx;			// magnif is a global variable
							// Lx is x-extent of the electret in metres

//   Lx = 100, Ly = 100 ;		//********** FOR MAKING DISPLAY EASIER **********
//				//********** make Lx, Ly 100 units

long double Lx2 = Lx / nx ;
long double Ly2 = Ly / ny ;

long double angle;    //  = PI/ 6 ;
//  long double time_t = 0 ;
omega = 2.0 * PI * 50; //  166; //     2.0 * PI * 100 ;		// angular frequency
		// 100 cycles per second initially then 166 cycles per second
//angle = time_t  *  omega ;
//angle = PI/2 ;   //    0; // PI/2; //  0.157 ;       //0 ; // PI/2 ; // PI/2 ;  //   PI / 6  + 0.2 ;

long double Q_upper = 50E-9 * 2.54*2.54 ;	//  Charge on upper face of Electret,  
Q_upper = 1.1E-9  *   2.54*2.54 ; 	// change on 11 Mar 2021, 
				// result of spreadsheet  Flux-and-Voltage-1.ods


						//  50 nanoCoulombs per cm2 * 2.54cm *2.54cm
//   Q_cell =  1.0; //  Q_upper / (nx * ny) ;		//  charge for just 1 cell, assume uniform distribution
	Q_cell = Q_upper / (nx * ny) ;		//  charge for just 1 grid cell, assume uniform distribution
					//  NOTE:  Positive charge is on UPPER FACE of electret		
//a_vec = sideT * h/2.0 ; 

int nn1, nn2;    // to go through grid-cells

//long double cell_x, cell_y, cell_z   ;
Vector Cell_centre_UP = Vector (0,0,0) ;   
Vector Cell_centre_DOWN = Vector (0,0,0) ;   
	// this is ORIGIN to point at centre of cell
long double r_from_axis ;       // distance from axis of rotation
long double speed ;
Vector yz_vec ;  
Vector vel_direc;
int tv_counter  ;		//  test velocity counter
x_row_num = 1;			//  row for calculation of velocity, was initially for testing
						
// ..... populate the Vector meg_array_pos_UP[][]  with vectors to grid cell centres
// ..... ALSO populate the 1D-array    array_velocity_UP[]    containing the velocity vectors
// ..... initially was used to TEST and DISPLAY the velocities but then we kept it

int nfx, nfy ;
long double step_x;
long double step_y;

n_angles = 48; //  13; //  6;//  25;// 6 ; 		// set to 48 on 6 mar 2021
int iia = 0 ;
long double B_flux ;



for (iia=0 ; iia < n_angles ; iia++ ) 
	{
//iia = 0; 
	angle = (2.0 * PI) / double(n_angles) * double(iia)  ;    // changed 6 mar 2021, PI becomes 2PI
	//   angle = PI/5		;//
	Angle_Array[iia] = angle ;

	if (Multiple_Angles == false) 
		{
		angle = fixed_angle;	
		}
				
	//angle =  0; 
	Vector sideA = Vector(  0 , cos(angle) , sin(angle)  )   ;			// unit vector along side A, function of ANGLE
	Vector sideT = Vector(  0 , cos(angle + PI/2.0) , sin(angle + PI/2.0)  )   ;	// unit vector along direction T, thickness

	tv_counter = 1;		// .... re-initalize the counter for the velocity vector array to 1 ....
for (nn1 = 1 ; nn1 <= nx ; nn1++ )        //  1 to 100 inclusive  (initially)
	{
		for (nn2 = 1 ; nn2 <= ny ; nn2++ )    // y-direction is the INNER LOOP
		{
		//	cell_x = (nn1-1) * Lx2 + 0.5 * Lx2   ; 
		//	cell_y = (-Ly/2.0 + (nn2-1) * Ly2 + 0.5 * Ly2)    ; 
		//	cell_z =

	 	Cell_centre_UP =      i_unit * (  (nn1-1) * Lx2 + 0.5 * Lx2  )
						+   sideA  *   (-Ly/2.0 + (nn2-1) * Ly2 + 0.5 * Ly2)
						+   sideT *  h/2.0 ;

		meg_array_pos_UP [nn1][nn2] = Cell_centre_UP  ;	

		Cell_centre_DOWN =      i_unit * (  (nn1-1) * Lx2 + 0.5 * Lx2  )
						+   sideA  *   (-Ly/2.0 + (nn2-1) * Ly2 + 0.5 * Ly2)
						-   sideT *  h/2.0 ;			// note SUBTRACTION along sideT

		meg_array_pos_DOWN [nn1][nn2] = Cell_centre_DOWN  ;	

	if (nn1 == x_row_num) 
		{
		// calculate velocity ..... use  |v| = r omega
		// first do for the UPPER face of the electret

		r_from_axis = std::sqrt (  
			std::pow(0.0 , 2)	+
			std::pow(Cell_centre_UP.y , 2) + 
			std::pow(Cell_centre_UP.z , 2)
				) ;
		speed = r_from_axis * omega  ;
		
		yz_vec.x = 0 ;
		yz_vec.y = Cell_centre_UP.y ;
		yz_vec.z = Cell_centre_UP.z ;

		vel_direc.x = 0;
		vel_direc.y = yz_vec.z   * (-1.0)  ;	//  changed 12 dec 2018, flip direction
		vel_direc.z = -yz_vec.y  * (-1.0)  ;	//  changed 12 dec 2018, flip direction

		vel_direc = vel_direc.Hat()  ;
		array_velocity_UP [tv_counter] = vel_direc;
		
		// just test the direction initially //
		// THEN multiply the unit vector by the speed

		array_velocity_UP [tv_counter]   = 
		array_velocity_UP [tv_counter]  *  speed ; 		
	
//	std::cout << "array_velocity_UP...Q " << tv_counter << " " 
//		<< array_velocity_UP [tv_counter].x << " " 
//		<< array_velocity_UP [tv_counter].y << " " 
//		<< array_velocity_UP [tv_counter].z << " " 
//		<< array_velocity_UP [tv_counter].Modulus() << " " << '\n' ;

//std::cout << 
//	   array_velocity_UP [tv_counter].x   *   Cell_centre_UP.x 
//	+ array_velocity_UP [tv_counter].y   *   Cell_centre_UP.y
//	+ array_velocity_UP [tv_counter].z   *   Cell_centre_UP.z
//	<< "\n" ;
						// next do for the LOWER face of the electret
						
						r_from_axis = std::sqrt (  
						std::pow(0.0 , 2)	+
						std::pow(Cell_centre_DOWN.y , 2) + 
						std::pow(Cell_centre_DOWN.z , 2)
							) ;
						speed = r_from_axis * omega  ;		// actually SPEED should be the same as for corresponding point on the UPPER face of the electret
		
						yz_vec.x = 0 ;
						yz_vec.y = Cell_centre_DOWN.y ;
						yz_vec.z = Cell_centre_DOWN.z ;

						vel_direc.x = 0;
						vel_direc.y = yz_vec.z   * (-1.0)  ;	//  changed 12 dec 2018, flip direction;
						vel_direc.z = -yz_vec.y  * (-1.0)  ;	//  changed 12 dec 2018, flip direction;

						vel_direc = vel_direc.Hat()  ;
						array_velocity_DOWN [tv_counter] = vel_direc;
		
						array_velocity_DOWN [tv_counter]   = 
						array_velocity_DOWN [tv_counter]  *  speed ; 		


		tv_counter++ ;
		}      	// end    (if    nn1 == x_row_num)

		
		}      	// end nn2 loop,,, nn2 for sideA, was along y-axis at angle=0
		//		if ( Multiple_Angles == true )  
		//		break ;    // break out of the nn1 loop
			// NO to above 2 lines ,,, if we're doing Multiple Angles we
			// still need Position and Velocity data for the full 100*100 grid

	}	// end nn1 loop,,, nn1 for x-direction (i_unit) at ANY angle value

// OK we have the position vector "mega-array",
// and the test velocity along the direction which was initially y-direction.
// .....  NEXT, we start in on the BIOT-SAVART FIELD calculation

//  dB = (mu_0 / 4 PI )   >>q<<    r_hat   CROSS   v       /  r2      (sign?)
//	page 138 of Grant-Phillips text... Biot-Savart Law given, page 129 right-hand screw rule
// there SHOULD be a (-) sign in the above equation actually
//  also floats change to long doubles in Vector header?
//  include dotproduct function?




Vector B_up_from_2by2;
Vector B_lo_from_2by2;

obs_point = Vector ( -.024 , .030, .01) ;
B_up_from_2by2 = Calc_B_from_2by2_THICK_UPPER(obs_point) ; 	// function call
B_lo_from_2by2 = Calc_B_from_2by2_THICK_LOWER(obs_point) ; 	// function call

//   std::cout << "B_up__total in teslas =  " << B_up_from_2by2.Modulus() <<  '\n'  ;
//   std::cout << "B_lo__total in teslas =  " << B_lo_from_2by2.Modulus() <<  '\n'  ;
//   std::cout << "for observation point x,y,z  =  " <<
//   			obs_point.x << " " << obs_point.y << " " << obs_point.z <<  '\n'  ;





// ............ TAKE THIS PART OUT for MAG03 program .................



// ............ ******************************************************
// ............ next, we calculate the FIELD in a 2x2 rectangular grid
// ............ located some distance AWAY FROM the rotating electret


 step_x = (Rect_obs_x2  -   Rect_obs_x1 ) / nx_obs ;
 step_y = (Rect_obs_y2  -   Rect_obs_y1 ) / ny_obs ;


//   Field_Position[3][3] . y = 3.4 ;

Field_Max = 0 ;		// global variable
Field_Min = 0 ;		// global variable
Field_xy_Max = 0 ;		// global variable
Field_xy_Min = 0 ;		// global variable
 
for (nfx = 1 ; nfx <= nx_obs ; nfx++)				//nfx =1;
	{
	for (nfy = 1 ; nfy <= ny_obs ; nfy++)
	{

// >>>>>>>>>>>>>>>>>>

	Field_Position [nfx][nfy]. x = Rect_obs_x1 + step_x * (nfx-1 ) + step_x * 0.5 ;
	Field_Position [nfx][nfy]. y = Rect_obs_y1 + step_y * (nfy-1 ) + step_y * 0.5 ;
	Field_Position [nfx][nfy]. z = Rect_obs_z  ;		

	obs_point = Field_Position [nfx][nfy] ;
//	std::cout << "FieldPos, nfx nfy " << nfx << " " << nfy << '\n' ;

			test_1a = false ;
			if (nfx ==1 && nfy ==1 ) 
				{
				// test_1a = true ;
				}
			
			B_up_from_2by2 = Calc_B_from_2by2_THICK_UPPER(obs_point) ; 	// function call
			B_lo_from_2by2 = Calc_B_from_2by2_THICK_LOWER(obs_point) ; 	// function call
			// std::cout << "fv " ;
			
	if (test_2a == true)
	{
			
			if		(nfx == 1 )     //  && nfy % 1 == 0
			{
				std::cout <<  '\n' << "test_2a:::: "  <<  '\n'  ;
			std::cout << "nfy = " << nfy <<  '\n'  ;
			std::cout << "B_up__xyz, || in teslas =  " <<  
				B_up_from_2by2.x << " " << B_up_from_2by2.y << " " << 
				B_up_from_2by2.z << " " << B_up_from_2by2.Modulus() <<  '\n'  ;
			std::cout << "B_lo__xyz, ||  in teslas =  " <<  
				B_lo_from_2by2.x << " " << B_lo_from_2by2.y << " " << 
				B_lo_from_2by2.z << " " << B_lo_from_2by2.Modulus() <<  '\n'  ;
			std::cout << "SUM___xyz, ||  in teslas =  " <<  
				B_lo_from_2by2.x + B_up_from_2by2.x << " " << B_lo_from_2by2.y + B_up_from_2by2.y << " " << 
				B_lo_from_2by2.z + B_up_from_2by2.z << " " << (B_lo_from_2by2 + B_up_from_2by2).Modulus() <<  '\n'  ;
			std::cout << "for observation point x,y,z  =  " <<
			obs_point.x << " " << obs_point.y << " " << obs_point.z <<  '\n'  ;

			
			}

	}


//std::cout << "enter a number : " ;
//int abj ;
//std::cin >> abj;
//std::exit(0);


	
	Field_Value[nfx][nfy] = B_up_from_2by2   + B_lo_from_2by2 ;
	Field_xy_value = sqrt( pow(Field_Value[nfx][nfy].x,2)  +  pow(Field_Value[nfx][nfy].y,2)  )   ;
	// do the MAX-MIN
	if (   Field_Value[nfx][nfy].Modulus() > Field_Max   )  Field_Max = Field_Value[nfx][nfy].Modulus()  ;
	if (   Field_xy_value > Field_xy_Max     )    Field_xy_Max  =  Field_xy_value  ;


	if (  nfx == 1  &&  nfy == 1  )
	{
		Field_Min = Field_Value[nfx][nfy].Modulus()  ;
		Field_xy_Min =  Field_xy_value     ;
	}

	if (  Field_Value[nfx][nfy].Modulus()   <   Field_Min ) 
	{
		Field_Min    =   Field_Value[nfx][nfy].Modulus()     ;
	}

	if (  Field_xy_value   <   Field_xy_Min ) 
	{
		Field_xy_Min    =   Field_xy_value     ;
	}


	if ( Multiple_Angles == true ) break;	// break out of the nfy loop when nfy=1
	}			// end nfy loop
	if ( Multiple_Angles == true ) break;	// break out of the nfx loop when nfx=1
	}			// end nfx loop


// ----------------------- here we have a similar pair of loops 
// ----------------------- this time for the PANCAKE, npolar 1 to 20, nradial 1 to 4

int ip,ir,ik ;  	// polar index, radial index

Field_xyz_Max = 0 ;		// global variable
Field_xyz_Min = 0 ;			// global variable
Field_xz_Max = 0 ;			// global variable
Field_xz_Min = 0;			// global variable
long double Field_xz_value ;

for (ik = 1 ; ik <= n_turns ; ik++)				
	{
for (ip = 1 ; ip <= 20 ; ip++)				//  ip = polar index
	{
	for (ir = 1 ; ir <= 4 ; ir++)
	{
	obs_point = granule [ip][ir][ik] ;
			
	B_up_from_2by2 = Calc_B_from_2by2_THICK_UPPER(obs_point) ; 	// function call
	B_lo_from_2by2 = Calc_B_from_2by2_THICK_LOWER(obs_point) ; 	// function call
			
	granule_field [ip][ir][ik] = B_up_from_2by2   +   B_lo_from_2by2 ;

	Field_xz_value = sqrt( pow(   granule_field [ip][ir][ik]  .x  ,  2   )  +  pow(  granule_field [ip][ir][ik]   .z  ,  2   )  )   ;
	// do the MAX-MIN
	if (   granule_field [ip][ir][ik].Modulus()   >   Field_xyz_Max   )  Field_xyz_Max = granule_field [ip][ir][ik].Modulus()    ;
	if (   Field_xz_value > Field_xz_Max     )    Field_xz_Max  =  Field_xz_value  ;


	if (  ip == 1  &&  ir == 1   && ik == 1 )
	{
		Field_xyz_Min =  granule_field [ip][ir][ik].Modulus()    ;
		Field_xz_Min = Field_xz_value   ;
	}

	if (  granule_field [ip][ir][ik].Modulus()   <   Field_xyz_Min ) 
	{
		Field_xyz_Min    =   granule_field [ip][ir][ik].Modulus()    ;
	}

	if (  Field_xz_value  <   Field_xz_Min ) 
	{
		Field_xz_Min    =   Field_xz_value   ;
	}

//	if ( Multiple_Angles == true ) break;	// break out of the ir loop when ir=1
	}			// end ir loop
//	if ( Multiple_Angles == true ) break;	// break out of the ip loop when ip=1
	}			// end ip loop
//	if ( Multiple_Angles == true ) break;	// break out of the ik loop when ik=1
	}			// end ik loop

//   	So,   granule_field [ ] [ ] [ ]  has now been calculated, and its max/min values 
// 	---------------------------------------------------------------------------------------------



	if ( Multiple_Angles == true )
			{
//			Field_Angle_Loop [iia] = Field_Value[1][1] ; 

//			here we put routine to calculate the total
//			MAGNETIC FLUX ......................................

			B_flux = 0;

			for (ik = 1 ; ik <= n_turns ; ik++)				
			{
			for (ip = 1 ; ip <= 20 ; ip++)		//  ip = polar index
			{
			for (ir = 1 ; ir <= 4 ; ir++)
			{		
			B_flux   +=   granule_field [ip][ir][ik] .y   *   granule_region [ir] ;
			}
			}
			}
			array_Flux [iia] = B_flux ;

			}
	
	

	//  ************* HERE we put the routine for ....... IF MAG03 == true .......
	
	if (MAG03_display == true)	
	{
	int iiq;	// loop index variable

				// first we place the LOCATIONS of the points within the sensor narrow coils .....

				for (iiq = 0   ;   iiq <= 10   ;   iiq++   )  // 11 not 10 values
					{
//					Y_sensor_position[iiq] =  i_unit * (-1.0 * ( z1_gap + DW_y) )
//											+ j_unit * y1_offset 
//											+ k_unit * (+1.0 * narrow_coil_length/2)
//											+ k_unit * (-1.0) *
//											(  narrow_coil_length / num_sensor_fields_minus_one  )  *  iiq
//											;		
					// changed 3 Jan 2019
					Y_sensor_position[iiq] =  i_unit * (-1.0 * ( z1_gap + DW_y ) )
											+ j_unit * (y1_offset + narrow_coil_length/2 )
											+ k_unit * 0.0
											+ j_unit * (-1.0) *
											(  narrow_coil_length / num_sensor_fields_minus_one  )  *  iiq
											;	


					obs_point = Y_sensor_position[iiq] ;
						B_up_from_2by2 = Calc_B_from_2by2_THICK_UPPER(obs_point) ; 	// function call
						B_lo_from_2by2 =  Calc_B_from_2by2_THICK_LOWER(obs_point) ; 	// function call
									//  Vector (0,0,0);   //  17jan2019 
					Y_sensor_fields[iiq] = B_up_from_2by2 + B_lo_from_2by2 ;			// 
						if (abs(Y_sensor_fields[iiq].y) > Largest_B_component  	) Largest_B_component = abs(Y_sensor_fields[iiq].y)  ;  // 3jan2019... changed .z to .y (component change)
					}	
				for (iiq = 0   ;   iiq <= 10   ;   iiq++   )  // 11 not 10 values
					{
					Z_sensor_position[iiq] =  i_unit * (-1.0 * ( z1_gap + DW_z + narrow_coil_length/2) )
											+ j_unit * y1_offset 
											+ k_unit * 0.0
											+ i_unit * 
											(  narrow_coil_length / num_sensor_fields_minus_one  )  *  iiq
											;
					obs_point = Z_sensor_position[iiq] ;
						B_up_from_2by2 = Calc_B_from_2by2_THICK_UPPER(obs_point) ; 	// function call
						B_lo_from_2by2 = 		 Calc_B_from_2by2_THICK_LOWER(obs_point) ; 	// function call
									//    Vector (0,0,0);   //  17jan2019
						Z_sensor_fields[iiq] = B_up_from_2by2 + B_lo_from_2by2 ;			// 
						if (abs(Z_sensor_fields[iiq].x) > Largest_B_component  	) Largest_B_component = abs(Z_sensor_fields[iiq].x)  ;

					}	
				for (iiq = 0   ;   iiq <= 10   ;   iiq++   )  // 11 not 10 values
					{
								X_sensor_position[iiq] =  i_unit * (-1.0 * ( z1_gap + DW_x) )
											+ j_unit * y1_offset 
											+ k_unit * (+1.0 * narrow_coil_length/2)
											+ k_unit * (-1.0) *
											(  narrow_coil_length / num_sensor_fields_minus_one  )  *  iiq
											;		
			
					obs_point = X_sensor_position[iiq] ;
						B_up_from_2by2 = Calc_B_from_2by2_THICK_UPPER(obs_point) ; 	// function call
						B_lo_from_2by2 = Calc_B_from_2by2_THICK_LOWER(obs_point) ; 	// function call
								//   Vector (0,0,0);   //  17jan2019      
					X_sensor_fields[iiq] = B_up_from_2by2 + B_lo_from_2by2 ;			// 
						if (abs(X_sensor_fields[iiq].z) > Largest_B_component  	) Largest_B_component = abs(X_sensor_fields[iiq].z)  ;    //3jan19  .y changed to .z

					}			// put y1_offset |!|...done 
		std::cout << " %%%%% " << 0 << " " 
		<< Z_sensor_position[0].x << " " 
		<< Z_sensor_position[0].y << " " 
		<< Z_sensor_position[0].z << " " << '\n' ;
		
		recalc_Y_sensor_avg();
		recalc_Z_sensor_avg();
		recalc_X_sensor_avg();
	
		Y_sensor_avg_many_angles[iia] = Y_sensor_avg;
		Z_sensor_avg_many_angles[iia] = Z_sensor_avg;
		X_sensor_avg_many_angles[iia] = X_sensor_avg;

	}		//	end MAG03_display == true braces



	if (Multiple_Angles == false ) break;	// break out of iia loop if we only are doing 1 angle

	}	// end iia loop  which started around line 490



// ............. Console Output of field values for the set of Angles

//		if (Multiple_Angles == true)
//			{
//			std::cout << '\n' ;
//
//				for (int ii = 0 ; ii < n_angles ; ii++)
//				{	
//				std::cout << "ii: " << ii << "  Angle: " << Angle_Array[ii] << "  Sensor YZX: " << 
//				Y_sensor_avg_many_angles [ii] << " " << 
//				Z_sensor_avg_many_angles [ii] << " " << 
//				X_sensor_avg_many_angles [ii] << " " << 
//				'\n' ;
//				}
//			}	

// >>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>> Next, work out the MAGNETIC FLUX through the set of Parallel Pancakes


B_flux = 0 ;
int ip,ir,ik ;

for (ik = 1 ; ik <= n_turns ; ik++)				
	{
for (ip = 1 ; ip <= 20 ; ip++)				//  ip = polar index
	{
for (ir = 1 ; ir <= 4 ; ir++)
	{		
	B_flux   +=   granule_field [ip][ir][ik] .y   *   granule_region [ir] ;
	}
	}
	}

std::cout << ">>> - - - - - - - <<<" << '\n'  ;
std::cout << "Magnetic Flux through all " << n_turns 
	<< " turns =  " << B_flux << " Webers " << '\n'  ;


find_maxmin_1coil () ;		//   function call


long double delta_angle_time =  ( 2.0 * PI / omega )   /   n_angles  ;
long double Coil_Voltage ;

if   (   Multiple_Angles  ==  true   )
	{	
	double delta_angle_time =  ( 2.0 * PI / omega )   /   n_angles  ;

	std::ofstream uafile("Flux-and-Voltage.csv")  ;

	for (i=0 ; i < n_angles   ; i++)
	{
	std::cout << "i  : " <<  i << " Mag. Flux thru all turns = " << array_Flux [i]  << "  Wb " ;

	if (uafile.is_open()   )	uafile	<<   i
					<< ','  << array_Flux [i] ;
		else std::cout << "the output file on disk is closed!!";

		if ( i >= 1  )  
			{
			Coil_Voltage = ( array_Flux [i] - array_Flux [i-1]  )
					/  delta_angle_time ;
			std::cout << ", Voltage = " << Coil_Voltage << " V" ; 

			if (uafile.is_open()   )	uafile	<<       ","      <<       Coil_Voltage      ;
			}


	std::cout << '\n' ;
	if (uafile.is_open()   )	uafile  << '\n' ;
	}


	uafile.close()   ;
	}


//************



glutDisplayFunc(display);
glutReshapeFunc(reshape);
//GlutMouseFunc(mouse);
glutMainLoop();
return 0;

}


void        field_pattern_display(void)
{
int ix,iy ;
long double ppx, ppy, Bxr, Byr, Bxrb, Byrb  ;
long double halfline = 5;
float FFColor ;

glColor3f(1,0,0);     // red
glLineWidth(3);
glBegin(GL_LINES);

for (ix = 1 ; ix <= nx_obs ; ix++)
{
for (iy = 1 ; iy <= ny_obs ; iy++)
{
ppx = Field_Position[ix][iy]  .  x  * magnif ;
ppy = Field_Position[ix][iy]  .  y  * magnif ;

Bxr = Field_Value [ix] [iy]  .  x ;
Byr = Field_Value [ix] [iy]  .  y ;

Bxrb = Bxr / sqrt(  Bxr*Bxr + Byr*Byr  ) ;
Byrb = Byr / sqrt(  Bxr*Bxr + Byr*Byr  ) ;

Field_xy_value = sqrt( pow(Field_Value[ix][iy].x,2)  +  pow(Field_Value[ix][iy].y,2)  )   ;

FFColor = 0.2 +  	0.8 *    (  Field_xy_value - Field_xy_Min )  / ( Field_xy_Max - Field_xy_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f( 1, 1 - FFColor , 1 - FFColor) ;

glVertex2f(ppx  -Bxrb*halfline ,    ppy  -Byrb*halfline);
glVertex2f(ppx+Bxrb*halfline ,    ppy+Byrb*halfline);

}
}


glEnd();

std::cout << "RED - Max Field : " << Field_xy_Max <<  " T "  <<  '\n'   ;
std::cout << "RED - Min Field : " << Field_xy_Min <<   " T "  <<  '\n'   ;

// color bar

glLineWidth(3);
glBegin(GL_LINES);
for (int iq = 0 ; iq <= 60 ; iq++)
		{
			FFColor = 0.2 +  	0.8 *    (  double(iq) - 0 )  / double(60) ;
			glColor3f( 1, 1 - FFColor , 1 - FFColor) ;
			
glVertex2f(iq , -140);
glVertex2f(iq , -160);

		}
glEnd();



}





void        field_pattern_display_pancake (void)
{
int ip,ir,ik ;  	// polar index, radial index
long double ppx, ppy, ppz, Bxr, Byr, Bzr, Bxrb, Byrb, Bzrb ;
long double halfline = 5;
long double Field_xyz_value;
long double Field_xz_value ;
float FFColor ;


glColor3f(1,0,0);     // red
glLineWidth(3);
glBegin(GL_LINES);

for (  ik = 1 ; ik <= n_turns ; ik++)				
{
if (      display_field_one_turn == true     &&     ik   !=   turn_for_display      )  continue;
		//  this means skip the following lines (within loop) for 
		//  the ik-loop and "continue" the ik loop using the 
		//  NEXT index value
for (  ip = 1 ; ip <= 20 ; ip++  )
{
for (  ir = 1 ; ir <=  4   ; ir++)
{
	ppx = granule [ip][ir][ik]  .  x  * magnif ;
	ppy = granule [ip][ir][ik]  .  y  * magnif ;
	ppz = granule [ip][ir][ik]  .  z  * magnif ;

	Bxr = granule_field [ip] [ir] [ik] .  x ;
	Byr = granule_field [ip] [ir] [ik] .  y ;
	Bzr = granule_field [ip] [ir] [ik] .  z ;

//	Bxrb = Bxr / sqrt(  Bxr*Bxr + Byr*Byr  + Bzr*Bzr   ) ;
//	Byrb = Byr / sqrt(  Bxr*Bxr + Byr*Byr  + Bzr*Bzr   ) ;
//	Bzrb = Bzr / sqrt(  Bxr*Bxr + Byr*Byr  + Bzr*Bzr   ) ;

	Bxrb = Bxr / sqrt(  Bxr*Bxr   +   Bzr*Bzr   ) ;
	Bzrb = Bzr / sqrt(  Bxr*Bxr   +   Bzr*Bzr   ) ;

	Field_xz_value = sqrt( pow(   granule_field[ip][ir][ik].x,2   )  +  pow(   granule_field[ip][ir][ik].z,2    )  )   ;

	FFColor = 0.2 +  	0.8 *    (  Field_xz_value - Field_xz_Min )  / ( Field_xz_Max - Field_xz_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min

if    (   display_field_one_turn   ==   true   )
		{

	FFColor = 0.2 +  	0.8 *    (  Field_xz_value - Min_1coil )  / (   Max_1coil   -   Min_1coil   ) ;  
		}


//   ******************************  Field_xyz_Min / Max ,,,,,,,,, new variable needed ************
	glColor3f( 1 - FFColor , 1 - FFColor,   1) ;

//std::cout << "FFColor " << FFColor << '\n'  ;
//std::cout << "Bxrb ,y, z : " <<  Bxrb << " " <<  Byrb  << " " <<  Bzrb << "\n"  ;

//	glColor3f(0,0,0);   //
//	glVertex3f(ppx  -Bxrb*halfline ,    ppy  -Byrb*halfline,     ppz  -Bzrb*halfline   );
//	glVertex3f(ppx+Bxrb*halfline ,    ppy+Byrb*halfline,      ppz  +Bzrb*halfline);


	glVertex3f(ppx  -Bxrb*halfline ,    ppy    ,     ppz  -Bzrb*halfline   );
	glVertex3f(ppx+Bxrb*halfline ,    ppy   ,      ppz  +Bzrb*halfline);


//std::cout << "Field x y z : " <<  granule_field [ip] [ir]  .  x << " " <<  granule_field [ip] [ir]  .  y  << " " <<  granule_field [ip] [ir]  .  z << "\n"  ;



}
}
}


glEnd();

// color bar
glLineWidth(3);
glBegin(GL_LINES);
for (int iq = 0 ; iq <= 60 ; iq++)
		{
			FFColor = 0.2 +  	0.8 *    (  double(iq) - 0 )  / double(60) ;
			glColor3f( 1 - FFColor , 1 - FFColor  ,  1) ;	
				glVertex3f(iq , -110, 000);
				glVertex3f(iq , -90, 000);
		}
glEnd();

std::cout << "BLUE - Max Field : " << Max_1coil << " T "    <<  '\n'   ;
std::cout << "BLUE - Min Field : " <<  Min_1coil <<   " T "  <<  '\n'   ;



}






void        field_pattern_display_MAG03(void)
{
			std::cout << "Instrumental ! ........ " << '\n'  ;
			// lets display the xy projection of the field for the X_sensor_MAG03 and Y_sensor_MAG03
			// and the xz projection for the Z_sensor_MAG03
	
	// .............. get the MAX / MIN .....................		
	double Field_2axes_Min, Field_2axes_Max, Field_2axes;
	Field_2axes_Min =   sqrt (  pow(Y_sensor_fields[0].x  , 2  )  +  pow(Y_sensor_fields[0].y  , 2)  );    // changed 3jan2019  .z to .y
	Field_2axes_Max = 0;
	int iq ;

	for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
	{
		Field_2axes =	sqrt (  pow(Y_sensor_fields[iq].x  , 2  )  +  pow(Y_sensor_fields[iq].y  , 2)  ) ;		// 3jan2019 changed [0] to [iq], fixed
			if (   Field_2axes  >   Field_2axes_Max )  Field_2axes_Max = Field_2axes ;							// also changed  .y .z
			if (   Field_2axes  <   Field_2axes_Min )  Field_2axes_Min = Field_2axes ;
		Field_2axes =	sqrt (  pow(Z_sensor_fields[iq].x  , 2  )  +  pow(Z_sensor_fields[iq].y  , 2)  ) ;
			if (   Field_2axes  >   Field_2axes_Max )  Field_2axes_Max = Field_2axes ;
			if (   Field_2axes  <   Field_2axes_Min )  Field_2axes_Min = Field_2axes ;
		Field_2axes =	sqrt (  pow(X_sensor_fields[iq].x  , 2  )  +  pow(X_sensor_fields[iq].z  , 2)  ) ;
			if (   Field_2axes  >   Field_2axes_Max )  Field_2axes_Max = Field_2axes ;
			if (   Field_2axes  <   Field_2axes_Min )  Field_2axes_Min = Field_2axes ;
	}


long double ppx, ppy, ppz, Bxr, Byr, Bzr, Bxrb, Byrb, Bzrb  ;
long double halfline = 10; //5;		//   5;
float FFColor ;


// .................................Y_sensor
Y_sensor_avg = 0 ;
glBegin(GL_LINES);
for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
{
ppx = Y_sensor_position[iq]  .  x  * magnif ;
ppy = Y_sensor_position[iq]  .  y  * magnif ;
ppz = Y_sensor_position[iq]  .  z  * magnif ;


Bxr = Y_sensor_fields [iq] .  x ;
Byr = Y_sensor_fields [iq] .  y ;			// changed 3 jan 2019

Bxrb = Bxr / sqrt(  Bxr*Bxr + Byr*Byr  ) ;
Byrb = Byr / sqrt(  Bxr*Bxr + Byr*Byr  ) ;

Field_2axes = sqrt( Bxr*Bxr + Byr*Byr   )   ;

FFColor = 0.2 +  	0.8 *    (  Field_xy_value - Field_xy_Min )  / ( Field_xy_Max - Field_xy_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f(   1    ,   1 - FFColor   ,    1 - FFColor) ;


glVertex3f(ppx  -  Bxrb*halfline ,    ppy -  Byrb*halfline ,   ppz   );
glVertex3f(ppx  +  Bxrb*halfline ,    ppy +  Byrb*halfline ,   ppz   );

Y_sensor_avg  +=  Y_sensor_fields [iq] .  y ;

if (sensor_textout == true)
	{
		std::cout << "Y sensor [" << iq << "] : " << 
		Y_sensor_fields [iq] .  x  <<  " "  <<
		Y_sensor_fields [iq] .  y  <<  " "  <<
		Y_sensor_fields [iq] .  z  <<  " "  <<	'\n'  ;
	}
}
glEnd();	


Y_sensor_avg /= 11  ;
std::cout << "Y_sensor_avg  " << Y_sensor_avg << '\n'  ;
std::cout << '\n'  ;


// .................................Z_sensor
Z_sensor_avg = 0 ;
glBegin(GL_LINES);
for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
{
ppx = Z_sensor_position[iq]  .  x  * magnif ;
ppy = Z_sensor_position[iq]  .  y  * magnif ;
ppz = Z_sensor_position[iq]  .  z  * magnif ;


Bxr = Z_sensor_fields [iq] .  x ;
Byr = Z_sensor_fields [iq] .  y ;

Bxrb = Bxr / sqrt(  Bxr*Bxr + Byr*Byr  ) ;
Byrb = Byr / sqrt(  Bxr*Bxr + Byr*Byr  ) ;

Field_2axes = sqrt( Bxr*Bxr + Byr*Byr   )   ;

FFColor = 0.2 +  	0.8 *    (  Field_xy_value - Field_xy_Min )  / ( Field_xy_Max - Field_xy_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f(   1    ,   1 - FFColor   ,    1 - FFColor) ;


glVertex3f(ppx  -Bxrb*halfline ,    ppy  -  Byrb*halfline,   ppz   );
glVertex3f(ppx  +Bxrb*halfline ,    ppy  +  Byrb*halfline,   ppz   );

Z_sensor_avg  +=  Z_sensor_fields [iq] .  x ;

if (sensor_textout == true)
{
std::cout << "Z sensor [" << iq << "] : " << 
		Z_sensor_fields [iq] .  x  <<  " "  <<
		Z_sensor_fields [iq] .  y  <<  " "  <<
		Z_sensor_fields [iq] .  z  <<  " "  <<	'\n'  ;
}
}
glEnd();	

Z_sensor_avg /= 11  ;
std::cout << "Z_sensor_avg  " << Z_sensor_avg << '\n'  ;
std::cout << '\n'  ;


// .................................X_sensor
X_sensor_avg = 0 ;
glBegin(GL_LINES);
for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
{
ppx = X_sensor_position[iq]  .  x  * magnif ;
ppy = X_sensor_position[iq]  .  y  * magnif ;
ppz = X_sensor_position[iq]  .  z  * magnif ;


Bxr = X_sensor_fields [iq] .  x ;
Bzr = X_sensor_fields [iq] .  z ;

Bxrb = Bxr / sqrt(  Bxr*Bxr + Bzr*Bzr  ) ;
Bzrb = Bzr / sqrt(  Bxr*Bxr + Bzr*Bzr  ) ;

Field_2axes = sqrt( Bxr*Bxr + Bzr*Bzr   )   ;

FFColor = 0.2 +  	0.8 *    (  Field_xy_value - Field_xy_Min )  / ( Field_xy_Max - Field_xy_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f(   1    ,   1 - FFColor   ,    1 - FFColor) ;


glVertex3f(ppx  -  Bxrb*halfline ,    ppy  ,   ppz  -  Bzrb*halfline );
glVertex3f(ppx  +  Bxrb*halfline ,    ppy  ,   ppz  +  Bzrb*halfline );

X_sensor_avg  +=  X_sensor_fields [iq] .  z ;

if (sensor_textout == true)
	{	std::cout << "X sensor [" << iq << "] : " << 
		X_sensor_fields [iq] .  x  <<  " "  <<
		X_sensor_fields [iq] .  y  <<  " "  <<
		X_sensor_fields [iq] .  z  <<  " "  <<	'\n'  ;
}
}
glEnd();	

X_sensor_avg /= 11  ;
//std::cout << "X_sensor_avg  " << X_sensor_avg << '\n'  ;
//std::cout << '\n'  ;

}		// end function












void        field_pattern_display_MAG03_xyz(void)
{
			std::cout << "Instrumental _xyz ! ........ " << '\n'  ;
			// 		all 3 components this time
	
	// .............. get the MAX / MIN .....................		
	double Field_3axes_Min, Field_3axes_Max, Field_3axes;
		Field_3axes_Min =   Y_sensor_fields[0].Modulus() ;
		Field_3axes_Max = 0;
	int iq ;

	for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
	{
		
		// ......[iq].... in previous function   ???
		// .........................................
		Field_3axes =	Y_sensor_fields[iq].Modulus() ;
			if (   Field_3axes  >   Field_3axes_Max )  Field_3axes_Max = Field_3axes ;
			if (   Field_3axes  <   Field_3axes_Min )  Field_3axes_Min = Field_3axes ;
		Field_3axes =	Z_sensor_fields[iq].Modulus() ;
			if (   Field_3axes  >   Field_3axes_Max )  Field_3axes_Max = Field_3axes ;
			if (   Field_3axes  <   Field_3axes_Min )  Field_3axes_Min = Field_3axes ;
		Field_3axes =	X_sensor_fields[iq].Modulus() ;
			if (   Field_3axes  >   Field_3axes_Max )  Field_3axes_Max = Field_3axes ;
			if (   Field_3axes  <   Field_3axes_Min )  Field_3axes_Min = Field_3axes ;
	}


long double ppx, ppy, ppz, Bxr, Byr, Bzr, Bxrb, Byrb, Bzrb  ;
long double halfline = 10; //5;		//   5;
float FFColor ;


// .................................Y_sensor
Y_sensor_avg = 0 ;
glBegin(GL_LINES);
for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
{
ppx = Y_sensor_position[iq]  .  x  * magnif ;
ppy = Y_sensor_position[iq]  .  y  * magnif ;
ppz = Y_sensor_position[iq]  .  z  * magnif ;


Bxr = Y_sensor_fields [iq] .  x ;
Byr = Y_sensor_fields [iq] .  y ;
Bzr = Y_sensor_fields [iq] .  z ;

Field_3axes = Y_sensor_fields [iq] . Modulus() ;
Bxrb = Bxr / Field_3axes ;
Byrb = Byr / Field_3axes ;
Bzrb = Bzr / Field_3axes ;

FFColor = 0.2 +  	0.8 *    (  Field_3axes - Field_3axes_Min )  / ( Field_3axes_Max - Field_3axes_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f(   1    ,   1 - FFColor   ,    1 - FFColor) ;


glVertex3f(ppx  -Bxrb*halfline ,    ppy - Byrb*halfline ,   ppz   -  Bzrb*halfline);
glVertex3f(ppx  +Bxrb*halfline ,    ppy + Byrb*halfline ,   ppz   +  Bzrb*halfline);

Y_sensor_avg  +=  Y_sensor_fields [iq] .  y ;

if (sensor_textout == true)
	{
		std::cout << "Y sensor [" << iq << "] : " << 
		Y_sensor_fields [iq] .  x  <<  " "  <<
		Y_sensor_fields [iq] .  y  <<  " "  <<
		Y_sensor_fields [iq] .  z  <<  " "  <<	'\n'  ;
	}
}
glEnd();	


Y_sensor_avg /= 11  ;
std::cout << "Y_sensor_avg  " << Y_sensor_avg << '\n'  ;
std::cout << '\n'  ;


// .................................Z_sensor
Z_sensor_avg = 0 ;
glBegin(GL_LINES);
for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
{
ppx = Z_sensor_position[iq]  .  x  * magnif ;
ppy = Z_sensor_position[iq]  .  y  * magnif ;
ppz = Z_sensor_position[iq]  .  z  * magnif ;

Bxr = Z_sensor_fields [iq] .  x ;
Byr = Z_sensor_fields [iq] .  y ;
Bzr = Z_sensor_fields [iq] .  z ;

Field_3axes = Z_sensor_fields [iq] . Modulus() ;
Bxrb = Bxr / Field_3axes ;
Byrb = Byr / Field_3axes ;
Bzrb = Bzr / Field_3axes ;


FFColor = 0.2 +  	0.8 *    (  Field_3axes - Field_3axes_Min )  / ( Field_3axes_Max - Field_3axes_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f(   1    ,   1 - FFColor   ,    1 - FFColor) ;


glVertex3f(ppx  -Bxrb*halfline ,    ppy - Byrb*halfline ,   ppz   -  Bzrb*halfline);
glVertex3f(ppx  +Bxrb*halfline ,    ppy + Byrb*halfline ,   ppz   +  Bzrb*halfline);


Z_sensor_avg  +=  Z_sensor_fields [iq] .  x ;

if (sensor_textout == true)
{
std::cout << "Z sensor [" << iq << "] : " << 
		Z_sensor_fields [iq] .  x  <<  " "  <<
		Z_sensor_fields [iq] .  y  <<  " "  <<
		Z_sensor_fields [iq] .  z  <<  " "  <<	'\n'  ;
}
}
glEnd();	

Z_sensor_avg /= 11  ;
std::cout << "Z_sensor_avg  " << Z_sensor_avg << '\n'  ;
std::cout << '\n'  ;


// .................................X_sensor
X_sensor_avg = 0 ;
glBegin(GL_LINES);
for (iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
{
ppx = X_sensor_position[iq]  .  x  * magnif ;
ppy = X_sensor_position[iq]  .  y  * magnif ;
ppz = X_sensor_position[iq]  .  z  * magnif ;

Bxr = X_sensor_fields [iq] .  x ;
Byr = X_sensor_fields [iq] .  y ;
Bzr = X_sensor_fields [iq] .  z ;

Field_3axes = X_sensor_fields [iq] . Modulus() ;
Bxrb = Bxr / Field_3axes ;
Byrb = Byr / Field_3axes ;
Bzrb = Bzr / Field_3axes ;

FFColor = 0.2 +  	0.8 *    (  Field_3axes - Field_3axes_Min )  / ( Field_3axes_Max - Field_xy_Min ) ;  // fixed 8 Jan 2019 , changed denominator from Max to Max-Min
glColor3f(   1    ,   1 - FFColor   ,    1 - FFColor) ;


glVertex3f(ppx  -Bxrb*halfline ,    ppy - Byrb*halfline ,   ppz   -  Bzrb*halfline);
glVertex3f(ppx  +Bxrb*halfline ,    ppy + Byrb*halfline ,   ppz   +  Bzrb*halfline);


X_sensor_avg  +=  X_sensor_fields [iq] .  z ;

if (sensor_textout == true)
	{	std::cout << "X sensor [" << iq << "] : " << 
		X_sensor_fields [iq] .  x  <<  " "  <<
		X_sensor_fields [iq] .  y  <<  " "  <<
		X_sensor_fields [iq] .  z  <<  " "  <<	'\n'  ;
}
}
glEnd();	

X_sensor_avg /= 11  ;
std::cout << "X_sensor_avg  " << X_sensor_avg << '\n'  ;
std::cout << '\n'  ;

std::cout << "Field_3axes_Min : " << Field_3axes_Min << "\n";
std::cout << "Field_3axes_Max : " << Field_3axes_Max << "\n" ;

}		// end function





Vector Calc_B_from_2by2_THICK_UPPER ( Vector obs_point_fn)     // note that the array of VELOCITIES is used
{
int fnn1, fnn2   ;
Vector B_total (0,0,0);

Vector r_vec ;
Vector r_hat ;
//  1 is for UPPER FACE of dielectric, 2 is for LOWER FACE
Vector dB_1 (0,0,0) ;
Vector B_total_1 (0,0,0);


for (fnn1 = 1 ; fnn1 <= nx ; fnn1++ )        //  1 to 100 inclusive  (initially)
	{
							// change on 17 jan 2019 .....
//		for (fnn2 = 1 ; fnn2 <= 50 ; fnn2++ )    // y-direction is the INNER LOOP
		for (fnn2 = 1 ; fnn2 <= ny ; fnn2++ )    // y-direction is the INNER LOOP
		{
			r_vec = meg_array_pos_UP [fnn1][fnn2]
				- obs_point_fn  ;
			r_hat = r_vec.Hat()   ;

			dB_1 = r_hat * array_velocity_UP[fnn2] * (-1.0)
				/ std::pow(   r_vec.Modulus()  ,  2 );
		
			// bring in the u0/4pi later ..... and the CHARGE
			B_total_1  =  B_total_1  +  dB_1 ;
		}
	}

	B_total_1 = B_total_1 *   mu_0   /   ( 4 * PI ) * Q_cell ;

	if (test_1a == true  )	//  && fnn1 == 1 && fnn2 == 1      && fnn1 == 1
			{
				std::cout << "test_1a :  B_total_1 / tesla = " << 
					B_total_1.x  << " " << B_total_1.y  << " " <<  B_total_1.z  <<  '\n';
				r_vec = meg_array_pos_UP [1][1]    
				- obs_point_fn  ;
				std::cout << "r_2 for meg_array_pos_UP[1][1]: " << 
					r_vec.Modulus()  <<  '\n';
			}

					


	

//	std::cout << '\n'  ;
//	std::cout << "B_total_1   |B|  =  " << B_total_1 . Modulus()  <<  '\n'  ;
//	std::cout << "B_total_2   |B|  =  " << B_total_2 . Modulus()  <<  '\n'  ;
//	std::cout << "B_total    |B1 + B2 |  =  " << 
//			(  B_total_1	+	B_total_2  ) . Modulus()  <<  '\n'  ;

	return B_total_1;
}




Vector Calc_B_from_2by2_THICK_LOWER ( Vector obs_point_fn)     // note that the array of VELOCITIES is used
{
int fnn1, fnn2   ;
Vector B_total (0,0,0);

Vector r_vec ;
Vector r_hat ;
//  1 is for UPPER FACE of dielectric, 2 is for LOWER FACE

Vector dB_2 (0,0,0) ;
Vector B_total_2 (0,0,0);


for (fnn1 = 1 ; fnn1 <= nx ; fnn1++ )        //  1 to 100 inclusive  (initially)
	{
		//  .. change on 17 jan 2019 ... ny  becomes 50
//		for (fnn2 = 1 ; fnn2 <= 50 ; fnn2++ )    // y-direction is the INNER LOOP
		for (fnn2 = 1 ; fnn2 <= ny ; fnn2++ )    // y-direction is the INNER LOOP
		{
			r_vec = meg_array_pos_DOWN [fnn1][fnn2]
				- obs_point_fn  ;
			r_hat = r_vec.Hat()   ;

			dB_2 = r_hat * array_velocity_DOWN[fnn2] * (-1.0)
				/ std::pow(   r_vec.Modulus()  ,  2 );
		
			// bring in the u0/4pi later ..... and the CHARGE
			B_total_2  =  B_total_2  +  dB_2 ;
		}
	}

	B_total_2 = B_total_2 *   mu_0   /   ( 4 * PI ) * Q_cell*(-1.0) ;

	if (test_1a == true  )	//  && fnn1 == 1 && fnn2 == 1 && fnn1 == 1
			{
				std::cout << "test_1a :  B_total_2 / tesla = " << 
					B_total_2.x  << " " << B_total_2.y  << " " <<  B_total_2.z  <<  '\n';
				r_vec = meg_array_pos_DOWN [1][1]    
				- obs_point_fn  ;
				std::cout << "r_2 for meg_array_pos_DOWN[1][1]: " << 
					r_vec.Modulus()  <<  '\n';
			}

	

//	std::cout << '\n'  ;
//	std::cout << "B_total_1   |B|  =  " << B_total_1 . Modulus()  <<  '\n'  ;
//	std::cout << "B_total_2   |B|  =  " << B_total_2 . Modulus()  <<  '\n'  ;
//	std::cout << "B_total    |B1 + B2 |  =  " << 
//			(  B_total_1	+	B_total_2  ) . Modulus()  <<  '\n'  ;

	return B_total_2;
}

/*
		// Vector Test.... ensure right-handed
		Vector vv1 = Vector (1,0,0);
		Vector vv2 = Vector (0,1,0);
		Vector vv3 = vv1 * vv2 ;
		std::cout << "Vector Test :  " <<  '\n' ; 
		std::cout << vv3.x << " " << vv3.y << " " << vv3.z << '\n' ; 
*/

void recalc_Y_sensor_avg(void)
{
	Y_sensor_avg = 0 ;
	for (int iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
		{
			Y_sensor_avg  +=  Y_sensor_fields [iq] .  y  ;	// changed 3jan2019
		}
	Y_sensor_avg /= 11 ;
}


void recalc_Z_sensor_avg(void)
{
	Z_sensor_avg = 0 ;
	for (int iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
		{
			Z_sensor_avg  +=  Z_sensor_fields [iq] .  x ;
		}
	Z_sensor_avg /= 11 ;
}


void recalc_X_sensor_avg(void)
{
	X_sensor_avg = 0 ;
	for (int iq = 0 ; iq <= num_sensor_fields_minus_one ; iq++)
		{
			X_sensor_avg  +=  X_sensor_fields [iq] .  z ;    // changed 3jan2019
		}
	X_sensor_avg /= 11 ;
}



void   find_maxmin_1coil (void)
{
long double Field_xz_value ;
int ip,ir,ik   ;


ik = turn_for_display   ;

Max_1coil = sqrt( pow(   granule_field [1][1][ik]  .x  ,  2   )  +  pow(  granule_field [1][1][ik]   .z  ,  2   )  )   	;
Min_1coil  = sqrt( pow(   granule_field [1][1][ik]  .x  ,  2   )  +  pow(  granule_field [1][1][ik]   .z  ,  2   )  )      ;


for (ip = 1 ; ip <= 20 ; ip++ )              // 
	{
		for (ir = 1; ir <= 4 ; ir++ )	// 
		{

Field_xz_value = sqrt( pow(   granule_field [ip][ir][ik]  .x  ,  2   )  +  pow(  granule_field [ip][ir][ik]   .z  ,  2   )  )   ;

	// do the MAX-MIN

	if (   Field_xz_value > Max_1coil     )    Max_1coil  =  Field_xz_value  ;

	if (  Field_xz_value  <   Min_1coil ) 
	{
		Min_1coil  =   Field_xz_value   ;
	}

}
}


}


//cd borland\bcc55\bin

//bcc32 -Ic:\borland\BCC55\include -Lc:\borland\bcc55\lib -Lc:\borland\BCC55\psdk -Lc:\borland\bcc55\lib\psdk Mechtenna--COMPAQ-Mar-21-D2.cpp


