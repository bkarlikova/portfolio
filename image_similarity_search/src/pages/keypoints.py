from plot_functions import * 
import zipfile
import shutil
from pathlib import Path
import streamlit as st


# this part is responsible for loading zipfile
img_filename = ""
if os.path.exists(os.getcwd() + X_IMG_DIR) and list(os.listdir(os.getcwd() + X_IMG_DIR)) != 0 \
    and os.path.exists(os.getcwd() + Y_IMG_DIR) and list(os.listdir(os.getcwd() + Y_IMG_DIR)) != 0:
    img_filename = st.selectbox('which pictures would you like to see', options=list(os.listdir(os.getcwd() + X_IMG_DIR)) + list(os.listdir(os.getcwd() + Y_IMG_DIR)))
else:
    st.write("There is no data. Please upload a zip file of pictures")
    try:
        uploaded_x = st.file_uploader("Choose a .zip of image(s)",type=["zip"], accept_multiple_files=False, key="y")
        if uploaded_x:
            if os.path.exists(os.getcwd()  + X_IMG_DIR):
                shutil.rmtree(os.getcwd()  + X_IMG_DIR)
            with zipfile.ZipFile(uploaded_x, 'r') as zip_ref:
                zip_ref.extractall(os.getcwd()  + X_IMG_DIR) 
            if os.path.exists( os.getcwd() + X_IMG_DIR + "__MACOSX"):
                os.rmdir( os.getcwd() + X_IMG_DIR + "__MACOSX") 
            img_filename = st.selectbox('which pictures would you like to see', options=list(os.listdir(os.getcwd() + X_IMG_DIR)))
    except:
        pass

# this part is responsible for loading an image
if img_filename:
    method = st.selectbox("Select method fort detecting keypoints and descriptors", ["SIFT", "KAZE", "AKAZE", "ORB", "BRISK"])
    
    """this part is responsible for ploting the image"""
    if st.button("Plot", type="primary"):
        fx, ax = plt.subplots(1, 1, figsize=(50,30))
        ax.imshow(plot_keypoints(img_filename, method))
        st.pyplot(fx)