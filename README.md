# A8 - An Educational App : cats
Developed by Team Chandler: Connor Blood, Josie Fiedel, Braden Fiedel, and Caden Erickson  
Software Practice II  
University of Utah  
Spring 2023  
  
## Introduction
We built this program as an assignment for our CS 3505- Software Practice II class, during the spring semester of 2023. The assignment was to create an "educational app", using the Qt GUI framework, with some feature making use of the Box2D physics engine library. We chose to teach about cat behaviors, given that three of our members own cats. As such, we very cleverly named our program "cats".
  
## How to Use
The program opens to a main menu, on which are three buttons: Learn, Quiz, and Exit.  
  
**Learn**  
The Learn section is where the user can learn about the different behaviors of a cat, and what those behaviors mean. The behaviors are broken down by body part, as shown in the cat diagram in the Learn section. The user can click on each of the body parts, which will display information about that body part in the panel on the right side. Once all of the sections have been viewed, the Quiz button will activate, and the user may proceed to the quiz section.
  
**Quiz**  
The Quiz section opens to an introduction screen, which outlines how to play the game.  
The game alternates between three "screens". On the first will be the "question" the user has to answer. The questions in this quiz take the form of videos and images, in which a cat is shown displaying some behavior. After viewing the behavior, the user will proceed to a second screen, which has a small mouse game. The user controls the mouse using their WASD keys, in order to guide the mouse to the hole in the wall corresponding to the correct answer of the cat's behavior. The user must also avoid mouse traps, and can collect extra chese.  
Once an answer has been selected, a third screen will be shown, representing behind the "wall" the mouse entered through. The player's cumulative score will be shown, along with cheese dropping from the top of the screen, which represents any points the user gained for this question. If the user answered correctly, they gain 1 point; if they also collected the bonus cheese, they gain 2 points. If they answered incorrectly, or ran into a mousetrap, they recieve no points, regardless of whether they collected the extra cheese.  
  
Once all 12 levels have been played, an end screen is displayed with the user's score, and they are given the option to exit the game, which closes the application.

**Exit**  
The Exit button on the main menu closes the application.
  
---
  
## Demonstration
A video demonstration of this program can be found [here](https://www.youtube.com/watch?v=FKs-v0z9n4M).
  
## Known Issues
There are issues we are aware of, and chose to ignore, as they do not affect the use of the program.  
First, when the game is begun, various error messages appear in the application output window of Qt Creator. We have researched this bug, and the error messages appear to come from the Qt Multimedia framework, and have been patched in Qt 6.5. However, this program was built in Qt 6.4.3, so the error messages remain.  
The second is in the Learn Section. If the diagram is clicked, and then the mouse is dragged off the diagram (with the mouse button still pressed), and *then* released, QImage "out of bounds" messages appear in the Qt Creator application window. This comes from the event handling within Qt's QImage class, and like the first bug, doesn't affect use of the program.
  
---
  
## Version History
04/27/2023 - Submitted for A8 assignment
