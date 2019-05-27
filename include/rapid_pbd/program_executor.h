#ifndef _RAPID_PBD_PROGRAM_EXECUTOR_H_
#define _RAPID_PBD_PROGRAM_EXECUTOR_H_

#include <string>

#include "actionlib/server/simple_action_server.h"
#include "rapid_pbd_msgs/ExecuteProgramAction.h"
#include "rapid_pbd_msgs/Program.h"

#include "rapid_pbd/action_clients.h"
#include "rapid_pbd/program_db.h"
#include "rapid_pbd/runtime_robot_state.h"
#include "rapid_pbd/visualizer.h"
#include "rapid_pbd/world.h"

namespace rapid {
namespace pbd {
class ProgramExecutionServer {
 public:
  ProgramExecutionServer(const std::string& action_name,
                         const ros::Publisher& is_running_pub,
                         ActionClients* action_clients,
                         const RuntimeRobotState& robot_state,
                         const RuntimeVisualizer& runtime_viz,
                         const ProgramDb& program_db,
                         const ros::Publisher& planning_scene_pub);
  void Start();

 private:
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<rapid_pbd_msgs::ExecuteProgramAction> server_;
  ros::ServiceClient freeze_arm_client_;
  ros::Publisher is_running_pub_;
  ActionClients* action_clients_;
  const RuntimeRobotState& robot_state_;
  RuntimeVisualizer runtime_viz_;
  const ProgramDb& program_db_;
  ros::Publisher planning_scene_pub_;

  void Execute(const rapid_pbd_msgs::ExecuteProgramGoalConstPtr& goal);
  static bool IsValid(const rapid_pbd_msgs::Program& program);
  void PublishIsRunning(bool is_running);
  void Cancel(const std::string& error);

  // Runs all necessary steps to finish up a program execution, regardless of
  // whether it succeeded or failed.
  void Finish(const World& world = World());
};
}  // namespace pbd
}  // namespace rapid

#endif  // _RAPID_PBD_PROGRAM_EXECUTOR_H_
