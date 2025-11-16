AI Drone Management System (Unreal Engine 5, C++, AWS Deployment)
Overview

This project is an autonomous drone management simulation built using Unreal Engine 5 and C++. Multiple AI-controlled drones detect burning trees, extinguish fires, return to a manager station to refill resources, and continuously operate without manual input. The simulation includes a fire-spread system, a local SQLite database, network communication features, and full cloud deployment on AWS using EC2 and S3.

The purpose of the project is to demonstrate autonomous agent behavior, game AI architecture, cloud-based simulation execution, and data logging techniques in a real-time environment.

Core Features

Autonomous drones controlled using C++ and Unreal’s Behavior Tree system.

Fire simulation system with trees that ignite and spread fire.

Manager actor that refills drones when their water storage is empty.

SQLite database system that stores drone information and telemetry.

Network Manager capable of DNS resolution, HTTP GET requests, and FTP uploads.

Cloud deployment using AWS EC2 for running the packaged build in headless mode.

Build storage and retrieval using AWS S3.

Support for scalable autonomous simulations with minimal manual intervention.

Architecture Summary
1. Drone AI (C++ Class + Blueprint)

Represents the main drone actor.

Handles flying, movement, extinguishing fire, and tracking water storage.

Registers itself in the SQLite database upon spawning.

Communicates with the Behavior Tree through the AI controller.

2. Drone AI Controller

Runs the Behavior Tree (BT_Drone) and manages Blackboard data.

Acts as the decision-making unit for the drone.

Switches between finding fire, extinguishing, or refilling.

3. Behavior Tree (BT_Drone)

The Behavior Tree controls drone logic:

Check if the drone has storage.

If storage is available, locate the nearest burning tree.

Fly to the location and extinguish the fire.

If storage is empty, return to the Manager to refill.

Repeat this loop continuously.

4. Blackboard (BB_Drone)

Stores AI data such as:

Destination

Self Actor reference

Current state

Flags for behavior switching

5. BT Tasks and Services

GetAirLocation: Finds the nearest burning tree or manager.

FlyToPoint: Moves the drone to the destination, extinguishes fire, or refills storage.

CheckStorage: Determines if the drone should extinguish fires or go refill.

Additional utility tasks/services used for environment checks and updates.

6. Tree Actor

Represents a tree object that can catch fire.

Uses Niagara particle systems for fire effects.

Contains logic to spread fire to nearby trees.

Supports claiming and releasing locks so multiple drones do not extinguish the same tree.

7. Manager (Refill Station)

Static actor that refills drones when their storage is empty.

Used as the fallback destination when drones cannot extinguish more trees.

8. Database Subsystem (SQLite)

Stores drone data in Saved/Databases/game.db.

Creates tables, inserts drone spawn logs, updates telemetry, and retrieves stored data.

Provides structured local logging for simulation runs.

9. Network Manager

Demonstrates external connectivity.

Can perform:

DNS resolution

HTTP GET requests

FTP uploads using curl

Useful for extending the system to cloud telemetry or remote monitoring.

10. GameMode, Character, PlayerController

GameMode initializes the simulation.

Character class allows a player to spectate the environment.

PlayerController handles input bindings.

AWS Cloud Deployment
S3

A bucket (example: ai-ue-project-storage) is used to store the packaged Unreal Engine build.

Build is uploaded using AWS CLI.

Example:

aws s3 cp "DroneManagementSys_Win64.zip" s3://ai-ue-project-storage/builds/

EC2

The packaged build is executed on an EC2 instance in headless mode.

Steps:

Connect using SSH:

ssh -i "key.pem" ubuntu@<public-ip>


Install dependencies:

sudo apt update
sudo apt install unzip wine winetricks awscli xvfb -y


Download the build:

aws s3 cp s3://ai-ue-project-storage/builds/DroneManagementSys_Win64.zip .
unzip DroneManagementSys_Win64.zip -d DroneManagementSys
cd DroneManagementSys


Run the simulation headlessly:

nohup xvfb-run -a wine DroneManagementSys.exe -server -nullrhi -log > server.log 2>&1 &


Check logs:

tail -n 50 server.log


This setup allows the simulation to run continuously in the cloud without requiring a local machine.

Performance Summary

Average FPS locally: 50–60 depending on the number of drones.

EC2 headless execution FPS: ~45–50 depending on instance type.

Pathfinding success rate: ~95% in the test environment.

AI decision latency: ~0.1 seconds.

Performance drops linearly after ~8 drones due to increased movement and BT updates.

Installation & Running Locally

Clone the repository into an Unreal Engine 5 project.

Open the project in Unreal Editor.

Build using Visual Studio (for C++).

Press Play to run the simulation inside the editor.

How to Package the Project

In Unreal Engine:
File → Package Project → Windows (64-bit)

Wait for the build to be generated.

Upload the .zip file to AWS S3 (if needed for cloud deployment).

Future Improvements

Multi-drone coordination using swarm algorithms.

Cloud telemetry logging using DynamoDB or S3.

Web dashboard for real-time monitoring.

Integration with a vision system for actual object detection.

Multi-instance deployment using AWS Auto Scaling.
