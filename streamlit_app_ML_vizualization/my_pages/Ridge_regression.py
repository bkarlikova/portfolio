"""This module covers:
- information about Ridge regression
- about the math behind the model
- possibility of training a model based on data modification and hyperparameter upon users choice
- scatter plot of predicted values
- model with the best hyperparameter alpha
- loss functions of the trained models"""
import streamlit as st
from sklearn.linear_model import Ridge
from sklearn.linear_model import RidgeCV
from support_files import support_functions as sf


header = st.container()
users_model_training = st.container()
best_model_training = st.container()
col1, col2 = st.columns(2)


def get_model(X, y, alpha=1):
    """This funcion train a ridge regression of any given alpha (if there is no alpha, alpha is equal to 1)."""
    ridge_reg = Ridge(alpha).fit(X, y)
    return ridge_reg


def get_opt_ridge_reg_and_alpha(X_train, y_train):
    """This function returns the optimal model of ridge regression and its optimal alpha"""
    ridge_cv = RidgeCV(alphas=[0.00001, 0.0001, 0.001, 0.01, 0.1, 0.7, 1, 1.5, 2, 3, 5, 8, 10, 15,
                               20, 25, 50, 75, 100, 200, 250, 500, 1000, 10000]).fit(X_train, y_train)
    return ridge_cv, ridge_cv.alpha_


with header:
    st.header("Hřebenová regrese")
    st.markdown("---")
    st.markdown(r'''Hřebenová regrese stejně jako lineární je řešena pomocí normální rovnice, která odpovídá gobámnímu
    minimu $RSS_{\alpha}$ (residuálními součtu čtverců), závislém na parametru $\alpha ≥ 0$ ''')
    st.latex(r'''RSS_{\alpha}(\omega) = ||Y - X\omega||^2 + \alpha \sum_{i=1}^p\omega_i^2''')
    st.markdown(r"Pro $\alpha = 0$ suma zmizí a rovnice je identická jako pro lineární regresi")
    st.markdown(r"Pro $\alpha > 0$ bude minimum cílit na vektory $\omega$, které mají co nejmenší složky ")
    st.markdown("Ekvivalend normální rovnice je tedy:")
    st.latex(r'''X^TY - X^TX\omega-\alpha I^{\prime}\omega = 0''')
    st.markdown(r'''Vzhledem k tomu, že Hassova matice je poitivně definitní a matice $X^TX + \alpha I^{\prime}$ je
    regulární, existuje vždy právě určená **normální rovnice**:''')
    st.latex(r'''\hat \omega_{\alpha} = (X^TX + \alpha I^{\prime})^{-1}X^TY''')
    st.markdown(r"Predikce v bodě $x$ je potom stejně jako u lineární regrese $\hat Y = x^T \hat \omega_{\alpha}$")
    st.markdown("---")
    st.markdown('''<p align="justify">
    Zde uživatel může zkoumat průběh <b>hřebenové regrese</b> v závislosti na parametru <b>&alpha;</b>. Pro porovnání 
    ve spodní části stránky je možné sledovat graf s optimálním parametrem <b>&alpha;</b>, který je 
    nalezený pomocí knihovní funkce <i>RidgeCV</i> z <i>sklearn</i>, která ze zadaného pole najde optimální 
    parametr.<br><br> Pokud uživatel zvolí různé úpravy dat, může si všimnout, že parametr alpha se vždy rapidně změní.
    Například pro normalizovaná data je nejlepší parametr <b>&alpha; = 0</b> a protože funkce <i>RidgeCV</i> přijímá 
    hodnoty <b>&alpha; > 0</b>, vždy zvolí tu nejmenší z daného pole. Tedy pro normalizovaná data hřebenová regrese 
    nemá význam. Stane se z ní regrese lineární.
    </p>''', unsafe_allow_html=True)
    st.markdown("---")

with users_model_training:
    data = sf.TrainTestValSets()
    st.markdown("---")
    chosen_alpha = st.slider('Nastavte hodnotu aplha: ', 0, 100, 1)
    model = get_model(data.X_train, data.y_train, chosen_alpha)
    sf.scatter_plot(sf.create_error_df(data.y_val, model.predict(data.X_val)),
                    "Hřebenová regrese s vlasní nastavenou hodnotou aplha",
                    "#31de93", "#a6d746", "#ffc014")
    sf.error_fce(data.y_test, model.predict(data.X_test), data.y_val, model.predict(data.X_val))

with best_model_training:
    st.markdown("---")
    st.header("Hřebenová regrese s optimální hodnotou aplha")
    st.caption("V tomto modelu optimalizujeme hodnotu aplha pomocí fukce RidgeCV z knihovny sklearn. ")
    best_model, best_alpha = get_opt_ridge_reg_and_alpha(data.X_train, data.y_train)

    sf.scatter_plot(sf.create_error_df(data.y_val, best_model.predict(data.X_val)),
                    "Hřebenová regrese s optimální hodnotou alpha",
                    "#31de93", "#a6d746", "#ffc014")
    st.markdown("Optimální alpha  je: ")
    st.write(best_alpha)
    sf.error_fce(data.y_test, best_model.predict(data.X_test), data.y_val, best_model.predict(data.X_val))
