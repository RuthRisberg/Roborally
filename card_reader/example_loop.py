import serial

ser = serial.Serial("/dev/ttyACM0", baudrate=115200)

cardcommand = ["Player ", " card ", ":"]
powerdown = ["Does player "]

cards = ["f2 100", "f3 100", "r 100", "f3 100", "r 100", 
         "f1 100", "r 100", "f3 100", "f2 100", "r 100"]
index = 0

while True:
    s = str(ser.readline())[2:-5]
    print(s)

    if s.startswith(cardcommand[0]):
        s.lstrip(cardcommand[0])
    
        if index < len(cards):
            ser.write((cards[index]+"\r\n").encode('utf-8'))
            index += 1
            index %= len(cards)
    
    if s.startswith(powerdown[0]):
        ser.write("n\r\n".encode('utf-8'))
    