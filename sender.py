import io
import json
import requests
import serial
import time

ard = False
while not ard:
    try: 
        arduino = serial.Serial(port='COM5', baudrate=9600, timeout=.1)
        ard = True
        print("Arduino connectée !")
    except:
        print("Arduino non connectée...")
        time.sleep(3)
    
def write(x):
    arduino.write(bytes(x, 'utf-8'))

while True:
    url = 'http://localhost:8080/getProdStats'
    done = False
    while not done:
        try:
            r = requests.get(url)
            done = True
            print("Serveur connecté!")
            data = json.loads(r.content.decode())
        except:
            print("Echec de la connexion, prochain essai dans 10 secondes...")
            time.sleep(10)
    dd={}
    i=1
    for dict_ in data:
        item = dict_['ItemName']
        prod = dict_['ProdPerMin']
        effprod = dict_['ProdPercent']
        effcons = dict_['ConsPercent']
        current = dict_['CurrentProd']
        if item in ["Charbon", "Ordinateur", "Supercalculateur", "Turbocarburant", "Cadre modulaire lourd", "Carburant","Plastique", "Caoutchouc"]:
            production = prod.split("-")[0][3:-5]
            prod_detail = production.split("/")
            if len(prod_detail) == 1:
                prod_detail.append(prod_detail[0])
            d = {"item":item,"prod":{"P":prod_detail[0], "C":prod_detail[1]},"effi":effprod,"current":current}
            dd[str(i)]=d
            i+=1
    write(str(dd))
    time.sleep(5)

# tesdt = {'1': {'item': 'Cadre modulaire lourd', 'prod': {'P': '0.0', 'C': '6.0'}, 'effi': 0.0, 'current': 0.0},
#  '2': {'item': 'Caoutchouc', 'prod': {'P': '201.0', 'C': '360.0'}, 'effi': 55.0, 'current': 201.0},
#  '3': {'item': 'Carburant', 'prod': {'P': '88392.0', 'C': '88392.0'}, 'effi': 72.0, 'current': 188392.0},
#  '4': {'item': 'Charbon', 'prod': {'P': '1963.0', 'C': '6855.0'}, 'effi': 28.0, 'current': 1963.0},
#  '5': {'item': 'Ordinateur', 'prod': {'P': '2.0', 'C': '24.0'}, 'effi': 9.0, 'current': 2.0},
#  '6': {'item': 'Plastique', 'prod': {'P': '1024.0', 'C': '1160.0'}, 'effi': 88.0, 'current': 1024.0},
#  '8': {'item': 'Turbocarburant', 'prod': {'P': '51216.0', 'C': '51216.0'}, 'effi': 97.0, 'current': 351216.0},
#  '7': {'item': 'Turbocarburant', 'prod': {'P': '51216.0', 'C': '51216.0'}, 'effi': 97.0, 'current': 351216.0}
#  }
# write(str(tesdt))

    # #print ("Your {} is being produced at {:.1%} efficiency ".format(item, effprod))
    # #print ("You are producing {} at a rate of {} U/Pm ".format(item, current))

    # if effprod < 0.95:
    #     print("{} production is operating at capacity. You are producing {} at a rate of {} U/pm".format(item, item, current))

    # else:
    #     print ("Your {} production is operating below 50% of capacity. Your current production rate is {} U/pm This is not very efficient ".format(item, current))

