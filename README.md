# Web Swing Mechanic 🕸
This project is a simple demonstration of a physics-based web swing mechanic I designed to emulate the movement seen in the spider 
man franchise. I made this for my CMP302 module at Abertay uni, and used Unreal Engine 5 with C++ (no blueprints) to implement it. 
The physics were custom made for this project by me, and I had a lot of fun developing the feel of the swinging motions 
as you move around the test scene.

## Demo Video ⏯
<a href="https://www.youtube.com/watch?v=yX-MT0WEeu4">
  <img src="https://img.youtube.com/vi/yX-MT0WEeu4/maxresdefault.jpg" alt="Watch the video" width="600" height="337">
</a>

## The Development Process 🛠

### 🐣 New Beginnings
The first step in development for this project was learning Unreal Engine 5. This module left us to our own devices mostly in terms 
of learning, which meant that this would be a test of my research skills. Unfortunately, an immediate barrier to learning showed 
itself as early as the first google search: a lot of the C++ information seemed to get lost in the swamps of blueprint tutorials.

Unreal Engine was very new to me and was completely different compared to the Unity I had some more experience with - which was my 
only frame of reference for a game engine. It felt quite important that I find at least a beginners guide of some sort, which went
beyond the blueprint functionality and into the setup for coding. I will admit that I tried - again - to lean on ChatGPT to sidestep
this learning process and - again - was burned when it couldn't complete the complex tasks I asked of it. It did a bad job, none of 
it worked, and I quickly realised that if I wanted this project to go anywhere I had to get off that path and take a new one. This
was my last time truly trying to use ChatGPT to avoid having to think. It failed me every time I leaned on it to hard, and it was 
clear that for complex tasks it was just not up to snuff. Not to mention, using it was not as enjoyable or fulfilling as working it 
out myself, even when it worked. 

### 📚 The Learning Process
In the end, I settled on my solution. I would learn to use Unreal Engine through a tutorial on how to make a grappling hook (available
[here](https://www.youtube.com/watch?v=HvKfbWPu79w)) and perhaps some other general tutorials. After I felt like I understood that, 
I would then move on to leaning more on documentation and creating my own expansions to the program. 

This was my first time really planning my learning, which was spurred on by the horrific start to this project with ChatGPT and 
general failed learning, and it actually went incredibly well. I plan on making a project in rust some time soon, and I hope to be 
able to plan a path of how to learn for that in a similar way thanks to my experiences with this.

### 🎮 An Unreal Experience
Adjusting to Unreal Engine was an interesting experience. To start with, there was a lot more boiler plate the Unity. While Unity 
would give you access to a GameObject with a script which inherits from 'MonoBehaviour' when you want to create a new script to add 
to your game, Unreal Engine has Actors, Pawns and Characters, which just show you all their code upfront. Constantly defining 
UProperties feels weird to me. From what I understand, there is a much more clear idea in Unreal Engine of exactly how they want 
you to structure everything. Unreal Engine also exposes the entirety of the engine to programmers, which was very daunting at first. 

However, as I got more used to these different systems I found that the biggest frustrations I had with the engine compared to Unity 
was the developer tools. Unreal Engine's live coding support sometimes just didn't seem to work, and it was far less seamless than
with Unity. I also just could not make semantic analysis work properly, an issue prevelant throughout the whole class from what I can 
tell. This was just plain annoying to work with, and reminds me how important setting up a good enviroment is. 

However, as I got into the flow of things I soon started to understand and even like Unreal Engine. It offered a lot of power and a
different outlook on how things are done, and I found it's deep systems quite interesting to explore.

### 🙄 A Classic Grappling Hook
The grapple hook I started with was designed in the style of link's hookshot from the legend of zelda, or something batman would use 
to get around gotham city. It would connect to a single point and pull the player towards it, rather than being something used to 
swing around. This was partly because I felt like since I was using tutorials it would be more suitable to do this and figure out 
the physics myself, even though the uni would have been ok with me leaning on tutorials more and making smaller changes to the final
product. 

This worked by using a raycast to connect to a point of terrain, and then applying a force to the player character to move towards 
that point. Add in a simple unreal engine cable for a visual effect to conenct the player to the point, and we have the most basic
form of a grapple hook imaginable which was where the tutorial left me. This does have some problems though. The movement while in the 
air as you grapple feelings very strange, as does falling immediately when you get to the top of a wall rather than getting on top of 
it.

### 💡 Ideas For Improvement
So what's next? 

To transform this simple grappling hook into the web swing I envisioned, there was a lot of work still to do. The idea of connecting
to a point on objects is useful, and it's fairly simply to make this check for objects of a specific type by using collision channels.
However, the weirdness in the movement when grappling has to be changed, and we need to get rid of the force drawing us towards the
point and replace it with some fun swinging mechanics.

It could also be nice to add some polish. The spider man games have some nice features for zipping towards a specific point, or a launch
to get you off the ground initially - things to make the movement flow between the swinging and the regular on the ground action. This
kind of thing is very important for the feel of these games, and if time allows then it would be very nice to add that to this project
too.

### 👨‍💻 Making The Changes
To make these changes, I decided to override Unreal Engines build in physics. The cables themselves were purely visual, with no physics
interactions with the player built in - they just attached one point to another. The behaviour I want is the arcing swings, and for
that some maths is required. I found [Rodrigues' rotation formula](https://en.wikipedia.org/wiki/Rodrigues'_rotation_formula), which can
be used to define rotations around a sphere in 3D space. The formula states:

'If v is a vector in ℝ3 and k is a unit vector describing an axis of rotation about which V rotates by an angle a according to the right 
hand rule, the Rodrigues formula for the rotated vector Vrot is:

*Vrot = V cos(a) + (k * V) sin(a) + k (k . V) (1 - cos(a))*'

The axis vector k can also be defined as the cross product of any two non-zero vectors which define the plane of rotation, which is a
useful tool for my purposes. My idea is to find the plane between the player and the point on the wall which they are attached to by 
using the point on the wall and the player's direction, and then apply the rotation formula to the player to create the swing effect.
This took me some time to figure out all the maths and how to make this into a computer algorithm, but the implementation went relatively
smooth. Unfortunately, there were still some unforseen effects to figure out.

### 🐜 The Bugs
The first and most obvious issue that emerged was that the player seemed to lose momentum very quickly. All speed seemed to be leaving 
the system. At first I tried to fix this by not discarding the radial velocity (since normally it is insignificant and we only need
tangential velocity), but this didn't solve it. What I believe the issue was is more to do with gravity, which consistently takes away
from the upward part of the swing. This kicks in at the bottom of the swing and really stops all acceleration upwards, which serves to
almost instantly kill all momentum the player has. This is where the issue with falling vs flying comes in - we can't just use the 
flying state to get around this because of the bugs it can cause when landing, which require complicated systems to govern how the 
exact interactions should behave and are error prone and hard to test all cases for.

The solution I settled on for this problem was to maintain velocity through the swing. The program would calculate the velocity the 
player had starting the swing, and scale the velocity at the end of each tick to match this magnitude. This kept the momentum in the 
system very well, but there was still an issue when the player hit the end of the swing. I solved this by applying a simple upward 
force at the end of the swing if the player was moving upwards.

### 🐛 And They Don't Stop Coming
As with most things, fixing the largest issue shed light on some smaller ones, so let's go through a few. 

- The player's momentum was killed on contact with walls, which was fixed with the same maintaining of momentum as the losing momentum
bug mentioned previously.
- The player could swing round in circles, so I released the player to the default physics in the upper half of the swing.
- The player was instantly dragged into a circular swing as soon as the grapple was connected, even if they were above the connection point,
which took them outwards from the wall and felt very unintuitive, which was fixed by making the grapple function through a maximum distance
from the grapple point which it constrained the player to.

### 🏁 Conclusions And Reflections
This project was very fun, although it was the least time consuming of my three modules in the first term of third year. Graphics took 
a lot more of my time because it was quite difficult and content heavy, so despite being small this project was a nice break. I do wish I
had more time to spend on it in the end, because there was a lot more I felt I could do. 

In hindsight I think it would have been more fun to have the player swing on a vertical axis, with maybe a small force towards the walls. 
I think I got caught up on the realistic maths and missed that something a bit simpler would make the game more fun. It would have been fun
to add those other features the spider man games have like the zip to get back to the ground and a launch to get into the air, to make the
movement more fluid.

But all in all I view this as a success, not least because I passed the module easily. Learning about Unreal Engine was a great experience,
and I think these skills will carry forward nicely to my future.





