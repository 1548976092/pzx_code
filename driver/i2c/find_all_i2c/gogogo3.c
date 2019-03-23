//#define debug printf 
#define debug

#include <stdio.h>  
#include <stdlib.h>   //exit()  
#include <fcntl.h>    //define O_RDWR  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>  
#include <unistd.h>

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
int pca9685_init(unsigned char addr)	// addr是7位的i2c从机地址，返回的是linux标准的设备描述符，调用它的地方视作pca9685的设备描述符
					//因为可以多个pca9685级联，通过设备描述符区别它们
					//此驱动仅作为驱动舵机使用，周期固定死位20ms，不允许外部设置
{
     int pca9685 = addr;

	double T = 20000;	//周期，单位是us
	unsigned char prescale;
	double osc_clock = 25000000;
	unsigned char oldmode, newmode;
	T /= 0.961;	//不知道为什么，会有所偏差，这里校准一下就ok了，从网上找的arduino代码也进行了校准。
	T /= 1000000;	//把T转换成秒
	prescale = (unsigned char)(osc_clock/4096*T - 1);
	printf("prescale = %d\n\n", prescale);
	
	oldmode = wiringPiI2CReadReg8(pca9685, PCA9685_MODE1);
	newmode = (oldmode&0x7f) | 0x10;	//准备进入sleep，设置时钟前必须先进入sleep模式
	wiringPiI2CWriteReg8(pca9685, PCA9685_MODE1, newmode);
	wiringPiI2CWriteReg8(pca9685, PCA9685_PRESCALE, prescale);
	oldmode &= 0xef;	//清除sleep位
	wiringPiI2CWriteReg8(pca9685, PCA9685_MODE1, oldmode);
	wiringPiI2CWriteReg8(pca9685, PCA9685_MODE1, oldmode | 0xa1);
    
    return pca9685;
}

void pca9685_setmk(int num, int mk)	//设置指定通道的脉宽。fd是在pca9685_init时获得的设备描述符，num是通道号（从0开始），mk是脉宽单位是us。周期已经固定为20ms了
{
	unsigned int on, off;

	on = 0;	//每次周期一开始就输出高电平
	off = (unsigned int)((((double)mk)/20000 * 4096)*1.0067114);	//最后的1.0067114是校准用的

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
void main()  
{  
	int value=1;
        int addr = 0;  
        char *filename = "/dev/i2c-0";  
        char wbuf[] = {0x20, 0x0f}; //first byte is address to write. others are bytes to be written  
        char read_start_buf[1];   
        char rbuf[1000] = {0};  
	printf("pzx i2c\n");
        if((file = open(filename, O_RDWR)) < 0)  
        {  
                perror("Failed to open i2c device.\n");  
                exit(1);  
        }  
	else
	{
		debug("open i2c device ok\n");  
	}

	for(addr =0;addr< 0x7f;addr++)
	{
		if( ioctl(file, I2C_SLAVE, addr) < 0)//设置从机地址
		{
			printf("error!\n");
		}  
		if( write( file, &value, 1 ) == 1) 
		{
            		debug("find drive 0x%x\n", addr);
			pzx_addr = addr;//获取设备地址（忽略0x70 超级地址）
			break;
		}		
	}

	printf_all_register();
   
	pca9685_init(pzx_addr);

//	wiringPiI2CWriteReg8(0x40,0x8,200);
	wiringPiI2CWriteReg16(0x40,0x8,300);
	wiringPiI2CReadReg16 (0x40,0x8);


	while(1)//500 ~ 2500 ms
	{
		for(i = 700;i<2300;)
		{
			pca9685_setmk(0,i);    
			pca9685_setmk(7,i); 
			pca9685_setmk(15,i); 
			usleep(10000);
			i+=50;
		}
		for(i = 2300;i>700;)
                { 
                        pca9685_setmk(0,i);
			pca9685_setmk(7,i); 
                        pca9685_setmk(15,i);     
                        usleep(10000);
			i-=50;
                }
//			pca9685_setmk(3,i);		
//		wiringPiI2CWriteReg16(0x40,0x8,300);
//		usleep(500000);
//pca9685_setmk(4,3000);
//		wiringPiI2CWriteReg16(0x40,0x8,200);
//		usleep(500000);
	}




        close(file);  
}  

