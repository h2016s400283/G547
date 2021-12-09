#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define SENSOR_FILE_NAME "/dev/ArduinoCOM"

#define PARAM_00 0
#define PARAM_01 1
#define PARAM_02 2
#define PARAM_03 3
#define PARAM_04 4
#define PARAM_05 5
#define PARAM_06 6
#define PARAM_07 7
#define PARAM_08 8
#define PARAM_09 9

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

#define MAJOR_NUM 511

int ioctl_get_val_from_sensor ( int my_file_desc, int param_number )
{
	int ret_val = 0, value = 0;
	unsigned long req_code = 0;
	switch ( param_number )
	{
		case 0:
			req_code = IOCTL_REQ_CMD_PARAM_00_VAL;
		break;
		case 1:
			req_code = IOCTL_REQ_CMD_PARAM_01_VAL;
		break;
		case 2:
			req_code = IOCTL_REQ_CMD_PARAM_02_VAL;
		break;
		case 3:
			req_code = IOCTL_REQ_CMD_PARAM_03_VAL;
		break;
		case 4:
			req_code = IOCTL_REQ_CMD_PARAM_04_VAL;
		break;
		case 5:
			req_code = IOCTL_REQ_CMD_PARAM_05_VAL;
		break;
		case 6:
			req_code = IOCTL_REQ_CMD_PARAM_06_VAL;
		break;
		case 7:
			req_code = IOCTL_REQ_CMD_PARAM_07_VAL;
		break;
		case 8:
			req_code = IOCTL_REQ_CMD_PARAM_08_VAL;
		break;
		case 9:
			req_code = IOCTL_REQ_CMD_PARAM_09_VAL;
		break;
	}
	ret_val = ioctl ( my_file_desc, req_code, value );
	if ( ret_val < 0 )
		printf ( "Cannot write %d\n", param_number );
	else
		printf ( "Successfully transmitted!!\n", param_number );
	return 0;
}

int main ()
{
	int param_number = 0;
	int my_file_desc = open ( SENSOR_FILE_NAME, O_WRONLY );
	
	if ( my_file_desc < 0 )
		printf ( "Unable to open the device file %s\n", SENSOR_FILE_NAME );
	else
	{
		printf ( "Enter Number: " );
		scanf ( "%d", &param_number );
		ioctl_get_val_from_sensor ( my_file_desc, param_number );
	}
	return 0;
}
