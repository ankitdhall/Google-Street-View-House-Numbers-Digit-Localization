#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include<string.h>
#include<fstream>
#include<iostream>
#include<algorithm>
using namespace cv;
using namespace std;

ofstream out;

struct sort_condition
{
    bool operator()(const Rect &i,const Rect &j)
    {
        //sorting based on their center x value
        return ((i.x+i.width/2)<(j.x+j.width/2));
        //return (i.x<j.x);

    }
};

class detect
{
public:

    CascadeClassifier digit_cascade[10];
    fstream out;
    string base, address;
    vector<Rect> digits, all_digits, combined, filtered;
    vector<float> area;
    Mat frame_gray,result, result2, gray, img[3], temp_copy;
    float ratios[5];
    Scalar colors[3];
    int curr_x, curr_y, curr_width, curr_height, imNo;
    Rect ans;

    vector <float> confidence;

    detect(String add="C:/Users/student/Desktop/adc/svhn/Google-Street-View-House-Numbers/cascades/cascade")
    {
        ratios[0] = 1;
        ratios[1] = 2;
        ratios[2] = 1.25;
        ratios[3] = 1.5;
        ratios[4] = 1.75;

        colors[0] = (0,0,255);
        colors[1] = (0,255,0);
        colors[2] = (255,0,0);

        base = add;
        imNo=0;
        out.open("location_5000.txt", ios::out);
        load();
    }

    void load()
    {
        for(int i=0; i<10; i++)
        {
            ostringstream ss;
            ss<<i;
            address = base + ss.str() + "/cascade.xml";
            if( !digit_cascade[i].load(address))
            {
                printf("--(!)Error loading cascade number : %d\n",i);
            }

        }
    }

    pair<float, float> stats(vector<float> &area)
    {
        float sum=0.0, sumsq=0.0;
        int len = area.size();
        for(int i=0; i<len; i++)
        {
            sum+=area[i];
            sumsq+=area[i]*area[i];
        }
        pair <float, float> musigma;
        musigma.first = sum/len;
        musigma.second = sqrt((sumsq/len)-(musigma.first*musigma.first));
        return musigma;
    }

    void areafilter(vector<Rect> &all_digits, vector<Rect> &filtered, float dist = 0.75)
    {
        area.clear();
        for(int i=0; i<all_digits.size(); i++)
        {
            area.push_back(all_digits[i].height*all_digits[i].width);
        }

        //get the mu and sugma value of the areas
        pair<float, float> musigma = stats(area);

        for(int i=0; i<all_digits.size(); i++)
        {
            //discard the bboxs thats don't lie within dist*sigma of mean
            if(abs(musigma.first-(all_digits[i].height*all_digits[i].width)) <= (dist*musigma.second+25))
            {
                filtered.push_back(all_digits[i]);
            }
        }
    }


    void cluster(vector<Rect> &all_digits, vector<Rect> &combined)
    {
        float cc = 1;
        Rect overlap, temp(all_digits[0]);
        for(int i=1; i<all_digits.size(); i++)
        {
            overlap = all_digits[i] & temp ;
            //clustered rectangle size = mean of all bbox in the cluster
            if( overlap.area() > 0.5*temp.area() || overlap.area() > 0.5*all_digits[i].area())
            {
                //calculation of running mean
                temp = Rect((temp.tl()*cc + all_digits[i].tl())*(1/(cc+1)),
                            (temp.br()*cc + all_digits[i].br())*(1/(cc+1)));
                cc++;
            }
            else
            {
                //no more rectangles can be added to cluster, save temp in combined.
                combined.push_back(temp);
                temp = all_digits[i];
                cout<<"Rectangles clustered:"<<cc<<"\n";
                confidence.push_back(cc);
                cc=1;
            }
        }
        combined.push_back(temp);//pushing the last cluster
        cout<<"Rectangles clustered:"<<cc<<"\n";
        confidence.push_back(cc);
    }

    void eval( Mat image, int enlarge=1)
    {
        result = image.clone();
        result2 = image.clone();
        temp_copy = image.clone();
        cvtColor( image, image, CV_BGR2GRAY );

        //running for k different scales, as initialized in the constructor
        for(int k=0; k<2; k++)
        {
            //resize image to required scale
            resize(image,img[k],Size(0,0),ratios[k]*enlarge,enlarge);
            //equalizeHist( img[k], img[k] );//Giving lots of false positives
            for(int i=0; i<10; i++)
            {
                digit_cascade[i].detectMultiScale( img[k], digits, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(20, 30), Size(400,600) );
                //cout<<i<<"'s detected: "<<digits.size()<<endl;
                ostringstream ss;
                ss<<i;
                for( int j = 0; j < digits.size(); j++ )
                {
                    //rescaling the bbox dimension to fit original image
                    curr_x = digits[j].x/(ratios[k]*enlarge);
                    curr_y = digits[j].y/enlarge;
                    curr_width = digits[j].width/(enlarge*ratios[k]);
                    curr_height = digits[j].height/enlarge;

                    //storing bbox results from all scales
                    all_digits.push_back(Rect(curr_x, curr_y, curr_width, curr_height));

                    rectangle(result, Rect(curr_x, curr_y, curr_width, curr_height), Scalar(255,255,0), 1, 8, 0);
                    putText(result, ss.str() , Point(curr_x, curr_y+curr_height),  CV_FONT_HERSHEY_PLAIN, 1.0, (255,0,0) );
                }
                digits.clear();
            }
        }
        sort(all_digits.begin(), all_digits.end(), sort_condition());
        //cout<<"bbox: "<<all_digits.size()<<endl;

        if(all_digits.size()>0)
        {
            //filter out noisy bbox
            areafilter(all_digits, filtered);

            //cluster filtered bboxs
            //cout<<"filtered bbox: "<<filtered.size()<<endl;
            cluster(filtered, combined);

            //display clustered bbox
            for(int i=0; i<combined.size(); i++)
            {
                ans = Rect(combined[i].x, combined[i].y + 0.1*combined[i].height,
                           combined[i].width, 0.8*combined[i].height );

                rectangle(result2, ans, Scalar(255), 1, 8, 0);
            }

            imNo++;
            write();

            imshow("result", result );
            imshow("combined", result2 );

        }
        else
        {
            //if no detection then call eval again with enlarged image
            cout<<"imNo: "<<imNo<<" "<<endl;
            cout<<result.rows<<" "<<result.cols<<endl;
            eval(temp_copy,2*enlarge);
        }


        all_digits.clear();
        filtered.clear();
        combined.clear();
        confidence.clear();
    }
    void write()
    {
        out<<imNo<<" "<<combined.size();
        for(int i =0; i<combined.size(); i++)
            out<<" "<<combined[i].x<<" "<<combined[i].y<<" "<<combined[i].width<<" "<<combined[i].height<<" "<<confidence[i];
        out<<"\n";

    }
    ~detect()
    {
        out.close();
    }

};

int main()
{

    namedWindow("result",2);
    namedWindow("combined",2);
    Mat image, image_truth, frame;
    detect detector;

    string address = "C:/Users/student/Desktop/adc/svhn/train/";
    string filename, cas, converted;
    for(int i =1; i<=100; i++)
    {
        cout<<i<<endl;
        ostringstream ss;
        ss<<i;
        //cout<<"address : "<<address<<endl;
        filename = address + ss.str() + ".png";

        image = imread(filename,1);
        detector.eval(image);

        /*int c = waitKey(0);
        if( (char)c == 27 )
        {
            destroyAllWindows();
            break;
        }*/

    }
    return 0;
}
