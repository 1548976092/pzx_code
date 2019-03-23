#include <stdio.h>  
#include <stdlib.h>   //exit()  
#include <fcntl.h>    //define O_RDWR  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>  
  
void main()  
{  
        int file,i;  
        int value;
        int addr = 0; //i2c device address of gyro  
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
                printf("open i2c device ok\n");  
        }

        for(addr =0;addr< 0x7f;addr++)
        {
                if( ioctl(file, I2C_SLAVE, addr) < 0)
                {
                        printf("error!\n");
                }  
                if( write( file, &value, 1 ) == 1) 
                        printf("find drive 0x%x\n", addr); 

        }

  
        close(file);  
}  




