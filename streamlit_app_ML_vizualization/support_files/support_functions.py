"""This module contains support functions for this project such as:
- Class that contains all useful parameters about dataset that is used for training
- modifications - function for users choice of data modification
- target_variable - function for users choice of target variable
- separate_target_variable - which handles the separation of target variable from dataset
- modify_data - which handles the data modification based on users choice returned in fce "modifications"
- comuputing accuracy and creating error column in dataframe, so the scatter plot can plot points in three colors based
on that
- scatter plot for linear and ridge regression
- function for plotting decision tree
- helper function for finding the best max depth of decision tree"""
import streamlit as st
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.tree import DecisionTreeRegressor, plot_tree
from support_files import data_preparation as dp


class PrepareData:
    """Class that handles making dataset for:
    - training and validating a model
    - showing decoded dataset

    parameters:
    pd.DataFrame: df - existing dataframe
    bool: call - if modification and target variable is to be initialized
    string: target - customized target variable"""
    def __init__(self, df=None, call=True, target=None, drop=[]):
        if df is None:
            self.df = dp.make_encoded_dataset()
        else:
            self.df = df
        self.df_decoded = pd.read_csv("shared_data/dataset.csv")
        self.df_decoded.drop(labels=drop, axis=1, inplace=True)
        self.info = self.get_info(call, target)
        self.X = self.separate_target_variable(self.df, self.info.target_var)
        self.y = self.df[self.info.target_var]

    @staticmethod
    def get_info(call=True, target=None):
        """This function just initializes the attributes of class InfoAboutData"""
        return InfoAboutData(call, target)

    @staticmethod
    def separate_target_variable(df, target_var) -> pd.DataFrame:
        """This function modifies the dataset based on chosen variable.
        If the chosen variable is "OSP_v_terminu", there is deleted "VO_v_terminu" column,
        otherwise it is "OSP_v_terminu" """
        X = pd.DataFrame(df)
        if target_var == "OSP_v_terminu":
            try:
                X.drop(labels=[target_var, "VO_v_terminu"], axis=1, inplace=True)
            finally:
                pass
        else:
            try:
                X.drop(labels=[target_var, "OSP_v_terminu"], axis=1, inplace=True)
            finally:
                pass
        return X


class InfoAboutData:
    """This class handles information from user about data modification and target variable"""
    def __init__(self, call=True, target=None):
        self.modification = self.modifications(call)
        if target is not None:
            self.target_var = target
        else:
            self.target_var = self.target_variable()

    @staticmethod
    def modifications(call):
        """This function is for users choice of data modification.
        It is called, when the parameter "call" is nat False"""
        if call:
            return st.radio('Vyber úpravu dat', ["neupravená", "normalizovaná", "standardizovaná"])
        return None

    @staticmethod
    def target_variable():
        """This function is for users choice of target variable.
        It is called, when the parameter "call" is nat False"""
        st.markdown("---")
        return st.selectbox('Vyberte predikovanou proměnnou', ['OSP_v_terminu', 'VO_v_terminu', 'AO_v_terminu'])


class TrainTestValSets:
    """This class stores training, testing and validation datasets. It modifies the datasets based on users input
    in class InfoAboutData, which is initialized in class PrepareData in function prepare_data"""
    def __init__(self, df=None, call=True, target=None, drop=[]):
        self.data = self.prepare_data(df=df, call=call, target=target, drop=drop)
        self.X_train, self.X_test, self.X_val, \
            self.y_train, self.y_test, self.y_val = self.modify_data(self.data.X,
                                                                     self.data.y,
                                                                     self.data.info.modification)


    @staticmethod
    def prepare_data(df=None, call=True, target=None, drop=[]):
        """This function just initializes attributes of class "PrepareData" """
        return PrepareData(df, call=call, target=target, drop=drop)

    @staticmethod
    def modify_data(values, target, data_modification):
        """This funcion splits data to train, test and validation sets and modifies them by chosen modification
        pd.DataFrame: valuse - "X" - dataset of columns for prediction
        pd.DataFrame: target - "y" - dataset of target column
        string: data_modification - chosen modification from previos functions"""
        X_train, X_test, X_val, y_train, y_test, y_val = dp.split_data(values, target)
        if data_modification == "standardizovaná":
            X_train, X_test, X_val = dp.standardize_data(X_train, X_test, X_val)
        elif data_modification == "normalizovaná":
            X_train, X_test, X_val = dp.normalize_data(X_train, X_test, X_val)
        return X_train, X_test, X_val, y_train, y_test, y_val


def compute_accuracy(row):
    """Helper function for computing accuracy of predicted values"""
    if abs(row.normalized_error) < 0.05:
        return 'low'
    if abs(row.normalized_error) < 0.2:
        return 'medium'
    return 'high'


def create_error_df(y, predicted):
    """Helper function for creating dataframe which contains new columns of real, predicted and accuracy,
     computed by the compute_accuracy function"""
    df = pd.DataFrame()
    df['real'] = y
    df['predicted'] = predicted
    df['error'] = y - predicted
    df['normalized_error'] = df['error'] / (y.max() - y.min())
    df['accuracy'] = df.apply(compute_accuracy, axis=1)
    return df


def scatter_plot(df, title, color1, color2, color3):
    """Scatter plot for linear and ridge regression, which scatters the point in three colors depending on accuracy"""
    st.markdown("---")
    low = df[df.accuracy == 'low']
    medium = df[df.accuracy == 'medium']
    high = df[df.accuracy == 'high']
    fig, ax = plt.subplots(figsize=(25, 15))
    ax.scatter(low.real, low.predicted, s=70, color=color1, alpha=0.3, label='Malá přesnost')
    ax.scatter(medium.real, medium.predicted, s=70, color=color2, alpha=0.3, label='Střední přesnost')
    ax.scatter(high.real, high.predicted, s=70, color=color3, alpha=0.3, label='Vysoká přesnost')
    ax.plot([0, 100], [0, 100], '#8fffff')
    ax.set_xlabel('Reálná hodnota', fontsize=35)
    ax.set_ylabel('Predikovaná hodnota', fontsize=35)
    ax.set_title(title, fontsize=40)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    st.pyplot(fig)


def error_fce(y_test, y_test_predicted, y_val, y_val_predicted):
    """This function computes and prints the RMSE and R^2 loss functions"""
    col1, col2 = st.columns(2)
    with col1:
        st.markdown("#### Root mean squared error")
        st.write("RMSE na testovacích datech je: ", round(mean_squared_error(y_test, y_test_predicted, squared=False), 4))
        st.write("RMSE na validačních datech je: ", round(mean_squared_error(y_val, y_val_predicted, squared=False), 4))
    with col2:
        st.markdown("#### Koeficient determinace $R^2$")
        st.write("$R^2$ na testovacích datech je: ", round(r2_score(y_test, y_test_predicted), 4))
        st.write("$R^2$ na validačních datech je: ", round(r2_score(y_val, y_val_predicted), 4))


def plot_dec_tree(model):
    """This function just plot tree by plot_tree from sklearn package which uses matplotlib"""
    fig = plt.figure(figsize=(25, 20))
    plot_tree(model, max_depth=3, filled=True, rounded=True, fontsize=20)
    st.pyplot(fig, clear_figure=False)


def best_depth_for_tree(X_train, y_train, X_val, y_val):
    """This function returns the best depth for decision tree depending on lowest RMSE based on validation datasets"""
    best_rmse = np.inf
    best_depth = 1
    for depth in range(1, 100):
        dt_reg = DecisionTreeRegressor(max_depth=depth, random_state=42).fit(X_train, y_train)
        curr_rmse = mean_squared_error(y_val, dt_reg.predict(X_val), squared=False)
        if curr_rmse < best_rmse:
            best_rmse = curr_rmse
            best_depth = depth
    return best_depth
