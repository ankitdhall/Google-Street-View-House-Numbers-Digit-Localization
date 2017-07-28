# Results
### Format (CPU boxes)  

**img_num    num_of_boxes    x1 y1 w1 h1 x2 y2 w2 h2 . . . xn yn wn hn**  

### Format (CPU boxes with confidence)

**img_num    num_of_boxes    x1 y1 w1 h1 confidenceScore1 x2 y2 w2 h2 confidenceScore2 . . . xn yn wn hn confidenceScoren**  

Each line contains an image number followed by the number of bounding boxes in the image and 4-tuple of top x co-ordinate, y co-ordinate, width and height

The above format is for both groundtruth as well as CPU generated bounding boxes.

#### intersectionOverUnion.txt
contains the ratios of the intersection of CPU and groundtruth boxes to that of their union for each digit.

#### confidence_clustersize
the confidence here is the number of boxes that were clustered together to get the box on the current digit in question. The confidence is an integer.

#### confidence_ratio
this has confidence in terms of a ratio = clustersize of this digit/max clustersize in the image. Confidence is a float value belonging to (0,1]

(Tested on first 100 images of dataset)

Accuracy calculated using CPUresponse.txt
