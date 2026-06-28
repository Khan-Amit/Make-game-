# Make-game # 📱 Make-game®


> **Make-game®** is an independent, zero-dependency, mobile-first game engine and application creation tool designed to build retro grid-based platforming games entirely on mobile phone hardware.

---

## 🔒 Proprietary License Notice

**Make-game®** is proprietary software belonging exclusively to **Seliim Ahmed**. 
* **Contact Email**: amit.khanna.1082@gmail.com

> Any alterations, adding, or subtracting of this software is strictly prohibited and may result in direct legal action without the explicit prior written consent of Seliim Ahmed. Please refer to the `LICENSE` file for full compliance details.

---

## 🚀 Engine Architecture & Features

This ecosystem runs entirely out of a single file (`editor.html`) that works natively across mobile internet browsers without requiring external installation setups or command-line compilation terminals.

* **Grid Map Layout Editor**: Visual canvas painter tool to paint ground floor configurations, trap maps, or entity spawn arrays.
* **Kinematic Human State Machine**: Exact emulation of classic *Prince of Persia* style platformer physics, incorporating running skids, mid-air leaping vectors, and automated ledge grabbing and pulling-up loops.
* **Autonomous Guard Patrol AI**: Runs loop pacing paths, projects operational 4-tile raycast field-of-vision alerts, and triggers collision death timers.
* **Inventory Mechanics Layer**: Tracks yellow key asset collection items to erase locked door blocker walls dynamically upon body overlap.
* **Hero Class Profiles**: Selectable skin configurations altering active character rendering color hex values along with structural parameter values (e.g., Assassin velocity shifts, Knight high jumping boosts).
* **In-House Audio Synthesizer**: Utilizes the native browser Web Audio API to create authentic 8-bit sound effects (waves) purely out of math equations.
* **Single-File App Compiler Build System**: Injected serialization exporter code block that bundles layouts and maps cleanly into an individual `.html` download file for instant packaging distribution.

---

## 🛠️ Workspace Initialization Guide

1. Open your smartphone storage folder and clone or save the core file as `editor.html`.

  
3. Tap the file to open it directly inside your default mobile browser (Chrome, Safari, or Firefox).
4. Paint your stage elements inside **Edit Grid** mode, then tap **Play Test** to evaluate physics using the on-screen mobile d-pad button controls.
5. Click **Build App File** to generate a compiled, independent production deployment version of your game.
