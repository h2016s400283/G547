#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/i2c-dev.h>

#define DRIVER_NAME "ArduinoCOM"
#define DRIVER_CLASS "ArduinoCOM_Class"

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

#define IOCTL_REQ_CMD_PARAM_00_VAL _IOW( MAJOR_NUM, PARAM_01, unsigned long)
#define IOCTL_REQ_CMD_PARAM_01_VAL _IOW( MAJOR_NUM, PARAM_01, unsigned long)
#define IOCTL_REQ_CMD_PARAM_02_VAL _IOW( MAJOR_NUM, PARAM_02, unsigned long)
#define IOCTL_REQ_CMD_PARAM_03_VAL _IOW( MAJOR_NUM, PARAM_03, unsigned long)
#define IOCTL_REQ_CMD_PARAM_04_VAL _IOW( MAJOR_NUM, PARAM_04, unsigned long)
#define IOCTL_REQ_CMD_PARAM_05_VAL _IOW( MAJOR_NUM, PARAM_05, unsigned long)
#define IOCTL_REQ_CMD_PARAM_06_VAL _IOW( MAJOR_NUM, PARAM_06, unsigned long)
#define IOCTL_REQ_CMD_PARAM_07_VAL _IOW( MAJOR_NUM, PARAM_07, unsigned long)
#define IOCTL_REQ_CMD_PARAM_08_VAL _IOW( MAJOR_NUM, PARAM_08, unsigned long)
#define IOCTL_REQ_CMD_PARAM_09_VAL _IOW( MAJOR_NUM, PARAM_09, unsigned long)

int MAJOR_NUM = 0, MINOR_NUM = 0;

static struct i2c_adapter *ArduinoCOM_i2c_adapter     = NULL;  // I2C Adapter Structure
static struct i2c_client  *ArduinoCOM_i2c_client = NULL;  // I2C Cient Structure

/* Meta Information */
MODULE_AUTHOR("Surabhi Mondal & Srinivasan M P");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for communicating with Arduino over I2C");

/* Defines for device identification */
#define I2C_BUS_AVAILABLE	1		/* The I2C Bus available on the raspberry */
#define I2C_SLAVE_DEVICE_NAME "ArduinoCOM"	/* Device and Driver Name */
#define ARDUINO_COM_I2C_SLAVE_ADDRESS 0x25 /* ArduinoCOM I2C address */

static int I2C_WRITE(unsigned char *buf, unsigned int len)
{
	int ret = i2c_master_send (ArduinoCOM_i2c_client, buf, len);
	return ret;
}

static const struct i2c_device_id arduino_i2c_id[] = { { I2C_SLAVE_DEVICE_NAME, 0 }, {} };

static struct i2c_board_info ArduinoCOM_i2c_board_info = {
	I2C_BOARD_INFO(I2C_SLAVE_DEVICE_NAME, ARDUINO_COM_I2C_SLAVE_ADDRESS)
};

/* Variables for Device and Deviceclass*/
static dev_t myDeviceNr;
static struct class *myClass;
static struct cdev myDevice;

static ssize_t device_file_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	printk ( KERN_INFO "ArduinoCOM : Device File read\n" );
	return 0;
}

static int device_file_open(struct inode *deviceFile, struct file *instance) {
	printk("ArduinoCOM : Device File Opened\n");
	return 0;
}

static int device_file_close(struct inode *deviceFile, struct file *instance) {
	printk("ArduinoCOM : Device File Closed\n");
	return 0;
}

static void ArduinoCOM_Write(bool is_cmd, unsigned char data)
{
	unsigned char buf[2] = {0};
	int ret;
	if( is_cmd == true )
		buf[0] = 0x00;
	else
		buf[0] = 0x40;
	buf[1] = data;
	ret = I2C_WRITE(buf, 2);
}

char char_msg_Tx = 0x00;
static int ArduinoCOM_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	ArduinoCOM_Write(true, char_msg_Tx);
	printk (KERN_INFO "ArduinoCOM : Probed!!!\n");
	return 0;
}

static int ArduinoCOM_remove(struct i2c_client *client)
{   
	printk (KERN_INFO "ArduinoCOM : Removed!!!\n");
	return 0;
}

static struct i2c_driver ArduinoCOM_i2c_driver = {
	.driver = {
	  .name   = I2C_SLAVE_DEVICE_NAME,
	  .owner  = THIS_MODULE,
	},
	.probe          = ArduinoCOM_probe,
	.remove         = ArduinoCOM_remove,
	.id_table       = arduino_i2c_id,
};

static long my_ioctl ( struct file * my_device_file, unsigned int ioctl_req_num, unsigned long ioctl_param )
{
	int value = 0;

	if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_00_VAL )
		char_msg_Tx = '0';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_01_VAL )
		char_msg_Tx = '1';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_02_VAL )
		char_msg_Tx = '2';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_03_VAL )
		char_msg_Tx = '3';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_04_VAL )
		char_msg_Tx = '4';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_05_VAL )
		char_msg_Tx = '5';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_06_VAL )
		char_msg_Tx = '6';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_07_VAL )
		char_msg_Tx = '7';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_08_VAL )
		char_msg_Tx = '8';
	else if ( ioctl_req_num == IOCTL_REQ_CMD_PARAM_09_VAL )
		char_msg_Tx = '9';
	else
		return -1;
	value = ArduinoCOM_probe (ArduinoCOM_i2c_client, arduino_i2c_id);
	printk (KERN_INFO "ArduinoCOM : Printed Char : %c", char_msg_Tx);
	
	return value;
}

/* Map the file operations */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_file_open,
	.release = device_file_close,
	.read = device_file_read,
	.unlocked_ioctl = my_ioctl,
};

static int __init ModuleInit(void) {
	int ret = -1;
	printk("ArduinoCOM : Hello Kernel\n");

	/* Allocate Device Nr */
	if ( alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
	}
	MAJOR_NUM = MAJOR(myDeviceNr);
	MINOR_NUM = MINOR(myDeviceNr);
	printk("ArduinoCOM : Device Registered <%d, %d >\n", MAJOR_NUM, MINOR_NUM);

	/* Create Device Class */
	if ((myClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device Class can not be created!\n");
		goto ClassError;
	}

	/* Create Device file */
	if (device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize Device file */
	cdev_init(&myDevice, &fops);

	/* register device to kernel */
	if (cdev_add(&myDevice, myDeviceNr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto KernelError;
	}

	ArduinoCOM_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

	if(ArduinoCOM_i2c_adapter != NULL) {
		ArduinoCOM_i2c_client = i2c_new_client_device(ArduinoCOM_i2c_adapter, &ArduinoCOM_i2c_board_info);
		if(ArduinoCOM_i2c_client != NULL) {
			if(i2c_add_driver(&ArduinoCOM_i2c_driver) != -1) {
				ret = 0;
			}
			else
				printk("Can't add driver...\n");
		}
		i2c_put_adapter(ArduinoCOM_i2c_adapter);
	}
	printk("ArduinoCOM : Driver added!\n");

	return ret;

KernelError:
	device_destroy(myClass, myDeviceNr);
FileError:
	class_destroy(myClass);
ClassError:
	unregister_chrdev(myDeviceNr, DRIVER_NAME);
	return (-1);
}

/**
 * @brief function, which is called when removing module from kernel
 * free alocated resources
 */
static void __exit ModuleExit(void) {
	printk("ArduinoCOM : Goodbye!\n");
	i2c_unregister_device(ArduinoCOM_i2c_client);
	i2c_del_driver(&ArduinoCOM_i2c_driver);
	cdev_del(&myDevice);
    device_destroy(myClass, myDeviceNr);
    class_destroy(myClass);
    unregister_chrdev_region(myDeviceNr, 1);
}

module_init(ModuleInit);
module_exit(ModuleExit);
