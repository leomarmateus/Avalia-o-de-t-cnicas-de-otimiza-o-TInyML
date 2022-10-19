# !pip install pyod            # normal install
# !pip install --upgrade pyod  # or update if needed
# generate data 
import numpy as np
import pandas as pd
from pyod.models.knn import KNN
from pyod.models.auto_encoder import AutoEncoder
from pyod.utils.data import generate_data


contamination = 0.05  # percentage of outliers
n_train = 1000  # number of training points
n_test = 500  # number of testing points
n_features = 25 # Number of features

X_train, X_test, y_train,  y_test = \
    generate_data(n_train=n_train,
                  n_test=n_test,
                  n_features=n_features,
                  contamination=contamination,
                  random_state=42,
                  offset = 10)
X_train = pd.DataFrame(X_train)
X_test = pd.DataFrame(X_test)
y_train= pd.DataFrame(y_train)
y_test= pd.DataFrame(y_test)



## usando apenas os dados normais 
a = int( n_train * (1-contamination))
X_train = X_train[:a]
b = int( n_train * (1-contamination))
y_train = y_train[:b]

X_train.to_csv(f'X_train_features_{n_features}.csv', sep=',', index=False)
X_test.to_csv(f'X_test_features_{n_features}.csv', sep=',', index=False)
y_train.to_csv(f'y_train_features_{n_features}.csv', sep=',', index=False)
y_test.to_csv(f'y_test_features_{n_features}.csv', sep=',', index=False)