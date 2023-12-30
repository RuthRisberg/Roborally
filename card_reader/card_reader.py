import paho.mqtt.client as mqtt
import serial

ser = serial.Serial("/dev/ttyACM1", baudrate=115200)

cardcommand = ["Player ", " card ", ":"]
powerdown = ["Does player "]

cardmap = ["unset", "forward1", "forward2", "forward3", "back", "right", "left", "uturn"]

waitingForCards = False

robots = ["Twonkey"]
cards = [[]]
def clearCards():
    global cards
    cards = [[]]

def allCardsPresent():
    global cards
    for card in cards:
        if len(card) != 5:
            return False
    return True

def getAllSerial():
    s = str(ser.read_all())[2:-1]
    if len(s) < 2:
        return ''
    while s[-2:] != '\\n':
        s += str(ser.read_all())[2:-1]
    return s

def getLineSerial():
    s = str(ser.readline())[2:-5]
    print(s)
    return s

def checkSerial():
    global waitingForCards, cards

    # print("checking serial...")

    strings = getAllSerial()
    if strings != '':
        strings = [s.strip('\\r') for s in  strings.split('\\n')]
        
        for s in strings:
    # if not waitingForCards:
    #     s = getLineSerial()
            print(s)

            if s.startswith(cardcommand[0]):
                waitingForCards = True
                clearCards()
    
    elif allCardsPresent():
        first = True
        for card in cards:
            for c in card:
                if first:
                    first = False
                else:
                    while not getLineSerial().startswith(cardcommand[0]):
                        continue
                    print("sending to robot:", c)
                ser.write((c+"\r\n").encode('utf-8'))

        
        ser.write("n\r\n".encode('utf-8'))
        waitingForCards = False
        clearCards()
    
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("RoboRally/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    topic = msg.topic.split('/')
    if len(topic) != 5:
        return
    if topic[2] not in robots or topic[3] != "Cards":
        return
    i = robots.index(topic[2])
    print(int(topic[4]), len(cards[i]))
    if int(topic[4]) != len(cards[i]):
        return
    card, prio = str(msg.payload)[2:-1].split(',')

    if int(card) < len(cardmap):
        cards[i].append(cardmap[int(card)]+' '+prio)
    
    checkSerial()


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message


client.connect("10.254.47.209", 1883, 60)

client.loop_forever()
# client.loop_start()

# while True:
#     checkSerial()