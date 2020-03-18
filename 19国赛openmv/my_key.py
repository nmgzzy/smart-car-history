from pyb import Pin
import time

key0  = Pin("P6", Pin.IN)
key1  = Pin("P9", Pin.IN)
key2  = Pin("P10", Pin.IN)

key_last = 0
cnt = 0
flag = 1

def get_key():
    global key_last
    global cnt
    global flag
    a = key0.value()
    b = key1.value() << 1
    c = key2.value() << 2
    key = a|b|c
    key_out = 7
    if key_last != key and key_last == 7:
        time.sleep(10)
        a = key0.value()
        b = key1.value() << 1
        c = key2.value() << 2
        key = a|b|c
        key_out = key
        cnt = 0
        flag = 1
    elif key_last == key and key_last != 7 and (key == 4 or key == 5):
        cnt = cnt + 1
        if cnt > flag*2+1:
            key_out = key
            cnt = 0
            flag = 0
    key_last = key
    return key_out

