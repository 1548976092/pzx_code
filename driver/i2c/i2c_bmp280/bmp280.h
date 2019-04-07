#ifndef __BMP280_H__  
#define __BMP280_H__  
 
  
#define  uint8_t unsigned char
#define   int8_t  	  char
 
#define uint16_t unsigned int
#define  int16_t  	  int

#define uint32_t unsigned long
#define  int32_t  	      long

#define doubledouble double



#define BMP280_ADDRESS 0xEC  
  
#define BMP280_RESET_VALUE 0xB6  
  
/*calibration parameters */  
#define BMP280_DIG_T1_LSB_REG                0x88  
#define BMP280_DIG_T1_MSB_REG                0x89  
#define BMP280_DIG_T2_LSB_REG                0x8A  
#define BMP280_DIG_T2_MSB_REG                0x8B  
#define BMP280_DIG_T3_LSB_REG                0x8C  
#define BMP280_DIG_T3_MSB_REG                0x8D  
#define BMP280_DIG_P1_LSB_REG                0x8E  
#define BMP280_DIG_P1_MSB_REG                0x8F  
#define BMP280_DIG_P2_LSB_REG                0x90  
#define BMP280_DIG_P2_MSB_REG                0x91  
#define BMP280_DIG_P3_LSB_REG                0x92  
#define BMP280_DIG_P3_MSB_REG                0x93  
#define BMP280_DIG_P4_LSB_REG                0x94  
#define BMP280_DIG_P4_MSB_REG                0x95  
#define BMP280_DIG_P5_LSB_REG                0x96  
#define BMP280_DIG_P5_MSB_REG                0x97  
#define BMP280_DIG_P6_LSB_REG                0x98  
#define BMP280_DIG_P6_MSB_REG                0x99  
#define BMP280_DIG_P7_LSB_REG                0x9A  
#define BMP280_DIG_P7_MSB_REG                0x9B  
#define BMP280_DIG_P8_LSB_REG                0x9C  
#define BMP280_DIG_P8_MSB_REG                0x9D  
#define BMP280_DIG_P9_LSB_REG                0x9E  
#define BMP280_DIG_P9_MSB_REG                0x9F  
  
#define BMP280_CHIPID_REG                    0xD0  /*Chip ID Register */  
#define BMP280_RESET_REG                     0xE0  /*Softreset Register */  
#define BMP280_STATUS_REG                    0xF3  /*Status Register */  
#define BMP280_CTRLMEAS_REG                  0xF4  /*Ctrl Measure Register */  
#define BMP280_CONFIG_REG                    0xF5  /*Configuration Register */  
#define BMP280_PRESSURE_MSB_REG              0xF7  /*Pressure MSB Register */  
#define BMP280_PRESSURE_LSB_REG              0xF8  /*Pressure LSB Register */  
#define BMP280_PRESSURE_XLSB_REG             0xF9  /*Pressure XLSB Register */  
#define BMP280_TEMPERATURE_MSB_REG           0xFA  /*Temperature MSB Reg */  
#define BMP280_TEMPERATURE_LSB_REG           0xFB  /*Temperature LSB Reg */  
#define BMP280_TEMPERATURE_XLSB_REG          0xFC  /*Temperature XLSB Reg */  
  
/* 在foreced mode下，1s的采样周期，温度和气压使用最低的精度采集并且使用最小的滤波器系数, 
 * 数据的采集时间大概在6ms，平均功率为3.27uA。 
 * */  
  
/* 在foreced mode下，1s的采样周期, 温度和气压使用最高的精度采集并且使用最大的滤波器系数， 
 * 数据的采集时间大概在70ms，平均功率为30uA。 
 * */  
  
typedef enum {  
    BMP280_T_MODE_SKIP = 0x0,   /*skipped*/  
    BMP280_T_MODE_1,            /*x1*/  
    BMP280_T_MODE_2,            /*x2*/  
    BMP280_T_MODE_3,            /*x4*/  
    BMP280_T_MODE_4,            /*x8*/  
    BMP280_T_MODE_5             /*x16*/  
} BMP280_T_OVERSAMPLING;  
  
typedef enum {  
    BMP280_SLEEP_MODE = 0x0,  
    BMP280_FORCED_MODE,  
    BMP280_NORMAL_MODE  
} BMP280_WORK_MODE;  
  
typedef enum {  
    BMP280_P_MODE_SKIP = 0x0,   /*skipped*/  
    BMP280_P_MODE_1,            /*x1*/  
    BMP280_P_MODE_2,            /*x2*/  
    BMP280_P_MODE_3,            /*x4*/  
    BMP280_P_MODE_4,            /*x8*/  
    BMP280_P_MODE_5             /*x16*/  
} BMP280_P_OVERSAMPLING;  
  
typedef enum {  
    BMP280_FILTER_OFF = 0x0,    /*filter off*/  
    BMP280_FILTER_MODE_1,       /*0.223*ODR*/  
    BMP280_FILTER_MODE_2,       /*0.092*ODR*/  
    BMP280_FILTER_MODE_3,       /*0.042*ODR*/  
    BMP280_FILTER_MODE_4        /*0.021*ODR*/  
} BMP280_FILTER_COEFFICIENT;  
  
typedef enum {  
    BMP280_T_SB1 = 0x0,     /*0.5ms*/  
    BMP280_T_SB2,           /*62.5ms*/  
    BMP280_T_SB3,           /*125ms*/  
    BMP280_T_SB4,           /*250ms*/  
    BMP280_T_SB5,           /*500ms*/  
    BMP280_T_SB6,           /*1000ms*/  
    BMP280_T_SB7,           /*2000ms*/  
    BMP280_T_SB8,           /*4000ms*/  
} BMP280_T_SB;  
  
struct bmp280 {  
    uint8_t I2cHandle;  
	uint8_t bmp280_id;  
    /* T1~P9 为补偿系数 */  
    uint16_t T1;  
    int16_t T2;  
    int16_t T3;  
    uint16_t P1;  
    int16_t P2;  
    int16_t P3;  
    int16_t P4;  
    int16_t P5;  
    int16_t P6;  
    int16_t P7;  
    int16_t P8;  
    int16_t P9;  
    int32_t t_fine;  
    uint8_t t_sb;  
    uint8_t mode;  
    uint8_t t_oversampling;  
    uint8_t p_oversampling;  
    uint8_t filter_coefficient;  
};  
  
extern struct bmp280 *bmp280_init(char *filename);  
  
extern void bmp280_reset(struct bmp280 *bmp280);  
  
extern void bmp280_set_standby_time(struct bmp280 *bmp280, BMP280_T_SB t_standby);  
  
extern void bmp280_set_work_mode(struct bmp280 *bmp280, BMP280_WORK_MODE mode);  
  
extern void bmp280_set_temperature_oversampling_mode(struct bmp280 *bmp280, BMP280_T_OVERSAMPLING t_osl);  
  
extern void bmp280_set_pressure_oversampling_mode(struct bmp280 *bmp280, BMP280_P_OVERSAMPLING p_osl);  
  
extern void bmp280_set_filter_mode(struct bmp280 *bmp280, BMP280_FILTER_COEFFICIENT f_coefficient);  
  
extern double bmp280_get_temperature(struct bmp280 *bmp280);  
  
extern double bmp280_get_pressure(struct bmp280 *bmp280);  
  
extern void bmp280_get_temperature_and_pressure(struct bmp280 *bmp280, doubledouble *temperature, doubledouble *pressure);  
  
extern void bmp280_forced_mode_get_temperature_and_pressure(struct bmp280 *bmp280, doubledouble *temperature, doubledouble *pressure);  
  
extern void bmp280_demo(char *filename, doubledouble *temperature, doubledouble *pressure);  
  
#endif  
