"""This module stores users input, create a row from it, and predict the percentile based on that"""
import pickle
import streamlit as st
import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.linear_model import RidgeCV
from sklearn.tree import DecisionTreeRegressor
from support_files import support_functions as sf
from support_files import data_preparation as dp


header = st.container()
users_input = st.container()
dataset = st.container()

with header:
    st.title("Napredikuj si percentil")
    st.markdown("---")
    st.markdown('''<p align="justify">
    Zde si uživatel může navolit jednotlivé <b>přiznaky</b> a <b>typ regrese</b>, na základě kterých mu bude předpověděn
    percentil u zkoušek. U otázek, kde je odpovědí nějaký získaný percentil, lze odpovědět <b> -1</b> pro případ, že
    žádný takový termín uživatel neabsolvoval.</p>''', unsafe_allow_html=True)
    st.markdown("---")


with users_input:
    asked_value = st.selectbox('Vyberte predikovanou proměnnou', ('OSP_v_terminu', 'VO_v_terminu', 'AO_v_terminu'))
    year_of_birth = st.slider("Zvolte rok narození", 1960, 2010, step=1)
    month_of_birth = st.slider("Zvolte mesíc narození", 1, 12, step=1)
    from_where = st.selectbox("Odkud se hlássíte na VŠ", ["střední škola nebo SOU", "zaměstnání", "mateřská dovolená",
                                                          "jiná vysoká škola", "pracovní úřad", "jiné"])
    if from_where == "střední škola nebo SOU":
        from_where = "a"
    elif from_where == "zaměstnání":
        from_where = "b"
    elif from_where == "mateřská dovolená":
        from_where = "c"
    elif from_where == "jiná vysoká škola":
        from_where = "d"
    elif from_where == "pracovní úřad":
        from_where = "e"
    else:
        from_where = "f"
    maturita = st.selectbox("Na které škole maturujete/jste maturoval/a?", ["gymnázium čtyřleté", "gymnázium víceleté",
                                                                            "obchodní akademie", "SOU", "SŠ průmyslová",
                                                                            "jiná SOŠ"])
    if maturita == "gymnázium čtyřleté":
        maturita = "a"
    elif maturita == "gymnázium víceleté":
        maturita = "b"
    elif maturita == "obchodní akademie":
        maturita = "c"
    elif maturita == "SOU":
        maturita = "d"
    elif maturita == "SŠ průmyslová":
        maturita = "e"
    else:
        maturita = "f"

    student_category = st.selectbox("Uchazeči o studium na vysoké škole se dělí do pěti hlavních skupin podle toho, "
                                    "jak si školu vybírají. Do které skupiny patříte vy?",
                                    ["Co nejlevnější studium. Rozhodující je vzdálenost od bydliště, náklady spojené"
                                     " se studiem.",
                                     "Snadnější cesta. Hlavní je, na školu se dostat a udržet se na ní.",
                                     "Studentský život. Kde mám přátele, jaká je možnost si při škole přivydělat, výběr"
                                     " města, ve kterém je škola.",
                                     "Uplatnění v praxi. Jistota nalezení dobrého místa, vysoký výdělek.",
                                     "Kvalita výuky. Co nejvyšší kvalita poskytovaného vzdělání (co se naučím),"
                                     " kvalitní učitelé."])
    if student_category == "Co nejlevnější studium. Rozhodující je vzdálenost od bydliště, náklady spojené se studiem.":
        student_category = "a"
    elif student_category == "Snadnější cesta. Hlavní je, na školu se dostat a udržet se na ní.":
        student_category = "b"
    elif student_category == "Studentský život. Kde mám přátele, jaká je možnost si při škole přivydělat, výběr města,"\
                             " ve kterém je škola.":
        student_category = "c"
    elif student_category == "Uplatnění v praxi. Jistota nalezení dobrého místa, vysoký výdělek.":
        student_category = "d"
    elif student_category == "Kvalita výuky. Co nejvyšší kvalita poskytovaného vzdělání (co se naučím)," \
                             " kvalitní učitelé.":
        student_category = "e"
    else:
        student_category = "f"

    after_uni_graduation = st.selectbox("Co pro Vás bude hned po dokončení vysoké školy nejdůležitější?",
                                        ["získat dobře placené místo dále se vzdělávat",
                                         "věnovat se tomu, co mě baví",
                                         "založit rodinu",
                                         "získat zkušenosti v zahraničí",
                                         "jiné"])
    if after_uni_graduation == "získat dobře placené místo dále se vzdělávat":
        after_uni_graduation = "a"
    elif after_uni_graduation == "věnovat se tomu, co mě baví":
        after_uni_graduation = "b"
    elif after_uni_graduation == "založit rodinu":
        after_uni_graduation = "c"
    elif after_uni_graduation == "získat zkušenosti v zahraničí":
        after_uni_graduation = "d"
    else:
        after_uni_graduation = "e"

    highest_edu_mother = st.selectbox("Jaké je nejvyšší dosažené vzdělání vaší matky?",
                                      ["základní",
                                       "střední bez maturity (např. vyučen)",
                                       "střední s maturitou",
                                       "vysokoškolské"])
    if highest_edu_mother == "základní":
        highest_edu_mother = "a"
    elif highest_edu_mother == "střední bez maturity (např. vyučen)":
        highest_edu_mother = "b"
    elif highest_edu_mother == "střední s maturitou":
        highest_edu_mother = "c"
    else:
        highest_edu_mother = "d"
    highest_edu_father = st.selectbox("Jaké je nejvyšší dosažené vzdělání vašeho otce?",
                                      ["základní",
                                       "střední bez maturity (např. vyučen)",
                                       "střední s maturitou",
                                       "vysokoškolské"])
    if highest_edu_father == "základní":
        highest_edu_father = "a"
    elif highest_edu_father == "střední bez maturity (např. vyučen)":
        highest_edu_father = "b"
    elif highest_edu_father == "střední s maturitou":
        highest_edu_father = "c"
    else:
        highest_edu_father = "d"

    gender = st.selectbox("Vaše pohlaví", ["žena", "muž", "jiné"])
    if gender == "žena":
        gender = 0
    elif gender == "muž":
        gender = 1
    else:
        gender = -1
    nationality = st.selectbox("Vaše národnost", ["cs", "sk", "und"])
    subject_of_faculty = st.selectbox("Zaměření vaší prioritní fakulty", ["humanitní a společenské vědy a nauky",
                                                                          "právní vědy a nauky",
                                                                          "ekonomické vědy a nauky",
                                                                          "technické vědy a nauky",
                                                                          "pedagogika, učitelství a socialní péče",
                                                                          "zemědělsko-lesnické a veter. vědy a nauky",
                                                                          "zdravotnictví, lékař. a farmac. vědy a nauky"
                                                                          ])
    num_of_exams = st.slider("Počet absolvovaných zkoušek celkem", 0, 6, step=1)
    num_of_exams_ZSV = st.slider("Počet absolvovaných ZSV zkoušek", 0, 6, step=1)
    num_of_exams_OSP = st.slider("Počet absolvovaných OSP zkoušek", 0, 6, step=1)
    max_ZSV = st.slider("Maximální dosažený percentil ze ZSV zkoušek", -1.0, 100.0, step=0.5)
    max_OSP = st.slider("Maximální dosažený percentil z OSP zkoušek", -1.0, 100.0, step=0.5)
    max_OSP_VO = st.slider("Maximální dosažený percentil z OSP zkoušky - analitycký oddíl", -1.0, 100.0, step=0.5)
    max_OSP_AO = st.slider("Maximální dosažený percentil z OSP zkoušky - verbální oddíl", -1.0, 100.0, step=0.5)
    ZSV_now = st.slider("Dosažený percentil ze ZSV zkoušeky ve sledovaném termínu", -1.0, 100.0, step=0.5)
    if asked_value == "AO_v_terminu":
        OSP_O_now = st.slider("Dosažený percentil z OSP zkoušky - analitycký oddíl", -1.0, 100.0, step=0.5)
    else:
        OSP_O_now = st.slider("Dosažený percentil z OSP zkoušky - verbální oddíl", -1.0, 100.0, step=0.5)
    asked_df = pd.DataFrame({'odkud_se_hlasi': [from_where],
                             'kde_maturita': [maturita],
                             'kategorie_studentu': [student_category],
                             'po_ukonceni': [after_uni_graduation],
                             'vzdelani_matka': [highest_edu_mother],
                             'vzdelani_otec': [highest_edu_father],
                             'rokNarozeni': [year_of_birth],
                             'mesicNarozeni': [month_of_birth],
                             'pohlaviMuz': [gender],
                             'narodnost': [nationality],
                             'zamereniFakultyPrvniVolby': [subject_of_faculty],
                             'OSP_v_terminu': [np.nan],
                             'VO_v_terminu': [OSP_O_now],
                             'AO_v_terminu': [OSP_O_now],
                             'ZSV_v_terminu': [ZSV_now],
                             'celkemZkousek': [num_of_exams],
                             'zkousek_OSP': [num_of_exams_OSP],
                             'zkousek_ZSV': [num_of_exams_ZSV],
                             'max_OSP': [max_OSP],
                             'max_OSP_VO': [max_OSP_VO],
                             'max_OSP _AO': [max_OSP_AO],
                             'max_ZSV': [max_ZSV]})
    dataset = pd.read_csv("shared_data/dataset.csv")
    dataset = pd.concat([asked_df, dataset])
    dataset = dp.make_encoded_dataset(dataset)
    ready_to_train_dataset = sf.TrainTestValSets(dataset, False, asked_value)
    asked_row = ready_to_train_dataset.data.X.head(1)
    regression = st.selectbox("Vybeste typ regresního modelu", ["Lineární regrese", "Hřebenová regrese",
                                                                "Rozhodovací strom", "Náhodný les"])
    if regression == "Lineární regrese":
        lin_reg = LinearRegression().fit(ready_to_train_dataset.X_train, ready_to_train_dataset.y_train)
        st.write("Napredikovaný percentil je: ", float(lin_reg.predict(asked_row)))
    elif regression == "Hřebenová regrese":
        rid_reg = RidgeCV(alphas=[0.00001, 0.0001, 0.001, 0.01, 0.1, 0.7, 1, 1.5, 2, 3, 5, 8, 10, 15,20, 25, 50, 75,
                                  100, 200, 250, 500, 1000, 10000]).fit(ready_to_train_dataset.X_train,
                                                                        ready_to_train_dataset.y_train)
        st.write("Napredikovaný percentil je: ", float(rid_reg.predict(asked_row)))
    elif regression == "Rozhodovací strom":
        best_depth = sf.best_depth_for_tree(ready_to_train_dataset.X_train, ready_to_train_dataset.y_train,
                                            ready_to_train_dataset.X_val, ready_to_train_dataset.y_val)
        dtr_reg = DecisionTreeRegressor(max_depth=best_depth, random_state=42).fit(ready_to_train_dataset.X_train,
                                                                                   ready_to_train_dataset.y_train)
        st.write("Napredikovaný percentil je: ", float(dtr_reg.predict(asked_row)))
    elif regression == "Náhodný les":
        if asked_value != "OSP_v_terminu":
            st.error("Omlouváme se, ale pro tuto cílovou proměnnou není optimální model předtrénován.")
        with open("random_forests/best_random_forest_unedited", 'rb') as file:
            best_model = pickle.load(file)
        st.write("Napredikovaný percentil je: ", float(best_model.predict(asked_row)))
    else:
        st.error("Omlouváme se, ale pro tento typ regrese není model natrénován.")
