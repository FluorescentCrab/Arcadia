# Why The Player Was Phasing Through Walls
### A simple explanation for future-you

---

## The Setup

Your maze is made of tiles. Each tile is a 100x100 pixel square.
The player is a circle with a 10-pixel radius.

```
+-------+-------+-------+
|       |       |       |
| WALL  | WALL  | WALL  |   <-- each box is 100x100
|       |       |       |
+-------+-------+-------+
|       |       |       |
| empty |  (P)  | empty |   <-- (P) is the player
|       |       |       |
+-------+-------+-------+
```

Every frame, the game does this:
1. Player presses keys → velocity changes
2. Position += velocity  (player moves)
3. Check 4 nearby wall tiles for collision
4. If overlapping a wall → push the player out

---

## Bug #1: "Where am I?" — Only checking 4 tiles

The collision system figures out which grid cell the player is in,
then checks ONLY the 4 cells around that spot:

```
+--------+--------+
| cell A | cell B |
|    +---+--+     |
|    | (P)  |     |   ← player is near the corner of 4 cells
|    +---+--+     |
| cell C | cell D |
+--------+--------+
```

This works fine for normal movement. But when the player DASHES,
they can jump 40+ pixels in a single frame. If they jump PAST
cell B entirely and land in a cell that wasn't one of A/B/C/D,
no collision is ever checked for the wall they flew through.

```
BEFORE:                      AFTER (same frame):
+-------+-------+-------+   +-------+-------+-------+
|       | CHECK | CHECK |   |       |       |       |
| empty |       |       |   | empty | WALL  | empty |
|  (P)  | WALL  | empty |   |       |       |  (P)  |  ← landed here
+-------+-------+-------+   +-------+-------+-------+
          ↑ checked                    ↑ never checked!
```

### What we did to help:
- Reduced dash speed from 100 to 40 (so the jump is ~20px, never skips a full 100px tile)
- Split collision into Y-pass then X-pass (explained below)

---

## Bug #2: "Push me UP" — Wrong direction when inside a wall

When the player's center ends up INSIDE a wall tile, the old code
couldn't figure out which direction to push. So it always pushed UP.

```
OLD BEHAVIOR:
+-------------+
|    WALL     |
|             |
|     (P) ←-- player center is inside the wall
|             |
+-------------+
       ↑
   always pushed UP,
   even if the player
   entered from the RIGHT
```

This meant:
- Enter a wall from the right → get teleported UP (wrong!)
- Enter a wall from the bottom → get teleported UP (maybe right, maybe not enough)

The push amount was also wrong. It always pushed 9.9 pixels (radius - 0.1).
But if the player was 40 pixels deep inside a 100px wall, 9.9px wasn't nearly
enough to escape. So the player slowly sank through frame by frame.

### What we fixed:
Now the code calculates distance to ALL edges and picks the NEAREST one:

```
NEW BEHAVIOR (wallCollisionX):
+-------------+
|    WALL     |
|         15px|←→| (P)   ← 15px to right edge
|  85px   |←------→|     ← 85px to left edge
|             |
+-------------+

Right edge is closer (15 < 85), so push RIGHT by (15 + radius).
Player ends up safely outside the wall on the right side.
```

Same logic for Y in wallCollisionY — picks nearest between top and bottom.

---

## Bug #3: "Stale GPS" — Using old position for second pass

We split collision into two passes:
1. **Y pass** — only push up/down
2. **X pass** — only push left/right

The reason: if you push diagonally on two walls at once, the pushes
can fight each other and the player gets "sandwiched" between tiles.
Splitting into axes avoids diagonal confusion.

BUT there was a bug. The code calculated the player's grid cell ONCE
at the start, then used it for BOTH passes:

```
BEFORE Y-PASS:              AFTER Y-PASS:
+-------+-------+           +-------+-------+
| cell A| cell B|           | cell A| cell B|
|  (P)  |       |           |       |       |
+-------+-------+           +-------+-------+
| cell C| cell D|           | cell C| cell D|
|       |       |           |  (P)  |       |  ← player moved down!
+-------+-------+           +-------+-------+

X-pass still thinks player is in cell A's neighborhood.
But player is now near cell C. WRONG cells get checked!
```

### What we fixed:
After the Y-pass finishes, re-read the player's actual position
and recalculate which grid cell they're in. Then do the X-pass
with the CORRECT cells.

---

## Bug #4: Crash at maze edges — No bounds checking

If the player is at the edge of the maze, the code tries to check
cells like `maze[36][10]` on a 35-cell maze. That's out of bounds
→ crash or garbage data.

### What we fixed:
Added a bounds check. Before looking up any cell, verify the index
is between 0 and mazeSize-1. If it's out of bounds, treat it as
"not a wall" (no collision).

---

## Quick Reference: What Changed in mazeMaker.cpp

| Function | What changed | Why |
|---|---|---|
| `isColliding()` | Added `isWall()` lambda with bounds check | Prevents crash at edges, also now actually checks if cell is a wall |
| `wallCollisionX()` | Fixed distance==0 case — finds nearest LEFT/RIGHT edge | Player no longer gets pushed UP when inside a wall |
| `wallCollisionY()` | Fixed distance==0 case — finds nearest TOP/BOTTOM edge | Same fix for vertical axis |
| `update()` | Re-reads player position between Y-pass and X-pass | X-pass now checks the correct grid cells |

---

## Other Stuff Still TODO

These aren't wall-phasing bugs but they're still broken:

1. **Levels 2-4 are commented out** in `GameManager.hpp` — uncomment them
2. **`entityList` is always empty** in every level — add enemies to it
3. **`enemy3` spawns dummy bullets** — fix direction, position, damage, speed
4. **No delta time** — movement speed depends on frame rate
5. **`damageResolver` memory leak** in level 1 — delete it in destructor

---

*Written March 14, 2026 at 2:53 AM because sleep is overrated.*
