from os.path import join
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow import keras
import seaborn as sns
sns.set(color_codes=True)
from pandas.io.parsers.readers import read_csv
from tensorflow.keras import models
from scipy import stats
import pandas as pd
import datetime
import time


# # Settings
models_path =      'model'  # Where we can find the model files (relative path location)
keras_model_name = 'test'           # Will be given .h5 suffix
array = ([0.122483,0.224439,0.543770,0.244897],[0.010065, 0.002441,0.003663,0.001428])
array = pd.DataFrame(array)

minmax = ([0.062343, 0.067652 ,0.058750, 0.047156],[ 0.053460 , 0.058970,0.052203, 0.041937])
minmax = pd.DataFrame(minmax)

# # Load model
#def testi(extest):
model = models.load_model(keras_model_name + '.h5')
model.summary()


    # from pandas.io.parsers.readers import read_csv
    # X_test = pd.DataFrame()
    # merge_data = pd.read_csv('saida.csv')
    # df = merge_data.drop(['tempo'],axis = 1)
    # merge_data['tempo'] = pd.to_datetime(merge_data['tempo'])
    # X_test.index = pd.to_datetime(merge_data['tempo'], format='%Y.%m.%d.%H.%M.%S');
    # X_test = X_test.sort_index();
    # df.index = merge_data['tempo']
    # X_test = df
def mapeia(x, in_min, in_max, out_min, out_max):
    df1 = x.reset_index()
    df1 = ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min
    return df1

def scaler_data(x):
    return (x-minmax.min())/(minmax.max()-minmax.min())


def testi(X_test):
    X_test = mapeia(X_test, 0,255,array.min(),array.max()) #converte pra avaliar no modelo
    #print(X_test[0])
    X_test.to_csv('leituras_sensor_'+'.csv', mode='a', index=datetime.datetime.now().strftime("%Y.%m.%d.%H.%M.%S"), header=False)
    X_test = scaler_data(X_test)
    #print(X_test)
    inicio = time.time() 
    X_pred = model.predict(np.array(X_test))
    fim = time.time()
    X_pred = pd.DataFrame(X_pred, columns=X_test.columns)
    X_pred.index = X_test.index

    scored = pd.DataFrame(index=X_test.index)
    scored['Loss_mae'] = np.mean(np.abs(X_pred-X_test), axis = 1)
    #scored['Threshold'] = 0.25
    scored['Anomaly'] = scored['Loss_mae'] > 0.25
    fim = time.time()
    print("Tempo Inferência =", (fim - inicio))   
    #scored['Loss_mae'] = np.mean(np.abs(X_pred-X_test), axis = 1)
    #scored.plot(logy= True , figsize = (10,6), ylim = [1e-2,1e2], color = ['blue','red'])
    #plt.show()
    print(scored)
    #scored.to_csv('mae_leitura_sensores'+'.csv', mode='a', index=datetime.datetime.now().strftime("%Y.%m.%d.%H.%M.%S"), header=False)

def update_data(X_serial): 
    X_test = X_serial
    for i, X_serial in enumerate(X_serial):
        if (i>9):
            if((i+2)%4==0):
                print(f'\n{int(i/4-2):>3} |', end = '')
            print(f'{X_serial:>4} ', end = '')
    print('\n' + '-' * (4*5 + 4))
    testi(X_test)


    # scored_train = pd.DataFrame(index=X_serial.index)
    # scored_train['Loss_mae'] = np.mean(np.abs(X_pred_train-X_serial), axis = 1)
    # scored_train['Threshold'] = 3e-3
    # scored_train['Anomaly'] = scored_train['Loss_mae'] > scored_train['Threshold']
    # scored = pd.concat([scored_train, scored])

