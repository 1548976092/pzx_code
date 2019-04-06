
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


#include <stdio.h>  
#include <stdlib.h>   //exit()  
#include <fcntl.h>    //define O_RDWR  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>  
#include <unistd.h>


/* module information */
MODULE_AUTHOR("Martin Kuhnle");
MODULE_DESCRIPTION("Test Driver for ISA-LED Board for EMC HAL");
MODULE_LICENSE("GPL");
/* static char *cfg = 0; */
/* config string
RTAPI_MP_STRING(cfg, "config string"); */

int file;
int i; 
int pzx_addr = 0;
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC

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


int wiringPiI2CReadReg8(int addr,int reg)
{
        char  tmp;
        if( ioctl(file, I2C_SLAVE, addr) < 0)//设置从机地址
        {
               printf("error!\n");
        }  

        write( file, &reg,     1);
        read(  file, &tmp     ,1); 
        debug("device addr:0x%x ,read-> reg:0x%x:0x%x\n",addr,reg,tmp); 
        return tmp;
}

int wiringPiI2CWriteReg8(int addr,int reg,int value)
{
        debug("device addr:0x%x ,write-> reg:0x%x:0x%x\n",addr,reg,value);
        char   wreg[2] = {reg,value}; 
        if( ioctl(file, I2C_SLAVE, addr) < 0)//设置从机地址
        {
               printf("error!\n");
        }  

        write( file, &wreg      ,2);
        return 0;
}

int wiringPiI2CReadReg16(int addr,int reg)
{
        debug("****R*********16**********************************************");
        wiringPiI2CReadReg8(addr,reg);
        wiringPiI2CReadReg8(addr,reg++);
}

int wiringPiI2CWriteReg16(int addr,int reg,int value)
{
        debug("—————w—————---16———device addr:0x%x ,write-> reg16:0x%x:0x%x\n",addr,reg,value);
        wiringPiI2CWriteReg8(addr,reg,value);
        wiringPiI2CWriteReg8(addr,reg+1,value>>8);       
        return 0;
}

int pca9685_init(unsigned char addr)    // addr是7位的i2c从机地址，返回的是linux标准的设备描述符，调用它的地方视作pca9685的$
                                        //因为可以多个pca9685级联，通过设备描述符区别它们
                                        //此驱动仅作为驱动舵机使用，周期固定死位20ms，不允许外部设置
{
     int pca9685 = addr;

        double T = 20000;       //周期，单位是us
        unsigned char prescale;
        double osc_clock = 25000000;
        unsigned char oldmode, newmode;
        T /= 0.915;     //不知道为什么，会有所偏差，这里校准一下就ok了，从网上找的arduino代码也进行了校准。
        T /= 1000000;   //把T转换成秒
        prescale = (unsigned char)(osc_clock/4096*T - 1);
        printf("prescale = 0x%x\n\n", prescale);

        oldmode = wiringPiI2CReadReg8(pca9685, PCA9685_MODE1);
        newmode = (oldmode&0x7f) | 0x10;        //准备进入sleep，设置时钟前必须先进入sleep模式
        wiringPiI2CWriteReg8(pca9685, PCA9685_MODE1, newmode);
        wiringPiI2CWriteReg8(pca9685, PCA9685_PRESCALE, prescale);
        oldmode &= 0xef;        //清除sleep位
        wiringPiI2CWriteReg8(pca9685, PCA9685_MODE1, oldmode);
        wiringPiI2CWriteReg8(pca9685, PCA9685_MODE1, oldmode | 0xa1);
    
    return pca9685;
}
void pca9685_setmk(int num, int mk)     //设置指定通道的脉宽。fd是在pca9685_init时获得的设备描述符，num是通道号（从0开始） $
{
        unsigned int on, off;

        on = 0; //每次周期一开始就输出高电平
        off = (unsigned int)((((double)mk)/20000 * 4096)*1.0067114);    //最后的1.0067114是校准用的

        debug("off = 0x%x", off);
        wiringPiI2CWriteReg16(pzx_addr,0x6+num*4,on);
        wiringPiI2CWriteReg16(pzx_addr,0x8+num*4,off);  
}
void printf_all_register()
{
        int addr;
        char value;
debug("*****************************************\n");
        for(addr =0;addr<= 0xff;addr++)
        {
               write( file, &addr , 1);
               read(  file, &value, 1); //read register 0x28 and save to rbuf
               debug("addr0x%x, value:0x%x\n",addr, value);    
        }
debug("*****************************************\n");
}

void get_i2cdev()
{

        int value=1;
        int addr = 0;  
        char *filename = "/dev/i2c-0";  
        char wbuf[] = {0x20, 0x0f}; //first byte is address to write. others are bytes to be written  
        char read_start_buf[1];   
        char rbuf[1000] = {0};  
        printf("pzx i2c\n");
        if((file = rtapi_open_as_root(filename, O_RDWR)) < 0)  
        {  
                perror("Failed to open i2c device.\n");  
                exit(1);  
        }  
        else
        {
                printf("open i2c device ok\n");  
        }

        for(addr =0;addr< 0x7f;addr++)
        {
                if( ioctl(file, I2C_SLAVE, addr) < 0)//设置从机地址
                {
                        printf("error!\n");
                }  
                if( write( file, &value, 1 ) == 1) 
                {
                        printf("find drive 0x%x\n", addr);
                        pzx_addr = addr;//获取设备地址（忽略0x70 超级地址）
                        break;
                }
        }

        printf_all_register();
   
        pca9685_init(pzx_addr);
}
int rtapi_app_main(void)
{
    char name[HAL_NAME_LEN + 1];
    int n, retval;

    /* only one port at the moment */
    num_ports = 16;
    n = 0; /* port number */

	get_i2cdev();
    /* STEP 1: initialise the driver */
    comp_id = hal_init("hal_skeleton_pwm");
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

static void write_port(void *arg, long period)
{
    skeleton_t *port;
    int outdata,out_put_num;
    port = arg;

    /* write it to the hardware */
    for(i=0;i<16;i++)
    {
    	pca9685_setmk( port[i].data_outnum, (hal_u32_t)( *(port[i].data_out)*10 + 1300 ) );

    }
}
