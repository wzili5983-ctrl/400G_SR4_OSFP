//wlyip
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MON2_HSIP_ENG_H
#define MON2_HSIP_ENG_H

/***** common SAL functions *******************/
void sal_init(void);
void sal_fini(void);
int sal_rdmdio(int phyad, int devad, int regad);
void sal_wrmdio(int phyad, int devad, int regad, int v);
void sal_addrmdio45(int phyad, int devad, int regad);
void sal_writemdio45(int phyad, int devad, int v);
int sal_readmdio45(int phyad, int devad);
unsigned int sal_rdEst(int phyad, unsigned int regad);
void sal_setAccMdEst(unsigned int access_mode);
int sal_getRcmEst();
void sal_setI2CDevEst(unsigned int address);
int sal_getI2CDevEst();
void sal_wrEst(int phyad, unsigned int regad, unsigned int v);
int sal_rdFPGA(int regad);
void sal_wrFPGA(int regad, int v);
void write_pcb(int addr, int wr_data);
int read_pcb(int addr);
void sal_settingView(unsigned int view_on);
void set_com(int port);
unsigned char sal_readAhbI2cByte(int dev, int addr);
void sal_writeAhbI2cByte(int dev, int addr, unsigned char val);
unsigned char sal_readI2c(int dev, int addr);
void sal_writeI2c(int dev, int addr, unsigned char val);
void sal_send2usb(int ocnt, unsigned char *obuff, int icnt, unsigned char *ibuff);
int sal_getAccMdEst();

int sal_readEstInc(int prtad, unsigned int addr, int cnt, unsigned int *data);
int sal_writeEstInc(int prtad, unsigned int addr, int cnt, unsigned int *data);
/**********************************************/

#endif

//wlyip
#ifdef __cplusplus
}
#endif
