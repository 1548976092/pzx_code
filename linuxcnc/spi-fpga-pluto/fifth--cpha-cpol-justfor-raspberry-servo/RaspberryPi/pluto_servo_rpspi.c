/* Autogenerated by /home/pi/new/linuxcnc-master/bin/halcompile on Sat Nov  9 09:45:08 2019 -- do not edit */
#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"

#define u32 unsigned int 
#define __u32 unsigned int
#define u16 unsigned short
#define __u16 unsigned short

static int comp_id;

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "component:pluto_servo_rpspi:Hardware driver and firmware for the Pluto-P parallel-port FPGA, for use with servo machines and RaspberryPi SPI .");
MODULE_INFO(linuxcnc, "descr:\nPluto_servo is a LinuxCNC software driver and associated firmware that allow the Pluto-P board to be used to control a servo-based CNC machine.\n");
MODULE_INFO(linuxcnc, "pin:encoder.#.count:s32:4:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.position:float:4:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.velocity:float:4:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.reset:bit:4:in::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.index-enable:bit:4:io::None:None");
MODULE_INFO(linuxcnc, "param:encoder.#.scale:float:4:rw::1.0:None");
MODULE_INFO(linuxcnc, "param:encoder.z-polarity:bit:0:rw:Set to TRUE if the index pulse is active low, FALSE if it is active high.  Affects all encoders.:None:None");
MODULE_INFO(linuxcnc, "pin:pwm.#.value:float:4:in::None:None");
MODULE_INFO(linuxcnc, "pin:pwm.#.enable:bit:4:in:pwm.\\fIM\\fR corresponds to the pins labeled UP\\fIM\\fR and DN\\fIM\\fR on the pinout diagram:None:None");
MODULE_INFO(linuxcnc, "param:pwm.#.offset:float:4:rw::None:None");
MODULE_INFO(linuxcnc, "param:pwm.#.scale:float:4:rw::1.0:None");
MODULE_INFO(linuxcnc, "param:pwm.#.max-dc:float:4:rw::1.0:None");
MODULE_INFO(linuxcnc, "param:pwm.#.min-dc:float:4:rw::0:None");
MODULE_INFO(linuxcnc, "param:pwm.#.pwmdir:bit:4:rw:Set to TRUE use PWM+direction mode.  Set to FALSE to use Up/Down mode.:0:None");
MODULE_INFO(linuxcnc, "param:pwm.is-pdm:bit:0:rw:Set to TRUE to use PDM (also called interleaved PWM) mode.  Set to FALSE to use traditional PWM mode.  Affects all PWM outputs.:None:None");
MODULE_INFO(linuxcnc, "pin:dout.##:bit:18:in::None:None");
MODULE_INFO(linuxcnc, "param:dout.##-invert:bit:18:rw:If TRUE, the output on the corresponding \\fBdout.\\fIMM\\fR is inverted.:None:None");
MODULE_INFO(linuxcnc, "pin:din.##:bit:20:out::None:None");
MODULE_INFO(linuxcnc, "pin:din.##_not:bit:20:out::None:None");
MODULE_INFO(linuxcnc, "param:debug_0:s32:0:rw::None:None");
MODULE_INFO(linuxcnc, "param:debug_1:s32:0:rw:These parameters can display values which are useful to developers or for debugging the driver and firmware.  They are not useful for integrators or users.:None:None");
MODULE_INFO(linuxcnc, "see_also:The \\fIpluto_servo\\fR section in the HAL User Manual, which shows the location of each physical pin on the pluto board.");
MODULE_INFO(linuxcnc, "funct:transfer:1:transfers data between master and slave devices");
MODULE_INFO(linuxcnc, "funct:spitest:1:test spi transfer (only for developers)");
MODULE_INFO(linuxcnc, "license:GPL");
MODULE_INFO(linuxcnc, "author:Matsche");
MODULE_LICENSE("GPL");
#endif // MODULE_INFO


struct __comp_state {
    struct __comp_state *_next;
    hal_s32_t *encoder_count[4];
    hal_float_t *encoder_position[4];
    hal_float_t *encoder_velocity[4];
    hal_bit_t *encoder_reset[4];
    hal_bit_t *encoder_index_enable[4];
    hal_float_t *pwm_value[4];
    hal_bit_t *pwm_enable[4];
    hal_bit_t *dout[18];
    hal_bit_t *din[20];
    hal_bit_t *din_not[20];
    hal_float_t encoder_scale[4];
    hal_bit_t encoder_z_polarity;
    hal_float_t pwm_offset[4];
    hal_float_t pwm_scale[4];
    hal_float_t pwm_max_dc[4];
    hal_float_t pwm_min_dc[4];
    hal_bit_t pwm_pwmdir[4];
    hal_bit_t pwm_is_pdm;
    hal_bit_t dout_invert[18];
    hal_s32_t debug_0;
    hal_s32_t debug_1;
    void *_data;
};
struct __comp_state *__comp_first_inst=0, *__comp_last_inst=0;

static void transfer(struct __comp_state *__comp_inst, long period);
static void spitest(struct __comp_state *__comp_inst, long period);
static int __comp_get_data_size(void);
static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg);
static void extra_cleanup(void);
#undef TRUE
#define TRUE (1)
#undef FALSE
#define FALSE (0)
#undef true
#define true (1)
#undef false
#define false (0)

static int export(char *prefix, long extra_arg) {
    char buf[HAL_NAME_LEN + 1];
    int r = 0;
    int j = 0;
    int sz = sizeof(struct __comp_state) + __comp_get_data_size();
    struct __comp_state *inst = hal_malloc(sz);
    memset(inst, 0, sz);
    inst->_data = (char*)inst + sizeof(struct __comp_state);
    r = extra_setup(inst, prefix, extra_arg);
    if(r != 0) return r;
    for(j=0; j < (4); j++) {
        r = hal_pin_s32_newf(HAL_OUT, &(inst->encoder_count[j]), comp_id,
            "%s.encoder.%01d.count", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_float_newf(HAL_OUT, &(inst->encoder_position[j]), comp_id,
            "%s.encoder.%01d.position", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_float_newf(HAL_OUT, &(inst->encoder_velocity[j]), comp_id,
            "%s.encoder.%01d.velocity", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_bit_newf(HAL_IN, &(inst->encoder_reset[j]), comp_id,
            "%s.encoder.%01d.reset", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_bit_newf(HAL_IO, &(inst->encoder_index_enable[j]), comp_id,
            "%s.encoder.%01d.index-enable", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_float_newf(HAL_IN, &(inst->pwm_value[j]), comp_id,
            "%s.pwm.%01d.value", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_bit_newf(HAL_IN, &(inst->pwm_enable[j]), comp_id,
            "%s.pwm.%01d.enable", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (18); j++) {
        r = hal_pin_bit_newf(HAL_IN, &(inst->dout[j]), comp_id,
            "%s.dout.%02d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (20); j++) {
        r = hal_pin_bit_newf(HAL_OUT, &(inst->din[j]), comp_id,
            "%s.din.%02d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (20); j++) {
        r = hal_pin_bit_newf(HAL_OUT, &(inst->din_not[j]), comp_id,
            "%s.din.%02d-not", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_param_float_newf(HAL_RW, &(inst->encoder_scale[j]), comp_id,
            "%s.encoder.%01d.scale", prefix, j);
        if(r != 0) return r;
    inst->encoder_scale[j] = 1.0;
    }
    r = hal_param_bit_newf(HAL_RW, &(inst->encoder_z_polarity), comp_id,
        "%s.encoder.z-polarity", prefix);
    if(r != 0) return r;
    for(j=0; j < (4); j++) {
        r = hal_param_float_newf(HAL_RW, &(inst->pwm_offset[j]), comp_id,
            "%s.pwm.%01d.offset", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_param_float_newf(HAL_RW, &(inst->pwm_scale[j]), comp_id,
            "%s.pwm.%01d.scale", prefix, j);
        if(r != 0) return r;
    inst->pwm_scale[j] = 1.0;
    }
    for(j=0; j < (4); j++) {
        r = hal_param_float_newf(HAL_RW, &(inst->pwm_max_dc[j]), comp_id,
            "%s.pwm.%01d.max-dc", prefix, j);
        if(r != 0) return r;
    inst->pwm_max_dc[j] = 1.0;
    }
    for(j=0; j < (4); j++) {
        r = hal_param_float_newf(HAL_RW, &(inst->pwm_min_dc[j]), comp_id,
            "%s.pwm.%01d.min-dc", prefix, j);
        if(r != 0) return r;
    inst->pwm_min_dc[j] = 0;
    }
    for(j=0; j < (4); j++) {
        r = hal_param_bit_newf(HAL_RW, &(inst->pwm_pwmdir[j]), comp_id,
            "%s.pwm.%01d.pwmdir", prefix, j);
        if(r != 0) return r;
    inst->pwm_pwmdir[j] = 0;
    }
    r = hal_param_bit_newf(HAL_RW, &(inst->pwm_is_pdm), comp_id,
        "%s.pwm.is-pdm", prefix);
    if(r != 0) return r;
    for(j=0; j < (18); j++) {
        r = hal_param_bit_newf(HAL_RW, &(inst->dout_invert[j]), comp_id,
            "%s.dout.%02d-invert", prefix, j);
        if(r != 0) return r;
    }
    r = hal_param_s32_newf(HAL_RW, &(inst->debug_0), comp_id,
        "%s.debug-0", prefix);
    if(r != 0) return r;
    r = hal_param_s32_newf(HAL_RW, &(inst->debug_1), comp_id,
        "%s.debug-1", prefix);
    if(r != 0) return r;
    rtapi_snprintf(buf, sizeof(buf), "%s.transfer", prefix);
    r = hal_export_funct(buf, (void(*)(void *inst, long))transfer, inst, 1, 0, comp_id);
    if(r != 0) return r;
    rtapi_snprintf(buf, sizeof(buf), "%s.spitest", prefix);
    r = hal_export_funct(buf, (void(*)(void *inst, long))spitest, inst, 1, 0, comp_id);
    if(r != 0) return r;
    if(__comp_last_inst) __comp_last_inst->_next = inst;
    __comp_last_inst = inst;
    if(!__comp_first_inst) __comp_first_inst = inst;
    return 0;
}
int rtapi_app_main(void) {
    int r = 0;
    comp_id = hal_init("pluto_servo_rpspi");
    if(comp_id < 0) return comp_id;
    r = export("pluto-servo-rpspi", 0);
    if(r) {
    extra_cleanup();
        hal_exit(comp_id);
    } else {
        hal_ready(comp_id);
    }
    return r;
}

void rtapi_app_exit(void) {
    extra_cleanup();
    hal_exit(comp_id);
}

#undef FUNCTION
#define FUNCTION(name) static void name(struct __comp_state *__comp_inst, long period)
#undef EXTRA_SETUP
#define EXTRA_SETUP() static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg)
#undef EXTRA_CLEANUP
#define EXTRA_CLEANUP() static void extra_cleanup(void)
#undef fperiod
#define fperiod (period * 1e-9)
#undef encoder_count
#define encoder_count(i) (*(__comp_inst->encoder_count[i]))
#undef encoder_position
#define encoder_position(i) (*(__comp_inst->encoder_position[i]))
#undef encoder_velocity
#define encoder_velocity(i) (*(__comp_inst->encoder_velocity[i]))
#undef encoder_reset
#define encoder_reset(i) (0+*(__comp_inst->encoder_reset[i]))
#undef encoder_index_enable
#define encoder_index_enable(i) (*(__comp_inst->encoder_index_enable[i]))
#undef pwm_value
#define pwm_value(i) (0+*(__comp_inst->pwm_value[i]))
#undef pwm_enable
#define pwm_enable(i) (0+*(__comp_inst->pwm_enable[i]))
#undef dout
#define dout(i) (0+*(__comp_inst->dout[i]))
#undef din
#define din(i) (*(__comp_inst->din[i]))
#undef din_not
#define din_not(i) (*(__comp_inst->din_not[i]))
#undef encoder_scale
#define encoder_scale(i) (__comp_inst->encoder_scale[i])
#undef encoder_z_polarity
#define encoder_z_polarity (__comp_inst->encoder_z_polarity)
#undef pwm_offset
#define pwm_offset(i) (__comp_inst->pwm_offset[i])
#undef pwm_scale
#define pwm_scale(i) (__comp_inst->pwm_scale[i])
#undef pwm_max_dc
#define pwm_max_dc(i) (__comp_inst->pwm_max_dc[i])
#undef pwm_min_dc
#define pwm_min_dc(i) (__comp_inst->pwm_min_dc[i])
#undef pwm_pwmdir
#define pwm_pwmdir(i) (__comp_inst->pwm_pwmdir[i])
#undef pwm_is_pdm
#define pwm_is_pdm (__comp_inst->pwm_is_pdm)
#undef dout_invert
#define dout_invert(i) (__comp_inst->dout_invert[i])
#undef debug_0
#define debug_0 (__comp_inst->debug_0)
#undef debug_1
#define debug_1 (__comp_inst->debug_1)
#undef data
#define data (*(internal*)(__comp_inst->_data))


#line 75 "pluto_servo_rpspi.comp"

#include "rtapi.h"
#include "rtapi_bitops.h"
#include "rtapi_app.h"
#include "hal.h"

#define BUILD_SYS_USER_DSO
#define TARGET_PLATFORM_RASPBERRY

#if !defined(BUILD_SYS_USER_DSO)
#error "This driver is for usermode threads only"
#endif

#if !defined(TARGET_PLATFORM_RASPBERRY)
#error "This driver is for the Raspberry Pi platform only"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#include "pluto_common_rpspi.h"

volatile unsigned *gpio, *spi;
static volatile u32 txBuf[BUFSIZE], rxBuf[BUFSIZE];
static platform_t platform;

static int map_gpio();
static void setup_gpio();
static void restore_gpio();
static int pluto_program(char *);
platform_t check_platform(void);

typedef struct {
    int64_t last_index[4];
    int64_t last_count[4];
    int64_t reset_count[4];
} internal;

#define W 14
#define MASK ((1<<W)-1)

int PWM(int enable, double value, double offset, double scale, double min_dc, double max_dc, int dio0, int dio1, int is_pdm, int is_pwmdir) {
    int result;
    if(enable == 0) return 0;

    value = value / scale + offset;
    if(value < -max_dc) value = -max_dc;
    else if(value > -min_dc && value < 0) value = -min_dc;
    else if(value > 0 && value < min_dc) value = min_dc;
    else if(value > max_dc) value = max_dc;

    value = 2047 * value;
    if(value < -2047) value = -2047;
    if(value > 2047) value = 2047;

    if(value < 0) {
	    if(is_pwmdir) {
            result = (1<<13) | (int)(-value);
	    } else {
            result = (1<<15) | (int)(-value);
	    }
    } else result = value;
    if(is_pdm) result |= 1<<14;
    if(dio0) result ^= 1<<12;
    if(dio1) result ^= 1<<13;

    return result;
}

EXTRA_SETUP() {
	int i, retval = 0;

	platform = check_platform();
	retval = map_gpio();
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
		        "HAL_pluto_servo_rpspi: ERROR: cannot map GPIO memory\n");
		return retval;
	}

	if(firmware != "")
	{
		retval = pluto_program(firmware);
		if(retval < 0)
		{
			rtapi_print_msg(RTAPI_MSG_ERR,
					"HAL_pluto_servo_rpspi: ERROR: uploading firmware failed!!!\n");
			return retval;
		}
	}
        
	setup_gpio();

	return 0;
}

EXTRA_CLEANUP() {
	// reset the FPGA
	GPIO_CLR = 1<<25;
	usleep(1000);
	GPIO_SET = 1<<25;
	usleep(1000);

	if (gpio != MAP_FAILED && spi != MAP_FAILED) {
		
		restore_gpio();
		munmap((void *)gpio,BLOCK_SIZE);
		munmap((void *)spi,BLOCK_SIZE);
	}
}

FUNCTION(spitest) {
	char *buf;
	int i;
	__u32 spidata;
	
	for (i=0; i<BUFSIZE; i++) {
		txBuf[i] = i;
	}
	
	/* activate transfer */
	BCM2835_SPICS = SPI_CS_TA|SPI_CS_CPHA;

	/* send txBuf */
	/* RPi SPI transfers 8 bits at a time only */
	buf = (char *)txBuf;
	for (i=0; i<SPIBUFSIZE; i++) {
		BCM2835_SPIFIFO = *buf++;
	}

	/* wait until transfer is finished */
	while (!(BCM2835_SPICS & SPI_CS_DONE));

	/* clear DONE bit */
	BCM2835_SPICS = SPI_CS_DONE;

	/*****************************************************************************/
	/* read buffer */
	/* RPi SPI transfers 8 bits at a time only */
	buf = (char *)rxBuf;
	for (i=0; i<SPIBUFSIZE; i++) {
		*buf++ = BCM2835_SPIFIFO;
	}
	
	for (i=5; i<BUFSIZE; i++) {
		spidata = rxBuf[i];
	}
}
	
FUNCTION(transfer) {
	char *buf;
	int i;
	__u32 spidata;
	
	/*
	for (i=0; i<BUFSIZE; i++) {
		txBuf[i] = _tx(i);
	}*/
	int r = 0;
	//if(communication_error) return;

	for(i=0; i<4; i++) {
			if(pwm_max_dc(i) > 1) pwm_max_dc(i) = 1;
			else if(pwm_max_dc(i) < 0) pwm_max_dc(i) = 0;
			if(pwm_min_dc(i) < 0) pwm_min_dc(i) = 0;
			else if(pwm_min_dc(i) > pwm_max_dc(i)) pwm_min_dc(i) = pwm_max_dc(i);
	}

#define D(x) (!dout(x) ^ !dout_invert(x))
#define P(x,y) PWM(pwm_enable(x), pwm_value(x), pwm_offset(x), pwm_scale(x), \
			pwm_min_dc(x), pwm_max_dc(x), D(y), D(y+1), pwm_is_pdm, pwm_pwmdir(x))
	//write32( P(0,10) | (P(1, 12) << 16));
	//write32( P(2,14) | (P(3, 16) << 16));
	txBuf[0] = P(0,10) | (P(1, 12) << 16);
	txBuf[1] = P(2,14) | (P(3, 16) << 16);

	for(i=0; i<10; i++) {
			if(!dout(i) ^ !dout_invert(i)) r |= (1<<i);
	}
	if (encoder_z_polarity) r |= 0x8000;
	//if (watchdog) r |= 0x4000;
	//if (test_encoder) r |= 0x2000;
	//write16(r);
	txBuf[2] = r;
	
	for (i=3; i<BUFSIZE; i++) {
		txBuf[i] = i;
	}
	//filltxbuf();
	
	/* activate transfer */
	BCM2835_SPICS = SPI_CS_TA|SPI_CS_CPHA|SPI_CS_CPOL;

	/* send txBuf */
	/* RPi SPI transfers 8 bits at a time only */
	buf = (char *)txBuf;
	for (i=0; i<SPIBUFSIZE; i++) {
		BCM2835_SPIFIFO = *buf++;
	}

	/* wait until transfer is finished */
	while (!(BCM2835_SPICS & SPI_CS_DONE));

	/* clear DONE bit */
	BCM2835_SPICS = SPI_CS_DONE|SPI_CS_CPHA|SPI_CS_CPOL;

	/* read buffer */
	/* RPi SPI transfers 8 bits at a time only */
	buf = (char *)rxBuf;
	for (i=0; i<SPIBUFSIZE; i++) {
		*buf++ = BCM2835_SPIFIFO;
	}
	/*
	for (i=0; i<BUFSIZE; i++) {
		_rx(i) = rxBuf[i];
	}*/
	
	for(i=0; i<4; i++) {
		int64_t count, index;
		int newlow, indlow, indexed;
		int reset;
		spidata = rxBuf[i];
		reset = encoder_reset(i);
		/*
		if(i == 0) {
				int status = inb(ioaddr+1) & 1;
				if(status) {
						communication_error ++;
				pluto_clear_error_register();
				}
				//if(communication_error) { EPP_DIR_WRITE(); return; }
		} */

		newlow = spidata & MASK;
		indlow = (spidata >> W) & MASK;
		indexed = spidata & (1<<(2*W));

		count = extend(data.last_count[i], newlow, W);
		if(indexed)
				index = extend(count, indlow, W);
		else
				index = data.last_index[i];

		if(encoder_index_enable(i) && indexed) {
				encoder_index_enable(i) = 0;
				data.reset_count[i] = index;
		}
		if(reset) encoder_velocity(i) = 0;
		else encoder_velocity(i) = (count - data.last_count[i]) /
				encoder_scale(i) / fperiod;
		data.last_index[i] = index;
		data.last_count[i] = count;
		if(reset) data.reset_count[i] = count;
		encoder_count(i) = count - data.reset_count[i];
		encoder_position(i) = encoder_count(i) / encoder_scale(i);

		if(i == 0) {
				debug_0 = spidata;
				debug_1 = count;
		}
	}

	spidata = rxBuf[4];

	for(i=0; i< 20; i++) {
			int b = spidata & (1<<i);
			din(i) = !!b;
			din_not(i) = !b;
	}
	
	for (i=5; i<BUFSIZE; i++) {
		spidata = rxBuf[i];
	}
	//getrxbuf(period);
}

int map_gpio() {
	int fd;
	static u32 mem_base, mem_spi_base;

	switch (platform) {
	case RPI:
		mem_base = BCM2835_GPIO_BASE;
		mem_spi_base = BCM2835_SPI_BASE;
		break;
	case RPI_2:
		mem_base = BCM2835_GPIO_BASE + BCM2709_OFFSET;
		mem_spi_base = BCM2835_SPI_BASE + BCM2709_OFFSET;
		break;
	}

	fd = rtapi_open_as_root("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,"HAL_pluto_servo_rpspi: can't open /dev/mem \n");
		return -1;
	}

	/* mmap GPIO */
	gpio = mmap(
	        NULL,
	        BLOCK_SIZE,
	        PROT_READ|PROT_WRITE,
	        MAP_SHARED,
	        fd,
	        mem_base);

	/* mmap SPI */
	spi = mmap(
	        NULL,
	        BLOCK_SIZE,
	        PROT_READ|PROT_WRITE,
	        MAP_SHARED,
	        fd,
	        mem_spi_base);

	close(fd);

	if (gpio == MAP_FAILED) {
		rtapi_print_msg(RTAPI_MSG_ERR,"HAL_pluto_servo_rpspi: can't map gpio\n");
		return -2;
	}

	if (spi == MAP_FAILED) {
		rtapi_print_msg(RTAPI_MSG_ERR,"HAL_pluto_servo_rpspi: can't map spi\n");
		return -3;
	}

	return 0;
}

void setup_gpio() {
	uint32_t x;

	/* change SPI pins */
	x = BCM2835_GPFSEL0;
	//x &= ~(0b111 << (7*3) | 0b111 << (8*3) | 0b111 << (9*3));
	//x |= (0b001 << (7*3) | 0b100 << (8*3) | 0b100 << (9*3));	// GPIO pin7 output, pin8 pin9 alternate0 (SPI)
	x &= ~(0b111 << (8*3) | 0b111 << (9*3));
	x |= (0b100 << (8*3) | 0b100 << (9*3));	// pin8 pin9 alternate0 (SPI)
	BCM2835_GPFSEL0 = x;

	x = BCM2835_GPFSEL1;
	x &= ~(0b111 << (0*3) | 0b111 << (1*3));
	x |= (0b100 << (0*3) | 0b100 << (1*3));	// pin10 pin11 alternate0 (SPI)
	BCM2835_GPFSEL1 = x;

	/* set up SPI */
	BCM2835_SPICLK = 400;

	BCM2835_SPICS = 0;

	/* clear FIFOs */
	BCM2835_SPICS |= SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;

	/* clear done bit */
	BCM2835_SPICS |= SPI_CS_DONE;
}

void restore_gpio() {
	uint32_t x;

	/* change SPI pins to inputs*/
	x = BCM2835_GPFSEL0;
	//x &= ~(0b111 << (7*3) | 0b111 << (8*3) | 0b111 << (9*3));
	x &= ~(0b111 << (8*3) | 0b111 << (9*3));
	BCM2835_GPFSEL0 = x;

	x = BCM2835_GPFSEL1;
	x &= ~(0b111 << (0*3) | 0b111 << (1*3));
	BCM2835_GPFSEL1 = x;
}


#define FIRMWARE_SIZE 19895
static int pluto_program(char *firmware) {
	int byte, bit;
	int i;
	FILE *f;
	int size = 0;
	
	f= fopen(firmware, "rb");
	if(!f) {
		rtapi_print_msg(RTAPI_MSG_ERR, "fopen-Error\n");
		return -EIO;
	}
	if(fseek(f, 0, SEEK_END))
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "fseek-Error!\n");
		return -EIO;
	}
	size = ftell(f);
	rewind(f);
	rtapi_print_msg(RTAPI_MSG_INFO, "Filesize: %d Bytes\n", size);
	if(size != FIRMWARE_SIZE)
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "Incorrect filesize!!\n");
		return -EIO;
	}
	unsigned char buf[size];
	if(fread(buf, 1, size, f) != FIRMWARE_SIZE) {
		rtapi_print_msg(RTAPI_MSG_ERR, "fread\n");
		return -EIO;
	}
	
	/***************************/
	/** Just a extra tuition	**/ 
	uint32_t x;
	/* setup GPIO 25 as OUTPUT connected to Data2 on Pluto (internal nRESET) */
	x = BCM2835_GPFSEL2;		// (25/10)
	x &= ~(0b111 << (5*3));	// (25%10*3)
	x |= (0b001 << (5*3));	// pin25 OUTPUT
	BCM2835_GPFSEL2 = x;
	
	/* setup GPIO 24 as INPUT connected to nPaperOut on Pluto */
	x = BCM2835_GPFSEL2;		// (24/10)
	x &= ~(0b111 << (4*3));	// (24%10*3) pin24 INPUT
	BCM2835_GPFSEL2 = x;
	/***************************/
	
	// Set GPIO pins 8-11 to output for Pluto Programming (CE0, MISO, MOSI, SCKL)
	for (i=8; i<=11; i++)
	{
		INP_GPIO(i); // must use INP_GPIO before we can use OUT_GPIO
		OUT_GPIO(i);
	}
	

	// reset the FPGA if it isn't
	GPIO_CLR = 1<<25;
	usleep(1000);
	GPIO_SET = 1<<25;
	usleep(1000);
	
		rtapi_print("uploading pluto-spi-firmware...\n");

	// pull the reset low -- bit 2 of Control register
	// keep it low 2 microseconds
	for(i=0; i<12; i++) GPIO_CLR = 1<<8;	//outb(0, ioaddr+2);

	// let it go high again
	// delay 10 microseconds to guarantee nStatus high
	for(i=0; i<40; i++) GPIO_SET = 1<<8;	//outb(4, ioaddr+2);

	// Now program the device...
	for(byte = 0; byte < FIRMWARE_SIZE; byte++) {
		for(bit = 0; bit < 8; bit++) {
			int v = buf[byte] & (1<<bit);
			if(v) GPIO_SET = 1<<10; else GPIO_CLR = 1<<10;
			for(i=0; i<22; i++) GPIO_CLR = 1<<11;		//outb(0|4, ioaddr+2);
			
			for(i=0; i<22; i++) GPIO_SET = 1<<11;		//outb(1|4, ioaddr+2);
			
			for(i=0; i<22; i++) GPIO_CLR = 1<<11;
		}
	}
	rtapi_print("...done\n");
}

platform_t check_platform(void)
{
	FILE *fp;
	char buf[2048];
	size_t fsize;

	fp = fopen("/proc/cpuinfo", "r");
	fsize = fread(buf, 1, sizeof(buf), fp);
	fclose(fp);
	
	if (fsize == 0 || fsize == sizeof(buf))
		return 0;

	/* NUL terminate the buffer */
	buf[fsize] = '\0';

	if (NULL != strstr(buf, "BCM2708"))
		return RPI;
	else if (NULL != strstr(buf, "BCM2709"))
		return RPI_2;
	else if (NULL != strstr(buf, "BCM2835"))
                return RPI_2;
	else
		return UNSUPPORTED;
}

static int __comp_get_data_size(void) { return sizeof(internal); }
