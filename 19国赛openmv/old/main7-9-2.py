import sensor, image, time, pyb
import my_ips, my_file, my_uart, my_key

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framerate(2<<11)
sensor.set_framesize(sensor.QQVGA)#160x120
sensor.skip_frames(time = 1000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.skip_frames(time = 100)
clock = time.clock()
ledR = pyb.LED(1)
ledG = pyb.LED(2)
ledB = pyb.LED(3)
ledB.on()
my_ips.init()
time.sleep(100)
colorxy = 0
img_cnt = 0

red_threshold = [12, 80, 16, 73, -1, 56]
blue_threshold = [0, 50, -128, 127, -128, -5]
black_threshold = [0, 15, -128, 127, -128, 127]
white_threshold = [40, 100, -128, 127, -128, 127]

roi_white = [(105,0,2,120),(109,0,2,120),(113,0,2,120)]
roi_blue = [(0,21,33,77),(12,21,56,77),(0,21,68,77)]
roi_red = (47,25,103,70)

ledR.on()
ledG.on()
def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob
'''
def find_obstacle(img):
    blobs = img.find_blobs([red_threshold], roi = (40,0,80,120), x_stride=10, y_stride=5, pixels_threshold=200)
    if blobs:
        max_blob = find_max(blobs)
        img.draw_rectangle(max_blob.rect())
        print(max_blob.density())
        return max_blob.pixels
    return 0

def find_break_road(img):
    blobs = img.find_blobs([blue_threshold], roi = (60,120,40,30), x_stride=10, y_stride=5, pixels_threshold=200)
    if blobs:
        max_blob = find_max(blobs)
        img.draw_rectangle(max_blob.rect())
        print(max_blob.density())
        return max_blob.pixels
    return 0
'''
def my_cmp(a, b):
    if a == b:
        return (20,255,20)
    else:
        return (255,255,255)

def display(page, x, y, k, save):
    global img_cnt
    clock.tick()
    img = sensor.snapshot()
    if save:
        img.save("img\\img%02d.jpg"%img_cnt)
        img_cnt += 1
    global colorxy
    blobs = img.find_blobs([red_threshold], roi = roi_red, x_stride=5, y_stride=10, pixels_threshold=200)
    if blobs:
        road = find_max(blobs)
        img.draw_rectangle(road.rect())
    blobs = img.find_blobs([blue_threshold, black_threshold], roi = roi_blue[2], x_stride=3, y_stride=6, pixels_threshold=64)
    if blobs:
        road = find_max(blobs)
        img.draw_rectangle(road.rect())
    blobs = img.find_blobs([white_threshold], roi = roi_white[1], pixels_threshold=9)
    if blobs:
        road = find_max(blobs)
        img.draw_rectangle(road.rect())
    print(clock.fps())
    if x == 3 and y == 1:
        img.binary([white_threshold], zero = True)
    else:
        if y < 2:
            img.binary([red_threshold], zero = True)
        else:
            img.binary([blue_threshold], zero = True)

    img.draw_string(0,0,  str(red_threshold[0]),color = my_cmp(colorxy, 0),x_spacing = -2)
    img.draw_string(0,10, str(red_threshold[1]),color = my_cmp(colorxy, 1),x_spacing = -2)
    img.draw_string(40,0, str(red_threshold[2]),color = my_cmp(colorxy, 2),x_spacing = -2)
    img.draw_string(40,10,str(red_threshold[3]),color = my_cmp(colorxy, 3),x_spacing = -2)
    img.draw_string(80,0, str(red_threshold[4]),color = my_cmp(colorxy, 4),x_spacing = -2)
    img.draw_string(80,10,str(red_threshold[5]),color = my_cmp(colorxy, 5),x_spacing = -2)
    img.draw_string(120,10,str(white_threshold[0]),color = my_cmp(colorxy, 6),x_spacing = -2)

    img.draw_string(0,100, str(blue_threshold[0]),color = my_cmp(colorxy, 7),x_spacing = -2)
    img.draw_string(0,110, str(blue_threshold[1]),color = my_cmp(colorxy, 8),x_spacing = -2)
    img.draw_string(40,100,str(blue_threshold[2]),color = my_cmp(colorxy, 9),x_spacing = -2)
    img.draw_string(40,110,str(blue_threshold[3]),color = my_cmp(colorxy, 10),x_spacing = -2)
    img.draw_string(80,100,str(blue_threshold[4]),color = my_cmp(colorxy, 11),x_spacing = -2)
    img.draw_string(80,110,str(blue_threshold[5]),color = my_cmp(colorxy, 12),x_spacing = -2)
    img.draw_string(120,100,str(black_threshold[0]),color = my_cmp(colorxy, 13),x_spacing = -2)
    img.draw_string(120,110,str(black_threshold[1]),color = my_cmp(colorxy, 14),x_spacing = -2)

    if page == 0:
        my_ips.display_QQVGA(0, 0, img)
        if x == 0:
            if y == 0:
                red_threshold[0] += k
                colorxy = 0
            elif y == 1:
                red_threshold[1] += k
                colorxy = 1
            elif y == 2:
                blue_threshold[0] += k
                colorxy = 7+0
            elif y == 3:
                blue_threshold[1] += k
                colorxy = 7+1
        elif x == 1:
            if y == 0:
                red_threshold[2] += k
                colorxy = 2
            elif y == 1:
                red_threshold[3] += k
                colorxy = 3
            elif y == 2:
                blue_threshold[2] += k
                colorxy = 7+2
            elif y == 3:
                blue_threshold[3] += k
                colorxy = 7+3
        elif x == 2:
            if y == 0:
                red_threshold[4] += k
                colorxy = 4
            elif y == 1:
                red_threshold[5] += k
                colorxy = 5
            elif y == 2:
                blue_threshold[4] += k
                colorxy = 7+4
            elif y == 3:
                blue_threshold[5] += k
                colorxy = 7+5
        elif x == 3:
            if y == 1:
                white_threshold[0] += k
                colorxy = 6
            elif y == 2:
                black_threshold[0] += k
                colorxy = 7+6
            elif y == 3:
                black_threshold[1] += k
                colorxy = 7+7


def systemUI():
    page = 0
    Xsite = 0
    Ysite = 0
    global red_threshold
    global blue_threshold
    global black_threshold
    global white_threshold
    [red_threshold, blue_threshold, black_threshold, white_threshold] = my_file.read_parameter()
    while(True):
        save = 0
        key = my_key.get_key()
        if key == 3:#r
            if Xsite<3:
                Xsite += 1
        elif key == 6:#l
            if Xsite>0:
                Xsite -= 1
        elif key == 2:#u
            if Ysite>0:
                Ysite -= 1
        elif key == 0:#d
            if Ysite<3:
                Ysite += 1
        elif key == 1:#m
            break;#save = 1
        if key == 4:
            k = 1
        elif key == 5:
            k = -1
        else:
            k = 0
        display(page, Xsite, Ysite, k, save)
        if my_uart.read() == 0x0F:
            break
    my_file.save_parameter(red_threshold, blue_threshold, black_threshold, white_threshold)
    ledR.off()
    ledB.off()
    my_ips.spi.deinit()
    time.sleep(200)
    ledG.off()

def main():
    while True:
        clock.tick()
        pix = 0
        break_road = 0
        cnt1 = 0
        cnt2 = 0
        mode = 0
        hh = 0
        cc = 0
        img = sensor.snapshot()
        t = my_uart.read()
        if t!= None:
            mode = t >> 7
            offset = t & 0x7F
        blobs = img.find_blobs([red_threshold], roi = roi_red, x_stride=5, y_stride=10, pixels_threshold=200)
        if blobs:
            road = find_max(blobs)
            pix = road.pixels() * 255 // 3000
            if pix > 255:
                pix = 255
            #img.draw_rectangle(obstacle.rect())
        if mode == 0:
            blobs = img.find_blobs([blue_threshold, black_threshold], roi = roi_blue[0], x_stride=3, y_stride=6, pixels_threshold=100)
            if blobs:
                road = find_max(blobs)
                break_road = road.pixels()*255//2500
                #img.draw_rectangle(road.rect())
        else:
            blobs = img.find_blobs([blue_threshold, black_threshold], roi = roi_blue[1], x_stride=3, y_stride=6, pixels_threshold=100)
            if blobs:
                road = find_max(blobs)
                break_road = (road.pixels()-150)*255//3400
        if break_road < 0:
            break_road = 0
        if break_road > 255:
            break_road = 255
        blobs = img.find_blobs([white_threshold], roi = roi_white[1], pixels_threshold=20)
        if blobs:
            road = find_max(blobs)
            hh = road.h()
            cc = road.cy()+1
        my_uart.send(pix,break_road,cc,hh)
        if pix > 30:
            ledR.on()
        else:
            ledR.off()
        if break_road > 170:
            ledB.on()
        else:
            ledB.off()
        print(clock.fps())

systemUI()
main()
