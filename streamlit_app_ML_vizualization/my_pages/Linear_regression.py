"""This module covers information about linear regression, model that can be trained by user based on chosen options
about dataset, shows graf of predicted points and prints values of loss functions"""
import streamlit as st
from sklearn.linear_model import LinearRegression
from support_files import support_functions as sf

header = st.container()
model_training = st.container()
graph = st.container()
error_stats = st.container()


def get_model(X, y):
    """Function, that trains a linear regression model"""
    lin_reg = LinearRegression().fit(X, y)
    return lin_reg


with header:
    st.header("Lineární regrese")
    st.markdown("---")
    st.markdown('''Lineární regresní model je takový, který se snaží predikovat vysvětlovanou proměnnou $Y$ na základě
    $p$ příznaků $X_1,... ,X_p$. Tuto závislost lze potom vyjádřit pomocí rovnice:''')
    st.latex(r'''Y = \omega_1x_1 + ... + \omega_px_p + \epsilon''')
    st.markdown(r'''kde $\omega$ jsou nějaké neznámé koeficienty a $\epsilon$ je náhodní veličina. Zkráceně lze psát
    $Y = \omega^T x + \epsilon$, kde $\omega = (\omega_1, ... , \omega_p)$ je tzv. vektor vah a $x = (x_1, ... , x_p)$
    je vektor hodnot''')
    st.markdown(r'''Pro minimalizace rovnice $y = \omega^T x$ se používá **metoda nejmenších čtverců**. Ta využvá
    kvadratické ztrátové funkce $L(Y,\hat Y) = (Y - \hat Y)^2$, která určuje velikost chyby v bode $x$. 
    Pro minimalizaci ztrátové funkce použijeme **reziduální součet čtverců**:''')
    st.latex(r'''RSS(\omega) = \sum_{i=1}^N L(Y_i,\omega^T x_i) = \sum_{i=1}^N L(Y_i - \omega^T x_i)^2 =
    ||Y-X\omega||''')
    st.markdown(r"tím získáme odhad $\hat \omega$.")
    st.markdown('''Pro získání minimální ztrátové funkce je nutné minimalizovat RSS. To lze udělat přes parciální
    derivaci a gradient, načež po úpravách získáme **normální rovnici**:''')
    st.latex(r'''X^T Y - X^T X\omega = 0''')
    st.markdown("která má jednoznačné řešení:")
    st.latex(r'''\omega_{OLS} = (X^TX)^{-1}X^TY''')
    st.markdown(r"A kýžená predikce $\hat Y$ v bodě $x$ je tedy: ")
    st.latex(r'''\hat Y = \hat \omega_{OLS}^Tx = x^T\hat \omega_{OLS} = x^T(X^TX)^{-1}(X^TY)''')
    st.markdown("---")
    st.markdown(r'''Při trénování lineární regrese si uživatel může maximálně zvolit úpravu dat, protože rovnice linerní
    regrese nemá žádné parametry, ale pouze hledá řešení rovnice $Y = \omega^T x + \epsilon$.''')
    st.markdown("---")

with model_training:
    data = sf.TrainTestValSets()
    model = get_model(data.X_train, data.y_train)

with graph:
    error_df = sf.create_error_df(data.y_val, model.predict(data.X_val))
    sf.scatter_plot(error_df, "Graf lineární regrese s barevně vyznačenou chybovou funkcí",
                    "#52d1ff", "#b29afd", "#ff4a80")

with error_stats:
    sf.error_fce(data.y_test, model.predict(data.X_test), data.y_val, model.predict(data.X_val))
