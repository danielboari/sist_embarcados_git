import RPi.GPIO as GPIO
from time import sleep

x=10
GPIO.setmode(GPIO.BCM)
GPIO.setup(18,GPIO.OUT)
GPIO.setup(24,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
sleep (0.1)

print("GPIO%d = %d; GPIO%d = %d" % (24, GPIO.input(24))

p = GPIO.PWM(18,100)
p.start(0)
while x<=90:
    p.ChangeDutyCycle(x)
    # print("x ",x)
    if GPIO.input(24) == GPIO.HIGH:
        if(x>0):
            x=x+10
            sleep(0.5)

GPIO.cleanup()