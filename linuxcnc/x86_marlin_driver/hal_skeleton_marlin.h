#include <unistd.h>   
#include <stdio.h>   
#include <termios.h>   
#include <fcntl.h>   
#include <string.h>   
#include <time.h>   
#include <sys/select.h>  
//为了保证用户输入的波特率是个正确的值，所以需要这两个数组验证，对于设置波特率时候，前面要加个B   
int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300,  
    B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };  
  
int name_arr[] = {115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300,  
    115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, };  
  
/*----------------------------------------------------------------------------- 
  函数名:      set_speed 
  参数:        int fd ,int speed 
  返回值:      void 
  描述:        设置fd表述符的串口波特率 
 *-----------------------------------------------------------------------------*/  
void set_speed(int fd ,int speed)  
{  
    struct termios opt;  
    int i;  
    int status;  
  
    tcgetattr(fd,&opt);  
    for(i = 0;i < sizeof(speed_arr)/sizeof(int);i++)  
    {  
        if(speed == name_arr[i])                        //找到标准的波特率与用户一致   
        {  
            tcflush(fd,TCIOFLUSH);                      //清除IO输入和输出缓存   
            cfsetispeed(&opt,speed_arr[i]);         //设置串口输入波特率   
            cfsetospeed(&opt,speed_arr[i]);         //设置串口输出波特率   
  
            status = tcsetattr(fd,TCSANOW,&opt);    //将属性设置到opt的数据结构中，并且立即生效   
            if(status != 0)  
                perror("tcsetattr fd:");                //设置失败   
            return ;  
        }  
        tcflush(fd,TCIOFLUSH);                          //每次清除IO缓存   
    }  
}  
/*----------------------------------------------------------------------------- 
  函数名:      set_parity 
  参数:        int fd 
  返回值:      int 
  描述:        设置fd表述符的奇偶校验 
 *-----------------------------------------------------------------------------*/  
int set_parity(int fd)  
{  
    struct termios opt;  
  
    if(tcgetattr(fd,&opt) != 0)                 //或许原先的配置信息   
    {  
        perror("Get opt in parity error:");  
        return -1;  
    }  
  
    /*通过设置opt数据结构，来配置相关功能，以下为八个数据位，不使能奇偶校验*/  
    opt.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP  
                | INLCR | IGNCR | ICRNL | IXON);  
    opt.c_oflag &= ~OPOST;  
    opt.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);  
    opt.c_cflag &= ~(CSIZE | PARENB);  
    opt.c_cflag |= CS8;  
  
    tcflush(fd,TCIFLUSH);                           //清空输入缓存   
  
    if(tcsetattr(fd,TCSANOW,&opt) != 0)  
    {  
        perror("set attr parity error:");  
        return -1;  
    }  
  
    return 0;  
}  
/*----------------------------------------------------------------------------- 
  函数名:      serial_init 
  参数:        char *dev_path,int speed,int is_block 
  返回值:      初始化成功返回打开的文件描述符 
  描述:        串口初始化，根据串口文件路径名，串口的速度，和串口是否阻塞,block为1表示阻塞 
 *-----------------------------------------------------------------------------*/  
int serial_init(char *dev_path,int speed,int is_block)  
{  
    int fd;  
    int flag;  
  
    flag = 0;  
    flag |= O_RDWR;                     //设置为可读写的串口属性文件   
    if(is_block == 0)  
        flag |=O_NONBLOCK;              //若为0则表示以非阻塞方式打开   
  
    fd = open(dev_path,flag);               //打开设备文件   
    if(fd < 0)  
    {  
        perror("Open device file err:");  
        close(fd);  
        return -1;  
    }  
  
    /*打开设备文件后，下面开始设置波特率*/  
    set_speed(fd,speed);                //考虑到波特率可能被单独设置，所以独立成函数   
  
    /*设置奇偶校验*/  
    if(set_parity(fd) != 0)  
    {  
        perror("set parity error:");  
        close(fd);                      //一定要关闭文件，否则文件一直为打开状态   
        return -1;  
    }  
  
    return fd;  
}  
/*----------------------------------------------------------------------------- 
  函数名:      serial_send 
  参数:        int fd,char *str,unsigned int len 
  返回值:      发送成功返回发送长度，否则返回小于0的值 
  描述:        向fd描述符的串口发送数据，长度为len，内容为str 
 *-----------------------------------------------------------------------------*/  
int serial_send(int fd,char *str,unsigned int len)  
{  
    int ret;  
  
    if(len > strlen(str))                    //判断长度是否超过str的最大长度   
        len = strlen(str);  
  
    ret = write(fd,str,len);  
    if(ret < 0)  
    {  
        perror("serial send err:");  
        return -1;  
    }  
  
    return ret;  
}  
  
/*----------------------------------------------------------------------------- 
  函数名:      serial_read 
  参数:        int fd,char *str,unsigned int len,unsigned int timeout 
  返回值:      在规定的时间内读取数据，超时则退出，超时时间为ms级别 
  描述:        向fd描述符的串口接收数据，长度为len，存入str，timeout 为超时时间 
 *-----------------------------------------------------------------------------*/  
int serial_read(int fd, char *str, unsigned int len, unsigned int timeout)  
{  
    fd_set rfds;  
    struct timeval tv;  
    int ret;                                //每次读的结果   
    int sret;                               //select监控结果   
    int readlen = 0;                        //实际读到的字节数   
    char * ptr;  
  
    ptr = str;                          //读指针，每次移动，因为实际读出的长度和传入参数可能存在差异   
  
    FD_ZERO(&rfds);                     //清除文件描述符集合   
    FD_SET(fd,&rfds);                   //将fd加入fds文件描述符，以待下面用select方法监听   
  
    /*传入的timeout是ms级别的单位，这里需要转换为struct timeval 结构的*/  
    tv.tv_sec  = timeout / 1000;  
    tv.tv_usec = (timeout%1000)*1000;  
  
    /*防止读数据长度超过缓冲区*/  
    //if(sizeof(&str) < len)   
    //  len = sizeof(str);   
  
  
    /*开始读*/  
    while(readlen < len)  
    {  
        sret = select(fd+1,&rfds,NULL,NULL,&tv);        //检测串口是否可读   
 
        if(sret == -1)                              //检测失败   
        {  
            perror("select:");  
            break;  
        }  
        else if(sret > 0)                       
        {
            ret = read(fd,ptr,1);  
            if(ret < 0)  
            {  
                perror("read err:");  
                break;  
            }  
            else if(ret == 0)  
                break;  

if(*ptr == '\n') break; 


            readlen += ret;                             //更新读的长度   
            ptr     += ret;                             //更新读的位置   
	   
        }  
        else                                                    //超时   
        {  
            printf("timeout!\n");  
            break;  
        }  
    }  
  
    return readlen;  
}  

