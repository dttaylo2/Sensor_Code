import pandas as pd
import numpy as np

vibration = pd.read_csv('AccelerometerTest040916.csv', delimiter=';')
sensors = pd.read_csv('SensorTest040916.csv', delimiter=';')

accTimes = np.unique(vibration.receivedTime)
senTimes = np.unique(sensors.receivedTime)

accAvgs = []
accDevs = []

for time in senTimes:
    acc = vibration[vibration.receivedTime==time]
    sen = sensors[sensors.receivedTime==time]

    accReadings = int(np.round(acc.shape[0] / sen.shape[0]))
    
    for i in range(sen.shape[0]):
        if i != sen.shape[0] - 1:
            start = i * accReadings
            stop = (i + 1) * accReadings
            accAvgs.append(np.average(acc.magnitude[start:stop]))
            accDevs.append(np.std(acc.magnitude[start:stop]))
        else:
            start = i * accReadings
            stop = acc.shape[0]
            accAvgs.append(np.average(acc.magnitude[start:stop]))
            accDevs.append(np.std(acc.magnitude[start:stop]))
        
sensors = pd.concat((sensors,
                     pd.DataFrame(accAvgs, columns=['accelerationAvg']),
                     pd.DataFrame(accDevs, columns=['accelerationDev'])),
                    axis=1)
