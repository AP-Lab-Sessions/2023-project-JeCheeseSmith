# APMan - README
> Variant on the popular game Pacman. A project for the course [Advanced Programming](https://www.uantwerpen.be/nl/studeren/aanbod/alle-opleidingen/informatica-studeren/bachelor/studieprogramma/) (INFORMAT 1500WETAPR)
> > By Kars van Velzen, s20223956

### How to start?
    Run main.cpp    Enjoy =)

### Functionalities: 
- [X] Game startup & Initialization
- [X] Player Controls & Interactions with Objects
- [X] Ghost Movement and AI
- [X] Coin Collection and Scoreboard
- [X] Fruits and Ghost Transformation
- [X] Clearing Levels and Scaling Difficulty
- [X] Multiple Lives and Game Over
- [x] Visuals and Aesthetics

### Code:
- [X] Use of classes: Game, Stopwatch, World, Camera, Score, Random
- [X] Use of design patterns: Model-View-Controller (MVC), Observer, Abstract Factory, Singleton, State
- [X] Use of namespaces, exception handling, keywords, virtual destructors, initialized primitives, no dynamic casts, polymorphism, templates clang-format, documentation, smart pointers

## Design Choices Overview

> I tried to write my whole project in a way that the code would not only be lowly coupled & highly cohesive,
> but also be easily extendable.
> Furthermore, I aimed to write comprehensive code with proper documentation.

I value structure in my project, as well as directory-wise as code-wise. 
The used namespaces are specifically created on functionality, domain or design patterns. 
Some namespaces may only contain one class, this is done for readability & extendability.

**MODEL**: The model is composed of the em- (Entity Model) namespace and for a small part of the w- (World) domain. 
The entities hold all data, however, the entities should be grouped somewhere together. 
I decided to keep them in the World class, since it also acts as the controller. 
As a result, the data can easily be modified. 
This idea is in line with the assignment, as it states that the World is an orchestrator and that the model should hold methodes to manipulate the data.

**VIEW:** Camera, EntityView, View (Window), ViewFactory. It is clear those parts form their own domain. The cohesive structure with the fact- (Abstract Factory Pattern) namespace from the logic library is expressed by the name of the concrete factory: ViewFactory.

**CONTROL:** I think we could say that there are 2 perspectives, from the user and from the logic of the game itself. 
I would say the heart of the control is in the States nature and the corresponding pattern. 
However, surely the World acts as the entity controller since we can manipulate the models' data by its functions.
From a little more user perspective,
it is the delegated task of the states to handle the user input and coordinate the right actions onto the world.
It's layered.

**CAMERA:** The assignment advised not to use SFML in the Camera class. Even though I did not use any SFML functionalities to translate my coordinates towards real pixels, it made sense to me to use some SFML in the class. At some point we had to connect an SFML Sprite to a coordinate. I let the camera class handle this too. As a benefit, the projection function now made a lot more sense towards its name.

**WORLD:** I wanted to find an easier way of implementing collisions,
since pacman is not such a high-performance-end game.
I combined storing the EntityModels into a Board together with this idea. 
Whiles matching the Entity coordinate with an Index on the board, collisions came down to a simple comparison.  

**STATES:** I tried to delegate the responsibility of handling input towards the States,
making the Game-class only responsible for instantiating the Game, as the name suggests.

### Generic Singleton

Whilst working out my project, I noticed the advantages of a Singleton. 
Thus, I decided to implement the Camera and View (Window) also as a Singleton. 
Now I used five times the same pattern, which made me wonder if I could use templates for it,
resulting in a generic Singleton using class templates.
(See Singleton.h)
However, I could've opted to implement the State manager as a Singleton. 
I decided not to, since I wanted to make use of references & smart pointers too. 
I took the critics of a Singleton being an anti-pattern in mind.

### A note on Inheritance

I made the Observer Class an abstract base class. 
Furthermore, I minimized the use of extra functions and focused on the correct override behavior,
everywhere inheritance was used (EntityView, EntityModel, Observer, ...).

### Minor Advantages

- By using a struct for my coordinates, I simplified working with it.
- Implementation of an additional GameOver State
- I tried writing my documentation as complete as possible and generated it with Doxygen. (See docs/html/*)
- No use of explicit casting

### License
    The project is written and provided by Kars van Velzen. The SFM Library was used (and therefor not written by Kars van Velzen) which licenses can be found on: https://github.com/SFML/SFML. 
    3 Files with minor modifications have been used from the course: Project Software Engineering (INFORMAT 1001WETISE), given by prof. Serge Demeyer. Their licenses can be found on: https://github.com/sergedemeyer/TicTacToe_Git
    Used Files: DesignByContract.h , helper.h , helper.cpp. These files were provided by prof. Serge Demeyer and modified by Kars van Velzen.
