"""This modul is used to find the best random forest with the best hyper parameters. Because the training of it takes
a long time, this module find the best hyper parameters and export the machine learning training model into a file.
So the web app can easily and quickly show the result of predicted values and best hyper parameters to the user"""
import pickle
import os
import sys
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import GridSearchCV

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__),os.pardir))
sys.path.append(PROJECT_ROOT)

import support_files.data_preparation as dp



df = dp.make_encoded_dataset()

X_train, X_test, X_val, y_train, y_test, y_val = dp.split_data(X=df.drop(labels=["OSP_v_terminu", "VO_v_terminu"], axis=1),
                                                               y=df["OSP_v_terminu"])
X_train_st, X_test_st, X_val_st, = dp.standardize_data(X_train, X_test, X_val)
X_train_nor, X_test_nor, X_val_nor, = dp.standardize_data(X_train, X_test, X_val)
param_grid = {
    'n_estimators': [50, 100, 200],
    'max_features': [0.1, 0.3, 0.5, 0.6, 0.7, 0.75, 0.8, 0.9, 1],
    'max_depth': range(5, 10)
}


def get_model(X, y, parameters):
    """This function returns the best model based on the best parameter combination"""
    random_forest_tuning = RandomForestRegressor(random_state=42)
    gs_random_forest_reg = GridSearchCV(estimator=random_forest_tuning, param_grid=parameters, cv=5)
    gs_random_forest_reg.fit(X, y)
    return gs_random_forest_reg.best_estimator_


best_model_for_unedited_data = get_model(X_train, y_train, param_grid)
print("done")
best_model_for_standardized_data = get_model(X_train_st, y_train, param_grid)
print("done")
best_model_for_normalized_data = get_model(X_train_nor, y_train, param_grid)
print("done")

with open("best_random_forest_unedited", 'wb') as file:
    pickle.dump(best_model_for_unedited_data, file)
with open("best_random_forest_standardized", 'wb') as file:
    pickle.dump(best_model_for_standardized_data, file)
with open("best_random_forest_normalized", 'wb') as file:
    pickle.dump(best_model_for_normalized_data, file)
