import time
import board
import busio
import fourwire
from adafruit_st7789 import ST7789
import displayio
import terminalio
from adafruit_display_text import label
from digitalio import DigitalInOut, Direction, Pull
import adafruit_ticks
from adafruit_debouncer import Debouncer
import simpleio

pin = DigitalInOut(board.GP5)
pin.pull = Pull.UP
btn_down = Debouncer(pin)

pin = DigitalInOut(board.GP4)
pin.pull = Pull.UP
btn_up = Debouncer(pin)

pin = DigitalInOut(board.GP3)
pin.pull = Pull.UP
btn_left = Debouncer(pin)

pin = DigitalInOut(board.GP2)
pin.pull = Pull.UP
btn_right = Debouncer(pin)


# notes frequencies
C4 = 261.63
Db4 = 277.18
D4 = 293.66
Eb4 = 311.13
E4 = 329.63
F4 = 349.23
Gb4 = 369.99
G4 = 392.00
Ab4 = 415.30
A4 = 440.00
Bb4 = 466.16
B4 = 493.88

C5 = 523.25
Db5 = 554.37
D5 = 587.33
Eb5 = 622.25
E5 = 659.25
F5 = 698.46
Gb5 = 739.99
G5 = 783.99
Ab5 = 830.61
A5 = 880.00
Bb5 = 932.33
B5 = 987.77

# basic note duration
T = 0.15

# initialize the LED
led = DigitalInOut(board.LED)
led.direction = Direction.OUTPUT

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

# Create a display group
# Group is a special type of displayio object that can hold other displayio objects indepently
# https://learn.adafruit.com/circuitpython-display-support-using-displayio/group
group = displayio.Group()

# Set the default font for the text
font = terminalio.FONT

# Create a text label
text = "Hello World!"
text_label = label.Label(font, text=text, color=0xFFFFFF, scale=2)

# Position the text label
text_label.x = 100
text_label.y = 110

# Add the text label to the display group
group.append(text_label)

# Create a bitmap object directly from the bitmap file stored on the disk - it saves some memory
# The bitmap is in 4bpp format.
# https://learn.adafruit.com/circuitpython-display-support-using-displayio/display-a-bitmap
image = displayio.OnDiskBitmap('images/picopad.bmp')
image_area = displayio.TileGrid(image, pixel_shader=image.pixel_shader)

# Position the bitmap
image_area.x = 70
image_area.y = 20

# Add the bitmap to the display group
group.append(image_area)

# Show the whole display group
# It will display the text and the bitmap as well
# You can move every item in the group independently later
display.root_group = group

melody = [
    (E4, 1*T),
    (F4, 1*T),
    (G4, 2*T),
    (C5, 4*T),
    
    (D4, 1*T),
    (E4, 1*T),
    (F4, 6*T),

    (G4, 1*T),
    (A4, 1*T),
    (B4, 2*T),
    (F5, 4*T),
    
    (A4, 1*T),
    (B4, 1*T),
    (C5, 2*T),
    (D5, 2*T),
    (E5, 2*T),
]

# Play the melody and blink the LED in the rhythm :-)
for note in melody:
    frequency, duration = note
    simpleio.tone(board.GP15, frequency, duration)
    led.value = not led.value
    time.sleep(0.05)


# Move the Picopad logo when the buttons are pressed (the debouncer detects the button down/fell event)

while True:
    # Update button debouncer status
    btn_down.update()
    btn_up.update()
    btn_left.update()
    btn_right.update()

    if (btn_down.fell):
        image_area.y += 5
        simpleio.tone(board.GP15, D4, T)

    
    if (btn_up.fell):
        image_area.y -= 5
        simpleio.tone(board.GP15, E4, T)


    if (btn_left.fell):
        image_area.x -= 5
        simpleio.tone(board.GP15, D5, T)


    if (btn_right.fell):
        image_area.x += 5
        simpleio.tone(board.GP15, E5, T)
