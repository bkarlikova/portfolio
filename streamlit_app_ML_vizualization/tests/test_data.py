"""This module is testing functionality of data_preparation."""
import pandas as pd
import numpy as np
from support_files import data_preparation as dp


def loading_any_df(df):
    """This function is helper function for test functions of each dataset.
    It tests:
    - whether there are any missing values in hash column.
    - whether it has the right columns
    """
    assert df["hash"].isna().sum() == 0
    assert (df.columns == ['hash', '1', '2', '3', '4', '5', '6', 'rokNarozeni', 'mesicNarozeni', 'pohlaviMuz',
                            'narodnost', 'zamereniFakultyPrvniVolby', 'OSP_v_terminu', 'VO_v_terminu', 'AO_v_terminu',
                            'ZSV_v_terminu', 'celkemZkousek', 'zkousek_OSP', 'zkousek_ZSV', 'max_OSP', 'max_OSP_VO',
                            'max_OSP _AO', 'max_ZSV']).all()


def test_df1_loading():
    """This function tests only dataset called dotazniky_T4_20_21.xlsx - data.csv.
    It tests:
    - whether the dataset is pd.DataFrame
    - shape
    - and other properties in "loading_any_df" function """
    df = dp.prepare_T4_20_21("data/dotazniky_T4_20_21.xlsx - data.csv")
    assert isinstance(df, pd.DataFrame)
    assert df.shape == (3162, 23)
    loading_any_df(df)


def test_df2_loading():
    """This function tests only dataset called dotazniky_T5_20_21.xlsx - data.csv.
    It tests:
    - whether the dataset is pd.DataFrame
    - shape
    - and other properties in "loading_any_df" function """
    df = dp.prepare_T5_20_21("data/dotazniky_T5_20_21.xlsx - data.csv")
    assert isinstance(df, pd.DataFrame)
    assert df.shape == (1819, 23)
    loading_any_df(df)


def test_df3_loading():
    """This function tests only dataset called dotazniky_T4_21_22.xlsx - data.csv.
    It tests:
    - whether the dataset is pd.DataFrame
    - shape
    - and other properties in "loading_any_df" function """
    df = dp.prepare_T4_5_21_22("data/dotazniky_T4_21_22.xlsx - data.csv")
    assert isinstance(df, pd.DataFrame)
    assert df.shape == (7504, 23)
    loading_any_df(df)


def test_df4_loading():
    """This function tests only dataset called dotazniky_T5_21_22.xlsx - data.csv.
    It tests:
    - whether the dataset is pd.DataFrame
    - shape
    - and other properties in "loading_any_df" function """
    df = dp.prepare_T4_5_21_22("data/dotazniky_T5_21_22.xlsx - data.csv")
    assert isinstance(df, pd.DataFrame)
    assert df.shape == (9182, 23)
    loading_any_df(df)


def test_whole_dataset():
    """This function tests dataset that is concatenated from other datasets.
    It tests:
    - whether the dataset is pd.DataFrame
    - whether there are some missing values in columns that are used as target values
    - whether there are some same values in hash column"""
    df = pd.read_csv("../shared_data/dataset.csv")
    assert isinstance(df, pd.DataFrame)
    assert df["OSP_v_terminu"].isna().sum() == 0
    assert df["VO_v_terminu"].isna().sum() == 0
    assert df["AO_v_terminu"].isna().sum() == 0
    assert (np.sort(df["hash"].values, kind="stable") == np.sort(np.unique(df["hash"].values), kind="stable")).all()


def test_encoded_dataset():
    """This function tests dataset that is concatenated from other datasets and encoded.
    It tests:
    - whether there are any missing values in na column
    - whether all the values are numeric
    - whether there are the right columns"""
    df = dp.make_encoded_dataset()
    assert df.isna().sum().sum() == 0
    assert pd.api.types.is_numeric_dtype(df.values)
    assert (df.columns == ['rokNarozeni', 'mesicNarozeni', 'pohlaviMuz', 'narodnost', 'zamereniFakultyPrvniVolby',
                           'OSP_v_terminu', 'VO_v_terminu', 'AO_v_terminu', 'ZSV_v_terminu', 'celkemZkousek',
                           'zkousek_OSP', 'zkousek_ZSV', 'max_OSP', 'max_OSP_VO', 'max_OSP _AO', 'max_ZSV',
                           'odkud_se_hlasi', 'kde_maturita', 'kategorie_studentu', 'po_ukonceni',
                           'vzdelani_matka', 'vzdelani_otec']).all()
