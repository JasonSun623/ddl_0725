#include <iostream>
#include <pcl_ros/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_ros/filters/statistical_outlier_removal.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <ros/duration.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "filter_node");
  ROS_INFO("Start filter_node");

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filterd(new pcl::PointCloud<pcl::PointXYZ>);

  pcl::PCDReader reader;
  reader.read<pcl::PointXYZ>("/home/lenovo/zh/workspace/catkin_ws/src/bag_file/pcd/get_2d_map/map_0803_1000_03.pcd", *cloud);

  std::cerr << "Cloud before filtering" << std::endl;
  std::cerr << *cloud << std::endl;

  pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
  sor.setInputCloud(cloud);
  sor.setMeanK(100);
  sor.setStddevMulThresh(1.0);
  sor.filter(*cloud_filterd);

  std::cerr << "Cloud after filtering" << std::endl;
  std::cerr << *cloud_filterd << std::endl;

  ros::NodeHandle nh;
  ros::Publisher pub_pc = nh.advertise<sensor_msgs::PointCloud2>("/filtered_cloud", 1);

  sensor_msgs::PointCloud2 msg_pc;
  pcl::toROSMsg(*cloud_filterd, msg_pc);
  msg_pc.header.frame_id = "/base_link";
  msg_pc.header.stamp = ros::Time::now();
  std::cout << "points: " << msg_pc.data.size() << std::endl;
  ros::Duration rate(1);
  while (ros::ok())
  {
    pub_pc.publish(msg_pc);
    rate.sleep();
    ros::spinOnce();
  }
  ros::spin();

  return 0;
}