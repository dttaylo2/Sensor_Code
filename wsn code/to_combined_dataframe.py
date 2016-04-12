import pandas as pd
import numpy as np

def to_combined_dataframe(readings, vibration):
    avg = []
    std = []
    sets = int(np.round(vibration.shape[0] / readings.shape[0]))
    for i in range(readings.shape[0]):
        if i < readings.shape[0] - 1:
            avg.append(np.average(vibration.vibration[i*sets:(i+1)*sets]))
            std.append(np.std(vibration.vibration[i*sets:(i+1)*sets]))
        else:
            avg.append(np.average(vibration.vibration[i*sets:]))
            std.append(np.std(vibration.vibration[i*sets:]))
    vibrAvg = pd.DataFrame(avg, columns = ['vibrAvg'])
    vibrStd = pd.DataFrame(std, columns = ['vibrStd'])
    return pd.concat((readings, vibrAvg, vibrStd), axis=1)
