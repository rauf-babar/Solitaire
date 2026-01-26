# 🃏 Console Solitaire (Klondike) – C++

A **terminal-based implementation of the classic Klondike Solitaire game**, built entirely in **C++** using **custom data structures**, object-oriented design, and command-driven gameplay. The project focuses on core computer-science concepts such as linked lists, stacks, iterators, and manual memory management, while providing a playable and feature-complete Solitaire experience.

---

## Features

* Full **Klondike Solitaire ruleset**
* Custom-built **Doubly Linked List** and **Stack** data structures
* **Iterator-based node manipulation** (no STL containers used)
* Unicode-based **card rendering** (♠ ♥ ♦ ♣)
* **Colored console output** using Windows API
* **Undo system** for both moves and card flips
* **Score (XP) tracking** based on player actions
* Command history display
* Automatic win detection

---

## Data Structures Used

### 1. Doubly Linked List (Template)

Used to represent:

* Tableau columns
* Deck storage and card movement

**Why:**

* Efficient insertion, deletion, and node shifting
* Supports iterator-based movement of multiple cards

### 2. Stack (Template)

Built on top of the doubly linked list and used for:

* Stock
* Waste
* Foundations
* Command history (Undo system)

### 3. Iterators

Custom iterators allow:

* Node-level manipulation without copying cards
* Efficient movement between lists and stacks

---

##  Gameplay Components

* **Stock**: Face-down cards to draw from
* **Waste**: Face-up drawn cards
* **Tableau (7 Columns)**: Main playing area
* **Foundations (4)**: One per suit, built from Ace → King

All rules follow standard **Klondike Solitaire** mechanics.

---

## ⌨️ Controls & Commands

The game uses a **command-based input system**.

### Command Format

```
Cmd Src Dest Num
```

### Commands

| Command          | Description                         |
| ---------------- | ----------------------------------- |
| `s`              | Draw card from Stock                |
| `m src dest [n]` | Move `n` cards from `src` to `dest` |
| `z`              | Undo last move                      |
| `q`              | Quit the game                       |

### Sources (Src)

* `w` → Waste
* `c1` – `c7` → Tableau columns
* `f1` – `f4` → Foundations

### Destinations (Dest)

* `c1` – `c7` → Tableau columns
* `f1` – `f4` → Foundations

### Examples

```
s
m w c3
m c4 c6 3
m c7 f1
z
q
```

---

##  Scoring System

| Action                  | Points          |
| ----------------------- | --------------- |
| Draw from stock         | +5              |
| Move card to foundation | +20             |
| Tableau move (per card) | +10             |
| Undo                    | Reverses points |
| Win game                | +250            |

---

##  Undo System

* Supports undoing:

  * Stock draws
  * Tableau moves
  * Foundation moves
* Automatically restores:

  * Card positions
  * Card visibility (flipped cards)
  * Score adjustments

Undo is powered by a **stack of Command objects**.

---

## Technical Details

* Language: **C++**
* Platform: **Windows**
* Compiler: Visual Studio (MSVC)
* Console Unicode enabled (`/utf-8`)
* Uses Windows API for:

  * Console colors
  * UTF-8 output

---

##  How to Run

1. Open `Solitaire.sln` in **Visual Studio**
2. Build the project (`Ctrl + Shift + B`)
3. Run the program
4. Use keyboard commands to play

---

