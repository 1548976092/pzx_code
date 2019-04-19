
//#define debug printf
#define debug  

/********************************************************************
* Description:  hal_skeleton.c
*               This file, 'hal_skeleton.c', is a example that shows 
*               how drivers for HAL components will work and serve as 
*               a skeleton for new hardware drivers.
*
* Author: John Kasunich
* License: GPL Version 2
*    
* Copyright (c) 2003 All rights reserved.
*
* Last change: 
********************************************************************/

/** This file, 'hal_skeleton.c', is a example that shows how
 drivers for HAL components will work and serve as a skeleton
 for new hardware drivers.
 
 Most of this code is taken from the hal_parport driver from John Kasunich,
 which is also a good starting point for new drivers.

 This driver supports only for demonstration how to write a byte (char)
 to a hardware adress, here we use the parallel port (0x378).

 This driver support no configuration strings so installing is easy:
 realtime: halcmd loadrt hal_skeleton

 The driver creates a HAL pin and if it run in realtime a function
 as follows:

 Pin: 'skeleton.<portnum>.pin-<pinnum>-out'
 Function: 'skeleton.<portnum>.write'

 This skeleton driver also doesn't use arguments you can pass to the driver
 at startup. Please look at the parport driver how to implement this if you need
 this for your driver.

 (added 17 Nov 2006)
 The approach used for writing HAL drivers has evolved quite a bit over the
 three years since this was written.  Driver writers should consult the HAL
 User Manual for information about canonical device interfaces, and should
 examine some of the more complex drivers, before using this as a basis for
 a new driver.

*/

/** Copyright (C) 2003 John Kasunich
                       <jmkasunich AT users DOT sourceforge DOT net>
                       Martin Kuhnle
                       <mkuhnle AT users DOT sourceforge DOT net>
*/

/** This program is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General
    Public License as published by the Free Software Foundation.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111 USA

    THE AUTHORS OF THIS LIBRARY ACCEPT ABSOLUTELY NO LIABILITY FOR
    ANY HARM OR LOSS RESULTING FROM ITS USE.  IT IS _EXTREMELY_ UNWISE
    TO RELY ON SOFTWARE ALONE FOR SAFETY.  Any machinery capable of
    harming persons must have provisions for completely removing power
    from all motors, etc, before persons enter any danger area.  All
    machinery must be designed to comply with local and national safety
    codes, and the authors of this software can not, and do not, take
    any responsibility for such compliance.

    This code was written as part of the EMC HAL project.  For more
    information, go to www.linuxcnc.org.
*/

#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */

#include "hal.h"		/* HAL public API decls */

#include <string.h>
#include <stdio.h>  
#include <stdlib.h>   //exit()  
#include <fcntl.h>    //define O_RDWR  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>  
#include <unistd.h>
#include "hal_skeleton_marlin.h"

/* module information */
MODULE_AUTHOR("Martin Kuhnle");
MODULE_DESCRIPTION("Test Driver for ISA-LED Board for EMC HAL");
MODULE_LICENSE("GPL");
/* static char *cfg = 0; */
/* config string
RTAPI_MP_STRING(cfg, "config string"); */

int file;
int i; 


/***********************************************************************
*                STRUCTURES AND GLOBAL VARIABLES                       *
************************************************************************/

/* this structure contains the runtime data needed by the
   driver for a single port/channel
*/

typedef struct {
    hal_float_t *data_out;		/* ptrs for output */
    hal_u32_t data_outnum;
} skeleton_t;

/* pointer to array of skeleton_t structs in shared memory, 1 per port */
static skeleton_t *port_data_array;

/* other globals */
static int comp_id;		/* component ID */
static int num_ports;		/* number of ports configured */

/***********************************************************************
*                  LOCAL FUNCTION DECLARATIONS                         *
************************************************************************/
/* These is the functions that actually do the I/O
   everything else is just init code
*/
static void write_port(void *arg, long period);

/***********************************************************************
*                       INIT AND EXIT CODE                             *
************************************************************************/

#define MAX_PORTS 16

#define MAX_TOK ((MAX_PORTS*2)+3)

    int ret;

int rtapi_app_main(void)
{
    char name[HAL_NAME_LEN + 1];
    int n, retval;

    /* only one port at the moment */
    num_ports = 16;
    n = 0; /* port number */

 
    file =  serial_init("/dev/ttyACM1",115200,1);  
    if(file < 0)  
    {  
        perror("serial init err:");  
        return -1;  
    }  

 char str1[]="G92 X0 Y0 Z0\n";
 char str2[]="M28 O\n";
 char str3[]="M114\n";
 char str4[]="G1 X1.2 Y1 Z1 F18000\n";
 char str5[]="M114\n";



ret = serial_send(file,str1,22);
printf("send %d bytes!\n",ret); 

ret = serial_send(file,str2,22);
printf("send %d bytes!\n",ret); 

ret = serial_send(file,str3,22);
ret = serial_send(file,str4,22);
ret = serial_send(file,str5,22);      



    /* STEP 1: initialise the driver */
    comp_id = hal_init("hal_skeleton_marlin");
    if (comp_id < 0) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "SKELETON: ERROR: hal_init() failed\n");
	return -1;
    }

int p=0;
    /* STEP 2: allocate shared memory for skeleton data */
	port_data_array = hal_malloc(num_ports * sizeof(skeleton_t));
    if (port_data_array == 0) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "SKELETON: ERROR: hal_malloc() failed\n");
	hal_exit(comp_id);
	return -1;
    }

    /* STEP 3: export the pin(s) */
     for(p=0;p<num_ports;p++)
     {	
	 retval = hal_pin_float_newf(HAL_IN, &(port_data_array[p].data_out),
			     comp_id, "skeleton.%d.pin-%02d-out", n, p+1);
         port_data_array[p].data_outnum=p;
     }
    if (retval < 0) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "SKELETON: ERROR: port %d var export failed with err=%i\n", n,
	    retval);
	hal_exit(comp_id);
	return -1;
    }

    /* STEP 4: export write function */
    rtapi_snprintf(name, sizeof(name), "skeleton.%d.write", n);
    retval = hal_export_funct(name, write_port, &(port_data_array[n]), 0, 0,
	comp_id);
    if (retval < 0) {
	rtapi_print_msg(RTAPI_MSG_ERR,
	    "SKELETON: ERROR: port %d write funct export failed\n", n);
	hal_exit(comp_id);
	return -1;
    }

    rtapi_print_msg(RTAPI_MSG_INFO,
	"SKELETON: installed driver for %d ports\n", num_ports);
  
    hal_ready(comp_id);
    return 0;
}

void rtapi_app_exit(void)
{
	 close(file);
    hal_exit(comp_id);
}

/**************************************************************
* REALTIME PORT WRITE FUNCTION                                *
**************************************************************/
char test[30]="G1 X1.1 Y1 Z1.3 F18000\n";
static void write_port(void *arg, long period)
{
    skeleton_t *port;
    int outdata,out_put_num;
    port = arg;

    /* write it to the hardware */
//    for(i=0;i<16;i++)
//    {
    	//pca9685_setmk( port[i].data_outnum, (hal_u32_t)( *(port[i].data_out)*10 + 700 ) );
//    }
//sprintf(test, "G1 X%.1f Y%.1f Z%.1f F18000\n",*(port[0].data_out),*(port[1].data_out),*(port[2].data_out));

//printf("G1 X%.2f Y%.2f Z%.2f F18000\n",*(port[0].data_out)* 0.1,*(port[1].data_out) * 0.1,*(port[2].data_out)*0.1 );



sprintf(test, "G1 X%d  F18000\n",(int)(*(port[0].data_out) ));

//printf(test, "G1 X%.1f  F18000\n",*(port[0].data_out));

//ret = serial_send(file,test,25);
//printf("send %d bytes!\n",ret); 
printf(test);




    
}
