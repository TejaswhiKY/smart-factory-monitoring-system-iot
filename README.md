🏭 Smart Factory Monitoring System using IoT
------------------------------------------------------------------------------------
📌 Overview

This project presents a Smart Factory Monitoring System using IoT for real-time industrial monitoring.

The system is divided into two main sections:

Node 1 → Production Line Monitoring

Node 2 → Warehouse Monitoring

Sensor data is collected using Arduino, transmitted via W5500 Ethernet, and visualized on the ThingsBoard Cloud Dashboard.

---------------------------------------------------------
🎯 Objective

The main objective of this project is to:

Monitor industrial and environmental parameters in real time

Improve safety and efficiency in factory operations

Enable remote monitoring using IoT

Provide alert mechanisms for abnormal conditions

-----------------------------------------------------
⚙️ Functional Requirements
---------------------------------------------------------

The system performs the following functions:

Collects sensor data every 5 seconds

Monitors:

Temperature

Humidity

Vibration

Light intensity

Motion / system status

Sends real-time data to ThingsBoard

Displays live data on dashboards

Supports alert generation (threshold-based)

Supports multiple nodes (Production + Warehouse)

Stores historical data

------------------------------------------------------------------------
📊 Non-Functional Requirements
-------------------------------------------------------------

The system is designed with:

⏱ Low latency (< 5 seconds)

📡 High availability (> 99%)

📈 Scalability (supports multiple nodes)

⚡ Fast dashboard loading

🔁 Reliable communication

💰 Cost-effective simulation (PICSimLab)

---------------------------------------------------------------------------------------
🏗 System Architecture
--------------------------------------------------------------------------------
Sensors → Arduino UNO → W5500 Ethernet → Internet → ThingsBoard → Dashboard

This architecture enables real-time monitoring and visualization of industrial data.

---------------------------------------------------------------------------------
🏭 Node 1 – Production Line Monitoring
----------------------------------------------------------------------------------------

📍 Location:
Factory production area


🔧 Components:

DHT22 (Temperature & Humidity)

LM35 (Precision temperature)

Vibration sensor

LED (Alert indicator)

W5500 Ethernet module


📊 Parameters:

Temperature

Humidity

LM35 Temperature

Vibration

System Status

⚙️ Working:

Monitors machine health

If temperature > threshold → LED ON

Detects abnormal vibration

Sends data to ThingsBoard

🎯 Purpose:

To monitor machine condition and safety in the production line.

------------------------------------------------------------------------
📦 Node 2 – Warehouse Monitoring
----------------------------------------------------------------------------

📍 Location:

Storage / warehouse area

🔧 Components:

DHT22 (Temperature & Humidity)

LDR (Light intensity)

Push Button (Motion simulation)

LED (System status)

W5500 Ethernet module

📊 Parameters:

Temperature

Humidity

Light intensity

Motion detection

System Status

⚙️ Working:

Monitors storage conditions

Detects light changes

Simulates motion/activity

Displays system status via LED

🎯 Purpose:

To monitor environmental conditions and security in warehouse.

-----------------------------------------------------------------------------------------
☁️ Platform Used: ThingsBoard
-------------------------------------------------------------------------------------------

ThingsBoard is an open-source IoT platform used for:

Device management

Data visualization

Dashboard creation

Rule engine (alerts)

Real-time telemetry

------------------------------------------------------------------------------
📊 Dashboard Features:
---------------------------------------------------------------------

Node 1:

Temperature Gauge

Humidity Gauge

LM35 Graph

Vibration Graph

LED Status


Node 2:

Temperature Gauge

Humidity Gauge

LDR Graph

Motion Indicator

System Status LED

-----------------------------------------------------------
🔗 Communication & Data Format
-------------------------------------------------------------------------------
📡 Protocol:

HTTP (used in this project)

MQTT (conceptual understanding)

📦 Data Format:

JSON (key-value format)

Example:
{
  "temperature": 32.5,
  "humidity": 60,
  "vibration": 720
}

---------------------------------------------------------------------------
🧪 Simulation Tool: PICSimLab
-------------------------------------------------------------

PICSimLab is used for:

Simulating Arduino circuits

Testing sensors (DHT22, LDR, etc.)

Debugging logic

Reducing hardware cost

This ensures system works before real deployment.

----------------------------------------------------------
🚨 Alerts & Automation
---------------------------------------------------------------

Using ThingsBoard Rule Engine:

Conditions:
Temperature > threshold

Humidity > threshold

Vibration > threshold

Actions:

LED indication

Dashboard alert

Email notification

Telegram notification

------------------------------------------------------------------------------
🏭 Applications
----------------------------------------------------------------------

Smart Factory

Industrial Monitoring

Preventive Maintenance

Warehouse Monitoring

IoT Automation

--------------------------------------------------------------------------------------
🚀 Future Scope
---------------------------------------------------------------------------------------

MQTT implementation

Mobile app integration

AI-based prediction

More nodes expansion

Advanced alert system

--------------------------------------------------------------------------------
✅ Conclusion
----------------------------------------------------------------------------

This project demonstrates how IoT can be used to build a Smart Factory Monitoring System.

By integrating:

Arduino

Ethernet communication

Cloud platform

Simulation tools

we achieved a scalable and real-time industrial monitoring solution.


---------------------------------------------------------------------------
This project was successfully designed and implemented by Tejaswhi K Y, demonstrating practical knowledge in IoT, sensor integration, and real-time industrial monitoring using ThingsBoard.

----------------------------------------------------------------------------

