import cv2

image = cv2.imread("./book.jpg");

image2 = image.copy()

for i in range(image.shape[0]):
    for j in range(image.shape[1]):
        image2[i][j][0] = 255
        image2[i][j][1] = 128
        image2[i][j][2] = 128 

for i in range(image.shape[0]):
    for j in range(image.shape[1]):
        diff = image[i][j][0] - 64
        image2[i][j][1] = image2[i][j][1] + diff * 2
        image2[i][j][2] = image2[i][j][2] + diff

cv2.imwrite("norm.jpg", image2);