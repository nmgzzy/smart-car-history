import sensor, image, time, math, mjpeg
import tft, key, para
para_temp=[0]*18
para.para_read(para_temp,18)
thresholds1=[tuple(para_temp[0:6])]
thresholds2=[tuple(para_temp[6:12])]
thresholds3=[tuple(para_temp[12:18])]
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
tft.init()
from pyb import UART
uart = UART(4, 38400)
uart.init(38400, bits=8, parity=None, stop=1, timeout_char=1000)
div_col=2
div_row=1.70212766
set_w=30
set_h=30
barrier_h=100
barrier_w=160
barrier=((int)((320-barrier_w)/2),240-barrier_h,barrier_w,barrier_h)
debug=1
menu_flag=1
menu_pos=0
state=0
color=0
tft_posx=96
tft_posy=40
key_res=0
rec_flag=0
while(True):
	img = sensor.snapshot()
	count=0
	start=0
	exist=0
	bflag=0
	for blob in img.find_blobs(thresholds1, pixels_threshold=5, area_threshold=20, merge=True):
		if blob.pixels()>count:
			count=blob.pixels()
			temp=blob
			start=1
	if start==1:
		for blob in img.find_blobs(thresholds3, roi=barrier, pixels_threshold=300, area_threshold=2000, merge=True):
			dist=abs(temp.cx()-blob.cx())+abs(temp.cy()-blob.cy())
			if dist>50:
				img.draw_rectangle(blob.rect())
				if blob.cx()>160:
					bflag=1
				else:
					bflag=2
		if count>0:
			roi_x=temp.x()-(int)(set_w/2)
			if roi_x<0:
				roi_x=0
			roi_y=temp.y()-(int)(set_h/2)
			if roi_y<0:
				roi_y=0
			roi_w=temp.w()+set_w
			if roi_w>320-roi_x:
				roi_w=320-roi_x
			roi_h=temp.h()+set_h
			if roi_h>240-roi_y:
				roi_h=240-roi_y
			img.draw_rectangle(roi_x,roi_y,roi_w,roi_h)
			for blob in img.find_blobs(thresholds2, roi=(roi_x,roi_y,roi_w,roi_h), merge=True):
				exist=1
			if exist==1:
				img.draw_cross(temp.cx(),temp.cy())
				img.draw_rectangle(temp.rect())
				pointrow=(int)(temp.cy()/div_col)
				pointcol=(int)(temp.cx()/div_row)
				str1="{0:03d}".format(pointcol)
				str2="{0:03d}".format(pointrow)
				if bflag==0:
					outstr="[%s%s]" % (str1,str2)
				else:
					if bflag==1:
						outstr="[%s%s)" % (str1,str2)
					else:
						outstr="(%s%s]" % (str1,str2)
				temp=uart.write(outstr)
				if debug==1:
					print(outstr)
					img.draw_string(tft_posx,tft_posy,(str)(pointrow))
					img.draw_string(tft_posx,tft_posy+10,(str)(pointcol))
	if debug==1:
		if rec_flag==1:
			img_rec.add_frame(sensor.snapshot())
			img.draw_string(136,190,"...")
		last_key_res=key_res
		key_res=key.key_read()
		if key_res==last_key_res:
			key_res=0
		if key_res==1:
			if menu_flag==1:
				menu_pos=menu_pos-1
				if menu_pos==-1:
					menu_pos=4
			if menu_flag==2:
				menu_pos=menu_pos-1
				if menu_pos==-1:
					menu_pos=2
			if menu_flag==3:
				if state==0:
					menu_pos=menu_pos-1
					if menu_pos==-1:
						menu_pos=5
				else:
					para_index=color*6+menu_pos
					para_temp[para_index]=para_temp[para_index]+1
					thresholds1=[tuple(para_temp[0:6])]
					thresholds2=[tuple(para_temp[6:12])]
					thresholds3=[tuple(para_temp[12:18])]
			if menu_flag==4:
				if state==0:
					menu_pos=1-menu_pos
				else:
					if menu_pos==0:
						tft_posx=tft_posx-10
					else:
						tft_posy=tft_posy-10
				if tft_posx<0:
					tft_posx=0
				if tft_posy<0:
					tft_posy=0
		if key_res==2:
			if menu_flag==1:
				debug=0
				if rec_flag==1:
					img_rec.close(20)
			if menu_flag==2:
				menu_flag=1
				menu_pos=0
			if menu_flag==3:
				menu_flag=2
				menu_pos=0
				state=0
			if menu_flag==4:
				menu_flag=1
				menu_pos=0
				state=0
		if key_res==3:
			if menu_flag==4:
				state=1
			if menu_flag==3:
				state=1
			if menu_flag==2:
				menu_flag=3
				color=menu_pos
				menu_pos=0
			if menu_flag==1:
				if menu_pos==0:
					menu_flag=2
					menu_pos=0
				if menu_pos==1:
					para.para_read(para_temp,18)
					img.draw_string(tft_posx+96,tft_posy+110,"ok!")
				if menu_pos==2:
					para.para_write(para_temp,18)
					img.draw_string(tft_posx+96,tft_posy+120,"ok!")
				if menu_pos==3:
					menu_flag=4
					menu_pos=0
				if menu_pos==4:
					if rec_flag==0:
						img_rec=mjpeg.Mjpeg("test.mjpeg")
						rec_flag=1
		if key_res==4:
			if menu_flag==1:
				menu_pos=menu_pos+1
				if menu_pos==5:
					menu_pos=0
			if menu_flag==2:
				menu_pos=menu_pos+1
				if menu_pos==3:
					menu_pos=0
			if menu_flag==3:
				if state==0:
					menu_pos=menu_pos+1
					if menu_pos==6:
						menu_pos=0
				else:
					para_index=color*6+menu_pos
					para_temp[para_index]=para_temp[para_index]-1
					thresholds1=[tuple(para_temp[0:6])]
					thresholds2=[tuple(para_temp[6:12])]
					thresholds3=[tuple(para_temp[12:18])]
			if menu_flag==4:
				if state==0:
					menu_pos=1-menu_pos
				else:
					if menu_pos==0:
						tft_posx=tft_posx+10
					else:
						tft_posy=tft_posy+10
				if tft_posx>192:
					tft_posx=192
				if tft_posy>80:
					tft_posy=80
		if menu_flag==3:
			if color==0:
				key.num_show(img,para_temp[0:6],tft_posx,tft_posy)
			if color==1:
				key.num_show(img,para_temp[6:12],tft_posx,tft_posy)
			if color==2:
				key.num_show(img,para_temp[12:18],tft_posx,tft_posy)
		key.menu_show(img,menu_flag,menu_pos,state,tft_posx,tft_posy)
		if debug==0:
			tft.TFT_Clear(img,tft_posx,tft_posy)
		tft.tftdisplay(img,tft_posx,tft_posy,128,160)
