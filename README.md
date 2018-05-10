# Autonomous Search and Identification
**COMP50 (Autonomous Intelligent Robots) - Final Project**<br>
*Matthew Lee, Joe Howarth, Kostas Tsiampouris*<br>
*Spring 2018*


## Setup
The final project folder along with the python file should be placed within the catkin_ws and compiled.

To setup the server running Tensorflow, the following must be done...

## Running the Program
To run the program, the following commands must be run:

```
$ roslaunch tbot2_launch tbot2.launch
$ roslaunch tbot2_launch amcl_navigation.launch
$ python publisher.py
```

The intial AMCL pose must then be set. The final command can then be run which runs the search and identification program.

```
$ rosrun final_project final_project_run
```

