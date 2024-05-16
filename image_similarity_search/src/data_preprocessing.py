import numpy as np
import cv2
import os
import pandas as pd

X_IMG_DIR = "/x_img/"
Y_IMG_DIR = "/y_img/"

TRESHOLD = 0.7

FLANN_SEARCH_PARAMS = dict(checks=50)
FLANN_INDEX_PARAMS = dict(algorithm = 1, trees = 5)
 
def get_keypoints_and_descriptors(dir_path, detection_method):
    """ Functun for keypoints and descriptors detection for all images from given path by method given by a string

    Args:
        dir_path (_string_): path to directory with images
        detection_method (__string__): shortcut of a detection algorithm
    Returns:
        _pd.DataFrame_: a dataframe containing name of file, kepoins and descriptors for each image in file
    """
    if os.path.exists(dir_path + ".DS_Store"):
        os.remove(dir_path + ".DS_Store")
    keypoints = []
    descriptors = []
    for filename in os.listdir(dir_path):
        image = cv2.imread(dir_path + filename, cv2.IMREAD_GRAYSCALE)
        image = cv2.blur(image,(5,5))
        kp, des = detection_method.detectAndCompute(image, None)
        keypoints.append(kp)
        descriptors.append(des)
    
    df = pd.DataFrame()
    df['image'] = os.listdir(dir_path)
    df['keypoints'] = keypoints
    df['descriptors'] = descriptors
    return df

def get_method_according_to_string(method_string):
    """Function which creates a detection algoritm 

    Args:
        method_string (_string_): shortcut of a detection algorithm

    Returns:
        _tuple_: tuple of detection algoritm and string
    """
    if method_string == "SIFT":
        return cv2.SIFT_create(),method_string
    if method_string == "KAZE":
        return cv2.KAZE_create(),method_string
    if method_string == "AKAZE":
        return cv2.AKAZE_create(),method_string
    if method_string == "ORB":
        return cv2.ORB_create(nfeatures=500), method_string
    if method_string == "BRISK":
        return cv2.BRISK_create(),method_string
    return None, None;
     
def get_dataframe_with_similar_images(method_string):
    """ this function creates two dataframes. One is for images to find similarities for and the second one is for findning similarity from.
    Args:
        method_string (_string_): shortcut of a detection algorithm

    Returns:
        _dataframe_: dataframe with images and similar images sorted from highest score
    """
    
    detect_method, method = get_method_according_to_string(method_string)
        
    x = get_keypoints_and_descriptors(os.getcwd() + X_IMG_DIR, detect_method)
    y = get_keypoints_and_descriptors(os.getcwd() + Y_IMG_DIR, detect_method)
    
    return find_similarity(x, y)    


def find_matches(des1,des2):
    """ This function finds matches between two images, res. given descriptors, by FLANN based kNN matcher and selects those matches which are considered as good ones.

    Args:
        des1 (_np.array_): numpy array of shape: number of key points x number given by detectio algprithm, ex for SIFT it is 128.
        des2 (_np.array_): numpy array of shape: number of key points x number given by detectio algprithm, ex for SIFT it is 128.

    Returns:
        _tuple_:pos_0: all found matches
                pos_1: only good matches 
                pos_2: binary mask of matches for ploting matches
    """
    flann = cv2.FlannBasedMatcher(FLANN_INDEX_PARAMS, FLANN_SEARCH_PARAMS)
    matches = flann.knnMatch(np.array(des1, dtype="float32"), np.array(des2, dtype="float32"),k=2)
    
    matches_mask = [[0,0] for i in range(len(matches))]
    good_matches = []
    for i,(m,n) in enumerate(matches):
        if m.distance < TRESHOLD *n.distance:
            matches_mask[i]=[1,0]
            good_matches.append(m)
    return matches, good_matches, matches_mask


def calculate_score(matches,n_keypoins_1,n_keypoins_2):
    """this function calculates the accuracy score in percents

    Args:
        matches (_list_): list of DMatch objets containing some matches
        n_keypoins_1 (_int_): the number of keypoints in first image
        n_keypoins_2 (_int_): the number of keypoints in second image

    Returns:
        _float32_: accuracy score in percents
    """
    return min((len(matches)/min(n_keypoins_1,n_keypoins_2))*100, 100)


def find_similarity(x, y):
    """this function creates a new column to dataframe x of similarity score to all pictures in dataframe y

    Args:
        x (_pd.DataFrame_): Dataframe of image for which the similarities will be found
        y (_pd.DataFrame_): DataFrame of images from which the similarities will be found

    Returns:
        _pd.DataFrame_: the same dataframe as on input, but with new column "similarities"
    """
    similarities = []
    for i, row_x in x.iterrows():
        scores_and_images = {}
        for _, row_y in y.iterrows():
            _, good_matches, _ = find_matches(row_x["descriptors"],row_y["descriptors"])
            scores_and_images.update({ row_y.image : calculate_score(good_matches, len(row_x["keypoints"]),len(row_y["keypoints"]))})
        scores_and_images = sorted(scores_and_images.items(), key=lambda x:x[1], reverse=True)
        similarities.append(scores_and_images)
    x["similarities"] = similarities
    return x





