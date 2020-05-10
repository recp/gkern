/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 References:
 [0] KhronosGroup's glTF-WebGL-PBR reference shader
     https://github.com/KhronosGroup/glTF-Sample-Viewer
 */

GK_STRINGIFY(
\n#ifdef USE_MORPHING\n

layout (std140) uniform TargetBlock {
  vec4 uTargetWeights;
};

/* POSITION */

\n#ifdef HAS_TARGET0_POSITION\n
in vec3 TARGET0_POSITION;
\n#endif\n

\n#ifdef HAS_TARGET1_POSITION\n
in vec3 TARGET1_POSITION;
\n#endif\n

\n#ifdef HAS_TARGET2_POSITION\n
in vec3 TARGET2_POSITION;
\n#endif\n

\n#ifdef HAS_TARGET3_POSITION\n
in vec3 TARGET3_POSITION;
\n#endif\n

/* NORMAL */

\n#ifdef HAS_TARGET0_NORMAL\n
in vec3 TARGET0_NORMAL;
\n#endif\n

\n#ifdef HAS_TARGET1_NORMAL\n
in vec3 TARGET1_NORMAL;
\n#endif\n

\n#ifdef HAS_TARGET2_NORMAL\n
in vec3 TARGET2_NORMAL;
\n#endif\n

\n#ifdef HAS_TARGET3_NORMAL\n
in vec3 TARGET3_NORMAL;
\n#endif\n

/* TANGENT */
             
\n#ifdef HAS_TARGET0_TANGENT\n
in vec3 TARGET0_TANGENT;
\n#endif\n

\n#ifdef HAS_TARGET1_TANGENT\n
in vec3 TARGET1_TANGENT;
\n#endif\n

\n#ifdef HAS_TARGET2_TANGENT\n
in vec3 TARGET2_TANGENT;
\n#endif\n

\n#ifdef HAS_TARGET3_TANGENT\n
in vec3 TARGET3_TANGENT;
\n#endif\n
             
/* Utilities */
             
vec4 targetPosition() {
  vec4 pos = vec4(0);

\n#ifdef HAS_TARGET0_POSITION\n
  pos.xyz += uTargetWeights.x * TARGET0_POSITION;
\n#endif\n

\n#ifdef HAS_TARGET1_POSITION\n
  pos.xyz += uTargetWeights.y * TARGET1_POSITION;
\n#endif\n

\n#ifdef HAS_TARGET2_POSITION\n
  pos.xyz += uTargetWeights.z * TARGET2_POSITION;
\n#endif\n

\n#ifdef HAS_TARGET3_POSITION\n
  pos.xyz += uTargetWeights.w * TARGET3_POSITION;
\n#endif\n

  return pos;
}
             
vec4 targetNormal() {
  vec4 normal = vec4(0);

\n#ifdef HAS_TARGET0_NORMAL\n
  normal.xyz += uTargetWeights.x * TARGET0_NORMAL;
\n#endif\n

\n#ifdef HAS_TARGET1_NORMAL\n
  normal.xyz += uTargetWeights.y * TARGET1_NORMAL;
\n#endif\n

\n#ifdef HAS_TARGET2_NORMAL\n
  normal.xyz += uTargetWeights.z * TARGET2_NORMAL;
\n#endif\n

\n#ifdef HAS_TARGET3_NORMAL\n
  normal.xyz += uTargetWeights.w * TARGET3_NORMAL;
\n#endif\n

  return normal;
}
             
vec4 targetTangent() {
  vec4 tangent = vec4(0);
  
\n#ifdef HAS_TARGET0_TANGENT\n
  tangent.xyz += uTargetWeights.x * TARGET0_TANGENT;
\n#endif\n
  
\n#ifdef HAS_TARGET1_TANGENT\n
  tangent.xyz += uTargetWeights.y * TARGET1_TANGENT;
\n#endif\n
  
\n#ifdef HAS_TARGET2_TANGENT\n
  tangent.xyz += uTargetWeights.z * TARGET2_TANGENT;
\n#endif\n
  
\n#ifdef HAS_TARGET3_TANGENT\n
  tangent.xyz += uTargetWeights.w * TARGET3_TANGENT;
\n#endif\n
  
  return tangent;
}
\n#endif\n
)
