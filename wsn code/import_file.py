import pandas as pd
import numpy as np

fileName = 'good_bearing_sharp_blade.txt'

f = open(fileName, 'r')
line = f.readline()

vibrations = []
readings = []

while(line):
    if line[0] == '1':
        value = line.strip().split(',')[1:]
        try:
            readings.append(np.array(value).astype('float'))
        except:
            pass
    else:
        value = line.strip().split(',')[1]
        try:
            vibrations.append(int(value))
        except:
            pass
    try:
        line = f.readline()
    except UnicodeDecodeError:
        print('Skipped Line')
        line = f.readline()

vibrations = np.array(vibrations)
readings = np.array(readings)

vibration_df = pd.DataFrame(vibrations, columns=['vibration'])
readings_df = pd.DataFrame(readings, 
                           columns=['current',
                                    'temp',
                                    'rpm'])

vibration_df.to_csv(fileName[:-4] + '_vibration.csv')
readings_df.to_csv(fileName[:-4] + '_readings.csv')
