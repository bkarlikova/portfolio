from data_preprocessing import *
import zipfile
import shutil
from pathlib import Path
import streamlit as st
from st_pages import Page, show_pages


#This is a page setup part using streamlit features and st_pages feature for multiple pages
st.set_page_config(layout="wide")

show_pages(
    [
        Page("app.py", "Search similarities", ":black_small_square:"),
        Page("pages/decriptors.py", "Plot two images with descriptors", ":black_small_square:"),
        Page("pages/keypoints.py", "Plot image with keypoints", ":black_small_square:")
    ]
)


st.title("Similarity search for images")
st.markdown("""---""")

#this part is handeling the loading part 
st.write("Here you can upload the zip file of images for which you would like to find similarities")
try:
    uploaded_x = st.file_uploader("Choose a .zip images", type=["zip"], accept_multiple_files=False, key="x")
    if uploaded_x:
        if os.path.exists(os.getcwd()  + X_IMG_DIR):
            shutil.rmtree(os.getcwd()  + X_IMG_DIR)
        with zipfile.ZipFile(uploaded_x, 'r') as zip_ref:
            zip_ref.extractall(os.getcwd()  +  X_IMG_DIR)
except:
    pass


st.write("Here you can upload the zip file of images from which you would like to find similarities")
try:
    uploaded_y = st.file_uploader("Choose a .zip images",type=["zip"], accept_multiple_files=False, key="y")
    if uploaded_y:
        if os.path.exists(os.getcwd()  + Y_IMG_DIR):
            shutil.rmtree(os.getcwd()  + Y_IMG_DIR)
        with zipfile.ZipFile(uploaded_y, 'r') as zip_ref:
            zip_ref.extractall(os.getcwd()  + Y_IMG_DIR) 
except:
    pass
        


#this part is responsiple for selecting algorithm, chosing n similarities, finding similarities and finaly printing them
if os.path.exists(os.getcwd() + Y_IMG_DIR) and len(os.listdir(os.getcwd() + Y_IMG_DIR)) > 0 \
    and os.path.exists(os.getcwd() + X_IMG_DIR) and len(os.listdir(os.getcwd() + X_IMG_DIR)) > 0 :
    st.markdown("""---""")
    st.write("Select method fort detecting keypoints and descriptors")
    method = st.selectbox("Choose method", ["SIFT", "KAZE", "AKAZE", "ORB", "BRISK"])

    st.markdown("""---""")
    st.write("Select number of **N** similar images")
    number_of_columns = st.slider("Choose number", min_value = 1, max_value = len(os.listdir(os.getcwd()  + Y_IMG_DIR))-1, value=3, step=1)
    st.markdown("""---""")

    if os.path.exists( os.getcwd() + X_IMG_DIR + "__MACOSX"):
        os.rmdir( os.getcwd() + X_IMG_DIR + "__MACOSX") 
    if os.path.exists( os.getcwd() + Y_IMG_DIR + "__MACOSX"):
        os.rmdir( os.getcwd() + Y_IMG_DIR + "__MACOSX") 
        
    if st.button("Start searching", type="primary"):
    
        x = get_dataframe_with_similar_images(method)
        cols = st.columns(number_of_columns + 1)
        for _,img in x.iterrows():
            cols[0].write("The original image")
            cols[0].image( os.getcwd() + X_IMG_DIR + img["image"], caption=str(img.image), use_column_width=True)
            
            for i in range(1 , number_of_columns+1):
                cols[i].write("The %i. most similar" %i)
                cols[i].image( os.getcwd() + Y_IMG_DIR +  img["similarities"][i-1][0], caption=str(np.round(img["similarities"][i-1][1], 2)) + "%", use_column_width=True)