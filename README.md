# COMP3015 Courswork 2
# Submission for CW2
This project contains a simple game, which requires gamers to cross two trees by operating a spaceship. In this project, two techniques are used, namely shadow map and animation based on particle system.

## Usage
Simply run the exe file in ./bin/Project_Template.exe, my working environment is Win 10 with VS 2022 and OpenGL 4.6
Run it by command line is recommended since if you cannot run it, you can still get an error:(

<ul>
<li>Use WASD to rotate the spaceship, LSHIFT to accelerate, R to reset the spaceship.</li>
<li>Use your mouse to control the camera</li>
<li>your goal is to pass the two trees, if you successfully make it, you can get a message from command line, saying "you made it!!!!" </li>
<li>However, I didn't make something to be played while loading the resouces, so if your programme stuck, be a little patient please</li>
</ul>


## Features
<ul>
<li>Animation based on particle system, ref:https://www.cs.cmu.edu/~baraff/sigcourse/notesd1.pdf, besides the theory, I used OpenGL feature called Transfrom feedback and vertex shader(file path:./shader/physical_compute_force.vert) to compute the force(and anything else that are necessary) for each particle, gather the force to compute the force for the whole body. In the end I compute the transfrom matrix for rendering. Collision(detection and computation) is achieved by spring model in vertex shader. Inplement source code:./helper/model.cpp, class:PhysicalModel</li>
<li>Inplement shadow map for directional light</li>
<li>skybox that failed to complete in CW1</li>
</ul>


## File content
<ul>
<li>./vendor the assimp for loading the model</li>
<li>./scene_c2.h(.cpp) for main scene</li>
<li>./helper/model.h(.cpp) for both physical and non-physical object</li>
<li>./helper/shader-printf.h，a very good tool to achieve printf function in shader. see https://github.com/msqrt/shader-printf</li>
<li>./helper/Input.h for input handling</li>
<li>./helper/skybox.h for creating the skybox</li>
<li>in ./shader
<ul>
<li>blinn_phone.vert(.frag) original blinn_phone shader, abandoned</li>
<li>physical_compute_force.vert for particle system</li>
<li>generate_shadow_map.vert(.frag) for first pass, generating the shadow map</li>
<li>blinn_phone_shadow.vert(.frag) for second pass, rendering</li>
<li>blinn_phone_light.frag for visualize the point light</li>
<li>skybox.vert(.frag) for rendering the skybox</li>
</ul>
</li>
</ul>

### Youtube:

https://youtu.be/rAAbjZpTF4g
