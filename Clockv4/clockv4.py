import serial
import time
from datetime import datetime

startMarker = '<'
endMarker = '>'
dataStarted = False
dataBuf = ""
messageComplete = False
flag = True;

#---FUNCTIONS---

def getHour():             #get hour from system
    now = datetime.now()
    return now.strftime("%H")
def getMinute():           #get minute from system
    now = datetime.now()
    return now.strftime("%M")
def getSecond():           #get second from system ->for future use<-
    now = datetime.now()
    return now.strftime("%S")

def setupSerial(baudRate, serialPortName):
    
    global  serialPort
    
    serialPort = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

    waitForArduino()

def sendToArduino(stringToSend):
    
    #this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPort
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += stringToSend
    stringWithMarkers += (endMarker)

    serialPort.write(stringWithMarkers.encode('utf-8')) #need enconde for sending bytes to serial

def recvLikeArduino():

    global startMarker, endMarker, serialPort, dataStarted, dataBuf, messageComplete

    if (serialPort.inWaiting() > 0 and messageComplete == False):
        x = serialPort.read().decode("utf-8") # decode needed for Python3
        
        if dataStarted == True:
            if x != endMarker:
                dataBuf = dataBuf + x
            else:
                dataStarted = False
                messageComplete = True
        elif x == startMarker:
            dataBuf = ''
            dataStarted = True
    
    if (messageComplete == True):
        messageComplete = False
        return dataBuf
    else:
        return "XXX"

def waitForArduino():

    # wait until the Arduino sends 'Arduino is ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    print("Waiting for Arduino to reset")
    
    msg = ""
    while msg.find("Arduino is ready") == -1:
        msg = recvLikeArduino()
        if not (msg == 'XXX'):
            print(msg)

#---MAIN---

setupSerial(115200, "COM3")
count = 0
prevTime = time.time()
while flag is True:
    # check for a reply
    try:
        arduinoReply = recvLikeArduino()
        if not (arduinoReply == 'XXX'):
            print ("Time %s  Reply %s" %(time.time(), arduinoReply))
        if (arduinoReply == (getHour() + getMinute() + getSecond())):
            print ("LMAO")
            serialPort.close()
        elif (time.time() - prevTime > 1.0):
            sendToArduino(getHour() + getMinute() + getSecond())
            prevTime = time.time()
            count += 1
    except IOError:
        flag = False
print ("The end")
