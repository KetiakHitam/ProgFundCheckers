# Sample Save States

Test-case save files for the special powers, winning condition, and movement rules.
Board coordinates: columns A-H across the top, rows 1-8 down the side. Player 1 = X
(and powers P/J/N). Player 2 = O (and powers p/j/n). All files set Player 1 to move.

## How to load a save
1. Copy the chosen file next to the program and rename it to `savegame.txt`
   (the program loads `savegame.txt` from its own folder).
2. Run `TT8L_G06_main.exe`.
3. At the start menu type `CONTINUE`.
4. Make the move listed under each sample below.
5. Screenshot the board after loading (the "before") and after the move (the "after").

---

## Special power demonstrations

### 1. save_phantom.txt  -  Phantom (P)
Mechanic: the Phantom moves through its own pieces.
- Before: Phantom P at A2, with your own X pieces at B3 and C4 directly on its diagonal.
- Move: select `A2`, target `D5`.
- What it shows: the only legal landing is D5, beyond both friendly pieces. A normal
  piece would be blocked by B3. The Phantom phases over them and the X pieces are unharmed.
- After: P at D5; X still at B3 and C4.

### 2. save_juggernaut.txt  -  Juggernaut (J)
Mechanic: after a capture the Juggernaut keeps capturing (chain kill).
- Before: Juggernaut J at A2; enemy O at B3 and D5; spare enemy O at G8.
- Move: select `A2`, target `C4`, then when prompted target `E6`.
- What it shows: A2 jumps B3 and lands C4; because another capture is available it
  continues ("Juggernaut chain kill") and jumps D5 to land E6. Two enemies removed in one turn.
- After: J at E6; B3 and D5 gone; O at G8 remains.

### 3. save_necromancer.txt  -  Necromancer (N)
Mechanic: on a capture the captured square becomes a new piece of your colour.
- Before: Necromancer N at B3; enemy O at C4; spare enemy O at F7.
- Move: select `B3`, target `D5`.
- What it shows: N jumps C4 to D5; the message "Necromancer raised a piece at C4" prints
  and C4 becomes a new X. You eliminate an enemy and gain a piece.
- After: N at D5; C4 now X; O at F7 remains.

---

## Winning condition and movement test cases

### 4. save_win.txt  -  Winning condition
- Before: your X at A2 and B3; the opponent has a single O at C4 (their last piece).
- Move: select `B3`, target `D5`.
- What it shows: capturing C4 removes the opponent's last piece, so the program prints
  "Game over. Player 1 wins!".

### 5. save_movement.txt  -  Movement rule (cannot move onto an occupied square)
- Before: your X at C4; your own X at B5 (blocking the left diagonal); enemy O at E6 and B7.
- Move: select `C4`. Only `D5` is offered because B5 is occupied by your own piece.
  Target `D5`. (Optional: first type `B5` to see "Invalid choice or square not available".)
- What it shows: pieces step one square diagonally forward and cannot land on an occupied square.

### 6. save_capture.txt  -  Elimination of an opponent piece
- Before: your X at A2 and B3; enemy O at C4; spare enemy O at F7.
- Move: select `B3`, target `D5`.
- What it shows: moving across the opponent piece at C4 destroys it. The game continues
  because the opponent still has O at F7.
