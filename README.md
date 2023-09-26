# Commands

## Motor Commands

- **Front Motor**:
  - Command: `motor:front`
  - Description: Activates or controls the front motor.

- **Left Motor**:
  - Command: `motor:left`
  - Description: Activates or controls the left motor.

- **Right Motor**:
  - Command: `motor:right`
  - Description: Activates or controls the right motor.

- **Forward Motor**:
  - Command: `motor:forward`
  - Description: Commands the motor to move forward.

- **Set Motor Speed**:
  - Command: `motor:speed:[x]`
  - Description: Sets the speed of the motor. Replace `[x]` with the desired speed value.

## Servo Commands

- **Camera Pitch**:
  - Command: `servo:cam_pitch:[x]`
  - Description: Adjusts the pitch of the camera servo. Replace `[x]` with the desired angle.

- **Camera Yaw**:
  - Command: `servo:cam_yaw:[x]`
  - Description: Adjusts the yaw of the camera servo. Replace `[x]` with the desired angle.

## Light Commands

- **Front Strip RGB Color Setting**:
  - Command: `lights:front:[r]:[g]:[b]`
  - Description: Sets the RGB values for the front strip. Replace `[r]`, `[g]`, and `[b]` with the desired red, green, and blue values respectively.

- **Back Strip RGB Color Setting**:
  - Command: `lights:back:[r]:[g]:[b]`
  - Description: Sets the RGB values for the back strip. Replace `[r]`, `[g]`, and `[b]` with the desired red, green, and blue values respectively.

- **Turn Off Both Strips**:
  - Command: `lights:off`
  - Description: Turns off both the front and back strips, setting their colors to black (0,0,0).

- **Blink Left Animation**:
  - Command: `lights:animation:blink_left`
  - Description: Initiates the blink left animation on both light strips.

- **Blink Right Animation**:
  - Command: `lights:animation:blink_right`
  - Description: Initiates the blink right animation on both light strips.

- **Blink All Animation**:
  - Command: `lights:animation:blink_all`
  - Description: Initiates the blink animation on both light strips.

- **Police Animation**:
  - Command: `lights:animation:police`
  - Description: Initiates the police lights animation on both strips.
