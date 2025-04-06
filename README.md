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
to your game, Unreal Engine has Actors, Pawns and Characters, which just show you all their code upfront. From what I understand, 
there is a much more clear idea in Unreal Engine of exactly how they want you to structure everything. Unreal Engine also exposes 
the entirety of the engine to programmers, which was very daunting at first. 

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
form of a grapple hook imaginable, and where the tutorial left me. This does have some problems though. The movement while in the air 
as you grapple feelings very strange, as does falling immediately when you get to the top of a wall rather than getting on top of it.

### 💡 Ideas For Improvement
So what's next? 

To transform this simple grappling hook into the web swing I envisioned, there was a lot of work still to do. The idea of connecting
to a point on objects is useful, and it's fairly simply to make this check for objects of a specific type by using collision channels.





