import time
import RPi.GPIO as gpio

class  myCallBack:
    def __init__(self):
        self.start = -1
        self.end   = -1
        self.distance = -1
        self.firetime = -1
        self.running = False
    def callback(self, channel):
        mytime = time.time()
        if  self.start < 0:
            if  gpio.input(27) > 0:
                self.start = mytime
        else:
            if gpio.input(27)  == 0:
                self.end = mytime
                print(mytime - self.firetime)
                self.distance = (self.end - self.start) * 17000
                self.start = -1
                self.end   = -1

              
    def callback1(self, channel):
        if d.running == False:
            onecycle()

def reading(sensor):
    d.running = True
    if sensor == 0:
        gpio.setup(17, gpio.OUT)
        gpio.setup(27, gpio.IN,  pull_up_down=gpio.PUD_DOWN)
        gpio.add_event_detect(27, gpio.BOTH, callback=d.callback)
        gpio.output(17, gpio.LOW)
        d.distance = -1
        time.sleep(0.03)
        gpio.output(17, True)
        time.sleep(0.00001)
        d.firetime = time.time()
        gpio.output(17, False)
        while d.distance < 0:
            if gpio.input(27) > 0:
                print "interrupt did not happen"
            time.sleep(0.001)
        return d.distance
    else:
        print "wrong parameter"
    d.running = False
def setupMotor():
    gpio.setmode(gpio.BCM)
    global a
    a = (12,16,20,21)
    for i in a:
        gpio.setup(i, gpio.OUT)
        gpio.output(i, gpio.LOW)

def  Step( s ):
    if s in (0,1,7):
        gpio.output(a[0], True)
    else:
        gpio.output(a[0], gpio.LOW)
    if s in (1,2,3):
        gpio.output(a[1], True)
    else:
        gpio.output(a[1], gpio.LOW)
    if s in (3,4,5):
        gpio.output(a[2], True)
    else:
        gpio.output(a[2], gpio.LOW)
    if s in (5,6,7):
        gpio.output(a[3], True)
    else:
        gpio.output(a[3], gpio.LOW)
    time.sleep(0.001)

def Forward():
    t = time.time()
    while time.time() - t < 10:
        for n in (0,1,2,3,4,5,6,7):
            Step(n)
def Reverse():
    t = time.time()
    while time.time() - t < 10:
        for n in (7,6,5,4,3,2,1,0):
            Step(n)
def onecycle():
    try:
        print reading(0)
        setupMotor()
        Forward()
        Reverse()
    except KeyboardInterrupt:
        gpio.cleanup()
def test():
    try:
        print "inside test"
        global d
        d = myCallBack()
        gpio.setwarnings(False)
        gpio.setmode(gpio.BCM)
        gpio.setup(23, gpio.IN)
        gpio.add_event_detect(23, gpio.FALLING, callback=d.callback1)
        onecycle()
        gpio.remove_event_detect(23)
        gpio.remove_event_detect(27)
        
        gpio.cleanup()
    except KeyboardInterrupt:
        gpio.cleanup()
if __name__ == "__main__":
    print " before test" 
    test()
        
