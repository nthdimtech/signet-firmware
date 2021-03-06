#ifndef __USBD_MULTI_H
#define __USBD_MULTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#include  "usbd_ioreq.h"

#define HID_KEYBOARD_EPOUT_ADDR           0x01U
#define HID_KEYBOARD_EPIN_ADDR            0x81U
#define HID_KEYBOARD_EPIN_SIZE            64U
#define HID_KEYBOARD_EPOUT_SIZE           64U

#define HID_CMD_EPOUT_ADDR                0x02U
#define HID_CMD_EPIN_ADDR                 0x82U
#define HID_CMD_EPIN_SIZE                 512U
#define HID_CMD_EPOUT_SIZE                512U

#define HID_FIDO_EPOUT_ADDR                0x03U
#define HID_FIDO_EPIN_ADDR                 0x83U
#define HID_FIDO_EPIN_SIZE                 64U
#define HID_FIDO_EPOUT_SIZE                64U

#define MSC_EPOUT_ADDR               0x04U
#define MSC_EPIN_ADDR                0x84U
#define MSC_EPIN_SIZE                (0x200)
#define MSC_EPOUT_SIZE               (0x200)

#define USB_HID_CONFIG_DESC_SIZ       (9 + \
				((9 + 9 + 7 + 7) * 3) + \
				((9 + 7 + 7) * 1))

#define USB_HID_DESC_SIZ              9U

#define HID_DESCRIPTOR_TYPE           0x21U
#define HID_REPORT_DESC               0x22U

#ifndef HID_HS_BINTERVAL
#define HID_HS_BINTERVAL            0x06U
#endif /* HID_HS_BINTERVAL */

#ifndef HID_FS_BINTERVAL
#define HID_FS_BINTERVAL            0x0AU
#endif /* HID_FS_BINTERVAL */

extern USBD_ClassTypeDef  USBD_Multi;
#define USBD_MULTI_CLASS    &USBD_Multi
int endpointToInterface(uint8_t epNum);
uint8_t interfaceToEndpointIn(int interfaceNum);
uint8_t interfaceToEndpointOut(int interfaceNum);

#ifdef __cplusplus
}
#endif

#endif  /* __USB_HID_H */
