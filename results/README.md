#Result
Format:

img_num    num_of_boxes    x1 y1 w1 h1 x2 y2 w2 h2 . . . xn yn wn hn

Each line contains an image number followed by the number of bounding boxes in the image and 4-tuple of top x co-ordinate, y co-ordinate, width and height

The above format is for both groundtruth as well as CPU generated bounding boxes.

intersectionOverUnion.txt contains the ratios of the intersection of CPU and groundtruth boxes to that of their union for each digit.
