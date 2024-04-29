#version 450
layout (location = 0) in vec3 aPos;

out vec3 force;
out vec3 torque;

uniform mat4 model;
uniform vec3 cm_position;
uniform vec3 velocity;
uniform vec3 gravity;
uniform vec3 totalforce;
uniform float collision_stiffness;
uniform float damping_stiffness_byground;

void main()
{
    force = totalforce + gravity;

    vec3 worldPos = (model * vec4(aPos,1.0)).xyz;

    if(worldPos.y<0.0||worldPos.x>330||worldPos.x<-330||worldPos.z>330||worldPos.z<-330){
        vec3 f_collision = vec3(0.0,collision_stiffness * (-worldPos.y),0.0);
        vec3 f_friction_by_velocity = - damping_stiffness_byground * velocity;
        force += (f_collision + f_friction_by_velocity);
    }
    torque = cross(worldPos-(model*vec4(cm_position,1.0)).xyz,force);
    gl_Position =vec4(aPos, 1.0);
}