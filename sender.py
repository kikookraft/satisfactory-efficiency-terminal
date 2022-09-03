#
# based on https://github.com/porisius/FicsitRemoteMonitoring/tree/main/Examples
#      and https://create.arduino.cc/projecthub/ansh2919/serial-communication-between-python-and-arduino-e7cce0
#

#all the imports
import json
import requests
import serial
import time

# while the arduino isn't connected, it ask for it
ard = False
while not ard:
    try: 
        arduino = serial.Serial(port='COM5', baudrate=9600, timeout=.1)
        #                             ^^^^ modify the port of your arduino (find it with the help of arduino IDE)
        ard = True
        print("Arduino connected !")
    except:
        print("Waiting for arduino connexion...")
        time.sleep(3) #re attempt every 3 seconds
    
def write(x): #the function that is used to send data to the arduino
    arduino.write(bytes(x, 'utf-8'))

while True:
    url = 'http://localhost:8080/getProdStats' # url on wich the Ficsit Remote Monitoring web server is running
    done = False
    while not done: # wait for the server to start, then get the data
        try:
            r = requests.get(url)
            done = True
            print("Server connected!") #remove this line of you dont want your console filling with this message
            data = json.loads(r.content.decode())
        except:
            print("No connexion to the server...")
            time.sleep(10)
    
    # transforming the data recieved and add a filter 
    # (the arduino can only handle 8 product)
    
    dd={} # dd will contain all the info sended to the arduino
    i=1 
    for dict_ in data: # getting all the data
        item = dict_['ItemName']
        prod = dict_['ProdPerMin']
        effprod = dict_['ProdPercent']
        effcons = dict_['ConsPercent']
        current = dict_['CurrentProd']
        
        # write the name of the product that you want displayed (8 maximum)
        # make sure you write correctly (or the arduino wont reiceve anything)
        i_want_this = ["Charbon", "Ordinateur", "Supercalculateur", "Turbocarburant", "Cadre modulaire lourd", "Carburant","Plastique", "Caoutchouc"]
        if item in i_want_this:
            # transforming the data so its simpler in the arduino code
            production = prod.split("-")[0][3:-5]
            prod_detail = production.split("/")
            if len(prod_detail) == 1:
                prod_detail.append(prod_detail[0])
            
            # making a sub-dict and put it in the main dict
            d = {"item":item,"prod":{"P":prod_detail[0], "C":prod_detail[1]},"effi":effprod,"current":current}
            dd[str(i)]=d
            i+=1
            
    write(str(dd)) #send data
    time.sleep(5) # modify this value if you want faster/slower refresh rate

#                   This below is what is sended to the arduino (more or less)
#
# test = {'1': {'item': 'Cadre modulaire lourd', 'prod': {'P': '0.0', 'C': '6.0'}, 'effi': 0.0, 'current': 0.0},
#         '2': {'item': 'Caoutchouc', 'prod': {'P': '201.0', 'C': '360.0'}, 'effi': 55.0, 'current': 201.0},
#         '3': {'item': 'Carburant', 'prod': {'P': '88392.0', 'C': '88392.0'}, 'effi': 72.0, 'current': 188392.0},
#         '4': {'item': 'Charbon', 'prod': {'P': '1963.0', 'C': '6855.0'}, 'effi': 28.0, 'current': 1963.0},
#         '5': {'item': 'Ordinateur', 'prod': {'P': '2.0', 'C': '24.0'}, 'effi': 9.0, 'current': 2.0},
#         '6': {'item': 'Plastique', 'prod': {'P': '1024.0', 'C': '1160.0'}, 'effi': 88.0, 'current': 1024.0},
#         '7': {'item': 'Turbocarburant', 'prod': {'P': '51216.0', 'C': '51216.0'}, 'effi': 97.0, 'current': 351216.0},
#         '8': {'item': 'Turbocarburant', 'prod': {'P': '51216.0', 'C': '51216.0'}, 'effi': 97.0, 'current': 351216.0}
#  }
# write(str(test))

