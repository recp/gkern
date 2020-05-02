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

GK_STRINGIFY(
\n#version 410 \n

uniform mat4 MVP; // Projection * View * Model matrix
uniform mat4 MV;  // View * Model Matrix
uniform mat4 NM;  // Normal matrix
uniform int  NMU; // Use normal matrix

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;

out vec3 vPos;
out vec3 vNormal;
out vec3 vEye;

void main() {
  vec4 pos4 = vec4(POSITION, 1.0);

  vPos = vec3(MV * pos4);
  vEye = normalize(-vPos);

  if (NMU == 1)
    vNormal = normalize(vec3(NM * vec4(NORMAL, 0.0)));
  else
    vNormal = normalize(vec3(MV * vec4(NORMAL, 0.0)));

  gl_Position = MVP * pos4;
}
)
