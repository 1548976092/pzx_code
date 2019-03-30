 /********************************************************************
* Description: trivkins.c
*   general trivkins for 3 axis Cartesian machine
*
*   Derived from a work by Fred Proctor & Will Shackleford
*
* License: GPL Version 2
*    
* Copyright (c) 2009 All rights reserved.
*
********************************************************************/

#include "motion.h"
#include "hal.h"
#include "rtapi.h"
#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "rtapi_math.h"
#include "rtapi_string.h"

struct data { 
    hal_s32_t joints[EMCMOT_MAX_JOINTS];
} *data;

#define SET(f) pos->f = joints[i]


//只需从正解
//Vf =               y          + z*L/2
//Vl = -x*cos(30°) - y*sin(30°) + z*L/2
//Vr =  x*cos(30°) - y*sin(30°) + z*L/2 

//反推即可得到
//x = (    - Vl + Vr) / (2 * cos(30°))
//y = (2Vf - Vl - Vr) /  3
//z = ( Vf + Vl + Vr) *  2 /   (3L)
double L=1;

#define VX_VALUE 0.5f //(0.5f)
#define VY_VALUE 0.72f //(sqrt(3)/2.0) 
#define L_value 1.0f // 0.2/角速度校准参数(0.915)

int kinematicsForward(const double *joints,
		      EmcPose * pos,
		      const KINEMATICS_FORWARD_FLAGS * fflags,
		      KINEMATICS_INVERSE_FLAGS * iflags)
{
    int i;
double a0, a1, a2;
double v0, v1, v2;
double w,l=1;

a0 = joints[0];
a1 = joints[1];
a2 = joints[2];

//v0 =  a0 +      +a1*0      +a2*l;
//v1 = -a0*cos(45)-a1*sin(45)+a2*l;
//v2 = -a0*sin(45)+a1*cos(45)+a2*l;

//v0 = (      -a1 +a2)/(2*cos(30));
//v1 = (2*a0  -a1 -a2)/3;
//v2 = (  a0  +a1 +a2)*2*/(3*L);


v0 = -(2.0*a2-a0-a1)/3.0;
v1 = -(a0 - a1)/1.442;
v2 = -(a0 + a1 + a2)/(L_value*3);

pos->tran.x = v0;
pos->tran.y = v1;
pos->tran.z = v2;

    for(i = 0; i < EMCMOT_MAX_JOINTS; i++) {
        switch(data->joints[i]) {
//            case 0: SET(tran.y); break;
//            case 1: SET(tran.x); break;
//            case 2: SET(tran.z); break;
            case 3: SET(a); break;
            case 4: SET(b); break;
            case 5: SET(c); break;
            case 6: SET(u); break;
            case 7: SET(v); break;
            case 8: SET(w); break;
	    //case 6: SET(tran.x); break;
            //case 7: SET(tran.y); break;
            //case 8: SET(tran.z); break;
        }
    }

    return 0;
}

int kinematicsInverse(const EmcPose * pos,
		      double *joints,
		      const KINEMATICS_INVERSE_FLAGS * iflags,
		      KINEMATICS_FORWARD_FLAGS * fflags)
{
    int i;
double a0, a1, a2;
double v0, v1, v2;
double w,l=10;

a0 =  pos->tran.x;
a1 =  pos->tran.y;
a2 =  pos->tran.z;

//v0 =  a0*cos(45)+sin(45)*a1+a2*0;
//v1 = -a0*sin(45)+a1*cos(45)+a2*0;
//v2 =  a0*0      +a1*0      +a2*1;

//Vf = y + z*L/2
//Vl = -x*cos(30°) - y*sin(30°) + z*L/2
//Vr = x*cos(30°) - y*sin(30°) + z*L/2 

//v0 =               a1         + a2*L/2;
//v1 = -a0*cos(30) - a1*sin(30) + a2*L/2;
//v2 =  a0*cos(30) - a1*sin(30) + a2*L/2;

//v0 =         1*a0 +      0  * a1 + L*a2;
//v1 =  -cos(60)*a0 - sin(60) * a1 + L*a2;
//v2 =  -sin(30)*a0 + cos(30) * a1 + L*a2;

v0 = (-VX_VALUE*a1 + VY_VALUE*a0 + L_value*a2);//正解函数，具体推算过程请查看我的博客
v1 = (-VX_VALUE*a1 - VY_VALUE*a0 + L_value*a2);
v2 = (a1 + L_value*a2);

joints[0] = v0;
joints[1] = v1;
joints[2] = v2;



    for(i = 0; i < EMCMOT_MAX_JOINTS; i++) {
        switch(data->joints[i]) {
//            case 0: joints[i] = pos->tran.y; break;
//            case 1: joints[i] = pos->tran.x; break;
//            case 2: joints[i] = pos->tran.z; break;
            case 3: joints[i] = pos->a; break;
            case 4: joints[i] = pos->b; break;
            case 5: joints[i] = pos->c; break;
            case 6: joints[i] = pos->u; break;
            case 7: joints[i] = pos->v; break;
            case 8: joints[i] = pos->w; break;
	    //case 6: joints[i] = pos->tran.x; break;
            //case 7: joints[i] = pos->tran.y; break;
            //case 8: joints[i] = pos->tran.z; break;
	 }
    }

    return 0;
}

/* implemented for these kinematics as giving joints preference */
int kinematicsHome(EmcPose * world,
		   double *joint,
		   KINEMATICS_FORWARD_FLAGS * fflags,
		   KINEMATICS_INVERSE_FLAGS * iflags)
{
    *fflags = 0;
    *iflags = 0;

    return kinematicsForward(joint, world, fflags, iflags);
}

static KINEMATICS_TYPE ktype = -1;

KINEMATICS_TYPE kinematicsType()
{
    return ktype;
}

static char *coordinates = "XYZABCUVW";
RTAPI_MP_STRING(coordinates, "Existing Axes");

static char *kinstype = "1"; // use KINEMATICS_IDENTITY
RTAPI_MP_STRING(kinstype, "Kinematics Type (Identity,Both)");

EXPORT_SYMBOL(kinematicsType);
EXPORT_SYMBOL(kinematicsForward);
EXPORT_SYMBOL(kinematicsInverse);
MODULE_LICENSE("GPL");

static int next_axis_number(void) {
    while(*coordinates) {
	switch(*coordinates) {
	    case 'x': case 'X': coordinates++; return 0;
	    case 'y': case 'Y': coordinates++; return 1;
	    case 'z': case 'Z': coordinates++; return 2;
	    case 'a': case 'A': coordinates++; return 3;
	    case 'b': case 'B': coordinates++; return 4;
	    case 'c': case 'C': coordinates++; return 5;
	    case 'u': case 'U': coordinates++; return 6;
	    case 'v': case 'V': coordinates++; return 7;
	    case 'w': case 'W': coordinates++; return 8;
	    case ' ': case '\t': coordinates++; continue;
	}
	rtapi_print_msg(RTAPI_MSG_ERR,
		"trivkins: ERROR: Invalid character '%c' in coordinates\n",
		*coordinates);
		return -1;
    }
    return -1;
}
int comp_id;
int rtapi_app_main(void) {
    int i;
    comp_id = hal_init("trivkins_pzx");
    if(comp_id < 0) return comp_id;

    data = hal_malloc(sizeof(struct data));

    for(i=0; i<EMCMOT_MAX_JOINTS; i++) {
	data->joints[i] = next_axis_number();
    }
    switch (*kinstype) {
      case 'b': case 'B': ktype = KINEMATICS_BOTH;         break;
      case 'f': case 'F': ktype = KINEMATICS_FORWARD_ONLY; break;
      case 'i': case 'I': ktype = KINEMATICS_INVERSE_ONLY; break;
      case '1': default:  ktype = KINEMATICS_IDENTITY;
    }

    hal_ready(comp_id);
    return 0;
}

void rtapi_app_exit(void) { hal_exit(comp_id); }
