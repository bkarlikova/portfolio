"""This module is for training a randome forest based on users input and showing the best model that are pretrained
in /random_forests/Best_random_forest.py"""
import pickle
import streamlit as st
from PIL import Image
from sklearn.ensemble import RandomForestRegressor
from support_files import support_functions as sf

header = st.container()
description = st.container()
users_model_training = st.container()
optimal_model = st.container()


def get_model(X, y, max_depth=2, n_estimators=100, max_features=1.0):
    """This function returns a model based on users input"""
    rf_reg = RandomForestRegressor(max_depth=max_depth,
                                   n_estimators=n_estimators,
                                   random_state=42,
                                   max_features=max_features).fit(X, y)
    return rf_reg


with header:
    st.header("Náhodný les")

with description:
    st.markdown("---")
    st.markdown(r'''<p align="justify">
    V posledním modelu využijeme tzn <b>esemble</b> metody, což jsou metody, které kombinjí více modelů tzv. 
    <b>weak learners</b> a v případě <b>Náhodného lesa</b> se jedná o několik <b>rozhodovacích stromů</b>.
    Základní myšlenka je v tom, že původní množina se rozdělí na několik menších podmnožin, na kterých se natrénují
    rozhodovací stromy a v případě <b>regresních</b> stromů se jejich výsledeky zprůměrují do výsledné predikované 
    hodnoty.
    </p>''', unsafe_allow_html=True)
    st.markdown("---")
    st.markdown(r'''<p align="justify">
    V tomto modelu si uživatel může nastavit hned tři hyper parametry. Hloubku stromů v lese, počet stromů a počet
    příznaků, které budou použity pro trénování jednotlivých stromů v lese. Vyhodnocování jednotlivých stromů, 
    respektive vrcholů ve stromu probíhá identicky jako ve funkci <b>DecissionTreeRegresor</b> pomocí minimalizace 
    <b>MSE (mean squared error)</b>
    </p>''', unsafe_allow_html=True)
    st.markdown("---")
    image = Image.open('random_forests/Random_forest_image.webp')
    st.image(image)
    st.markdown("---")


with users_model_training:
    data = sf.TrainTestValSets(drop=["hash"])
    depth = st.slider('Vyberte hloubku stromu: ', 1, 10, step=1)
    estimators = st.slider('Vyberte počet stromů v náhodném lese (n_estimators): ', 1, 500, step=25)
    features = st.slider('Vyberte počet náhodně vybraných příznaků (max_features): ',
                             0.001, 1.0, step=0.05, format="%f")
    model = get_model(data.X_train, data.y_train, max_depth=depth, n_estimators=estimators, max_features=features)
    sf.error_fce(data.y_test, model.predict(data.X_test), data.y_val, model.predict(data.X_val))

with optimal_model:
    st.markdown("### ")
    if data.data.info.target_var == "OSP_v_terminu":
        container = st.empty()
        button = container.button("Zobrazit výsledky optimálního náhodného lesa pro neupravená data")
        if button:
            st.header("Náhodný les s optimálními parametry")
            if data.data.info.modification == "neupravená":
                with open("random_forests/best_random_forest_unedited", 'rb') as file:
                    best_model = pickle.load(file)
            elif data.data.info.modification == "standardizovaná":
                with open("random_forests/best_random_forest_standardized", 'rb') as file:
                    best_model = pickle.load(file)
            elif data.data.info.modification == "normalizovaná":
                with open("random_forests/best_random_forest_normalized", 'rb') as file:
                    best_model = pickle.load(file)
            st.write("Optimální hodnota parametru n_estimators: ", best_model.n_estimators)
            st.write("Optimální hloubka stromů: ", best_model.max_depth)
            st.write("Optimální hodnota parametru max_features: ", best_model.max_features)
            sf.error_fce(data.y_test, best_model.predict(data.X_test), data.y_val, best_model.predict(data.X_val))
            container.empty()
            button = container.button("Schovat výsledky optimálního náhodného lesa pro neupravená data")
    else:
        st.error("Omlouváme se, ale pro tuto cílovou proměnnou není optimální model předtrénován.")
