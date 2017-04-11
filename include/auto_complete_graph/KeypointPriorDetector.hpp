#ifndef AUTOCOMPLETEGRAPH_KEYPOINTPRIORDETECTOR_07042017
#define AUTOCOMPLETEGRAPH_KEYPOINTPRIORDETECTOR_07042017

#include "opencv2/opencv_modules.hpp"
#include <stdio.h>
# include "opencv2/core/core.hpp"
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/nonfree/features2d.hpp"

namespace AASS{
	namespace acg{

		/**
		 * @brief The class with all the hardcoded first guess things
		 */
		class KeypointPriorDetector{
			
		protected:
			std::vector<std::tuple<cv::KeyPoint, cv::Mat> > keypoints_;
			cv::SiftDescriptorExtractor extractor_;
			cv::FlannBasedMatcher matcher_;
			
		public:
			
			KeypointPriorDetector(){};
			void descriptors();
			
			
		private:
			
			void priorToMat();
			
			/**
			 * @brief extract SIFT descriptors from the image
			 */
			void extractDescriptors(const cv::Mat& img){
				std::vector<cv::KeyPoint> keypoints_1;
				cv::Mat descriptors_1;
				extractor_.compute( img, keypoints_1, descriptors_1 );
				
				for(int i = 0 ; i < keypoints_1.size() ; ++i){
				}
				
			}
			
		};
	}
}
#endif