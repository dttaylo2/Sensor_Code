import pickle
import pandas as pd
from keras.models import  Sequential
from keras.layers.core import Dense
from keras.optimizers import SGD
from keras.layers.normalization import BatchNormalization
from keras.utils.np_utils import to_categorical

data = pd.read_csv('test_data_041816.csv')
X = data.iloc[:, :-1].values
y = data.iloc[:, -1].values
y_ohe = to_categorical(y)

modelWeights = pickle.load(open('modelWeights.pkl', 'rb'))

model = Sequential()
model.add(BatchNormalization(input_shape=(X.shape[1],), weights = modelWeights[0]))
model.add(Dense(output_dim=500, activation='tanh', weights=modelWeights[1]))
model.add(Dense(output_dim=350, activation='tanh', weights=modelWeights[2]))
model.add(Dense(output_dim=200, activation='tanh', weights=modelWeights[3]))
model.add(Dense(output_dim=50, activation='tanh', weights=modelWeights[4]))
model.add(Dense(output_dim=y_ohe.shape[1], activation='softmax', weights=modelWeights[5]))

sgd = SGD(lr=0.01, decay=1e-6, momentum=0.95)
model.compile(loss='categorical_crossentropy', optimizer=sgd, metrics=['accuracy'])
