"""This module is used to edit dataset to representative od ready-to-train state. Because there are four different
datasets with different columns, there are lots of functions for just one or two datasets. """
import re
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import Normalizer


def drop_nan_in_hash(df):
    """This function is droping rows that has not the hash column filled,
     because the row is invalid without any identification"""
    df.dropna(subset="hash", inplace=True)
    return df


def replace_x_to_nan(df):
    """This function replaces all valuse of "x" to np.nan"""
    df.replace(["x"], np.nan, inplace=True)
    return df


def replace_5ab_56(df):
    """Thos funciton renames two columns, so they are called the same in each dataset"""
    df["5"] = df["5a"]
    df["6"] = df["5b"]
    df.drop(["5a", "5b"], axis=1, inplace=True)
    return df


def code_num_to_abc(df):
    """This function is replacing numeric answers to alphabetic,
     becouse in other datasets the answers are alphabetic"""
    for i in ["2", "5", "6"]:
        df[i] = df[i].replace(["1"], "a")
        df[i] = df[i].replace(["2"], "b")
        df[i] = df[i].replace(["3"], "c")
        df[i] = df[i].replace(["4"], "d")
        try:
            df[i] = df[i].replace(["5"], "e")
        finally:
            pass
        try:
            df[i] = df[i].replace(["6"], "f")
        finally:
            pass
    return df


def get_rid_of_1(df):
    """This function replaces 1 by letter that is in the name of the column,
     to be able to merge all colums like 1a, 1b, etc. to one column called 1"""
    for column in df.columns:
        if re.findall("^[0-9][a-z]$", column):
            x = re.findall("[a-z]$", column)
            df[column] = df[column].replace(["1"], x)
    return df


def del_columns(df):
    """This function deletes columst like 1a, 1b, 1c, etc..."""
    for column in df.columns:
        if re.findall("^[0-9][a-z]$", column):
            df.drop([column], axis=1, inplace=True)
    return df


def combine_columns(df):
    """This function combines columns like 1a, 1b, 1c to one column called 1 with a,b,c, values"""
    df["1"] = df["1a"].combine_first(df["1b"]).combine_first(df["1c"]).combine_first(df["1d"]) \
        .combine_first(df["1e"]).combine_first(df["1f"])
    df["3"] = df["3a"].combine_first(df["3b"]).combine_first(df["3c"]).combine_first(df["3d"]) \
        .combine_first(df["3e"])
    df["4"] = df["4a"].combine_first(df["4b"]).combine_first(df["4c"]).combine_first(df["4d"]) \
        .combine_first(df["4e"]).combine_first(df["4f"])
    return df


def get_right_columns(df):
    """This function returns a dataset with column, that are used in regression model"""
    return df[['hash', '1', '2', '3', '4', '5', '6', 'rokNarozeni', 'mesicNarozeni', 'pohlaviMuz', 'narodnost',
               'zamereniFakultyPrvniVolby', 'OSP_v_terminu', 'VO_v_terminu', 'AO_v_terminu', 'ZSV_v_terminu',
               'celkemZkousek', 'zkousek_OSP', 'zkousek_ZSV', 'max_OSP', 'max_OSP_VO', 'max_OSP _AO', 'max_ZSV']]


def rename_columns(df):
    """This function just renames the columns to be readable for user in the web app"""
    df["odkud_se_hlasi"] = df["1"]
    df["kde_maturita"] = df["2"]
    df["kategorie_studentu"] = df["3"]
    df["po_ukonceni"] = df["4"]
    df["vzdelani_matka"] = df["5"]
    df["vzdelani_otec"] = df["6"]
    df.drop(['1', '2', '3', '4', '5', '6'], axis=1, inplace=True)
    return df


def prepare_T4_20_21(dataset):
    """This function prepares dataset called "T4_20_21", because it has some unique columns from others"""
    df = pd.read_csv(dataset)
    df.drop(['6', '7', '8', '9a', '9b', '9c', '9d', '9e', '9f', '10', '11a', '11b', '11c', '11d', '11e',
             '11f', '12a', '12b', '12c', '12d', '12e', '13', '14', '15', '16a', '16b', '16c', '16d', '16e', '17'],
            axis=1, inplace=True)
    df = drop_nan_in_hash(df)
    df = replace_5ab_56(df)
    df = replace_x_to_nan(df)
    df = get_rid_of_1(df)
    df = combine_columns(df)
    df = del_columns(df)
    df = code_num_to_abc(df)
    df = get_right_columns(df)
    return df


def prepare_T5_20_21(dataset):
    """This function prepares dataset called "T5_20_21", because it has some unique columns from others"""
    df = pd.read_csv(dataset)
    df.drop(['6', '7', '8', '9a', '9b', '9c', '9d', '9e', '9f', '10', '11', '12', '13', '14a', '14b', '14c', '14d',
            '14e', '15', '16'], axis=1, inplace=True)
    df = drop_nan_in_hash(df)
    df = replace_5ab_56(df)
    df = replace_x_to_nan(df)
    df = get_rid_of_1(df)
    df = combine_columns(df)
    df = del_columns(df)
    df = code_num_to_abc(df)
    df = get_right_columns(df)
    return df


def prepare_T4_5_21_22(dataset):
    """This function prepares datasets called "T4_21_22" and "T5_21_22",
     because these two datasets have the same columns"""
    df = pd.read_csv(dataset)
    df.drop(['7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18'], axis=1, inplace=True)
    df = drop_nan_in_hash(df)
    df = get_right_columns(df)
    return df


def set_dtypes_and_cat_codes(df):
    """This function converts each dataset to encoded datasets, which is prepared for training"""
    df = df.apply(lambda x: x.astype(str).str.lower())
    df = df.astype({
        'odkud_se_hlasi': "category",
        'kde_maturita': "category",
        'kategorie_studentu': "category",
        'po_ukonceni': "category",
        'vzdelani_matka': "category",
        'vzdelani_otec': "category",
        'rokNarozeni': "float64",
        'mesicNarozeni': "float64",
        'pohlaviMuz': "float64",
        'narodnost': "category",
        'zamereniFakultyPrvniVolby': "category",
        'OSP_v_terminu': "float64",
        'VO_v_terminu': "float64",
        'AO_v_terminu': "float64",
        'ZSV_v_terminu': "float64",
        'celkemZkousek': "float64",
        'zkousek_OSP': "float64",
        'zkousek_ZSV': "float64",
        'max_OSP': "float64",
        'max_OSP_VO': "float64",
        'max_OSP _AO': "float64",
        'max_ZSV': "float64"})
    cat_columns = df.select_dtypes(['category']).columns
    df[cat_columns] = df[cat_columns].apply(lambda x: pd.factorize(x)[0])
    return df


def make_one_dataset(test_T4_20_21="data/dotazniky_T4_20_21.xlsx - data.csv",
                     test_T5_20_21="data/dotazniky_T5_20_21.xlsx - data.csv",
                     test_T4_21_22="data/dotazniky_T4_21_22.xlsx - data.csv",
                     test_T5_21_22="data/dotazniky_T5_21_22.xlsx - data.csv"):
    """This fucnction is making, concatenating and preparing the whole dataset to be in representative state and
     ready for encoding."""
    # prepare each dataset
    df1 = prepare_T4_20_21(test_T4_20_21)
    df2 = prepare_T5_20_21(test_T5_20_21)
    df3 = prepare_T4_5_21_22(test_T4_21_22)
    df4 = prepare_T4_5_21_22(test_T5_21_22)
    # concatenate datasets
    df = pd.concat([df1, df2, df3, df4], axis=0)
    df = rename_columns(df)
    # drop nan in predicted value
    df.dropna(subset=["OSP_v_terminu"], inplace=True)
    # drop duplicates by hash
    df.drop_duplicates(subset=["hash"], keep='last', inplace=True)
    return df


def make_encoded_dataset(dataset=None):
    """This function is filling all np.nan values and encodes the dataset"""
    if dataset is None:
        df = pd.read_csv("shared_data/dataset.csv")
    else:
        df = dataset
    df.drop(labels=["hash"], axis=1, inplace=True)
    df["odkud_se_hlasi"].fillna("f", inplace=True)
    df["po_ukonceni"].fillna("e", inplace=True)
    df.fillna(-1, inplace=True)
    df = set_dtypes_and_cat_codes(df)
    return df


def split_data(X, y):
    """This function splits the dataset into train, test and validation datasets, train to 1/2,
    test to 1/4 and validation to 1/4"""
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)
    X_train, X_val, y_train, y_val = train_test_split(X_train, y_train, test_size=0.5, random_state=42)
    return X_train, X_test, X_val, y_train, y_test, y_val


def standardize_data(X_train, X_test, X_val):
    """Function for standardizing data"""
    scaler = StandardScaler()
    scaler.fit(X_train)
    return scaler.transform(X_train), scaler.transform(X_test), scaler.transform(X_val)


def normalize_data(X_train, X_test, X_val):
    """Function for normalizing data"""
    normalizer = Normalizer()
    normalizer.fit(X_train)
    return normalizer.transform(X_train), normalizer.transform(X_test), normalizer.transform(X_val)
