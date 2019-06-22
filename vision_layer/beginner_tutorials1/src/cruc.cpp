#include "cruc.h"

Cruc::Cruc() {
	this->loadParams();
   // service = nh.advertiseService("/anahita/marker_angle", &Cruc::markerAngle, this);
}


void Cruc::loadParams() {
	nh.getParam("/anahita/vision/cruc/b_min", bottom_low_b_);
	nh.getParam("/anahita/vision/cruc/b_max", bottom_high_b_);
	nh.getParam("/anahita/vision/cruc/g_min", bottom_low_g_);
	nh.getParam("/anahita/vision/cruc/g_max", bottom_high_g_);
	nh.getParam("/anahita/vision/cruc/r_min", bottom_low_r_);
	nh.getParam("/anahita/vision/cruc/r_max", bottom_high_r_);
	nh.getParam("/anahita/vision/cruc/closing_mat_point", bottom_closing_mat_point_);
	nh.getParam("/anahita/vision/cruc/closing_iter", bottom_closing_iter_);
	nh.getParam("/anahita/vision/cruc/opening_mat_point", bottom_opening_mat_point_);
	nh.getParam("/anahita/vision/cruc/opening_iter", bottom_opening_iter_);
	nh.getParam("/anahita/vision/cruc/bilateral_iter", bottom_bilateral_iter_);
}

//bool Cruc::markerAngle (master_layer::RequestMarkerAngle::Request &req,
 //                              master_layer::RequestMarkerAngle::Response &res) {
 //   res.major_angle = MAJOR;
 //   res.minor_angle = MINOR;
 //   return true;
//}

void Cruc::spinThreadBottom()
{
	cv::Mat temp_src;
    cv::Mat canny_edge;
    cv::Mat blu;
    cv::Mat blur1;
    cv::Mat mask;
    cv::Mat hsv;
    cv::Mat edges;
	// std::vector<cv::Point> largest_contour;
	// cv::Rect bound_rect;
  //	cv::Scalar bound_rect_color(255, 255, 255);  
  //	cv::Point bound_rect_center;
	sensor_msgs::ImagePtr bottom_image_marked_msg;
	sensor_msgs::ImagePtr bottom_image_thresholded_msg;
	ros::Rate loop_rate(15);
  //  double angles1_avg = 0;
  //  double angles2_avg = 0;
  //  std::vector<double> angles;
  //  std::vector<double> angles1;
  //  std::vector<double> angles2;
    int arr[20][2];
	for(int i=0; i<20; i++)
	{ for(int j=0; j<2; j++)
	 { arr[i][j]= 0;}}
	int count = 0;

	while (ros::ok())
	{
		if (close_task) {
			close_task = false;
			break;
		}
		if (!image_bottom.empty()) {
			temp_src = image_bottom.clone();
			// vision_commons::Filter::bilateral(temp_src, bottom_bilateral_iter_);
			// image_bottom_thresholded = vision_commons::Threshold::threshold(temp_src, bottom_low_b_, bottom_high_b_,
			// 																bottom_low_g_, bottom_high_g_,
			// 																bottom_low_r_, bottom_high_r_);
			// vision_commons::Morph::open(image_bottom_thresholded, 2 * bottom_opening_mat_point_ + 1, 
			// 							bottom_opening_mat_point_, bottom_opening_mat_point_, bottom_opening_iter_);
			// vision_commons::Morph::close(image_bottom_thresholded, 2 * bottom_closing_mat_point_ + 1, 
			// 							bottom_closing_mat_point_, bottom_closing_mat_point_, bottom_closing_iter_);

            // std::vector<cv::Vec4i> lines;
            // cv::Canny(image_bottom_thresholded, canny_edge, 50, 200, 3);
            // HoughLinesP(canny_edge, lines, 1, CV_PI/180, 50, 50, 10 );
            cv::bilateralFilter(temp_src,blu, 9, 75, 75);
            cv::GaussianBlur(blu,blur1, cv::Size(15, 15), 0);
            cv::cvtColor(blur1,hsv, cv::COLOR_BGR2HSV);
            
            cv::inRange(hsv, cv::Scalar(30,70,70), cv::Scalar(100, 255, 255), mask);
            cv::Canny(mask,edges, 50, 150,3);

            std::vector<cv::Vec2f> lines; 
            cv::HoughLines(edges,lines, 1, 3.14 / 180, 150);
            
    /*        for( size_t i = 0; i < lines.size(); i++ )
            {
                double theta = atan(static_cast<double>(lines[i][2] - lines[i][0]) / (lines[i][1] - lines[i][3])) * 180.0 / 3.14159;
                angles.push_back(theta);
                cv::line(temp_src, cv::Point(lines[i][0], lines[i][1]), 
                        cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0, 0, 255), 1, CV_AA);
            }
			*/
        int nov=0,noh=0;
        int countv=0,counth=0,counti=0;
        float meanx=0,meany=0;
        float x=0,y=0,xm=0,ym=0;
         
		 for(size_t i=0;i<lines.size();i++){
            float a = cos(lines[i][1]);
            float b = sin(lines[i][1]);
            float x0 = a*lines[i][0];
            float y0 = b*lines[i][0];
            float x1 = x0 + 700*(-b);
            float y1 = y0 + 700*(a);
            float x2 = x0 - 700*(-b);
            float y2 = y0 - 700*(a);
            
            if(abs(x1-x2)<80) nov= nov+1;
            else if(abs(y1-y2)<80) noh = noh+1;
                    }
    
        float arrv[nov][4];
        float arrh[noh][4];
        int inter = noh*nov;
        float arri[inter][2];
   
        for(size_t i=0;i<lines.size();i++){
            float a = cos(lines[i][1]);
            float b = sin(lines[i][1]);
            float x0 = a*lines[i][0];
            float y0 = b*lines[i][0];
            float x1 = x0 + 700*(-b);
            float y1 = y0 + 700*(a);
            float x2 = x0 - 700*(-b);
            float y2 = y0 - 700*(a);

            
            if(abs(x1-x2)<80){
                arrv[countv][0] = x1;
                arrv[countv][1] = y1;                
                arrv[countv][2] = x2;                
                arrv[countv][3] = y2;
                countv = countv+1;
                }    

                if(abs(y1-y2)<80){
                arrh[counth][0] = x1;
                arrh[counth][1] = y1;                
                arrh[counth][2] = x2;                
                arrh[counth][3] = y2;
                counth = counth +1;
                }

            cv::line(image_bottom,cv::Point(x1,y1),cv::Point(x2,y2),cv::Scalar(0,0,255),2, 8, 0);
            }

   for(int i=0;i<countv;i++){
       for(int j=0;j<counth;j++){
                float div = (arrv[i][0]-arrv[i][2])*(arrh[j][1]-arrh[j][3])-(arrv[i][1]-arrv[i][3])*(arrh[j][0]-arrh[j][2]);

                if(div!=0){
                    arri[counti][0] =( (arrv[i][0]*arrv[i][3] - arrv[i][1]*arrv[i][2])*(arrh[j][0]-arrh[j][2]) - (arrv[i][0]-arrv[i][2])*(arrh[j][0]*arrh[j][3]-arrh[j][1] * arrh[j][2]))/((arrv[i][0]-arrv[i][2])*(arrh[j][1]-arrh[j][3])-(arrv[i][1]-arrv[i][3])*(arrh[j][0]-arrh[j][2]));
                    arri[counti][1] =( (arrv[i][0]*arrv[i][3] - arrv[i][1]*arrv[i][2])*(arrh[j][1]-arrh[j][3]) - (arrv[i][1]-arrv[i][3])*(arrh[j][0]*arrh[j][3]-arrh[j][1] * arrh[j][2]))/((arrv[i][0]-arrv[i][2])*(arrh[j][1]-arrh[j][3])-(arrv[i][1]-arrv[i][3])*(arrh[j][0]-arrh[j][2])) ;
                counti=counti+1;
                }
        }
    }


    for(int i=0; i<counti;i++){
            meanx = meanx + arri[i][0];
            meany = meany + arri[i][1];
    }

    if(counti!=0){
            x= (1.0 * meanx)/counti;
            y= (1.0 * meany)/counti;
    }

    if(x!=0){
            arr[count][0] = x;
            arr[count][1] = y;
            count=count+1;
            if( count>19) count= count-20;
    }

    if(x!=0){    
          //  chatter_pub.publish(str(x)+ ' ' + str(y));    
            bottom_x_coordinate.data = x;
			bottom_y_coordinate.data = y;
			bottom_z_coordinate.data = 0;

			bottom_x_coordinate_pub.publish(bottom_x_coordinate);
			bottom_y_coordinate_pub.publish(bottom_y_coordinate);
			bottom_z_coordinate_pub.publish(bottom_z_coordinate);
            bottom_image_marked_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", temp_src).toImageMsg();
        	bottom_marked_pub.publish(bottom_image_marked_msg);

			bottom_image_thresholded_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", image_bottom_thresholded).toImageMsg();
        	bottom_thresholded_pub.publish(bottom_image_thresholded_msg);
            cv::circle(image_bottom, cv::Point(int(x), int(y)), 5, cv::Scalar(255, 0, 0), 5, 8, 0);
    }
    else{
            for(int i=0; i<20; i++){
                xm = xm+arr[i][0];
                ym = ym+arr[i][1];
                }
        
         //   chatter_pub.publish(str(xm/20)+ ' ' + str(ym/20));   
            bottom_x_coordinate.data = xm/20;
			bottom_y_coordinate.data = ym/20;
			bottom_z_coordinate.data = 0;

			bottom_x_coordinate_pub.publish(bottom_x_coordinate);
			bottom_y_coordinate_pub.publish(bottom_y_coordinate);
			bottom_z_coordinate_pub.publish(bottom_z_coordinate);
            bottom_image_marked_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", temp_src).toImageMsg();
        	bottom_marked_pub.publish(bottom_image_marked_msg);

			bottom_image_thresholded_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", image_bottom_thresholded).toImageMsg();
        	bottom_thresholded_pub.publish(bottom_image_thresholded_msg);
            cv::circle(image_bottom,  cv::Point((int)(xm/20), (int)(ym/20)), 5,  cv::Scalar(255, 0, 0), 5, 8, 0);
            }
        }

			// bottom_x_coordinate.data = bound_rect_center.y - temp_src.rows/2;
			// bottom_y_coordinate.data = bound_rect_center.x - temp_src.cols/2;
			// bottom_z_coordinate.data = 0;

			// bottom_x_coordinate_pub.publish(bottom_x_coordinate);
			// bottom_y_coordinate_pub.publish(bottom_y_coordinate);
			// bottom_z_coordinate_pub.publish(bottom_z_coordinate);
		else {
			ROS_INFO("Image empty");
		}
		loop_rate.sleep();
		ros::spinOnce();
	}
}
