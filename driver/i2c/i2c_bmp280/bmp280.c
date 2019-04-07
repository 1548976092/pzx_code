//#define debug printf
#define debug

#include <stdlib.h>  
#include "bmp280.h"
#include <stdio.h>  
#include <stdlib.h>   //exit()  
#include <fcntl.h>    //define O_RDWR  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>  
#include <unistd.h>

#define dig_T1 bmp280->T1  
#define dig_T2 bmp280->T2  
#define dig_T3 bmp280->T3  
#define dig_P1 bmp280->P1  
#define dig_P2 bmp280->P2  
#define dig_P3 bmp280->P3  
#define dig_P4 bmp280->P4  
#define dig_P5 bmp280->P5  
#define dig_P6 bmp280->P6  
#define dig_P7 bmp280->P7  
#define dig_P8 bmp280->P8  
#define dig_P9 bmp280->P9  
  
int value=1;
int addr = 0; 
uint8_t I2cHandle;//address

int file;
char *filename = "/dev/i2c-0";  
//#define I2cHandle 0x76




static uint8_t bmp280_read_register(uint8_t Bmp280_I2cHandle, uint8_t reg_addr)  
{  
    uint8_t reg_data;  

	if( ioctl(file, I2C_SLAVE, Bmp280_I2cHandle) < 0)//设置从机地址
	{
	       printf("error!\n");
	}  

	write( file, &reg_addr,     1);
	read(  file, &reg_data     ,1); 
	debug("device addr:0x%x ,read-> reg:0x%x:0x%x\n",Bmp280_I2cHandle,reg_addr,reg_data); 
	return reg_data;
}  
  
static void bmp280_write_register(uint8_t Bmp280_I2cHandle, uint8_t reg_addr, uint8_t reg_data)  
{  
	uint8_t tx_data[2];

	tx_data[0] =  reg_addr;
    tx_data[1] =  reg_data;

	debug("device addr:0x%x ,write-> reg:0x%x:0x%x\n",Bmp280_I2cHandle,reg_addr,reg_data);

        if( ioctl(file, I2C_SLAVE, Bmp280_I2cHandle) < 0)//设置从机地址
        {
               printf("error!\n");
        }  

        write( file, tx_data      ,2);
}  
  
/** 
 * 在bmp280_init()函数里默认初始化t_standby为0.5ms， 
 * 温度和气压的采样精度设为最低， 
 * 滤波器系数设为最低， 
 * 并且进入sleep mode。 
 */  
struct bmp280 *bmp280_init(char *filename)  
{  
    uint8_t bmp280_id;  
    uint8_t lsb, msb;  
    uint8_t ctrlmeas_reg, config_reg;  
    struct bmp280 *bmp280;  


	
	printf("pzx i2c\n");
    if((file = open(filename, O_RDWR)) < 0)  
    {  
            printf("Failed to open i2c device.\n");  
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
			I2cHandle = addr;//获取设备地址（first）
			break;
		}		
	}

    bmp280_id = bmp280_read_register(I2cHandle, BMP280_CHIPID_REG);  
    if(bmp280_id == 0x58) {  
        bmp280 = malloc(sizeof(struct bmp280));  
  		
		bmp280->bmp280_id = bmp280_id;
        bmp280->I2cHandle = I2cHandle;  
        bmp280->mode = BMP280_SLEEP_MODE;  
        bmp280->t_sb = BMP280_T_SB1;  
        bmp280->p_oversampling = BMP280_P_MODE_1;  
        bmp280->t_oversampling = BMP280_T_MODE_1;  
        bmp280->filter_coefficient = BMP280_FILTER_MODE_1;  
    } else {  
        printf("Read BMP280 id error!\r\n");  
        return NULL;  
    }  
  
    /* read the temperature calibration parameters */  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_T1_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_T1_MSB_REG);  
    dig_T1 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_T2_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_T2_MSB_REG);  
    dig_T2 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_T3_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_T3_MSB_REG);  
    dig_T3 = msb << 8 | lsb;  
  
    /* read the pressure calibration parameters */  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P1_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P1_MSB_REG);  
    dig_P1 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P2_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P2_MSB_REG);  
    dig_P2 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P3_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P3_MSB_REG);  
    dig_P3 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P4_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P4_MSB_REG);  
    dig_P4 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P5_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P5_MSB_REG);  
    dig_P5 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P6_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P6_MSB_REG);  
    dig_P6 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P7_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P7_MSB_REG);  
    dig_P7 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P8_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P8_MSB_REG);  
    dig_P8 = msb << 8 | lsb;  
    lsb = bmp280_read_register(I2cHandle, BMP280_DIG_P9_LSB_REG);  
    msb = bmp280_read_register(I2cHandle, BMP280_DIG_P9_MSB_REG);  
    dig_P9 = msb << 8 | lsb;  
  
    bmp280_reset(bmp280);  
  
    ctrlmeas_reg = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;  
    config_reg = bmp280->t_sb << 5 | bmp280->filter_coefficient << 2;  
  
    bmp280_write_register(I2cHandle, BMP280_CTRLMEAS_REG, ctrlmeas_reg);  
    bmp280_write_register(I2cHandle, BMP280_CONFIG_REG, config_reg);  
  
    usleep(200000);  
  
    return  bmp280;  
}  
  
void bmp280_reset(struct bmp280 *bmp280)  
{  
	debug("bmp280_reset\t\n");
    bmp280_write_register(bmp280->I2cHandle, BMP280_RESET_REG, BMP280_RESET_VALUE);  
}  
  
void bmp280_set_standby_time(struct bmp280 *bmp280, BMP280_T_SB t_standby)  
{  
    uint8_t config_reg;  
  
    bmp280->t_sb = t_standby;  
    config_reg = bmp280->t_sb << 5 | bmp280->filter_coefficient << 2;  
  
    bmp280_write_register(bmp280->I2cHandle, BMP280_CONFIG_REG, config_reg);  
}  
  
void bmp280_set_work_mode(struct bmp280 *bmp280, BMP280_WORK_MODE mode)  
{  
    uint8_t ctrlmeas_reg;  
  
    bmp280->mode = mode;  
    ctrlmeas_reg = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;  
  
    bmp280_write_register(bmp280->I2cHandle, BMP280_CTRLMEAS_REG, ctrlmeas_reg);  
}  
  
void bmp280_set_temperature_oversampling_mode(struct bmp280 *bmp280, BMP280_T_OVERSAMPLING t_osl)  
{  
    uint8_t ctrlmeas_reg;  
  
    bmp280->t_oversampling = t_osl;  
    ctrlmeas_reg = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;  
  
    bmp280_write_register(bmp280->I2cHandle, BMP280_CTRLMEAS_REG, ctrlmeas_reg);  
}  
  
void bmp280_set_pressure_oversampling_mode(struct bmp280 *bmp280, BMP280_P_OVERSAMPLING p_osl)  
{  
    uint8_t ctrlmeas_reg;  
  
    bmp280->t_oversampling = p_osl;  
    ctrlmeas_reg = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;  
  
    bmp280_write_register(bmp280->I2cHandle, BMP280_CTRLMEAS_REG, ctrlmeas_reg);  
}  
  
void bmp280_set_filter_mode(struct bmp280 *bmp280, BMP280_FILTER_COEFFICIENT f_coefficient)  
{  
    uint8_t config_reg;  
  
    bmp280->filter_coefficient = f_coefficient;  
    config_reg = bmp280->t_sb << 5 | bmp280->filter_coefficient << 2;  
  
    bmp280_write_register(bmp280->I2cHandle, BMP280_CONFIG_REG, config_reg);  
}  
  
/* Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC. */  
static double bmp280_compensate_temperature_double(struct bmp280 *bmp280, int32_t adc_T)  
{  
    double var1, var2, temperature;  
  
    var1 = (((double) adc_T) / 16384.0 - ((double) dig_T1) / 1024.0)  
            * ((double) dig_T2);  
    var2 = ((((double) adc_T) / 131072.0 - ((double) dig_T1) / 8192.0)  
            * (((double) adc_T) / 131072.0 - ((double) dig_T1) / 8192.0))  
            * ((double) dig_T3);  
    bmp280->t_fine = (int32_t) (var1 + var2);  
    temperature = (var1 + var2) / 5120.0;  
  
    return temperature;  
}  
  
  
/* Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa */  
static double bmp280_compensate_pressure_double(struct bmp280 *bmp280, int32_t adc_P)  
{  
    double var1, var2, pressure;  
  
    var1 = ((double) bmp280->t_fine / 2.0) - 64000.0;  
    var2 = var1 * var1 * ((double) dig_P6) / 32768.0;  
    var2 = var2 + var1 * ((double) dig_P5) * 2.0;  
    var2 = (var2 / 4.0) + (((double) dig_P4) * 65536.0);  
    var1 = (((double) dig_P3) * var1 * var1 / 524288.0  
            + ((double) dig_P2) * var1) / 524288.0;  
    var1 = (1.0 + var1 / 32768.0) * ((double) dig_P1);  
  
    if (var1 == 0.0) {  
        return 0; // avoid exception caused by division by zero  
    }  
  
    pressure = 1048576.0 - (double) adc_P;  
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;  
    var1 = ((double) dig_P9) * pressure * pressure / 2147483648.0;  
    var2 = pressure * ((double) dig_P8) / 32768.0;  
    pressure = pressure + (var1 + var2 + ((double) dig_P7)) / 16.0;  
  
    return pressure;  
}  
   
  
/* Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC. */  
double bmp280_get_temperature(struct bmp280 *bmp280)  
{  
    uint8_t lsb, msb, xlsb;  
    int32_t adc_T;  
    double temperature;  
  
    xlsb = bmp280_read_register(bmp280->I2cHandle, BMP280_TEMPERATURE_XLSB_REG);  
    lsb = bmp280_read_register(bmp280->I2cHandle, BMP280_TEMPERATURE_LSB_REG);  
    msb = bmp280_read_register(bmp280->I2cHandle, BMP280_TEMPERATURE_MSB_REG);  
  
    adc_T = (msb << 12) | (lsb << 4) | (xlsb >> 4);  
    temperature = bmp280_compensate_temperature_double(bmp280, adc_T);  
  
    return temperature;  
}  
  
/* Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa */  
double bmp280_get_pressure(struct bmp280 *bmp280)  
{  
    uint8_t lsb, msb, xlsb;  
    int32_t adc_P;  
    double pressure;  
  
  
    xlsb = bmp280_read_register(bmp280->I2cHandle, BMP280_PRESSURE_XLSB_REG);  
    lsb = bmp280_read_register(bmp280->I2cHandle, BMP280_PRESSURE_LSB_REG);  
    msb = bmp280_read_register(bmp280->I2cHandle, BMP280_PRESSURE_MSB_REG);  
  
    adc_P = (msb << 12) | (lsb << 4) | (xlsb >> 4);  
    pressure = bmp280_compensate_pressure_double(bmp280, adc_P);  
  
    return pressure;  
}  
  
/** 
 * 仅在BMP280被设置为normal mode后， 
 * 可使用该接口直接读取温度和气压。 
 */  
void bmp280_get_temperature_and_pressure(struct bmp280 *bmp280, doubledouble *temperature, doubledouble *pressure)  
{  
    *temperature = bmp280_get_temperature(bmp280);  
    *pressure = bmp280_get_pressure(bmp280);  
}  
  
/** 
 * 当BMP280被设置为forced mode后， 
 * 可使用该接口直接读取温度和气压。 
 */  
void bmp280_forced_mode_get_temperature_and_pressure(struct bmp280 *bmp280, doubledouble *temperature, doubledouble *pressure)  
{  
    bmp280_set_work_mode(bmp280, BMP280_FORCED_MODE);  
  
    usleep(1000000);  
  
    bmp280_get_temperature_and_pressure(bmp280, temperature, pressure);  
}  
  
/** 
 * 此demo使用forced mode以1s为周期， 
 * 对温度和气压进行数据采集并打印。 
 */  
void bmp280_demo(char *filename, doubledouble *temperature, doubledouble *pressure)  
{  
    struct bmp280 *bmp280;  
    bmp280 = bmp280_init(filename);  
  
    if(bmp280 != NULL) {  
        while(1) {  
            bmp280_forced_mode_get_temperature_and_pressure(bmp280, temperature, pressure);  
            printf("temperature=%ld   pressure=%ld\r\n", (int32_t)*temperature, (uint32_t)*pressure);  
  
           usleep(1000000);  
        }  
    } else  
        printf("create bmp280 error!\r\n");  
}  




void main(void)
{
	struct bmp280 *bmp280;
	uint8_t bmp280_id = 0;  
    uint8_t ctr_reg = 0;  
    uint8_t status_reg = 0;  
  
    double tem = 0;  
    double pressure = 0;  
  
    bmp280 = bmp280_init(filename);  
	debug("bmp280_id:0x%x\t\n",bmp280->bmp280_id);

	if(bmp280->bmp280_id== 0x58) {  
		bmp280_reset(bmp280);  

		ctr_reg = bmp280_read_register(I2cHandle, 0xF4);  

		debug("ctr_reg1111===0x%x\r\n", ctr_reg);  

		bmp280_write_register(I2cHandle, 0xF4, 0xFF);  

		bmp280_write_register(I2cHandle, 0xF5, 0x14);  

		usleep(1000);   

		while(1) {  
			ctr_reg = bmp280_read_register(I2cHandle, 0xF4);  

			debug("ctr_reg2222===0x%x\r\n", ctr_reg);  

			status_reg = bmp280_read_register(I2cHandle, 0xF3);  
			debug("status == 0x%x\r\n", status_reg);  


			tem = bmp280_get_temperature(bmp280);  
			pressure = bmp280_get_pressure(bmp280);  

			printf("bmp280_id = 0x%x  tem=%f    pressure=%f   \r\n",bmp280->bmp280_id, tem, pressure);  

			usleep(1000000);   
		}  
	}
	return;
}
