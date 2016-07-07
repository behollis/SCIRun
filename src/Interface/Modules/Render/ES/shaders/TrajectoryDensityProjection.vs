/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

// Uniforms
uniform mat4    uProjIVObject;      // Projection * Inverse View * World XForm
uniform vec4    uColor;             // Uniform color
uniform mat4    uInverseView;       // world -> view

// Attributes
attribute vec3  aPos;
attribute float aRadius;
attribute vec3 aTangent;


// Outputs to the fragment shader.
varying vec4    fColor;
varying vec4    vPos;//for clipping plane calc
varying vec4    vFogCoord;// for fog calculation
varying float expansion_dir;
varying float fall_off;

void main( void )
{
  vec4 tangent = vec4(aTangent.xyz, 0.0);
  expansion_dir = aRadius;
  
  // instead of what Kuhn does, just take gl_Vertex in View space. 
  vec4 viewsp_vtx_pos = uInverseView * vec4(aPos.xyz, 0.0);
  vec4 viewsp_tangent = uInverseView * tangent; 
  
  // additive inverse of viewspace vtx pos is vector pointing 
  // to camera from vertex in view space 
  vec3 viewsp_vtx_pos3 = vec3(viewsp_vtx_pos.xyz);
  vec3 viewsp_offset_dir = cross(vec3(viewsp_tangent.xyz), -viewsp_vtx_pos3); 
  vec3 viewsp_offset_dir_norm = normalize( viewsp_offset_dir );
  
  float offset_scale = 0.5; 
  vec4 new_viewsp_vtx_pos = vec4((viewsp_offset_dir_norm * offset_scale * expansion_dir) 
   + viewsp_vtx_pos3, 1); 

//  mat4 projMatrix = MCDCMatrix * inverse(MCVCMatrix); //here we obtain the projection matrix 
  mat4 projMatrix = uProjIVObject * inverse(uInverseView); //here we obtain the projection matrix 
  gl_Position = projMatrix * new_viewsp_vtx_pos; 

//  gl_Position = uProjIVObject * vec4(aPos, 1.0);
  fColor      = vec4(0.0, 1.0, 0.0, 1.0); //uColor;
  vPos = vec4(aPos, 1.0);
  vFogCoord = uInverseView * vPos;
  
  fall_off = 1.0;
}

/*
attribute vec4 vertexMC;
attribute vec3 normalMC; // using this for tangent 
attribute vec3 tcoordMC; // using this for expansion dir and possibly more...

//camera and actor matrix values
uniform mat4 MCDCMatrix;
uniform mat4 MCVCMatrix;\n' + \
uniform vec3 diffuseColorUniform; // intensity weighted color
uniform vec3 specularColorUniform; // intensity weighted color
varying vec3 color;
varying float expansion_dir;
varying float fall_off;

void main() {
  vec4 tangent = vec4(normalMC.xyz, 0.0);
  expansion_dir = tcoordMC.x;
  
  // instead of what Kuhn does, just take gl_Vertex in View space. 
  vec4 viewsp_vtx_pos = MCVCMatrix * vertexMC;
  vec4 viewsp_tangent = MCVCMatrix * tangent; 
  
  // additive inverse of viewspace vtx pos is vector pointing 
  // to camera from vertex in view space 
  vec3 viewsp_vtx_pos3 = vec3(viewsp_vtx_pos.xyz);
  vec3 viewsp_offset_dir = cross(vec3(viewsp_tangent.xyz), -viewsp_vtx_pos3); 
  vec3 viewsp_offset_dir_norm = normalize( viewsp_offset_dir ); 
  float offset_scale = tcoordMC.y; 
  vec4 new_viewsp_vtx_pos = vec4((viewsp_offset_dir_norm * offset_scale * expansion_dir) 
   + viewsp_vtx_pos3, 1); 
  mat4 projMatrix = MCDCMatrix * inverse(MCVCMatrix); //here we obtain the projection matrix 
  gl_Position = projMatrix * new_viewsp_vtx_pos;
  
  //test built-in attribures tcoordsMC and normalMC...
  color = vec3(diffuseColorUniform.rgb);
  fall_off = tcoordMC.z;
}
*/