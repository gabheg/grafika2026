# Pong Game - Features & Controls

## 🎮 Game Controls

### Left Paddle (Red)
- **W** - Move up
- **S** - Move down

### Right Paddle (Green)
- **Mouse** - Move with cursor position

### Ball
- **Left Click** - Move ball to clicked position
- **↑ / ↓ Arrow Keys** - Resize ball (+ increase, - decrease)
- **+ / - Keys** - Resize ball (alternative shortcut)

### Game
- **ESC** - Quit game

## ✨ Game Features

### 1. Ball Speed
- Increased base speed from 100 to 150 pixels/sec
- Ball moves faster for more dynamic gameplay

### 2. Smart Collision
- Ball only bounces off paddles if the paddle is positioned underneath it
- Requires Y-axis overlap between ball and paddle
- If ball misses the paddle horizontally, it passes through and scores

### 3. Ball Rotation
- Ball continuously rotates with a white indicator line
- Base rotation speed: 360 degrees per second (one full rotation/sec)

### 4. Dynamic Rotation Speed
- **Top/Bottom Wall Collision**: Rotation speed increases by 180°/sec
- **Upper Part of Paddle**: Rotation speed increases by 240°/sec
- **Lower Part of Paddle**: Rotation speed decreases by 120°/sec
- Creates interesting visual feedback during gameplay

### 5. Ball Resizing
- Minimum size: 20 pixels radius
- Maximum size: 80 pixels radius
- Use arrow keys or +/- to adjust
- Size changes affect collision detection

### 6. Scoring System
- Left player (Red) scores when ball goes off right side
- Right player (Green) scores when ball goes off left side
- Scores displayed at top of screen
- Ball automatically resets to center after scoring

## 🎯 Game Mechanics

1. **Objective**: Keep the ball in play longer than opponent
2. **Paddle Positioning**: Must intercept ball at correct Y position
3. **Ball Dynamics**: Speed and rotation change based on collisions
4. **Strategic Play**: 
   - Hitting upper/lower parts of paddle affects ball rotation
   - Ball size can be adjusted for difficulty
   - Click to reposition ball for strategic advantage

## 💡 Tips

- Use W/S to predict ball trajectory and position paddle correctly
- Adjusted ball size affects difficulty - larger ball = easier to hit
- Rotation speed provides visual feedback on ball "energy"
- Click strategically to reposit tion ball during rallies

---

**Version**: Enhanced Pong
**Status**: All features implemented and tested
