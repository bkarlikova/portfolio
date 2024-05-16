from plot_functions import *

import zipfile
import shutil
from pathlib import Path
import streamlit as st

#this part is responsible for loading zipfiles
if not os.path.exists(os.getcwd() + X_IMG_DIR) or list(os.listdir(os.getcwd() + X_IMG_DIR)) == 0: 
    st.write("There is no data. Please upload a zip file of pictures")
    try:
        uploaded_x = st.file_uploader("Choose a .zip images",type=["zip"], accept_multiple_files=False, key="x")
        if uploaded_x:
            if os.path.exists(os.getcwd()  + X_IMG_DIR):
                shutil.rmtree(os.getcwd()  + X_IMG_DIR)
            with zipfile.ZipFile(uploaded_x, 'r') as zip_ref:
                zip_ref.extractall(os.getcwd()  + X_IMG_DIR)
            if os.path.exists( os.getcwd() + X_IMG_DIR + "__MACOSX"):
                os.rmdir( os.getcwd() + X_IMG_DIR + "__MACOSX") 

    except:
        pass

if not os.path.exists(os.getcwd() + Y_IMG_DIR) or list(os.listdir(os.getcwd() + Y_IMG_DIR)) == 0: 
    st.write("There is no data. Please upload a zip file of pictures")
    try:
        uploaded_y = st.file_uploader("Choose a .zip images",type=["zip"], accept_multiple_files=False, key="y")
        if uploaded_y:
            if os.path.exists(os.getcwd()  + Y_IMG_DIR):
                shutil.rmtree(os.getcwd()  + Y_IMG_DIR)
            with zipfile.ZipFile(uploaded_y, 'r') as zip_ref:
                zip_ref.extractall(os.getcwd()  + Y_IMG_DIR) 
            if os.path.exists( os.getcwd() + Y_IMG_DIR + "__MACOSX"):
                os.rmdir( os.getcwd() + Y_IMG_DIR + "__MACOSX") 
    except:
        pass
    
#this part is responsible for choosing images
if os.path.exists(os.getcwd() + X_IMG_DIR)  and os.path.exists(os.getcwd() + Y_IMG_DIR) \
    and list(os.listdir(os.getcwd() + X_IMG_DIR)) != 0 and list(os.listdir(os.getcwd() + Y_IMG_DIR)) != 0:
    method = st.selectbox("Select method fort detecting keypoints and descriptors", ["SIFT", "KAZE", "AKAZE", "ORB", "BRISK"])

    img1 = st.selectbox('which pictures would you like to see', options=list(os.listdir(os.getcwd() + X_IMG_DIR)), key="x" )
    img2 = st.selectbox('which pictures would you like to see', options=list(os.listdir(os.getcwd() + Y_IMG_DIR)), key="y"  )
    
    """this part is responsible for ploting the images and their matches"""
    if st.button("Plot", type="primary"):
        fx, ax = plt.subplots(1, 1, figsize=(100,80))
        ax.imshow(plot_matches(img1, img2, method))
        st.pyplot(fx)