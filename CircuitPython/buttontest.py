import time
import board
import digitalio
import displayio
import busio
import fourwire
from adafruit_st7789 import ST7789
from adafruit_display_shapes.rect import Rect

# 2 Zoll 320x240 Waveshare Display 19888
cs=board.GP21
dc=board.GP17
reset=board.GP20
bl=board.GP16
# Release any resources currently in use for the displays
displayio.release_displays()
spi = busio.SPI(board.GP18, board.GP19)
display_bus = fourwire.FourWire(spi, command=dc, chip_select=cs, reset=reset)
display = ST7789(display_bus, rotation=270, width=320, height=240, backlight_pin=bl, rowstart=0, colstart=0)
# Backlight: 0 - aus, 1 - an
display.brightness = 1
main = displayio.Group()

# linke Reihe
button_up = digitalio.DigitalInOut(board.GP4)
button_up.switch_to_input(pull=digitalio.Pull.UP)
button_down = digitalio.DigitalInOut(board.GP5)
button_down.switch_to_input(pull=digitalio.Pull.UP)
button_right = digitalio.DigitalInOut(board.GP2)
button_right.switch_to_input(pull=digitalio.Pull.UP)
button_left = digitalio.DigitalInOut(board.GP3)
button_left.switch_to_input(pull=digitalio.Pull.UP)

button_X = digitalio.DigitalInOut(board.GP9)
button_X.switch_to_input(pull=digitalio.Pull.UP)
button_Y = digitalio.DigitalInOut(board.GP8)
button_Y.switch_to_input(pull=digitalio.Pull.UP)
button_A = digitalio.DigitalInOut(board.GP7)
button_A.switch_to_input(pull=digitalio.Pull.UP)
button_B = digitalio.DigitalInOut(board.GP6)
button_B.switch_to_input(pull=digitalio.Pull.UP)

## Make a background color fill for the weather
color_bitmap_temp = displayio.Bitmap(display.width, display.height, 3)
color_palette = displayio.Palette(1)
color_palette[0] = 0x660088
bg_sprite = displayio.TileGrid(color_bitmap_temp, pixel_shader=color_palette, x=0, y=0)
main.append(bg_sprite)

rect1 = Rect(60, 40, 40, 40, fill=0x666666)
main.append(rect1)
rect2 = Rect(60, 140, 40, 40, fill=0x666666)
main.append(rect2)
rect3 = Rect(110, 90, 40, 40, fill=0x666666)
main.append(rect3)
rect4 = Rect(10, 90, 40, 40, fill=0x666666)
main.append(rect4)

rect_X = Rect(200, 60, 40, 40, fill=0x666666)
main.append(rect_X)
rect_Y = Rect(270, 40, 40, 40, fill=0x666666)
main.append(rect_Y)
rect_A = Rect(200, 140, 40, 40, fill=0x666666)
main.append(rect_A)
rect_B = Rect(270, 120, 40, 40, fill=0x666666)
main.append(rect_B)

display.root_group = main

while True:
    if button_up.value == False:
        rect1.fill = 0xff0000
    else:
        rect1.fill = 0x666666
    if button_down.value == False:
        rect2.fill = 0xff0000
    else:
        rect2.fill = 0x666666
    if button_right.value == False:
        rect3.fill = 0xff0000
    else:
        rect3.fill = 0x666666
    if button_left.value == False:
        rect4.fill = 0xff0000
    else:
        rect4.fill = 0x666666
    if button_X.value == False:
        rect_X.fill = 0xff0000
    else:
        rect_X.fill = 0x666666
    if button_Y.value == False:
        rect_Y.fill = 0xff0000
    else:
        rect_Y.fill = 0x666666
    if button_A.value == False:
        rect_A.fill = 0xff0000
    else:
        rect_A.fill = 0x666666
    if button_B.value == False:
        rect_B.fill = 0xff0000
    else:
        rect_B.fill = 0x666666
