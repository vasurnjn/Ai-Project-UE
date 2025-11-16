🚁 AI Drone Management System
Unreal Engine 5 • C++ • Behavior Trees • AWS Cloud Deployment

The AI Drone Management System is a fully autonomous simulation built in Unreal Engine 5 using C++, Behavior Trees, and Blackboard AI.
Drones detect burning trees, extinguish fires, manage their own resources, refill at a base station, and log all activity locally.
The project also integrates AWS EC2 and AWS S3 to run the entire simulation remotely in the cloud.

🔥 Key Features

🤖 Autonomous Drone AI using Behavior Trees (BT_Drone)

🌲 Dynamic Fire Simulation using Niagara VFX

🚉 Manager Station for drone refilling and resource handling

🗄️ SQLite Database Subsystem for storing drone telemetry

🌐 Network Manager capable of DNS, HTTP GET, and FTP operations

☁️ AWS Integration: EC2 headless server + S3 build storage

⚙️ Fully Modular C++ System (extensible & optimized)

🛰️ Designed for scalable simulations and experimentation

🏗️ System Architecture (Summary)
1. Drone AI (C++ + Blueprint)

Handles:

Movement & thrust

Extinguishing actions

Resource management (water storage)

Database registration on spawn

2. Drone AI Controller

Runs the Behavior Tree

Updates Blackboard keys

Converts decisions into actions

3. Behavior Tree (BT_Drone)

Main logic:

Check storage

Locate nearest burning tree

Fly and extinguish fire

If empty, return to Manager

Repeat autonomously

4. Blackboard (BB_Drone)

Stores:

Destination vector

Self reference

Drone state

Boolean flags for behavior switching

5. Tree Actor

Can catch fire

Spreads fire to nearby trees

Allows only one drone to extinguish at a time

6. Manager (Refill Station)

Refills drone storage

Acts as fallback destination

7. DatabaseSubsystem (SQLite)

Database stored at: Saved/Databases/game.db

Inserts drone logs

Updates telemetry

Handles SQL queries internally

8. NetworkManager

Supports:

DNS Lookup

HTTP GET

FTP file upload
Use cases: Cloud telemetry, remote monitoring

9. GameMode / Character / PlayerController

Simulation initialization

Basic player movement to observe drones

Input handling

☁️ AWS Cloud Deployment

The complete simulation runs remotely using AWS.

Amazon S3

Stores packaged Unreal build (DroneManagementSys_Win64.zip).

Upload example:

aws s3 cp "DroneManagementSys_Win64.zip" s3://ai-ue-project-storage/builds/

Amazon EC2

Runs the simulation headlessly through Wine + Xvfb.

Steps:

Connect to EC2:

ssh -i "key.pem" ubuntu@<public-ip>


Install dependencies:

sudo apt update
sudo apt install unzip wine winetricks awscli xvfb -y


Download build:

aws s3 cp s3://ai-ue-project-storage/builds/DroneManagementSys_Win64.zip .
unzip DroneManagementSys_Win64.zip -d DroneManagementSys
cd DroneManagementSys


Run headless:

nohup xvfb-run -a wine DroneManagementSys.exe -server -nullrhi -log > server.log 2>&1 &


View logs:

tail -n 50 server.log

📊 Performance Summary

Local FPS: 50–60

EC2 headless FPS: 45–50

Pathfinding success rate: ~95%

AI decision latency: ~0.1s

Stable up to ~8 drones before linear FPS drop

🧩 Local Setup
Requirements:

Unreal Engine 5

Visual Studio 2022

Windows 64-bit

Steps:

Clone the repository

Open the project in Unreal Engine

Build using Visual Studio

Click Play to start simulation

📦 Packaging Instructions

Unreal Engine → File → Package Project → Windows (64-bit)

Generate .zip

Upload to AWS S3 if deploying to cloud

🚀 Future Improvements

Multi-drone coordination (swarm AI)

AWS DynamoDB or S3 telemetry logging

Real-time monitoring dashboard

Multi-instance cloud scaling

Machine vision integration for tree detection
