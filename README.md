# Arcadia: Descend

A custom 2D vector-based arcade navigation game built from scratch. Control an agile craft to descend through a series of hazardous geometric caverns while managing momentum, line-of-sight pathing, and high-velocity collision vectors.

---

## 📸 Media Showcase

### Title Screen
The minimalist main entry menu introducing the core theme and aesthetic of the game:

![Descend Title Screen](image_26597b.png)

### Vector Navigation & Cavern Traversal
Demonstrating real-time physics, boundary tracking, and pathing calculations as the player maneuvers through high-contrast geometric obstacles:

| Obstacle Tracking & Raycasting | Edge Alignment & Stability |
|:---:|:---:|
| ![Cavern Traversal 1](image_26595b.png) | ![Cavern Traversal 2](image_265961.png) |
| *(Visualizing the trailing trail mechanics and active line-of-sight vectors)* | *(Precision gliding along boundary walls with dynamic UI indicators)* |

---

## 🛠️ Core Features Implemented

* **Vector Physics Engine:** Implemented custom momentum, thrust, and orientation kinematics for smooth, high-precision control of the triangle craft.
* **Dynamic Trail Generation:** Features a custom visual tracking system that leaves a fading geometric trail to map out previous flight positions.
* **Line-of-Sight Raycasting:** Integrates real-time ray-vector intersections to determine distance to impending hazards and track vector proximity.
* **High-Contrast Collision Matrices:** Precision collision boundaries separating dynamic travel paths (yellow vectors) from hard environmental hazards (red geometry blocks).
* **Minimalist HUD Systems:** Compact state tracking using real-time resource indicators (health/energy bars) anchored relative to the active player position.

---

## 🧰 Tech Stack

* **Language:** C / C++
* **Graphics & Audio Framework:** [Raylib](https://www.raylib.com/) 
* **Math Operations:** Custom Linear Vector Algebra (built on top of Raymath)
* **Build System:** CMake
