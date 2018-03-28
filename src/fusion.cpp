

void fusionImages(vector<string> image_files):
    # 读取第一张图作为初始结果
    result = cv2::imread("zirconSmall/1/{}".format(image_files[0]), 0)
    imgin1 = result

    for img in image_files:
        print ("Reading file {}".format(img))
        imgin2 = cv2.imread("zirconSmall/1/{}".format(img), 0)
        result, imgin1 = fusion.detect(imgin1, imgin2, result)
        del imgin2
    cv2.imwrite("result/" + save_name, result)