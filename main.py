import cv2
import matplotlib.pyplot as plt
def getPoints(contours):
    delete = list()
    x = list()
    y = list()
    for i in contours[1]:
        x.append(i[0][0])
        y.append(i[0][1])

    for i in range(len(x)):
        for k in range(i + 1, len(x)):
            if ((x[i] < x[k] + 10 and x[i] > x[k] - 10) and (y[i] < y[k] + 10 and y[i] > y[k] - 10)):
                if k not in delete:
                    delete.append(k)
    delete = sorted(delete, reverse=True)
    for i in range(len(delete)):
        x.pop(delete[i])
        y.pop(delete[i])

    return x,y

def printToObj(x,y):
    file = open("out.obj", "w")
    file.write("mtllib figure.mtl" + '\n')
    file.write("o figure" + '\n')

    for i in range(len(x)):
        str1 = "v"
        str1 += ' ' + str(x[i]) + ' 0' + ' -' + str(y[i])
        file.write(str1 + '\n')

    file.write("usemtl Material" + '\n')
    str1 = "f"
    for i in range(1, len(x) + 1):
        str1 += ' ' + str(i)
    file.write(str1)

image = cv2.imread('rectangle.jpg')
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

_, binary = cv2.threshold(gray, 165,255, cv2.THRESH_BINARY)

contours, hierarchy = cv2.findContours(binary, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
image = cv2.drawContours(image, contours, -1, (0, 255, 0), 2)
# plt.imshow(image)
# plt.show()
# print(contours)
x,y = getPoints(contours)
printToObj(x,y)
