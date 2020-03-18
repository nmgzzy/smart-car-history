from pyb import UART

uart = UART(4, 115200)
uart.init(115200, bits=8, parity=None, stop=1, timeout=1)

def send(*data):
    buff = [0xF0, 0xA5, len(data)] + list(data)
    c = sum(data) & 0xFF
    buff.append(c)
    for i in buff:
        uart.writechar(i)

def read():
    a = uart.read()
    if a:
        return a[-1]
    else:
        return None
