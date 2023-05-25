#include "usbd_uvc_if.h"

uint8_t UVC_flag;

void Camera_On(void);
void Camera_Off(void);
uint8_t *Camera_GetFrame(uint32_t *pFrameLength);
void Camera_FreeFrame(uint8_t *frame);


USBD_UVC_CameraTypeDef USBD_UVC_Camera =
{
	Camera_On,
	Camera_Off,
	Camera_GetFrame,
	Camera_FreeFrame
};

void Camera_On(void){}

void Camera_Off(void){}

void Camera_FreeFrame(uint8_t *frame){}

uint8_t *Camera_GetFrame(uint32_t *pFrameLength)
{
	//HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	pFrameLength[0] = UVC_VIDEO_MAX_FRAME_BUF_SIZE;
	return UVC_flag?((uint8_t *)&ucv_buf):(0);
}
