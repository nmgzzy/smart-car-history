#include "SD_SPI.h"

uint8 SPI_ReadWriteByte(uint8 data)
{
    uint8 buf;
    spi_mosi(SD_SPIN,SD_CS,&data,&buf,1,1);
    return buf;
}

uint8 SD_SendCommand(uint8 cmd,uint32 arg,uint8 crc,uint8 reset)
{
	unsigned char r1;
	unsigned int Retry = 0;
	SD_CS_DISABLE();
	//发送8个时钟，提高兼容性
	SPI_ReadWriteByte(0xff);
	//选中SD卡
	SD_CS_ENABLE();
	//按照SD卡的命令序列开始发送命令
	//cmd参数的第二位为传输位，数值为1，所以或0x40
	SPI_ReadWriteByte(cmd | 0x40);
	//参数段第24-31位数据[31..24]
	SPI_ReadWriteByte((uint8)(arg >> 24));
	//参数段第16-23位数据[23..16]
	SPI_ReadWriteByte((uint8)(arg >> 16));
	//参数段第8-15位数据[15..8]
	SPI_ReadWriteByte((uint8)(arg >> 8));
	//参数段第0-7位数据[7..0]
	SPI_ReadWriteByte((uint8)arg);
	SPI_ReadWriteByte(crc);
	//等待响应或超时退出
	while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;
		if(Retry > 800)  break;  //超时次数
	}
    if(reset)
    {
        //关闭片选
        SD_CS_DISABLE();
        //在总线上额外发送8个时钟，让SD卡完成剩下的工作
        SPI_ReadWriteByte(0xFF);
    }
	//返回状态值
	return r1;
}

uint8 SD_ReceiveData(uint8 *data, uint16 len, uint8 release)
{
	uint16 retry;
	uint8 r1;
	//启动一次传输
	SD_CS_ENABLE();
	retry = 0;
	do
    {
		r1 = SPI_ReadWriteByte(0xFF);
		retry++;
		if(retry>4000)  //4000次等待后没有应答，退出报错（可多试几次）
		{
			SD_CS_DISABLE();
			return 1;
		}
	}while(r1 != 0xFE || r1 != 0xFC);  //等待SD卡发回数据起始令牌0xFE
	//跳出循环后，开始接收数据
	while(len--)
	{
		*data = SPI_ReadWriteByte(0xFF);
		data++;
	}
	//发送2个伪CRC
	SPI_ReadWriteByte(0xFF);
	SPI_ReadWriteByte(0xFF);
	//按需释放总线
	if(release == RELEASE)
	{
		SD_CS_DISABLE();
		SPI_ReadWriteByte(0xFF);
	}
	return 0;
}

uint8 SD_Init(void)
{
	uint16 i;
	uint8 r1;
	uint16 retry;
    gpio_init(SD_CS_PIN, GPO, 1, NOPULL);
	spi_init(SD_SPIN, SD_CS, SD_SCK, SD_MOSI, SD_MISO, 2, SPI_SPEED_LOW);//SPI初始化,SD卡初始化时时钟不能超过400KHz
	SD_CS_ENABLE();	//选中SD卡
	for(i=0;i<0xf00;i++);//延时，等待SD卡上电稳定
	for(i=0;i<15;i++)//先产生至少74个脉冲，让SD卡初始化完成
	{
		SPI_ReadWriteByte(0xff);//参数可随便写，经过10次循环，产生80个脉冲
	}
	//-----------------SD卡复位到idle状态----------------
	retry=0;
	do//循环发送CMD0，直到SD卡返回0x01,进入idle状态,超时则直接退出
	{
		r1=SD_SendCommand(CMD0,0,0x95,1);//发送CMD0，CRC为0x95
		retry++;
        if(retry>500) return 1;//超时
	}
	while(r1!=0x01);
	//下面是SD2.0卡的初始化
    r1=SD_SendCommand(CMD8,0x1aa,0x87,0);
	if(r1==0x01)
	{
        // V2.0的卡，CMD8命令后会传回4字节的数据，要跳过再结束本命令
        SPI_ReadWriteByte(0xFF);
        SPI_ReadWriteByte(0xFF);
        SPI_ReadWriteByte(0xFF);
        SPI_ReadWriteByte(0xFF);
		SD_CS_DISABLE();
		SPI_ReadWriteByte(0xFF);//多发8个时钟
		retry = 0;
		do//发卡初始化指令CMD55+ACMD41
		{

			if(SD_SendCommand(CMD55, 0, 0, 1)==0x01)//应返回0x01
                if(SD_SendCommand(ACMD41, 0x40000000, 1, 1)==0)
                    break;

            //r1 = SD_SendCommand(CMD1, 0, 0, 1);

            /*r1=SD_SendCommand(CMD55, 0, 0, 1);
            if(r1!=0x01)//应返回0x01
                continue;
            r1=SD_SendCommand(ACMD41, 0x40000000, 1, 1);
            if(r1==0)
                break;*/

            retry++;
		}while(retry<301);
        if(retry >= 300)
            return r1;

		spi_setspeed(SD_SPIN, SPI_SPEED_HIGH);
	}
    return 0;
}

uint8 SD_ReadSingleBlock(uint32 sector, uint8 *buffer)
{
	uint8 r1;
	r1 = SD_SendCommand(CMD17, sector, 1, 1); //发送CMD17 读命令
	if(r1 != 0x00) return r1;
	r1 = SD_ReceiveData(buffer, 512, RELEASE); //一个扇区为512字节
	if(r1 != 0)
		return r1;   //读数据出错
	else
		return 0;    //读取正确，返回0
}

uint8 SD_ReadMultiBlock(uint32 sector, uint8 *buffer, uint8 count)
{
	uint8 r1;
	r1 = SD_SendCommand(CMD18, sector, 1, 1);    //读多块命令
	if(r1 != 0x00)  return r1;
	do  //开始接收数据
	{
		if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)
		{
			break;
		}
		buffer += 512;
	}while(--count);

	SD_SendCommand(CMD12, 0, 1, 0);  //全部传输完成，发送停止命令
	SD_CS_DISABLE();  //释放总线
	SPI_ReadWriteByte(0xFF);
	if(count != 0)
		return count;   //如果没有传完，返回剩余个数
	else
		return 0;
}

uint8 SD_WaitReady(void)
{
	uint8 r1;
	uint16 retry=0;
	do
	{
		r1 = SPI_ReadWriteByte(0xFF);
		retry++;
		if(retry==0xfffe)
		return 1;
	}while(r1!=0xFF);
	return 0;
}

uint8 SD_WriteSingleBlock(uint32 sector, const uint8 *data)
{
    uint8 r1;
	uint16 i;
	uint16 retry;
	//写扇区指令
	r1 = SD_SendCommand(CMD24, sector, 0x00, 1);
	if(r1 != 0x00)
	{
		//应答错误，直接返回
		return r1;
	}
	//开始准备数据传输
	SD_CS_ENABLE();
	//先发3个空数据，等待SD卡准备好
	SPI_ReadWriteByte(0xff);
	SPI_ReadWriteByte(0xff);
	SPI_ReadWriteByte(0xff);
	//放起始令牌0xFE
	SPI_ReadWriteByte(0xFE);
	//发一个sector数据
	for(i=0;i<512;i++)
	{
		SPI_ReadWriteByte(*data++);
	}
	//发送2个伪CRC校验
	SPI_ReadWriteByte(0xff);
	SPI_ReadWriteByte(0xff);
	//等待SD卡应答
	r1 = SPI_ReadWriteByte(0xff);
	//如果为0x05说明数据写入成功
	if((r1&0x1F)!=0x05)
	{
		SD_CS_DISABLE();
		return r1;
	}
	//等待操作完成
	retry = 0;
	//卡自编程时，数据线被拉低
	while(!SPI_ReadWriteByte(0xff))
	{
		retry++;
		if(retry>65534)        //如果长时间没有写入完成，退出报错
		{
			SD_CS_DISABLE();
			return 1;           //写入超时，返回1
		}
	}
	//写入完成，片选置1
	SD_CS_DISABLE();
	SPI_ReadWriteByte(0xff);
	return 0;
}

uint8 SD_WriteMultiBlock(uint32 sector, const uint8 *data, uint8 count)
{
	uint8 r1;
	uint16 i;
	//写多块指令CMD25
	r1 = SD_SendCommand(CMD25, sector, 0x01, 1);
	//应答不正确，直接返回
	if(r1 != 0x00)  return r1;
	//开始准备数据传输
	SD_CS_ENABLE();
	//放3个空数据让SD卡准备好
	SPI_ReadWriteByte(0xff);
	SPI_ReadWriteByte(0xff);
	SPI_ReadWriteByte(0xff);
	//下面是N个sector循环写入的部分
	do
	{
		//放起始令牌0xFC，表明是多块写入
		SPI_ReadWriteByte(0xFC);
		//发1个sector的数据
		for(i=0;i<512;i++)
		{
			SPI_ReadWriteByte(*data++);
		}
		//发2个伪CRC
		SPI_ReadWriteByte(0xff);
		SPI_ReadWriteByte(0xff);
		//等待SD卡回应
		r1 = SPI_ReadWriteByte(0xff);
		//0x05表示数据写入成功
		if((r1&0x1F)!=0x05)
		{
			SD_CS_DISABLE();
			return r1;
		}
		//检测SD卡忙信号
		if(SD_WaitReady()==1)
		{
			SD_CS_DISABLE();    //长时间写入未完成，退出
			return 1;
		}
	}while(--count);
	//发送传输结束令牌0xFD
	SPI_ReadWriteByte(0xFD);
	//等待准备好
	if(SD_WaitReady())
	{
		SD_CS_DISABLE();
		return 1;
	}
	//写入完成，片选置1
	SD_CS_DISABLE();
	SPI_ReadWriteByte(0xff);
	//返回count值，如果写完，则count=0,否则count=未写完的sector数
	return count;
}

uint8 SD_WriteImage(uint16 imgNum, const uint8 *img)
{
	return SD_WriteMultiBlock(imgNum*32, img, 32);
}




