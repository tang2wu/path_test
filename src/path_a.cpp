/*workflow

1.create a map,2D;need define rows and columns
2.create start node,end node, current node ,neighbor node
3.create a container to store accessible nodes

*/

#include "path_test/path_a.h"


int main(int argc,char **argv)
{
  ros::init (argc,argv,"path_A");
  ros::NodeHandle nh;
  //sub rostopic "agent_feedback"
  ros::Subscriber sub_agent_feedback = nh.subscribe("agent_feedback", 1000,feedbackCB);
  
  //rosservice update_goal
   ros::ServiceServer service = nh.advertiseService("update_goal",goalCB);

  ros::spin();
  return 0;

}

//remove point Beyond boundary
bool needAdd(MyPoint pos,int init_map[ROWS][COLS])
{
  //determine whether it exceeds the map boundary
  if(pos.row > ROWS || pos.row < 0 || pos.col > COLS || pos.col < 0)
  {
    // cout<<"Beyond boundary" <<endl;
    return false;
  }
  
  return true;
}

// pos is current node position
int getH(MyPoint end,MyPoint pos)
{
  int x = ((pos.row > end.row)?(pos.row - end.row):(end.row - pos.row));
  int y = ((pos.col > end.col)?(pos.col - end.col):(end.col - pos.col));

  return (x+y)*L_COST;
}

//print map
void printMap(int init_map[ROWS][COLS])
{
  for(int i = 0;i<ROWS;i++)
  {
    for(int j = 0;j<COLS;j++)
    {
      if(init_map[i][j] == 0)
      {
        printf("0");
      }
      else
      {
        printf("1");
      }
      
    }
    printf("\n");
  }
}

//sub agent_feedback start position
void feedbackCB(const path_test::agent_feedbackConstPtr &feedback_msg)
{
  startPos = {feedback_msg->x,feedback_msg->y};
}

bool goalCB(path_test::update_goal::Request &req, path_test::update_goal::Response &res)
{
  endPos = {req.x,req.y};
  res.path.header.frame_id = "map";
  
  // create a tree , current node is root node
  TreeNode* pNew = new TreeNode;
  memset(pNew,0,sizeof(TreeNode));
  // create root node
  TreeNode* pRoot = pNew;
  pRoot->pos = startPos;

  // planning A* 
  TreeNode* pCurrent = pRoot;
  TreeNode* pChild = nullptr;

  while(1)
  {
    // Looking for nodes that can be passed around
    for (int i = 0;i < 4;i++)
    {
      pChild = new TreeNode;
      memset(pChild,0,sizeof(pChild));
      pChild->pos = pCurrent->pos; 
      
      switch (i)
      {
      case p_up:
        pChild->pos.row--;
        pChild->pos.g += L_COST;
        break;
      case p_down:
        pChild->pos.row++;
        pChild->pos.g += L_COST;
        break;
      case p_left:
        pChild->pos.col--;
        pChild->pos.g += L_COST;
        break;
      case p_right:
        pChild->pos.col++;
        pChild->pos.g += L_COST;
        break;

      default:
        break;
      }
      // printf("(%d,%d),%d\n",pChild->pos.row,pChild->pos.col,pChild->pos.g);
      //Calculate g(n) h(n) f(n)
      pChild->pos.h = getH(endPos,pChild->pos);
      pChild->pos.setF();

      // determine whether need push back tree or container
      if(needAdd(pChild->pos,init_map))
      {
        //push tree
        pCurrent->childs.push_back(pChild);
        pChild->pParent = pCurrent;
        //push container
        buff.push_back(pChild);
      }
      else
      {
        delete pChild;
      }

    }

    // looking for minmum f(n) in container
    itMin = buff.begin();
    for(it = buff.begin();it != buff.end();it++)
    {
      if((*itMin)->pos.f > (*it)->pos.f )
      {
        itMin = it;
      }
    }

    // delete mimum f(n) node, change it to the current node
    pCurrent = *itMin;
    buff.erase(itMin);

    // determine whether current node is end node
    if((pCurrent->pos.row == endPos.row) && (pCurrent->pos.col == endPos.col))
    {
      isFindEnd = true;
      // buff.clear();
      break;

    }
    if(buff.empty())
    {
      break;
    }
  }
  if(isFindEnd)
  {
    MyPoint temp_point;
    geometry_msgs::PoseStamped path_element;
    printf("\n");
    printf("************Got the path************\n");
    while(pCurrent)
    {
      temp_point.row = pCurrent->pos.row;
      temp_point.col = pCurrent->pos.col;
      temp_path.push_back(temp_point);
      // printf("(%d,%d)",pCurrent->pos.row,pCurrent->pos.col);
      pCurrent = pCurrent->pParent;
    }

    for(it_point = temp_path.end()-1;it_point != temp_path.begin()-1;it_point--)
    {
      path_element.pose.position.x = (*it_point).row;
      path_element.pose.position.y = (*it_point).col;
      res.path.poses.push_back(path_element);
    }

    //pay attention!!!!!
    delete pChild;
    isFindEnd = false;
    printf("\n");
  }

  return true;
}
