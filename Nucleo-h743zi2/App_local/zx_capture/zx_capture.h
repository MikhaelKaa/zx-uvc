// Header: zx capture abstraction
// File Name: zx_capture.h
// Author: Mikhail Kaa
// Date: 31.05.2023

#ifndef ZX_CAPTURE
#define ZX_CAPTURE

int ZX_CAP_Proc(void);
void init_pix_table(void);
void zx_copy_pix_gmx_sc(void);
void zx_copy_pix_gmx_pent(void);

#endif /* ZX_CAPTURE */