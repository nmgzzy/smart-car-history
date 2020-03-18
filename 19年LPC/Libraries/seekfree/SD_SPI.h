#ifndef __SD_SPI_H_
#define __SD_SPI_H_

#include "headfile.h"

#define SD_SPIN    SPI_7//SPI_0//SPI_7
#define SD_SCK     SPI7_SCK_A21//SPI0_SCK_A28//SPI7_SCK_A21
#define SD_MOSI    SPI7_MOSI_A20//SPI0_MOSI_A24//SPI7_MOSI_A20
#define SD_MISO    SPI7_MISO_A19//SPI0_MISO_A25//SPI7_MISO_A19
#define SD_CS      SPI_CS_NOT_USE
#define SD_CS_PIN  A15//A2//A15

#define CMD0  0x00  //NONE	R1	复位SD卡
#define CMD1  0x01
#define CMD8  0X08  //VHS+Check Pattern	R7	发送接口状态命令
#define CMD9  0X09  //NONE	R1	读取卡特定数据寄存器
#define CMD10 0X0A  //NONE	R1	读取卡标志数据寄存器
#define CMD12 0X0C  //NONE	R1	读取卡标志数据寄存器
#define CMD16 0X10  //块大小	R1	设置块大小（字节数）
#define CMD17 0X11  //地址	R1	读取一个块的数据
#define CMD18 0X12  //地址	R1	读取一个块的数据
#define CMD24 0X18  //地址	R1	写入一个块的数据
#define CMD25 0X19  //地址	R1	写入一个块的数据
#define CMD41 0X29  //NONE	R3	发送给主机容量支持信息和激活卡初始化过程
#define CMD55 0X37  //NONE	R1	告诉SD卡，下一个是特定应用命令
#define CMD58 0X3A  //NONE	R3	读取OCR寄存器
#define ACMD41 0X29

#define RELEASE 1
#define NO_RELEASE 0

#define SPI_SPEED_LOW  20000
#define SPI_SPEED_HIGH 10000000

#define SD_CS_ENABLE()   	gpio_set(SD_CS_PIN,0);
#define SD_CS_DISABLE()   	gpio_set(SD_CS_PIN,1);

uint8 SD_Init(void);
uint8 SD_ReadSingleBlock(uint32 sector, uint8 *buffer);
uint8 SD_ReadMultiBlock(uint32 sector, uint8 *buffer, uint8 count);
uint8 SD_WriteSingleBlock(uint32 sector, const uint8 *data);
uint8 SD_WriteMultiBlock(uint32 sector, const uint8 *data, uint8 count);
uint8 SD_WriteImage(uint16 imgNum, const uint8 *img);

#endif