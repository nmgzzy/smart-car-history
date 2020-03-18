import time, tft
from pyb import Pin
up	= Pin('P10',Pin.IN_PUP)
left  = Pin('P6' ,Pin.IN_PUP)
right = Pin('P1' ,Pin.IN_PUP)
down  = Pin('P9' ,Pin.IN_PUP)
def key_read():
	key_temp=0
	if up.value()==0:
		key_temp=3
	if left.value()==0:
		key_temp=2
	if right.value()==0:
		key_temp=4
	if down.value()==0:
		key_temp=1
	return key_temp
def key_delay():
	time.sleep(50)
Menu_Lv1=["Adjust","Read  ","Save  ","Pos   ","Record"]
Menu_Lv2=["Red   ","Blue  ","White "]
Menu_Lv3=["L_min ","L_max ","A_min ","A_max ","B_min ","B_max "]
Menu_Pos=["HorZ  ","VerT  "]
def menu_show(img,n,sel,sta,pos_x,pos_y):
	if n==1:
		img.draw_string(pos_x+40,pos_y+100,Menu_Lv1[0])
		img.draw_string(pos_x+40,pos_y+110,Menu_Lv1[1])
		img.draw_string(pos_x+40,pos_y+120,Menu_Lv1[2])
		img.draw_string(pos_x+40,pos_y+130,Menu_Lv1[3])
		img.draw_string(pos_x+40,pos_y+140,Menu_Lv1[4])
	if n==2:
		img.draw_string(pos_x+40,pos_y+100,Menu_Lv2[0])
		img.draw_string(pos_x+40,pos_y+110,Menu_Lv2[1])
		img.draw_string(pos_x+40,pos_y+120,Menu_Lv2[2])
	if n==3:
		img.draw_string(pos_x+40,pos_y+100,Menu_Lv3[0])
		img.draw_string(pos_x+40,pos_y+110,Menu_Lv3[1])
		img.draw_string(pos_x+40,pos_y+120,Menu_Lv3[2])
		img.draw_string(pos_x+40,pos_y+130,Menu_Lv3[3])
		img.draw_string(pos_x+40,pos_y+140,Menu_Lv3[4])
		img.draw_string(pos_x+40,pos_y+150,Menu_Lv3[5])
	if n==4:
		img.draw_string(pos_x+40,pos_y+100,Menu_Pos[0])
		img.draw_string(pos_x+40,pos_y+110,Menu_Pos[1])
	img.draw_string(pos_x+24,pos_y+100+sel*10,">")
	if sta==1:
		img.draw_string(pos_x+32,pos_y+100+sel*10,"*")
def num_show(img,num,pos_x,pos_y):
	img.draw_string(pos_x+96,pos_y+100,(str)(num[0]))
	img.draw_string(pos_x+96,pos_y+110,(str)(num[1]))
	img.draw_string(pos_x+96,pos_y+120,(str)(num[2]))
	img.draw_string(pos_x+96,pos_y+130,(str)(num[3]))
	img.draw_string(pos_x+96,pos_y+140,(str)(num[4]))
	img.draw_string(pos_x+96,pos_y+150,(str)(num[5]))
