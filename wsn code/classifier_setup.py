import pickle
import sys
import pandas as pd
from keras.models import Sequential
from keras.layers.core import Dense, Dropout
from keras.layers.normalization import BatchNormalization
from keras.optimizers import SGD
from keras.utils.np_utils import to_categorical

model_weights = pickle.load(open('model_weights.pkl', 'rb'))
training_data = pd.read_csv('../ML/training_data.csv')

X = training_data.iloc[:, :-1].values
y = training_data.iloc[:, -1].values
y_ohe = to_categorical(y)

model = Sequential()
model.add(BatchNormalization(input_shape=(X.shape[1],), weights = model_weights[0]))
model.add(Dense(output_dim=500, activation='tanh', weights =  model_weights[1]))
model.add(Dense(output_dim=350, activation='tanh', weights = model_weights[2]))
model.add(Dense(output_dim=200, activation='tanh', weights = model_weights[3]))
model.add(Dense(output_dim=50, activation='tanh', weights = model_weights[4]))
#model.add(Dropout(0.2, weights = model_weights[5]))
model.add(Dense(output_dim=y_ohe.shape[1], activation='softmax', weights = model_weights[6]))

sgd = SGD(lr=0.001, decay=1e-7, momentum=0.9)
print('Compiling model...')
model.compile(loss='categorical_crossentropy', optimizer=sgd)

# Dump model
sys.setrecursionlimit(10000)
pickle.dump(model, open('model.pkl', 'wb'))

