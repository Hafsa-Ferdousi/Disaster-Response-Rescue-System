#  Disaster Response & Rescue System 

A **well-structured console-based Disaster Response and Rescue Management System**
implemented in **C++**, focusing on **real-life emergency decision-making** using
**data structures, graph algorithms, and greedy strategies**.

This system simulates how victims are prioritized, how rescue teams and vehicles
are allocated, and how the most suitable hospital is selected under constraints
such as distance, capacity, and medical facilities.

---

## Features

- Victim priority handling using Priority Queue
- Injury-based decision making (Minor / Moderate / Critical)
- Smart hospital selection considering:
  - Distance
  - Bed availability
  - Emergency / Trauma units
- Vehicle allocation (Ambulance & Van)
- Rescue team availability management
- Dynamic road block & unblock system
- Shortest path calculation using Dijkstra Algorithm
- All-pairs shortest path using Floyd–Warshall Algorithm

---


##  Core Logic

### System Workflow Overview
![Overall System Workflow](https://github.com/user-attachments/assets/0b1b681b-945f-4851-8378-37925fe6f057)


###  Victim Management
- Victims are assigned:
  - Unique ID
  - Injury level (1 = Minor, 2 = Moderate, 3 = Critical)
  - Zone location
- Victims are stored in a **priority queue**
- Higher injury level ⇒ higher priority

**Priority Formula**

Priority = Injury Level × 10

Critical victims are always dispatched first.

---

### Hospital Selection Formula
Score = Distance
+ Penalty (missing emergency/trauma unit)
- Capacity benefit

#### Penalties
- Critical victim + no emergency unit → +10000
- Critical victim + no trauma unit → +2000

#### Capacity Benefit
Hospitals with more available beds get preference.

**Hospital with the lowest score is selected**

---

## Zones

| Index | Zone Name |
|------:|----------|
| 0 | Dhanmondi |
| 1 | Gulshan |
| 2 | Uttara |
| 3 | Mirpur |
| 4 | Banani |
| 5 | Motijheel |

---
###  Rescue Team Management
- Multiple rescue teams available
- Each team can handle only one operation at a time
- Teams are marked busy during dispatch and freed afterward

---

##  Hospitals

- City Hospital
- General Hospital
- Emergency Medical Center
- Burn & Trauma Center

| Hospital Name | Emergency | Trauma | Burn | Capacity |
|--------------|----------|--------|------|----------|
| City Hospital | Yes | Yes | No | 5 |
| General Hospital | No | No | Yes | 8 |
| Emergency Medical Center | Yes | Yes | Yes | 4 |
| Burn & Trauma Center | No | Yes | Yes | 6 |

Each hospital has limited capacity and different medical units.

---

##  Vehicles

| Vehicle | Speed |
|--------|-------|
| Ambulance | 1.0 |
| Van | 0.8 |

Decision logic:
- **Critical victims → Ambulance first**
- **Minor/Moderate victims → Van first**
- Falls back to available option if preferred vehicle is busy

---
## Road & Traffic Handling
 Roads between zones are weighted

Roads can be:

- Blocked

- Unblocked dynamically

Blocked roads are ignored during shortest path calculation

---
## System Architecture

<img width="421" height="611" alt="246Architecture Diagram drawio" src="https://github.com/user-attachments/assets/664572cb-bee0-4cf6-9fe0-f6516ac59cf4" />

##  Algorithms Used

- Priority Queue (Victim scheduling)
- Dijkstra’s Algorithm (Shortest path)
- Floyd–Warshall Algorithm (All-pairs shortest path)
- Greedy decision-making

---
## Program Flowchart
<img width="1024" height="1536" alt="flowchart" src="https://github.com/user-attachments/assets/94726323-8463-4b5c-9946-572ab1a1ab1b" />



## Menu Options

1.Add victim

2.Dispatch rescue

3.Show status

4.Block road

5.Unblock road

6.Exit



---

##  Limitations

- Console-based only
- Static map and resources
- No real-time data or database
- No graphical user interface

---

##  Future Improvements

- GUI using React / Qt
- Backend API integration
- Database support
- Real-time traffic handling
- AI-based hospital load prediction



---

##  Note

This project demonstrates:
- Practical use of Data Structures & Algorithms
- Real-life problem simulation
- Clean and modular C++ design



---
