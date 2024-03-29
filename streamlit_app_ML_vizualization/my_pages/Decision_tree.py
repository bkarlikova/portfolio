"""This module contains:
- information about decision trees, mainly for regressions
- some users instructions for choosing hyperparameter max_depth, training the model and reading its graph
- a model with best value of maximal depth and its graph"""
import streamlit as st
from sklearn.tree import DecisionTreeRegressor
from support_files import support_functions as sf


header = st.container()
description = st.container()
users_model_training = st.container()
optimal_model = st.container()


def get_model(X, y, max_depth=None):
    """This function train a decision tree of given maximal depth. If there is no given depth,
    then nodes are expanded until all leaves are pure.
    (see: https://scikit-learn.org/stable/modules/generated/sklearn.tree.DecisionTreeRegressor.html)"""
    dtr_reg = DecisionTreeRegressor(max_depth=max_depth, random_state=42).fit(X, y)
    return dtr_reg


with header:
    st.header("Rozhodovací strom")


with description:
    st.markdown("---")
    st.markdown('''<p align="justify">Další regresním modeleme je <b>rozhodovací strom</b>. Stromy jsou hojně využívané
    pro klasifikační úlohy, ve kterých je cílová proměnná pouze dvou kategorií(např. ano/ne). Tázaná hodnoty "propadává"
    stromem až do listu, ve kterém je vyhodnocena jako ano, nebo ne. Při spojité tázané proměnné jako v případě 
    percentilu je výsledkem <b>průměr</b> všech hodnot v koncovém listu</p>''', unsafe_allow_html=True)
    st.markdown('''<p align="justify">Cílem je aby hodnoty vysvětlované proměnné byly co nejblíže střední
    hodnotě, což budeme měřit pomocí <b>MSE (mean square error)</b></p>''', unsafe_allow_html=True)
    st.latex(r'''MSE(Y) = \frac{1}{N}\sum_{i=0}^{N-1}(Y_i - \bar Y)^2''')
    st.markdown('''V každém vrcholu strom rozděluje množinu $D$ na množiny $D_L$ a $D_R$ na základě:''')
    st.latex(r'''MSE(D) - t_L MSE(D_L) - t_R MSE (D_R)''')
    st.markdown(r'''kde $t_L = {\# D_L \over \# D}$ a $t_R = {\#D_R \over \#D}$''')
    st.markdown('''---''')
    st.markdown('''<p align="justify">Uživatel má možnost si u toho modelu zvolit hloubku stromu. Muže si všimnout, že
    čím větší hloubka, tím není vždy výsledek lepší. Ve spodní části se uživatel může podívat na strom s ideální 
    hloubkou získanou na základě minimílního <b>RMSE</b>.</p>''', unsafe_allow_html=True)
    st.markdown(r'''<p align="justify">Uživateli je i k dispozici vizualize stromu. Je možné v každém listu sledovat
    právě zmíněnou hodnptu <b>MSE</b>, <b>počet hodnot</b> a prediokovanou hodnotu, která je rovna zmíněnému
    <b>průměru</b>. Pokud uživatel zvolá hloubku <b>menší než 3</b>, vykreslí se strom hlouby zvolené
    <b>hloubky + 1</b>. Pokud zvolí větší hloubku zobrazí se pouze <b>4</b> z důvodu čitelnosti.
    </p>''', unsafe_allow_html=True)
    st.markdown('''---''')

with users_model_training:
    data = sf.TrainTestValSets()
    depth = st.slider('Vyberte hloubku stromu: ', 1, 10, 1)
    model = get_model(data.X_train, data.y_train, max_depth=depth)
    sf.error_fce(data.y_test, model.predict(data.X_test), data.y_val, model.predict(data.X_val))
    sf.plot_dec_tree(model)


with optimal_model:
    st.markdown("---")
    st.header("Rozhodovací strom s optimální hloubkou")
    optimal_depth = sf.best_depth_for_tree(data.X_train, data.y_train, data.X_val, data.y_val)
    best_model = get_model(data.X_train, data.y_train, max_depth=optimal_depth)
    st.write("Optimální hloubka stromu je:", optimal_depth)
    sf.error_fce(data.y_test, best_model.predict(data.X_test), data.y_val, best_model.predict(data.X_val))
    sf.plot_dec_tree(best_model)
