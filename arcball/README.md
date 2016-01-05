# Arcball.h

Tags : class

---

2015.11.24 update:
Modify the structure.

The position of the camera should be somewhere at the positive Z 
axis with the up vector pointing to the positive y axis,otherwise
we need to do a translate in the motion func.

---

A C++ class that implements the Arcball.
This class takes as input mouse events and create
the appropriate quaternions and 4x4 matrices to 
represent the rotation given by the mouse.

This class is used as follows:
* initialize the center position (x,y) of the 
  screen, and the radius.
* on mouse down, call mouse_down with the mouse
  position.
* as the mouse is dragged, repeatedly call 
  mouse_motion with the current mouse location.
* when the mouse button is released, call mouse_up.

Note:
* set your camera looking at (0.0, 0.0, 0.0).
* draw your object at the center of the world space (which
  is (0.0, 0.0, 0.0)), or translate it yourself.