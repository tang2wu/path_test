# 基于ros框架的路径规划demo

## 运行环境

Ubuntu 18.04 

ros-melodic

## 实现功能

* 手动创建一个2D栅格地图，地图大小为10*10，移动方式为上下左右（可以自行拓展四个对角线方向）。
* 模拟初始位置输入，使用topic机制发布2D位姿以及id信息，topic name 为“agent_feedback”。
* 创建一个rosservice服务端，订阅初始位姿，接收service客户端传输的目标位置，使用A*算法计算最短路径，并将路径返回客户端。
* 创建一个rosservice客户端，接收路径信息，并使用rostopic机制将路径发送为nav_msgs/Path格式，用于rviz显示。

## 简要介绍实现步骤

### 发布初始位置

创建一个自定义的消息类型`agent_feedback.msg` ,用来描述2D位姿。

```bash
uint8 x
uint8 y
float32 yaw
string serial_id
```

其它就是ros基本的发布，不再细说，具体查看`pub_agent_feedback.cpp`。

### A*计算最短路径，并创建rosservice服务端

为啥不用rostopic这种机制去写呢？

其实功能上两种方式均可以实现，但是service有个优点就是在客户端你输入目标点，就可以反馈得到规划后的路径点！

具体的A*算法实现可以看`path_a.cpp`，我在map地图部分，没有在map上加入障碍物信息，0表示没有障碍物，1表示障碍物。

同样，在rosservice也需要自定义srv类型`update_goal.srv`，描述路径的消息格式为nav_msgs/Path，这个是ros常用的描述路径的格式。

```bash
uint8 x
uint8 y
float32 yaw
---
nav_msgs/Path path
```

### rosservice客户端

客户端可以直接在终端使用命令也可以，这里专门写一个客户端，是想在这里将路径通过rostopic 发布出去，这一步也可以放在计算完路径直接发布！

具体细节查看`target_client.cpp`既可！

## launch功能集成

上述的三部分内容均使用一个launch进行打包，同时，会为了方便调试查看效果，将模拟的起点和终点位姿均使用ros的param进行实现，这样的话每次改动起点和终点，直接在launch中编辑就可以了，不许要在每次更改后进行编译操作！！

具体直接查看`run_planer.launch`即可！！launch中已经配置了rviz。

## 效果展示

<img src="/home/zero/Cpp_test/Tang_FULLTIME/imag/agent_1.png" alt="agent_1" style="zoom:75%;" />



<img src="/home/zero/Cpp_test/Tang_FULLTIME/imag/agent_2.png" alt="agent_2" style="zoom:75%;" />