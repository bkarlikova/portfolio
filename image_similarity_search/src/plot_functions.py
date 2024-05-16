from data_preprocessing import *
import matplotlib.pyplot as plt



def plot_matches(img1, img2, method_string):
    """_summary_

    Args:
        img1 (_string_): file name of lefthandside image
        img2 (_string_): file name of righthendside image
        method_string (_string_): shortcut of a detection algorithm
    Returns:
        _plot_: plot of two images and their good matches
    """
    image1 = cv2.imread(X_IMG_DIR[1:] + img1)
    image2 = cv2.imread(Y_IMG_DIR[1:] + img2)
    image1 = cv2.cvtColor(image1, cv2.COLOR_BGR2RGB)
    image2 = cv2.cvtColor(image2, cv2.COLOR_BGR2RGB)
    
    detect_method, matcher = get_method_according_to_string(method_string)
    
    kp_1, des_1 = detect_method.detectAndCompute(image1, None)
    kp_2, des_2 = detect_method.detectAndCompute(image2, None)
    
    matches, _, matches_mask = find_matches(des_1, des_2)
    draw_params = dict(matchColor = (89, 126, 212),
                        singlePointColor = (241, 241, 241),
                        matchesMask = matches_mask,
                        flags = cv2.DrawMatchesFlags_DEFAULT)
    match_plot = cv2.drawMatchesKnn(image1, kp_1, image2, kp_2, matches, None,**draw_params)
    return match_plot


def plot_keypoints(img_path, method_string):
    """_summary_

    Args:
        img_path (_string_): file name of image
        method_string (_string_): shortcut of a detection algorithm
    Returns:
        _plot_: plot of one images and its keypoints
    """
    method, _ = get_method_according_to_string(method_string)
    try:
        image = cv2.imread(X_IMG_DIR[1:] + img_path)
    except:
        image = cv2.imread(Y_IMG_DIR[1:] + img_path)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    kp, _ = method.detectAndCompute(image, None)
    keypoints_plot = cv2.drawKeypoints(image, kp, image, flags = cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    return keypoints_plot