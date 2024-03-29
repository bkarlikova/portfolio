"""This module is used to introduce this app to user, describe the dataset, what is regression in general,
 what features can be used and what do they mean for machine learning regression models"""
import streamlit as st
from st_pages import Page, show_pages
from support_files import data_preparation as dp

header = st.container()
description = st.container()
dataset = st.container()
data_modification = st.container()
error_fce = st.container()

show_pages(
    [
        Page("the_app.py", "Úvod", ":black_small_square:"),
        Page("my_pages/Linear_regression.py", "Lineární regrese", ":black_small_square:"),
        Page("my_pages/Ridge_regression.py", "Hřebenová regrese", ":black_small_square:"),
        Page("my_pages/Decision_tree.py", "Rozhodovací strom", ":black_small_square:"),
        Page("my_pages/Random_forest.py", "Náhodný les", ":black_small_square:"),
        Page("my_pages/Users_prediction.py", "Napredikuj si percentil", ":black_small_square:")
    ]
)

with header:
    st.title('Trénování regresních modelů')
    st.markdown("---")

with description:
    st.markdown('''<p align="justify">
    V následujících stránkách se uživatel seznámí s několika známými regresními modely, které spadají do tzn. 
    supervidovaného učení. Regresní úlohy se používají pro predikci spojité proměnné. Uživatel si zde může vyzkoušet
    nastavit hyperparametry regresních modelů a svůj výsledek porovnat s optimalizovaným modelem.
    </p>''', unsafe_allow_html=True)
    st.markdown("---")
    st.markdown("## Informace o datasetu")
    st.markdown('''<p align="justify">
    Modely jsou trénovány na datech o uchazečích, kteří psali test OSP(obecné studijní předpoklady)
    slopečnosti Scio. OSP test se skládá ze dvou oddílů, analitického a verbálního, ze kterých je možné
    určit nezávislý (na druhém oddílu) percentil. OSP test testuje především schopnost studenta orientovat se v textu a
    logisky přemýšlet nikoliv tvrdé znalosti. V datasetu se uvádí i informace o percentilu z testu ZSV
    (základy společenských věd), pokud jej student absolvoval. Test ZSV zjištuje především tvrdé znalosti
    z oblasti humanitní věd. Pokud student některý test nikdy neabsolvoval je u něj uvedena hodnota -1. V
    datasetu se nachází zahešované jméno uživatele, 6 otázek, které nesouvisí primárně s výsledkem
    testu, dále pohlaví, národnost a zaměření fakuly na kterou se uchazeč hlásí, počet zkoušek celkem,
    které uživatel v minulosti absolvoval, počet zkoušk OSP a ZSV zvlášt,
    maximální dosažený percentil u testu OSP, verbálním oddíle, analitickém oddíle a v testu ZSV. Nakonec obsahuje
    informacio dosaženém percentilu v termínu, který je predikovanou proměnnou. S podrobným zněním otázek a odpovědí se
    uževatel může setkat na poslední stránce "Napredikuj si percentil", na které na základě jim zvolených odpovědí může
    zjistit predikovaný percentil.
    </p>''', unsafe_allow_html=True)
    st.markdown("---")
    st.markdown("## Cílová proměnná")
    st.markdown('''Pro variabilutu projektu si uživatel může vybrat ze tří predikovaných proměnných:''')
    st.markdown("+ _OSP_v_terminu - celkový percentil získaný ve zkoumaném termínu_")
    st.markdown("+ _AO_v_termínu - jako analytický oddíl_")
    st.markdown("+ _VO_v_terminu - jako verbální oddíl_")
    st.markdown('''<p align="justify">
    Pokud uživatel zvolí <i>OSP_v_terminu</i> z datasetu se odebere predikovaná proměnná a <i>VO_v_terminu</i>, protože 
    hodnota <i>OSP_v_terminu</i> je přímo závislá na hodnotám <i>VO_v_terminu</i> a </i>AO_v_terminu</i>. Pokud
    uživatel zvolí jako cílovou proměnnou jenden z oddílů, z datasetu se odstraní predikovaná proměnná a 
    <i>OSP_v_termínu</i>
    </p>''', unsafe_allow_html=True)

with data_modification:
    st.markdown("---")
    st.markdown("## Úprava dat")
    st.markdown('''<p align="justify">
    Pro zisk lepšího výledku si může uživatel u každého modelu zvolit, zda chce data nějak upravit. Nabízí
    se data neupravená, normalizovaná a standardizovaná.
    </p>''', unsafe_allow_html=True)
    st.markdown("##### Normalizovaná data")
    st.markdown('''<p align="justify">
    Normalizace dat převede všechny příznaky na škálu $[0,1]$, čímž zabráníme vychýlení způsobené 
    pestrou škálou příznaků. Normalizaci je dobré použít pokud příznaky mají různá měřítka (např. jeden je v cm a jiný 
    v km). V tomto datasetu uvidíme, že normalizace obvykle nepřinese dobré výsledky, protože data jsou na podobné 
    škále.
    </p>''', unsafe_allow_html=True)
    st.latex(r'''x_i \gets \frac{x_i - min_x}{max_x - min_x}''')
    st.markdown("##### Standardizazovaná data")
    st.markdown(r'''Standardizace dat je další způsob normalizace, která přiřadí hodnotu $x_i$ na základě výběrového
    rozptylu $\bar{x} = \frac{1}{n} \sum_i x_i$ a výběrový rozptyl $s_x^2 = \frac{1}{1-n}\sum_i(x_i - \bar{x})^2$''')
    st.latex(r'''x_i \gets \frac{x_i - \bar{x}}{\sqrt{s_x^2}}''')

with error_fce:
    st.markdown("---")
    st.markdown("## Chybové funkce")
    st.markdown('''Uživatel má v této aplikaci možnost porovnat vlatní model podle dvou chybových funkcí''')
    st.markdown("##### RMSE - root mean squared error")
    st.markdown('''<p align="justify">
    Nejčastější volba kvadratické chyby vede na evaluaci pomocí MSE - mean squared error. Tato míra"
    penalizuje především velké odchylky a je citlivá na odlehlé hodnoty. Pro lepší představu však použijeme její 
    odmocninu, tedy RMSE, které má stejné vlastnosti, akorá je ve stejných jednotkách jako vysvětlovaná proměnná.
    </p>''', unsafe_allow_html=True)
    st.latex(r'''RMSE = \sqrt{\frac{1}{N}\sum_{i=}^N(Y_i - \hat Y_i)^2}''')
    st.markdown("##### Koeficient determinizace $R^2$")
    st.markdown('''Nebo také $R^2$ kvadrát vysvětluje, jaký podíl variability cílové proměnné model vysvětluje''')
    st.latex(r'''R^2 = 1 - \frac{RSS}{SST}''')
    st.markdown("kde RSS(residuální součet čtverců - residual sum of squares)")
    st.latex(r'''{RSS = \sum_{i=1}^{N}(Y_i - \hat Y_i)^2}''')
    st.markdown("kde SST(totální součet čtverců - total sum of squares)")
    st.latex(r'''{SST = \sum_{i=1}^{N}(Y_i - \bar Y)^2}''')
    st.markdown('''$R^2$ kvadrát nabývá hodnot $[0,1]$ s tím, že $1$ je nejlepší scóre a $0$ nejhorší.''')

with dataset:
    container = st.empty()
    df_decoded = dp.pd.read_csv("shared_data/dataset.csv")
    button = container.button("Zobrazit ukázku dat")
    if button:
        st.write(df_decoded.head(20))
        container.empty()
        button = container.button("Schovat ukázku dat")
