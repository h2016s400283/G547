/*
	Kernel program to obtain Accelerometer, Gyroscope, Magnetic Compass and Pressure values from MPU9255 and BMP280
*/

/* Header files */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/version.h>

/* "sensor_params.h" */
#define DRIVER_NAME "imu_char"
#define SENSOR_FILE_NAME "/dev/imu_char"

/* Request codes for the 10 measured values */
#define PARAM_01 1
#define PARAM_02 2
#define PARAM_03 3
#define PARAM_04 4
#define PARAM_05 5
#define PARAM_06 6
#define PARAM_07 7
#define PARAM_08 8
#define PARAM_09 9
#define PARAM_10 10

/* IOCTL request commands for each parameter to be measured */
#define IOCTL_REQ_CMD_PARAM_01_VAL _IOR( MAJOR_NUM, PARAM_01, unsigned long)
#define IOCTL_REQ_CMD_PARAM_02_VAL _IOR( MAJOR_NUM, PARAM_02, unsigned long)
#define IOCTL_REQ_CMD_PARAM_03_VAL _IOR( MAJOR_NUM, PARAM_03, unsigned long)
#define IOCTL_REQ_CMD_PARAM_04_VAL _IOR( MAJOR_NUM, PARAM_04, unsigned long)
#define IOCTL_REQ_CMD_PARAM_05_VAL _IOR( MAJOR_NUM, PARAM_05, unsigned long)
#define IOCTL_REQ_CMD_PARAM_06_VAL _IOR( MAJOR_NUM, PARAM_06, unsigned long)
#define IOCTL_REQ_CMD_PARAM_07_VAL _IOR( MAJOR_NUM, PARAM_07, unsigned long)
#define IOCTL_REQ_CMD_PARAM_08_VAL _IOR( MAJOR_NUM, PARAM_08, unsigned long)
#define IOCTL_REQ_CMD_PARAM_09_VAL _IOR( MAJOR_NUM, PARAM_09, unsigned long)
#define IOCTL_REQ_CMD_PARAM_10_VAL _IOR( MAJOR_NUM, PARAM_10, unsigned long)

#define I2C_ADDR_MPU_9255 0xD1
#define I2C_ADDR_BMP_280 0x00

/* Register Address for Accelerometer sensor readings */
#define ACCEL_X_HIGH_REG 59
#define ACCEL_X_LOW_REG 60
#define ACCEL_Y_HIGH_REG 61
#define ACCEL_Y_LOW_REG 62
#define ACCEL_Z_HIGH_REG 63
#define ACCEL_Z_LOW_REG 64

/* Register Address for Gyroscope sensor readings */
#define GYRO_X_HIGH_REG 67
#define GYRO_X_LOW_REG 68
#define GYRO_Y_HIGH_REG 69
#define GYRO_Y_LOW_REG 70
#define GYRO_Z_HIGH_REG 71
#define GYRO_Z_LOW_REG 72

/* Register Address for Magnetic compass sensor readings */
#define COMP_X_LOW_REG 0x03
#define COMP_X_HIGH_REG 0x04
#define COMP_Y_LOW_REG 0x05
#define COMP_Y_HIGH_REG 0x06
#define COMP_Z_LOW_REG 0x07
#define COMP_Z_HIGH_REG 0x08

/* Register Address for Pressure sensor readings */
#define PRESS_REG_MSB 0xF7
#define PRESS_REG_LSB 0xF8
#define PRESS_REG_XLSB 0xF9


/* My Device ID */
static dev_t my_device_id;
int MAJOR_NUM = 0, MINOR_NUM = 0;

/* Creating a device node */
static struct class * cls;
static struct cdev my_cdev;

int sensor_measured_value = 0;

/* Function to perform I2C read operation */
int I2C_read ( int i2c_slave_address, int data_reg_address )
{
	return 0xFF;
}

/*	
	1 Bit data for Accelerometer, Gyroscope and Magnetic Compass sensor data in X, Y and Z directions.
	Read the data bytewise using I2C.
	2 I2C read operations needed for each value.
	
	Pressure sensor data is 20 bit data.
	Read the data bytewise using I2C.
	3 I2C read operations needed.
*/

static ssize_t device_read ( struct file * file, unsigned int param_number )
{
	sensor_measured_value = 0;
	
	switch ( param_number )
	{
		case PARAM_01:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, ACCEL_X_HIGH_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, ACCEL_X_LOW_REG );
		break;
		case PARAM_02:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, ACCEL_Y_HIGH_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, ACCEL_Y_LOW_REG );
		break;
		case PARAM_03:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, ACCEL_Z_HIGH_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, ACCEL_Z_LOW_REG );
		break;
		case PARAM_04:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, GYRO_X_HIGH_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, GYRO_X_LOW_REG );
		break;
		case PARAM_05:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, GYRO_Y_HIGH_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, GYRO_Y_LOW_REG );
		break;
		case PARAM_06:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, GYRO_Z_HIGH_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, GYRO_Z_LOW_REG );
		break;
		case PARAM_07:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, COMP_X_LOW_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, COMP_X_HIGH_REG );
		break;
		case PARAM_08:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, COMP_Y_LOW_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, COMP_Y_HIGH_REG );
		break;
		case PARAM_09:
			sensor_measured_value = I2C_read ( I2C_ADDR_MPU_9255, COMP_Z_LOW_REG );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_MPU_9255, COMP_Z_HIGH_REG );
		break;
		case PARAM_10:
			sensor_measured_value = I2C_read ( I2C_ADDR_BMP_280, PRESS_REG_MSB );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_BMP_280, PRESS_REG_LSB );
			sensor_measured_value = sensor_measured_value << 8;
			sensor_measured_value += I2C_read ( I2C_ADDR_BMP_280, PRESS_REG_XLSB );
		break;
	}
	return sensor_measured_value;
}

/* File operation Function Definitions */
/* IOCTL Function Definition */
static long my_ioctl ( struct file * my_device_file, unsigned int ioctl_req_num, unsigned long ioctl_param )
{
	int value = 0, param_number = 0;

	if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_01_VAL )
	{
		value = device_read ( my_device_file, PARAM_01 );
		param_number = 1;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_02_VAL )
	{
		value = device_read ( my_device_file, PARAM_02 );
		param_number = 2;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_03_VAL )
	{
		value = device_read ( my_device_file, PARAM_02 );
		param_number = 3;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_04_VAL )
	{
		value = device_read ( my_device_file, PARAM_02 );
		param_number = 4;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_05_VAL )
	{
		value = device_read ( my_device_file, PARAM_05 );
		param_number = 5;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_06_VAL )
	{
		value = device_read ( my_device_file, PARAM_06 );
		param_number = 6;
	}
	
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_07_VAL )
	{
		value = device_read ( my_device_file, PARAM_07 );
		param_number = 7;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_08_VAL )
	{
		value = device_read ( my_device_file, PARAM_08 );
		param_number = 8;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_09_VAL )
	{
		value = device_read ( my_device_file, PARAM_09 );
		param_number = 9;
	}

	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_10_VAL )
	{
		value = device_read ( my_device_file, PARAM_10 );
		param_number = 10;
	}

	else
		return -1;
	printk ( KERN_INFO "%s : Param#%d value = %d", DRIVER_NAME, param_number, value );
	
	return value;
}

static int my_open ( struct inode * i, struct file * f)
{
	printk ( KERN_INFO "%s : open()\n", DRIVER_NAME );
	return 0;
}

static int my_close ( struct inode * i, struct file * f)
{
	printk ( KERN_INFO "%s : close()\n", DRIVER_NAME );
	return 0;
}

/* Create a file_operations struct */
static struct file_operations f_ops = 
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	#if ( LINUX_VERSION_CODE < KERNEL_VERSION ( 2, 6, 35) )
		.ioctl = my_ioctl,
	#else
		.unlocked_ioctl = my_ioctl,
	#endif
};

static int __init myChar_init ( void )
{
	/* Create Major and Minor numbers */
	printk ( KERN_INFO "%s : Driver registered ", DRIVER_NAME );
	
	/* Reserve <Major, Minor> */ 
	if ( alloc_chrdev_region ( &my_device_id, 0, 1, "IMU-Sensor" ) < 0 )
	{
		return -1;
	}
	
	MAJOR_NUM = MAJOR(my_device_id);
	MINOR_NUM = MINOR(my_device_id);
	printk (KERN_INFO "%s : <Maj, Min> = <%d, %d>\n", DRIVER_NAME, MAJOR_NUM, MINOR(my_device_id) );
	
	/* Create a device node in /dev directory */
	if ( ( cls = class_create ( THIS_MODULE, "chardev" ) ) == NULL )
	{
		printk ( KERN_INFO "%s : Unable to create device class", DRIVER_NAME );
		unregister_chrdev_region ( my_device_id, 1 );
		return -1;
	}
	
	if ( device_create ( cls, NULL, my_device_id, NULL, DRIVER_NAME ) == NULL )
	{
		printk ( KERN_INFO "%s : Unable to create device", DRIVER_NAME );
		class_destroy ( cls );
		unregister_chrdev_region ( my_device_id, 1 );
		return -1;
	}
	
	/* Link device file with the File Operations struct*/
	cdev_init ( &my_cdev, &f_ops);
	if ( cdev_add ( &my_cdev, my_device_id, 1 ) == -1)
	{
		printk ( KERN_INFO "%s : Unable to add device", DRIVER_NAME );
		device_destroy ( cls, my_device_id);
		class_destroy ( cls );
		unregister_chrdev_region ( my_device_id, 1 );
		return -1;
	}

	return 0;
}

static void __exit myChar_exit ( void )
{
	cdev_del ( &my_cdev );
	device_destroy ( cls, my_device_id );
	class_destroy ( cls );
	unregister_chrdev_region ( my_device_id, 1 );
	printk ( KERN_INFO "%s : Driver unregistered\n\n", DRIVER_NAME );
}

module_init ( myChar_init );
module_exit ( myChar_exit );

MODULE_LICENSE ( "GPL" );
MODULE_AUTHOR ( "Srinivasan M P" );


