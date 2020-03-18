import sensor, image, time
from pyb import Pin, SPI
CS  = Pin("P3", Pin.OUT)
RST = Pin("P4", Pin.OUT)
RS  = Pin("P5", Pin.OUT)
spi = SPI(1, SPI.MASTER, baudrate=int(1000000000/66), polarity=0, phase=0)
def write_command_byte(c):
	CS.low()
	RS.low()
	spi.send(c)
	CS.high()
def write_data_byte(c):
	CS.low()
	RS.high()
	spi.send(c)
	CS.high()
def write_command(c, *data):
	write_command_byte(c)
	if data:
		for d in data:
			write_data_byte(d)
def init():
	RST.low()
	time.sleep(50)
	RST.high()
	time.sleep(50)
	write_command_byte(0x11)
	time.sleep(5)
	write_command_byte(0x3a)
	write_data_byte(0x55)
	write_command_byte(0x26)
	write_data_byte(0x04)
	write_command_byte(0xf2)
	write_data_byte(0x01)
	write_command_byte(0xe0)
	write_data_byte(0x3f)
	write_data_byte(0x25)
	write_data_byte(0x1c)
	write_data_byte(0x1e)
	write_data_byte(0x20)
	write_data_byte(0x12)
	write_data_byte(0x2a)
	write_data_byte(0x90)
	write_data_byte(0x24)
	write_data_byte(0x11)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_command_byte(0xe1)
	write_data_byte(0x20)
	write_data_byte(0x20)
	write_data_byte(0x20)
	write_data_byte(0x20)
	write_data_byte(0x05)
	write_data_byte(0x00)
	write_data_byte(0x15)
	write_data_byte(0xa7)
	write_data_byte(0x3d)
	write_data_byte(0x18)
	write_data_byte(0x25)
	write_data_byte(0x2a)
	write_data_byte(0x2b)
	write_data_byte(0x2b)
	write_data_byte(0x3a)
	write_command_byte(0xb1)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_command_byte(0xb4)
	write_data_byte(0x07)
	write_command_byte(0xc0)
	write_data_byte(0x0a)
	write_data_byte(0x02)
	write_command_byte(0xc1)
	write_data_byte(0x02)
	write_command_byte(0xc5)
	write_data_byte(0x4f)
	write_data_byte(0x5a)
	write_command_byte(0xc7)
	write_data_byte(0x40)
	write_command_byte(0x2a)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0xa8)
	write_command_byte(0x2b)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0x00)
	write_data_byte(0xb3)
	write_command_byte(0x36)
	write_data_byte(0xC0)
	write_command_byte(0xb7)
	write_data_byte(0x00)
	write_command_byte(0x29)
	write_command_byte(0x2c)
def TFTSPI_Set_Pos(xs,ys,xe,ye):
	write_command_byte(0x2A)
	write_data_byte(0x00)
	write_data_byte(xs)
	write_data_byte(0x00)
	write_data_byte(xe)
	write_command_byte(0x2B)
	write_data_byte(0x00)
	write_data_byte(ys)
	write_data_byte(0x00)
	write_data_byte(ye)
	write_command_byte(0x2C)
def write_image(img):
	CS.low()
	RS.high()
	spi.send(img)
	CS.high()
def TFT_Clear(img,pos_x,pos_y):
	img.draw_rectangle(pos_x,pos_y,128,160,color=0x0000,fill=True)
def tftdisplay(img,x,y,pw,ph):
	img2=img.copy(roi=[x,y,pw,ph])
	write_command(0x2C)
	write_image(img2)
