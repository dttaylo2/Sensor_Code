import pandas as pd
import numpy as np

file = open('bad_bearing_no_cut.txt', 'r')

vibration = []
readings = []

lines = file.readlines()
for line in lines:
    line = line.strip()
    text = line.split(',')
    if len(text) == 4 and text[3] != '':
        readings.append(text[1:])
        #readings[len(readings) - 1][0] = readings[len(readings) - 1][0][:-1]
    elif len(text[0]) > 0:
        vibration.append(text[1])
readings = np.array(readings).astype('float')
vibration = np.array(vibration).astype('float')

readings_df = pd.DataFrame(readings, columns=['curr', 'temp', 'rpm'])
vibration_df = pd.DataFrame(vibration, columns=['vibr'])
vibration_var = vibration_df.rolling(window=20).var()
for i in range(19):
    vibration_var.iloc[i] = np.var(vibration_df.iloc[:i+1].values)

step = int(vibration.shape[0] / readings.shape[0])
vibration_sample = []

for i in range(0, readings_df.shape[0]):
    vibration_sample.append(np.average(vibration_var.iloc[i * step:(i+1)*step].values))

vibration_sample = np.array(vibration_sample).reshape((len(vibration_sample), 1))
vibration_sample = pd.DataFrame(vibration_sample, columns=['vibr_var'])

readings_df = pd.concat((readings_df, vibration_sample), axis=1)
classifier = [3] * readings.shape[0]
classifier = np.array(classifier).reshape((readings.shape[0], 1))
classifier = pd.DataFrame(classifier, columns=['classifier'])
readings_df = pd.concat((readings_df, classifier), axis=1)
