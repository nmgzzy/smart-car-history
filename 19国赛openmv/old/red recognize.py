import sensor, image, time, lcd

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)  #QVGA (320x240) QQVGA (160x120)
sensor.set_auto_whitebal(False)
sensor.skip_frames(time = 500)
clock = time.clock()
gry_threshold = (0,80)
red_threshold = (10, 80, 16, 73, -1, 56)
blue_threshold = (0, 100, -128, 127, -128, -10)

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

def main():
    i=0
    while(True):
        clock.tick()
        i=i+1
        img = sensor.snapshot()

        blobs = img.find_blobs([red_threshold], x_stride=20, y_stride=10, pixels_threshold=100)
        if blobs:
            max_blob = find_max(blobs)
            img.draw_rectangle(max_blob.rect())

        img_bin1 = img.to_grayscale(copy=True).binary([gry_threshold])
        if i%10==0:
            img_bin1.save("bin"+str(i/10)+".jpg")
        #img_bin2 = img.binary([red_threshold]).b_or(img.binary([blue_threshold]))
        #img_bin = img_bin1.b_or(img_bin2).invert()

        #lcd.display(img)

        print(clock.fps())


main()
