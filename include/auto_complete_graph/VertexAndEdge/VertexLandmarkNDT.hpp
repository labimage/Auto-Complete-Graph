#ifndef AUTOCOMPLETEGRAPH_VERTEXLANDMARKNDT_10112017
#define AUTOCOMPLETEGRAPH_VERTEXLANDMARKNDT_10112017

#include "ndt_map/ndt_cell.h"
#include "g2o/types/slam2d/vertex_point_xy.h"
#include "EdgeInterfaceMalcolm.hpp"
#include "VertexSE2RobotPose.hpp"
#include "opencv2/opencv.hpp"
#include "VertexSE2Prior.hpp"

namespace AASS {
namespace acg{	
	
class VertexLandmarkNDT: public g2o::VertexPointXY
	{
		public:
			
		//TESTING
		std::vector<boost::shared_ptr< perception_oru::NDTCell > > cells_that_gave_it_1;
		std::vector<boost::shared_ptr< perception_oru::NDTCell > > cells_that_gave_it_2;
		g2o::SE2 robotpose_seen_from;
		
		//END OF TESTING
			
			
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		EdgeInterfaceMalcolm interface;

		cv::Point2f position;
		cv::KeyPoint keypoint;
// 		cv::Point2d position;
		cv::Mat descriptor;
		std::vector < std::pair<double, double> > angle_orientation;
		AASS::acg::VertexSE2RobotPose* first_seen_from;
		
		VertexLandmarkNDT() : first_seen_from(NULL), g2o::VertexPointXY(){};
		
		const std::vector < std::pair<double, double> >& getAnglesAndOrientations() const {return angle_orientation;}
		double getAngleWidth(int i){return angle_orientation[i].first;}
		double getOrientation(int i){return angle_orientation[i].second;}
// 		double getOrientation() const {return angle_orientation.second;}
		void addAngleOrientation(double a, double d){
			if (a < 0) a += 2 * M_PI;
			if (d < 0) d += 2 * M_PI;
			angle_orientation.push_back(std::pair<double, double>(a, d));};
		void addAnglesOrientations(const std::vector<double>& angles, const std::vector<double>& orientations){
			assert(angles.size() == orientations.size());
			for(int i = 0 ; i < angles.size(); ++i)
			{
				angle_orientation.push_back(std::pair<double, double>(angles[i], orientations[i]));
			}
		}
		double getOrientationGlobal(int i) const {
			assert(first_seen_from != NULL);
			auto vec = first_seen_from->estimate().toVector();
// 			std::cout << "Got estimate " << std::endl;
			double angle = vec(2) + angle_orientation[i].second;
			if(angle >= 2 * M_PI){
				angle = angle - 2 * M_PI;
			}
			else if(angle < 0){
				angle = angle + 2 * M_PI;
			}
			return angle;
		}
		
		bool sameOrientation(const VertexSE2Prior& v){
			
			auto angles_orientations_v = v.getAnglesAndOrientations();
			
			for(auto it_ao_v = angles_orientations_v.begin(); it_ao_v != angles_orientations_v.end() ; ++it_ao_v){
				double angle_width_v = it_ao_v->first;
				double orientation_v = it_ao_v->second;
				assert(angle_width_v >= 0.08);
				for(int i_ao = 0; i_ao < getAnglesAndOrientations().size() ; ++i_ao){
					double landmark_angle = getAngleWidth(i_ao);
					double landmark_orientation = getOrientationGlobal(i_ao);
					assert(landmark_orientation >= 0);
					assert(orientation_v >= 0);
					assert(landmark_orientation < 2 * M_PI);
					assert(orientation_v < 2 * M_PI);
					
					//Smallest angle difference
					double orientation_diff = orientation_v - landmark_orientation;
					orientation_diff = fmod( (orientation_diff + M_PI), (2 * M_PI) ) - M_PI;
					
					double angle_diff = std::abs(angle_width_v - landmark_angle);
					
					//They are close enough in orientation
					if( orientation_diff <= M_PI / 4){
						std::cout << "Same Orientation" << std::endl;
						if(angle_diff <= M_PI / 4){
							std::cout << "Good angle" << std::endl;
							return true;
						}
					}
					
					//ATTENTION magic numbers here.
					//Landmark orientation + 45deg
// 					double landmark_orientation_over = landmark_orientation + 0.785;
// 					if (landmark_orientation_over < 0) landmark_orientation_over += 2 * M_PI;
// 					//Landmark orientation - 45deg
// 					double landmark_orientation_under = landmark_orientation - 0.785;
// 					if (landmark_orientation_under < 0) landmark_orientation_under += 2 * M_PI;
// 					//Landmark angle - 20deg
// 					double landmark_angle_under = landmark_angle - 0.349;
// 					if (landmark_angle_under < 0) landmark_angle_under += 2 * M_PI;
// 					//Landmark angle + 20deg
// 					double landmark_angle_over = landmark_angle + 0.785;
// 					if (landmark_angle_over >= 2 * M_PI) landmark_angle_over -= 2 * M_PI;
// 					
// 					//BUG Those comparisons are wrong in some cases because the angles are modulo numbers.
// 					if( orientation_v <= landmark_orientation_over && orientation_v >= landmark_orientation_under){
// // 						if (angle_width_v <= landmark_angle_over && angle_width_v >= landmark_angle_under){
// 							std::cout << "Good angle" << std::endl;
// 							return true;
// // 						}
// 					}
				}
				
			}
			return false;
// 			}
// 			double landmark_angle = getAngleWidth();
// 			double landmark_direction = getOrientationGlobal();
// 			
// 			std::cout << "Orientation calc" << std::endl;
// 			auto angledirection = v.getAngleOrientation();
// 			for(auto it = angledirection.begin() ; it != angledirection.end() ; ++it){
// 				
// 				double angle_width = it->first;
// 				double direction = it->second;
// 				assert(angle_width >= 0.08);
// 				std::cout << " Angles " << angle_width << " " << direction << " and " << landmark_angle << " " << landmark_direction << std::endl;
// 				std::cout <<  direction << " <= " << landmark_direction + 0.785 << " && " << direction << " >= " << landmark_direction - 0.785 << " && " <<	angle_width << " <= " << landmark_angle + 0.785 << " && " << angle_width << " >= " << landmark_angle - 0.349 << std::endl;
// 				
// 				double landmark_direction_over = landmark_direction + 0.785;
// 				if (landmark_direction_over < 0) landmark_direction_over += 2 * M_PI;
// 				double landmark_direction_under = landmark_direction - 0.785;
// 				if (landmark_direction_under < 0) landmark_direction_under += 2 * M_PI;
// 				double landmark_angle_under = landmark_angle - 0.349;
// 				if (landmark_angle_under < 0) landmark_angle_under += 2 * M_PI;
// 				double landmark_angle_over = landmark_angle + 0.785;
// 				if (landmark_angle_over >= 2 * M_PI) landmark_angle_over -= 2 * M_PI;
// 				
// 				if( direction <= landmark_direction_over && direction >= landmark_direction_under &&
// 					angle_width <= landmark_angle_over && angle_width >= landmark_angle_under
// 				){
// 		// 				if( direction <= landmark_direction + 0.785 && direction >= landmark_direction - 0.785
// 		// 				){
// 					std::cout << "Good angle" << std::endl;
// 		// 					int a;
// 		// 					std::cin>>a;
// 					return true;
// 				}
// 			}
// 		// 			std::cout << "Not good " << std::endl;
// 		// 			int a;
// 		// 			std::cin>>a;
// 			return false;
		}

	};

}
}
#endif