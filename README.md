# Google-Street-View-House-Numbers:
**Aspiring Researchers' Challenge 2015**

**Objective: Localize the digits in images**

![dataset](http://d212dsb2sdisoj.cloudfront.net/wp-content/uploads/2014/04/logo48-257x171.jpg "SVHN dataset ground truth samples")

Link to dataset: http://ufldl.stanford.edu/housenumbers/

#Accuracy:
Precision:0.7818

Recall:0.5512

F-score:0.6465

![Alt text](https://github.com/ankitdhall/Google-Street-View-House-Numbers/blob/master/results/plot.png "Distribution of ground truth and CPU boxes overlapping ratio(x-axis: # of bounding boxes(digits) , y-axis: intersection over union ratio)")

_Plot: Distribution of ground truth and CPU boxes overlapping ratio
(x-axis: # of bounding boxes(digits) , y-axis: intersection over union ratio)_

#Format:

**img_num num_of_boxes x1 y1 w1 h1 x2 y2 w2 h2 . . . xn yn wn hn**

Each line contains an image number followed by the number of bounding boxes in the image and 4-tuple of top x co-ordinate, y co-ordinate, width and height

The above format is for both groundtruth as well as CPU generated bounding boxes.

intersectionOverUnion.txt contains the ratios of the intersection of CPU and groundtruth boxes to that of their union for each digit.

(Tested on first 100 images of dataset)

Refer to the Results folder for details.
