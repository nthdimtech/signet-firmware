#ifndef SIGNETDEV_COMMON_PRIV_H
#define SIGNETDEV_COMMON_PRIV_H

#include "signetdev_common.h"

//
// Device specific constants
//

//Signet HC
#define SIGNET_HC_BLK_SIZE (16384)
#define SIGNET_HC_NUM_STORAGE_BLOCKS (1024)
#define SIGNET_HC_STARTUP_RESP_INFO_SIZE (8)
#define SIGNET_HC_FLASH_PAGE_SIZE (16384)
#define SIGNET_HC_RAW_HID_PACKET_SIZE (512)

//Original Signet
#define SIGNET_BLK_SIZE (2048)
#define SIGNET_NUM_STORAGE_BLOCKS (192/2)
#define SIGNET_STARTUP_RESP_INFO_SIZE (6)
#define SIGNET_FLASH_PAGE_SIZE (2048)
#define SIGNET_RAW_HID_PACKET_SIZE (64)

#define CMD_PACKET_HEADER_SIZE (6)

#ifdef FIRMWARE

#ifdef SIGNET_HC
#define BLK_SIZE (SIGNET_HC_BLK_SIZE)
#define NUM_STORAGE_BLOCKS (SIGNET_HC_NUM_STORAGE_BLOCKS)
#define STARTUP_RESP_INFO_SIZE (SIGNET_HC_STARTUP_RESP_INFO_SIZE)
#define FLASH_PAGE_SIZE (SIGNET_HC_FLASH_PAGE_SIZE)
#define RAW_HID_PACKET_SIZE (SIGNET_HC_RAW_HID_PACKET_SIZE)
#else
#define BLK_SIZE (SIGNET_BLK_SIZE)
#define NUM_STORAGE_BLOCKS (SIGNET_NUM_STORAGE_BLOCKS)
#define STARTUP_RESP_INFO_SIZE (SIGNET_STARTUP_RESP_INFO_SIZE)
#define FLASH_PAGE_SIZE (SIGNET_FLASH_PAGE_SIZE)
#define RAW_HID_PACKET_SIZE (SIGNET_RAW_HID_PACKET_SIZE)
#endif

#define MAX_DATA_BLOCK (NUM_STORAGE_BLOCKS - 1)
#define NUM_DATA_BLOCKS (MAX_DATA_BLOCK - MIN_DATA_BLOCK + 1)
#define RAW_HID_PAYLOAD_SIZE (RAW_HID_PACKET_SIZE - RAW_HID_HEADER_SIZE)

#define CMD_PACKET_PAYLOAD_SIZE (BLK_SIZE + CMD_PACKET_HEADER_SIZE + 1)
#define CMD_PACKET_BUF_SIZE (CMD_PACKET_PAYLOAD_SIZE + CMD_PACKET_HEADER_SIZE + RAW_HID_PAYLOAD_SIZE - 1)
#define STARTUP_RESP_SIZE (STARTUP_RESP_INFO_SIZE + HASH_FN_SZ + SALT_SZ_V2)

#endif //FIRMWARE

#define MAX_HID_PACKET_SIZE ((SIGNET_HC_RAW_HID_PACKET_SIZE > SIGNET_RAW_HID_PACKET_SIZE) ? SIGNET_HC_RAW_HID_PACKET_SIZE : SIGNET_RAW_HID_PACKET_SIZE)
#define MAX_BLK_SIZE ((SIGNET_HC_BLK_SIZE > SIGNET_BLK_SIZE) ? SIGNET_HC_BLK_SIZE : SIGNET_BLK_SIZE)
#define MAX_NUM_STORAGE_BLOCKS ((SIGNET_HC_NUM_STORAGE_BLOCKS > SIGNET_NUM_STORAGE_BLOCKS) ? SIGNET_HC_NUM_STORAGE_BLOCKS : SIGNET_NUM_STORAGE_BLOCKS)


#define MAX_HID_PAYLOAD_SIZE (MAX_HID_PACKET_SIZE - RAW_HID_HEADER_SIZE)
#define MAX_CMD_PACKET_PAYLOAD_SIZE (MAX_BLK_SIZE + CMD_PACKET_HEADER_SIZE + 1)
#define MAX_CMD_PACKET_BUF_SIZE (MAX_CMD_PACKET_PAYLOAD_SIZE + CMD_PACKET_HEADER_SIZE + MAX_HID_PAYLOAD_SIZE - 1)

unsigned int signetdev_priv_cmd_payload_size();
int signetdev_priv_hid_packet_size();
int signetdev_priv_hid_payload_size();
int signetdev_priv_startup_resp_info_size();
unsigned int signetdev_priv_startup_resp_size();
int signetdev_priv_init_rand_data_size();

//
// Derived from device specific defines
//
//
// Common defines
//
#define CBC_IV_SZ (AES_BLK_SIZE)
#define INIT_RAND_DATA_SZ ((AES_256_KEY_SIZE * 2) + CBC_IV_SZ)

#define ROOT_DATA_BLOCK 0
#define MIN_DATA_BLOCK 1

#define SUB_BLK_SIZE (AES_BLK_SIZE)
#define SUB_BLK_MASK_SIZE (2)
#define SUB_BLK_DATA_SIZE (14)

#define SIZE_TO_SUB_BLK_COUNT(sz) (((sz) + (SUB_BLK_DATA_SIZE - 1))/SUB_BLK_DATA_SIZE)
#define SUB_BLK_COUNT(sz) (((sz) + (SUB_BLK_SIZE - 1))/SUB_BLK_SIZE)

#define STARTUP_RESP_SIZE (STARTUP_RESP_INFO_SIZE + HASH_FN_SZ + SALT_SZ_V2)

#define CBC_IV_SZ (AES_BLK_SIZE)
#define INITIALIZE_CMD_SIZE (LOGIN_KEY_SZ + HASH_FN_SZ + SALT_SZ_V2 + INIT_RAND_DATA_SZ)

#define USB_SIGNET_VENDOR_ID (0x5E2A)
#define USB_SIGNET_PRODUCT_ID (0x0001)
#define USB_SIGNET_HC_VENDOR_ID (0x1209)
#define USB_SIGNET_HC_PRODUCT_ID (0xDF11)

#define USB_REV_ID (0x0483)
#define USB_RAW_HID_USAGE_PAGE	0xFFAB	// recommended: 0xFF00 to 0xFFFF
#define USB_RAW_HID_USAGE	0x0200	// recommended: 0x0100 to 0xFFFF
#define RAW_HID_HEADER_SIZE 1

struct root_page
{
	u8 signature[AES_BLK_SIZE];
	union {
		struct {
			u32 crc;
			u8 db_version;
			u8 auth_rand[AES_256_KEY_SIZE];
			u8 auth_rand_ct[AES_256_KEY_SIZE];
			u8 encrypt_key_ct[AES_256_KEY_SIZE];
			u8 cbc_iv[AES_BLK_SIZE];
			u8 salt[SALT_SZ_V2];
			u8 hashfn[HASH_FN_SZ];

			struct cleartext_pass cleartext_passwords[NUM_CLEARTEXT_PASS];
		} v2;
	} header;
} __attribute__((__packed__));

struct db_uid_ent {
	u16 info; //[0:11] = uid, [12:13] = rev, [14] = first, [15] = padding/reserved
	u16 sz;
	u16 blk_next;
} __attribute__((__packed__));

struct db_block_header {
	u32 crc;
	u16 part_size;
	u16 occupancy;
} __attribute__((__packed__));

struct db_block {
	struct db_block_header header;
	struct db_uid_ent uid_tbl[];
} __attribute__((__packed__));

#define INVALID_BLOCK (0)
#define INVALID_PART_SIZE (0xffff)
#define INVALID_CRC (0xffffffff)

enum commands {
	STARTUP,
	OPEN_ID, //Defunct
	CLOSE_ID, //Defunct
	GET_DATA, //Defunct
	SET_DATA, //Defunct
	TYPE,
	LOGIN,
	LOGOUT,
	DELETE_ID,
	BUTTON_WAIT,
	GET_PROGRESS,
	GET_DEVICE_STATE,
	ERASE_FLASH_PAGES,
	WRITE_FLASH,
	RESET_DEVICE,
	INITIALIZE,
	CONNECT, //Defunct
	DISCONNECT,
	WIPE,
	CANCEL_BUTTON_PRESS,
	CHANGE_MASTER_PASSWORD,
	BACKUP_DEVICE,
	READ_BLOCK,
	BACKUP_DEVICE_DONE,
	RESTORE_DEVICE,
	WRITE_BLOCK,
	ERASE_BLOCK,
	RESTORE_DEVICE_DONE,
	GET_DEVICE_CAPACITY,
	UPDATE_FIRMWARE,
	GET_ALL_DATA,
	UPDATE_UID,
	READ_UID,
	READ_ALL_UIDS,
	GET_RAND_BITS,
	ENTER_MOBILE_MODE,
	LOGIN_TOKEN,
	READ_CLEARTEXT_PASSWORD_NAMES,
	READ_CLEARTEXT_PASSWORD,
	WRITE_CLEARTEXT_PASSWORD,
        UPDATE_UIDS,
        SWITCH_BOOT_MODE,
	WRITE_BLOCK_HC,
	READ_BLOCK_HC,
	ERASE_BLOCK_HC,
};

#endif
