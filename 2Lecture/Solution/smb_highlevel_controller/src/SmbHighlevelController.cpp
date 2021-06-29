#include <cmath>
#include <algorithm>
#include <smb_highlevel_controller/SmbHighlevelController.hpp>


namespace smb_highlevel_controller {

SmbHighlevelController::SmbHighlevelController(ros::NodeHandle &nodeHandle)
        :
        nodeHandle_(nodeHandle),
        subscriberQueueSize_(10),
        scanTopic_("/scan") {
  if (!readParameters()) {
    ROS_ERROR("Could not read parameters.");
    ros::requestShutdown();
  }
  scanSubscriber_ = nodeHandle_.subscribe(scanTopic_, subscriberQueueSize_,
                                          &SmbHighlevelController::scanCallback,
                                          this);
  pclSubscriber_ = nodeHandle_.subscribe(
      "/rslidar_points", 1, &SmbHighlevelController::pointcloudCallback, this);
}

SmbHighlevelController::~SmbHighlevelController() {
}

bool SmbHighlevelController::readParameters() {
  bool success = true;
  success &= nodeHandle_.getParam(
      "/smb_highlevel_controller/scan_subscriber_topic_name", scanTopic_);
  success &= nodeHandle_.getParam(
      "/smb_highlevel_controller/scan_subscriber_queue_size",
      subscriberQueueSize_);
  success &= nodeHandle_.getParam(
        "/smb_highlevel_controller/proportional_param",
        subscriberQueueSize_);
  return success;
}

/* bonus task solution */
void SmbHighlevelController::pointcloudCallback(
    const sensor_msgs::PointCloud2 &msg) {
  //ROS_INFO_STREAM_THROTTLE(2.0, "num poins in 3D cloud: " << msg.data.size());
}

void SmbHighlevelController::scanCallback(
    const sensor_msgs::LaserScan::ConstPtr &msg) {
  double min = msg->range_max;
  // angle index
  int first_index = -1;
  int last_index = -1;

  for (int i = 0; i < msg->ranges.size(); ++i) {
    if (msg->ranges[i] < min)
      min = msg->ranges[i];

    if (first_index < 0 and msg->ranges[i] < msg->range_max and msg->ranges[i] > msg->range_min)
      // this should be the initial angle of the pillar
      if (first_index < 0) first_index = i;
  }

  for (int i = msg->ranges.size(); i > 0 ; --i){

    if (last_index < 0 and msg->ranges[i] < msg->range_max and msg->ranges[i] > msg->range_min)
      // this should be the initial angle of the pillar
      if (last_index < 0) last_index = i;

  }

  ROS_INFO_STREAM_THROTTLE(2.0, "Distance to pillar : " << min);
  ROS_INFO_STREAM_THROTTLE(2.0, "Size of ranges[]   : " << msg->ranges.size());

  // command to the robot to move in some direction

  if (last_index > 0 or first_index > 0){

    ROS_INFO_STREAM_THROTTLE(2.0, "First angle   : " << first_index*msg->angle_increment * (3.14 / 180);
);
    // ROS_INFO_STREAM_THROTTLE(2.0, "Last angle    : " << ;

  }

  // (msg->ranges.size() - last_index)*msg->angle_increment)* (3.14 / 180);


}

}  // namespace smb_highlevel_controller
