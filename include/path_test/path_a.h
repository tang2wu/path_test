#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>
#include "path_test/agent_feedback.h"
#include "path_test/update_goal.h"

using namespace std;

#define ROWS 10      //X axis
#define COLS   10     //Y axis
#define L_COST 10 //straight line cost

// point node
struct MyPoint{
  int row; //point x
  int col;  //point y
  double yaw = 0; //point yaw

  int g;
  int h;
  int f; //f = g + n

  void setF()
  {
    f = g + h;
  }
};

//tree node
struct TreeNode{
  MyPoint                         pos;      //curent point position
  vector<TreeNode*>  childs;   
  TreeNode*                    pParent;
};

// start node and end node
MyPoint startPos = {0,0};
MyPoint endPos   = {0,0};

//map :  0 = no obstacle; 1 = no obstacle
int init_map[ROWS][COLS] = {0};

enum direct{p_up,p_down,p_left,p_right};
//print map
void printMap(int init_map[ROWS][COLS]);
//calculate h(n)
int getH(MyPoint end,MyPoint pos);
//remove point Beyond boundary
bool needAdd(MyPoint pos,int init_map[ROWS][COLS]);


vector<TreeNode*>::iterator it;
vector<TreeNode*>::iterator itMin;
vector<MyPoint>  temp_path;

bool isFindEnd = false;
vector<MyPoint>::iterator it_point;

  // create a container
vector<TreeNode*> buff;

void feedbackCB(const path_test::agent_feedbackConstPtr &feedback_msg);
bool goalCB(path_test::update_goal::Request &req, path_test::update_goal::Response &res);
