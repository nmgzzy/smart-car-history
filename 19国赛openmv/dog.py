import sensor, image, time, pyb
import my_ips, my_file, my_uart, my_key

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_framerate(2<<11)
my_ips.init()
sensor.skip_frames(time = 500)
sensor.set_auto_whitebal(False)
sensor.skip_frames(time = 100)
clock = time.clock()
ledR = pyb.LED(1)
ledG = pyb.LED(2)
ledB = pyb.LED(3)
ledB.on()
ledR.on()
ledG.on()

threshold = [[0, 90, 30, 127, -20, 70],[0, 85, -20, 85, -128, -50]]
threshold_ch = 0
colorxy = 0
img_cnt = 0

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

def my_cmp(a, b):
    if a == b:
        return (20,255,20)
    else:
        return (255,255,255)


# def display(page, x, y, k, save):
#     global img_cnt
#     global red_ch
#     clock.tick()
#     img = sensor.snapshot()
#     if save:
#         img.save("img\\img%02d.jpg"%img_cnt)
#         img_cnt += 1
#     global colorxy
#     blobs = img.find_blobs([red_threshold[red_ch]], roi = roi_red, x_stride=5, y_stride=10, pixels_threshold=200)
#     if blobs:
#         road = find_max(blobs)
#         img.draw_rectangle(road.rect())
#     blobs = img.find_blobs([blue_threshold, black_threshold], roi = roi_blue[2], x_stride=3, y_stride=6, pixels_threshold=64)
#     if blobs:
#         road = find_max(blobs)
#         img.draw_rectangle(road.rect())
#     blobs = img.find_blobs([white_threshold], roi = roi_white[chioce], pixels_threshold=9)
#     if blobs:
#         road = find_max(blobs)
#         img.draw_rectangle(road.rect())
#     print(clock.fps())
#     if x == 3 and y == 0:
#         img.binary([white_threshold], zero = True)
#     else:
#         if y < 2:
#             img.binary([red_threshold[red_ch]], zero = True)
#         else:
#             img.binary([blue_threshold], zero = True)

#     img.draw_string(0,0,  str(red_threshold[red_ch][0]),color = my_cmp(colorxy, 0),x_spacing = -2)
#     img.draw_string(0,10, str(red_threshold[red_ch][1]),color = my_cmp(colorxy, 1),x_spacing = -2)
#     img.draw_string(40,0, str(red_threshold[red_ch][2]),color = my_cmp(colorxy, 2),x_spacing = -2)
#     img.draw_string(40,10,str(red_threshold[red_ch][3]),color = my_cmp(colorxy, 3),x_spacing = -2)
#     img.draw_string(80,0, str(red_threshold[red_ch][4]),color = my_cmp(colorxy, 4),x_spacing = -2)
#     img.draw_string(80,10,str(red_threshold[red_ch][5]),color = my_cmp(colorxy, 5),x_spacing = -2)
#     img.draw_string(0, 20,"red",                 color = (255,255,255),    x_spacing = -2)
#     img.draw_string(120,0,"wt:",                 color = my_cmp(colorxy, 6),x_spacing = -2)
#     img.draw_string(140,0,str(white_threshold[0]),color = my_cmp(colorxy, 6),x_spacing = -2)
#     img.draw_string(120,10,str(red_ch),            color = (255,255,0),      x_spacing = -2)

#     img.draw_string(0,90, "blue",                 color = (255,255,255),    x_spacing = -2)
#     img.draw_string(0,100, str(blue_threshold[0]),color = my_cmp(colorxy, 8),x_spacing = -2)
#     img.draw_string(0,110, str(blue_threshold[1]),color = my_cmp(colorxy, 9),x_spacing = -2)
#     img.draw_string(40,100,str(blue_threshold[2]),color = my_cmp(colorxy, 10),x_spacing = -2)
#     img.draw_string(40,110,str(blue_threshold[3]),color = my_cmp(colorxy, 11),x_spacing = -2)
#     img.draw_string(80,100,str(blue_threshold[4]),color = my_cmp(colorxy, 12),x_spacing = -2)
#     img.draw_string(80,110,str(blue_threshold[5]),color = my_cmp(colorxy, 13),x_spacing = -2)
#     img.draw_string(120,110, "blk",               color = my_cmp(colorxy, 14),x_spacing = -2)
#     img.draw_string(140,110,str(black_threshold[1]),color = my_cmp(colorxy, 14),x_spacing = -2)

#     if page == 0:
#         my_ips.display_QQVGA(0, 0, img)
#         if x == 0:
#             if y == 0:
#                 red_threshold[red_ch][0] += k
#                 colorxy = 0
#             elif y == 1:
#                 red_threshold[red_ch][1] += k
#                 colorxy = 1
#             elif y == 2:
#                 blue_threshold[0] += k
#                 colorxy = 8+0
#             elif y == 3:
#                 blue_threshold[1] += k
#                 colorxy = 8+1
#         elif x == 1:
#             if y == 0:
#                 red_threshold[red_ch][2] += k
#                 colorxy = 2
#             elif y == 1:
#                 red_threshold[red_ch][3] += k
#                 colorxy = 3
#             elif y == 2:
#                 blue_threshold[2] += k
#                 colorxy = 8+2
#             elif y == 3:
#                 blue_threshold[3] += k
#                 colorxy = 8+3
#         elif x == 2:
#             if y == 0:
#                 red_threshold[red_ch][4] += k
#                 colorxy = 4
#             elif y == 1:
#                 red_threshold[red_ch][5] += k
#                 colorxy = 5
#             elif y == 2:
#                 blue_threshold[4] += k
#                 colorxy = 8+4
#             elif y == 3:
#                 blue_threshold[5] += k
#                 colorxy = 8+5
#         elif x == 3:
#             if y == 0:
#                 white_threshold[0] += k
#                 colorxy = 6
#             elif y == 1:
#                 red_ch += k
#                 colorxy = 7
#             elif y == 3:
#                 black_threshold[1] += k
#                 colorxy = 8+6


# def systemUI():
#     page = 0
#     Xsite = 0
#     Ysite = 0
#     global red_threshold
#     global blue_threshold
#     global black_threshold
#     global white_threshold
#     global red_ch
#     [red_threshold, blue_threshold, black_threshold, white_threshold, red_ch] = my_file.read_parameter()
#     my_ips.showstr(161, 0, "bR 30:127")
#     my_ips.showstr(161, 1, "  -50:50")
#     my_ips.showstr(161, 3, "R  40:127")
#     my_ips.showstr(161, 4, "  -20:70")
#     my_ips.showstr(161, 6, "yR 30:127")
#     my_ips.showstr(161, 7, "   10:127")

#     while(True):
#         save = 0
#         key = my_key.get_key()
#         if key == 3:#r
#             if Xsite<3:
#                 Xsite += 1
#         elif key == 6:#l
#             if Xsite>0:
#                 Xsite -= 1
#         elif key == 2:#u
#             if Ysite>0:
#                 Ysite -= 1
#         elif key == 0:#d
#             if Ysite<3:
#                 Ysite += 1
#         elif key == 1:#m
#             save = 1
#         if key == 4:
#             k = 1
#         elif key == 5:
#             k = -1
#         else:
#             k = 0
#         display(page, Xsite, Ysite, k, save)
#         if my_uart.read() == 0x0F:
#             break
#     my_file.save_parameter(red_threshold, blue_threshold, black_threshold, white_threshold, red_ch)
#     ledR.off()
#     ledB.off()
#     my_ips.spi.deinit()
#     time.sleep(200)
#     ledG.off()

def main():
    while True:
        clock.tick()
        cx = 0
        cy = 0
        pix = 0
        flag = 0
        img = sensor.snapshot()

        blobs = img.find_blobs([threshold[threshold_ch]], x_stride=10, y_stride=10, pixels_threshold=100)
        if blobs:
            road = find_max(blobs)
            pix = road.pixels() // 250
            if pix > 255:
                pix = 255
            flag = 1
            cx = road.cx()
            cy = road.cy()
            img.draw_rectangle(road.rect())
        my_uart.send(flag,cx,cy,pix)
        if pix > 30:
            ledR.on()
        else:
            ledR.off()
        print(flag,cx,cy,pix,clock.fps())

# systemUI()
main()
