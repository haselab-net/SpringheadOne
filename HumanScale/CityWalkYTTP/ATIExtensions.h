/*
** Copyright (C) 2002 ATI Technologies Inc.
*/

#ifndef ATI_EXTENSIONS_H
#define ATI_EXTENSIONS_H

#define __GL_SUPPORT_GL2_EXTENSIONS

#include <gl/gl.h>
#include "glATI.h"

#ifdef WIN32
#include "wglATI.h"
#else //Linux
#include "glxATI.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GL_ARB_multisample
extern GLboolean GL_ARB_multisample_Flag;
extern PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB;
#endif

#ifdef GL_ARB_multitexture
extern GLboolean GL_ARB_multitexture_Flag;
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB;
extern PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB;
extern PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB;
extern PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB;
extern PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB;
extern PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB;
extern PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB;
extern PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB;
extern PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
extern PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB;
extern PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB;
extern PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB;
extern PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB;
extern PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB;
extern PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB;
extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB;
extern PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB;
extern PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB;
extern PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB;
extern PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB;
extern PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB;
extern PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB;
extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
extern PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB;
extern PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB;
extern PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB;
extern PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB;
extern PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB;
#endif

#ifdef GL_ARB_texture_compression
extern GLboolean GL_ARB_texture_compression_Flag;
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3DARB;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1DARB;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImageARB;
#endif

#ifdef GL_ARB_transpose_matrix
extern GLboolean GL_ARB_transpose_matrix_Flag;
extern PFNGLLOADTRANSPOSEMATRIXFARBPROC glLoadTransposeMatrixfARB;
extern PFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixdARB;
extern PFNGLMULTTRANSPOSEMATRIXFARBPROC glMultTransposeMatrixfARB;
extern PFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixdARB;
#endif

#ifdef GL_ARB_vertex_blend
extern GLboolean GL_ARB_vertex_blend_Flag;
extern PFNGLWEIGHTBVARBPROC glWeightbvARB;
extern PFNGLWEIGHTSVARBPROC glWeightsvARB;
extern PFNGLWEIGHTIVARBPROC glWeightivARB;
extern PFNGLWEIGHTFVARBPROC glWeightfvARB;
extern PFNGLWEIGHTDVARBPROC glWeightdvARB;
extern PFNGLWEIGHTUBVARBPROC glWeightubvARB;
extern PFNGLWEIGHTUSVARBPROC glWeightusvARB;
extern PFNGLWEIGHTUIVARBPROC glWeightuivARB;
extern PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB;
extern PFNGLVERTEXBLENDARBPROC glVertexBlendARB;
#endif

#ifdef GL_ARB_window_pos
extern GLboolean GL_ARB_window_pos_Flag;
extern PFNGLWINDOWPOS2DARBPROC glWindowPos2dARB;
extern PFNGLWINDOWPOS2FARBPROC glWindowPos2fARB;
extern PFNGLWINDOWPOS2IARBPROC glWindowPos2iARB;
extern PFNGLWINDOWPOS2SARBPROC glWindowPos2sARB;
extern PFNGLWINDOWPOS2IVARBPROC glWindowPos2ivARB;
extern PFNGLWINDOWPOS2SVARBPROC glWindowPos2svARB;
extern PFNGLWINDOWPOS2FVARBPROC glWindowPos2fvARB;
extern PFNGLWINDOWPOS2DVARBPROC glWindowPos2dvARB;
extern PFNGLWINDOWPOS3IARBPROC glWindowPos3iARB;
extern PFNGLWINDOWPOS3SARBPROC glWindowPos3sARB;
extern PFNGLWINDOWPOS3FARBPROC glWindowPos3fARB;
extern PFNGLWINDOWPOS3DARBPROC glWindowPos3dARB;
extern PFNGLWINDOWPOS3IVARBPROC glWindowPos3ivARB;
extern PFNGLWINDOWPOS3SVARBPROC glWindowPos3svARB;
extern PFNGLWINDOWPOS3FVARBPROC glWindowPos3fvARB;
extern PFNGLWINDOWPOS3DVARBPROC glWindowPos3dvARB;
#endif

#ifdef GL_ARB_vertex_program
extern GLboolean GL_ARB_vertex_program_Flag;
extern PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
extern PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
extern PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
extern PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
extern PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
extern PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
extern PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
extern PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
extern PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
extern PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
extern PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
extern PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
extern PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
extern PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
extern PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
extern PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
extern PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
extern PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
extern PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
extern PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
extern PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
extern PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
extern PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
extern PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
extern PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
extern PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
extern PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
extern PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
extern PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
extern PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
extern PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
extern PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
extern PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
extern PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
extern PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
extern PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
extern PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;;
extern PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;;
extern PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
extern PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
extern PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
extern PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
extern PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
extern PFNGLISPROGRAMARBPROC glIsProgramARB;
#endif

#ifdef GL_ARB_fragment_program
extern GLboolean GL_ARB_fragment_program_Flag;
#ifndef GL_ARB_vertex_program
extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
extern PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
extern PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
extern PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
extern PFNGLISPROGRAMARBPROC glIsProgramARB;
#endif
#endif

#ifdef GL_ARB_depth_texture
extern GLboolean GL_ARB_depth_texture_Flag;
#endif

#ifdef GL_ARB_shadow
extern GLboolean GL_ARB_shadow_Flag;
#endif

#ifdef GL_ARB_shadow_ambient
extern GLboolean GL_ARB_shadow_ambient_Flag;
#endif

#ifdef GL_ARB_texture_cube_map
extern GLboolean GL_ARB_texture_cube_map_Flag;
#endif 

#ifdef GL_ARB_texture_env_add
extern GLboolean GL_ARB_texture_env_add_Flag;
#endif 

#ifdef GL_ARB_texture_env_combine
extern GLboolean GL_ARB_texture_env_combine_Flag;
#endif 

#ifdef GL_ARB_texture_env_crossbar
extern GLboolean GL_ARB_texture_env_crossbar_Flag;
#endif

#ifdef GL_ARB_texture_env_dot3
extern GLboolean GL_ARB_texture_env_dot3_Flag;
#endif

#ifdef GL_ARB_texture_border_clamp
extern GLboolean GL_ARB_texture_border_clamp_Flag;
#endif

#ifdef GL_ARB_texture_mirror_repeat
extern GLboolean GL_ARB_texture_mirror_repeat_Flag;
#endif

#ifdef GL_EXT_texture_object
extern GLboolean GL_EXT_texture_object_Flag;
extern PFNGLGENTEXTURESEXTPROC glGenTexturesEXT;
extern PFNGLDELETETEXTURESEXTPROC glDeleteTexturesEXT;
extern PFNGLBINDTEXTUREEXTPROC glBindTextureEXT;
extern PFNGLPRIORITIZETEXTURESEXTPROC glPrioritizeTexturesEXT;
extern PFNGLARETEXTURESRESIDENTEXTPROC glAreTexturesResidentEXT;
extern PFNGLISTEXTUREEXTPROC glIsTextureEXT;
#endif

#ifdef GL_EXT_fog_coord
extern GLboolean GL_EXT_fog_coord_Flag;
extern PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
extern PFNGLFOGCOORDDEXTPROC glFogCoorddEXT;
extern PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT;
extern PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT;
extern PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT;
#endif

#ifdef GL_EXT_secondary_color
extern GLboolean GL_EXT_secondary_color_Flag;
extern PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT;
extern PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT;
extern PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT;
extern PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT;
extern PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT;
extern PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT;
extern PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT;
extern PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT;
extern PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT;
extern PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT;
extern PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT;
extern PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT;
extern PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT;
extern PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT;
extern PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;
#endif

#ifdef GL_EXT_draw_range_elements
extern GLboolean GL_EXT_draw_range_elements_Flag;
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT;
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElements;
#endif

#ifdef GL_EXT_multi_draw_arrays
extern GLboolean GL_EXT_multi_draw_arrays_Flag;
extern PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT;
extern PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT;
#endif

#ifdef GL_EXT_texture3D
extern GLboolean GL_EXT_texture3D_Flag;
extern PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3DEXT;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3DEXT;
#endif

#ifdef GL_EXT_point_parameters
extern GLboolean GL_EXT_point_parameters_Flag;
extern PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT;
extern PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT;
#endif

#ifdef GL_ARB_point_parameters
extern GLboolean GL_ARB_point_parameters_Flag;
extern PFNGLPOINTPARAMETERFEXTPROC glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvARB;
#endif

#ifdef GL_EXT_vertex_shader
extern GLboolean GL_EXT_vertex_shader_Flag;
extern PFNGLBEGINVERTEXSHADEREXTPROC glBeginVertexShaderEXT;
extern PFNGLENDVERTEXSHADEREXTPROC glEndVertexShaderEXT;
extern PFNGLBINDVERTEXSHADEREXTPROC glBindVertexShaderEXT;
extern PFNGLGENVERTEXSHADERSEXTPROC glGenVertexShadersEXT;
extern PFNGLDELETEVERTEXSHADEREXTPROC glDeleteVertexShaderEXT;
extern PFNGLSHADEROP1EXTPROC glShaderOp1EXT;
extern PFNGLSHADEROP2EXTPROC glShaderOp2EXT;
extern PFNGLSHADEROP3EXTPROC glShaderOp3EXT;
extern PFNGLSWIZZLEEXTPROC glSwizzleEXT;
extern PFNGLWRITEMASKEXTPROC glWriteMaskEXT;
extern PFNGLINSERTCOMPONENTEXTPROC glInsertComponentEXT;
extern PFNGLEXTRACTCOMPONENTEXTPROC glExtractComponentEXT;
extern PFNGLGENSYMBOLSEXTPROC glGenSymbolsEXT;
extern PFNGLSETINVARIANTEXTPROC glSetInvariantEXT;
extern PFNGLSETLOCALCONSTANTEXTPROC glSetLocalConstantEXT;
extern PFNGLVARIANTBVEXTPROC glVariantbvEXT;
extern PFNGLVARIANTSVEXTPROC glVariantsvEXT;
extern PFNGLVARIANTIVEXTPROC glVariantivEXT;
extern PFNGLVARIANTFVEXTPROC glVariantfvEXT;
extern PFNGLVARIANTDVEXTPROC glVariantdvEXT;
extern PFNGLVARIANTUBVEXTPROC glVariantubvEXT;
extern PFNGLVARIANTUSVEXTPROC glVariantusvEXT;
extern PFNGLVARIANTUIVEXTPROC glVariantuivEXT;
extern PFNGLVARIANTPOINTEREXTPROC glVariantPointerEXT;
extern PFNGLENABLEVARIANTCLIENTSTATEEXTPROC glEnableVariantClientStateEXT;
extern PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC glDisableVariantClientStateEXT;
extern PFNGLBINDLIGHTPARAMETEREXTPROC glBindLightParameterEXT;
extern PFNGLBINDMATERIALPARAMETEREXTPROC glBindMaterialParameterEXT;
extern PFNGLBINDTEXGENPARAMETEREXTPROC glBindTexGenParameterEXT;
extern PFNGLBINDTEXTUREUNITPARAMETEREXTPROC glBindTextureUnitParameterEXT;
extern PFNGLBINDPARAMETEREXTPROC glBindParameterEXT;
extern PFNGLISVARIANTENABLEDEXTPROC glIsVariantEnabledEXT;
extern PFNGLGETVARIANTBOOLEANVEXTPROC glGetVariantBooleanvEXT;
extern PFNGLGETVARIANTINTEGERVEXTPROC glGetVariantIntegervEXT;
extern PFNGLGETVARIANTFLOATVEXTPROC glGetVariantFloatvEXT;
extern PFNGLGETVARIANTPOINTERVEXTPROC glGetVariantPointervEXT;
extern PFNGLGETINVARIANTBOOLEANVEXTPROC glGetInvariantBooleanvEXT;
extern PFNGLGETINVARIANTINTEGERVEXTPROC glGetInvariantIntegervEXT;
extern PFNGLGETINVARIANTFLOATVEXTPROC glGetInvariantFloatvEXT;
extern PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC glGetLocalConstantBooleanvEXT;
extern PFNGLGETLOCALCONSTANTINTEGERVEXTPROC glGetLocalConstantIntegervEXT;
extern PFNGLGETLOCALCONSTANTFLOATVEXTPROC glGetLocalConstantFloatvEXT;
#endif

#ifdef GL_EXT_blend_minmax
extern GLboolean GL_EXT_blend_minmax_Flag;
extern PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;
#endif

#ifdef GL_EXT_blend_func_separate
extern GLboolean GL_EXT_blend_func_separate_Flag;
extern PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT;
#endif

#ifdef GL_EXT_blend_color
extern GLboolean GL_EXT_blend_color_Flag;
extern PFNGLBLENDCOLOREXTPROC glBlendColorEXT;
#endif

#ifndef GL_EXT_polygon_offset
extern GLboolean GL_EXT_polygon_offset_Flag;
#endif 

#ifdef GL_EXT_vertex_array
extern GLboolean GL_EXT_vertex_array_Flag;
extern PFNGLARRAYELEMENTEXTPROC glArrayElementEXT;
extern PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT;
extern PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT;
extern PFNGLNORMALPOINTEREXTPROC glNormalPointerEXT;
extern PFNGLCOLORPOINTEREXTPROC glColorPointerEXT;
extern PFNGLINDEXPOINTEREXTPROC glIndexPointerEXT;
extern PFNGLTEXCOORDPOINTEREXTPROC glTexCoordPointerEXT;
extern PFNGLEDGEFLAGPOINTEREXTPROC glEdgeFlagPointerEXT;
extern PFNGLGETPOINTERVEXTPROC glGetPointervEXT;
#endif

#ifdef GL_EXT_compiled_vertex_array
extern GLboolean GL_EXT_compiled_vertex_array_Flag;
extern PFNGLLOCKARRAYSEXTPROC glLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT;
#endif

#ifdef GL_EXT_abgr 
extern GLboolean GL_EXT_abgr_Flag;
#endif 

#ifdef GL_EXT_bgra
extern GLboolean GL_EXT_bgra_Flag;
#endif 

#ifdef GL_EXT_blend_subtract
extern GLboolean GL_EXT_blend_subtract_Flag;
#endif

#ifdef GL_EXT_clip_volume_hint
extern GLboolean GL_EXT_clip_volume_hint_Flag;
#endif 

#ifdef GL_EXT_packed_pixels
extern GLboolean GL_EXT_packed_pixels_Flag;
#endif 

#ifdef GL_EXT_rescale_normal
extern GLboolean GL_EXT_rescale_normal_Flag;
#endif

#ifdef GL_EXT_separate_specular_color
extern GLboolean GL_EXT_separate_specular_color_Flag;
#endif 

#ifdef GL_EXT_texgen_reflection
extern GLboolean GL_EXT_texgen_reflection_Flag;
#endif 

#ifdef GL_EXT_texture_cube_map
extern GLboolean GL_EXT_texture_cube_map_Flag;
#endif 

#ifdef GL_EXT_texture_compression_s3tc
extern GLboolean GL_EXT_texture_compression_s3tc_Flag;
#endif

#ifdef GL_EXT_texture_edge_clamp
extern GLboolean GL_EXT_texture_edge_clamp_Flag;
#endif 

#ifdef GL_EXT_texture_env_add
extern GLboolean GL_EXT_texture_env_add_Flag;
#endif

#ifdef GL_EXT_texture_env_combine
extern GLboolean GL_EXT_texture_env_combine_Flag;
#endif 

#ifdef GL_EXT_texture_env_dot3
extern GLboolean GL_EXT_texture_env_dot3_Flag;
#endif 

#ifdef GL_EXT_texture_filter_anisotropic
extern GLboolean GL_EXT_texture_filter_anisotropic_Flag;
#endif 

#ifdef GL_EXT_stencil_wrap
extern GLboolean GL_EXT_stencil_wrap_Flag;
#endif

#ifdef GL_EXT_texture_rectangle
extern GLboolean GL_EXT_texture_rectangle_Flag;
#endif

#ifdef GL_ATI_envmap_bumpmap
extern GLboolean GL_ATI_envmap_bumpmap_Flag;
extern PFNGLTEXBUMPPARAMETERIVATIPROC glTexBumpParameterivATI;
extern PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI;
extern PFNGLGETTEXBUMPPARAMETERIVATIPROC glGetTexBumpParameterivATI;
extern PFNGLGETTEXBUMPPARAMETERFVATIPROC glGetTexBumpParameterfvATI;
#endif

#ifdef GL_ATI_fragment_shader
extern GLboolean GL_ATI_fragment_shader_Flag;
extern PFNGLGENFRAGMENTSHADERSATIPROC glGenFragmentShadersATI;
extern PFNGLBINDFRAGMENTSHADERATIPROC glBindFragmentShaderATI;
extern PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI;
extern PFNGLBEGINFRAGMENTSHADERATIPROC glBeginFragmentShaderATI;
extern PFNGLENDFRAGMENTSHADERATIPROC glEndFragmentShaderATI;
extern PFNGLPASSTEXCOORDATIPROC glPassTexCoordATI;
extern PFNGLSAMPLEMAPATIPROC glSampleMapATI;
extern PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI;
extern PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI;
extern PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI;
extern PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI;
extern PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI;
extern PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI;
extern PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI;
#endif

#ifdef GL_ATI_vertex_array_object
extern GLboolean GL_ATI_vertex_array_object_Flag;
extern PFNGLNEWOBJECTBUFFERATIPROC glNewObjectBufferATI;
extern PFNGLISOBJECTBUFFERATIPROC glIsObjectBufferATI;
extern PFNGLUPDATEOBJECTBUFFERATIPROC glUpdateObjectBufferATI;
extern PFNGLGETOBJECTBUFFERFVATIPROC glGetObjectBufferfvATI;
extern PFNGLGETOBJECTBUFFERIVATIPROC glGetObjectBufferivATI;
extern PFNGLFREEOBJECTBUFFERATIPROC glFreeObjectBufferATI;
extern PFNGLARRAYOBJECTATIPROC glArrayObjectATI;
extern PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectfvATI;
extern PFNGLGETARRAYOBJECTIVATIPROC glGetArrayObjectivATI;
extern PFNGLVARIANTARRAYOBJECTATIPROC glVariantArrayObjectATI;
extern PFNGLGETVARIANTARRAYOBJECTFVATIPROC glGetVariantArrayObjectfvATI;
extern PFNGLGETVARIANTARRAYOBJECTIVATIPROC glGetVariantArrayObjectivATI;
#endif

#ifdef GL_ATI_element_array
extern GLboolean GL_ATI_element_array_Flag;
extern PFNGLELEMENTPOINTERATIPROC glElementPointerATI;
extern PFNGLDRAWELEMENTARRAYATIPROC glDrawElementArrayATI;
extern PFNGLDRAWRANGEELEMENTARRAYATIPROC glDrawRangeElementArrayATI;
#endif

#ifdef GL_ATI_vertex_attrib_array_object
extern GLboolean GL_ATI_vertex_attrib_array_object_Flag;
extern PFNGLVERTEXATTRIBARRAYOBJECTATIPROC glVertexAttribArrayObjectATI;
extern PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC glVertexAttribArrayObjectfvATI;
extern PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC glVertexAttribArrayObjectivATI;
#endif

#ifdef GL_ATI_pn_triangles
extern GLboolean GL_ATI_pn_triangles_Flag;
extern PFNGLPNTRIANGLESIATIPROC glPNTrianglesiATI;
extern PFNGLPNTRIANGLESFATIPROC glPNTrianglesfATI;
#endif

#ifdef GL_ATI_vertex_streams
extern GLboolean GL_ATI_vertex_streams_Flag;
extern PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC glClientActiveVertexStreamATI;
extern PFNGLVERTEXBLENDENVIATIPROC glVertexBlendEnviATI;
extern PFNGLVERTEXBLENDENVFATIPROC glVertexBlendEnvfATI;
extern PFNGLVERTEXSTREAM2SATIPROC glVertexStream2sATI;
extern PFNGLVERTEXSTREAM2SVATIPROC glVertexStream2svATI;
extern PFNGLVERTEXSTREAM2IATIPROC glVertexStream2iATI;
extern PFNGLVERTEXSTREAM2IVATIPROC glVertexStream2ivATI;
extern PFNGLVERTEXSTREAM2FATIPROC glVertexStream2fATI;
extern PFNGLVERTEXSTREAM2FVATIPROC glVertexStream2fvATI;
extern PFNGLVERTEXSTREAM2DATIPROC glVertexStream2dATI;
extern PFNGLVERTEXSTREAM2DVATIPROC glVertexStream2dvATI;
extern PFNGLVERTEXSTREAM3SATIPROC glVertexStream3sATI;
extern PFNGLVERTEXSTREAM3SVATIPROC glVertexStream3svATI;
extern PFNGLVERTEXSTREAM3IATIPROC glVertexStream3iATI;
extern PFNGLVERTEXSTREAM3IVATIPROC glVertexStream3ivATI;
extern PFNGLVERTEXSTREAM3FATIPROC glVertexStream3fATI;
extern PFNGLVERTEXSTREAM3FVATIPROC glVertexStream3fvATI;
extern PFNGLVERTEXSTREAM3DATIPROC glVertexStream3dATI;
extern PFNGLVERTEXSTREAM3DVATIPROC glVertexStream3dvATI;
extern PFNGLVERTEXSTREAM4SATIPROC glVertexStream4sATI;
extern PFNGLVERTEXSTREAM4SVATIPROC glVertexStream4svATI;
extern PFNGLVERTEXSTREAM4IATIPROC glVertexStream4iATI;
extern PFNGLVERTEXSTREAM4IVATIPROC glVertexStream4ivATI;
extern PFNGLVERTEXSTREAM4FATIPROC glVertexStream4fATI;
extern PFNGLVERTEXSTREAM4FVATIPROC glVertexStream4fvATI;
extern PFNGLVERTEXSTREAM4DATIPROC glVertexStream4dATI;
extern PFNGLVERTEXSTREAM4DVATIPROC glVertexStream4dvATI;
extern PFNGLNORMALSTREAM3BATIPROC glNormalStream3bATI;
extern PFNGLNORMALSTREAM3BVATIPROC glNormalStream3bvATI;
extern PFNGLNORMALSTREAM3SATIPROC glNormalStream3sATI;
extern PFNGLNORMALSTREAM3SVATIPROC glNormalStream3svATI;
extern PFNGLNORMALSTREAM3IATIPROC glNormalStream3iATI;
extern PFNGLNORMALSTREAM3IVATIPROC glNormalStream3ivATI;
extern PFNGLNORMALSTREAM3FATIPROC glNormalStream3fATI;
extern PFNGLNORMALSTREAM3FVATIPROC glNormalStream3fvATI;
extern PFNGLNORMALSTREAM3DATIPROC glNormalStream3dATI;
extern PFNGLNORMALSTREAM3DVATIPROC glNormalStream3dvATI;
#endif

#ifdef GL_ATI_separate_stencil
extern GLboolean GL_ATI_separate_stencil_Flag;
extern PFNGLSTENCILOPSEPARATEATIPROC glStencilOpSeparateATI;
extern PFNGLSTENCILFUNCSEPARATEATIPROC glStencilFuncSeparateATI;
#endif

#ifdef GL_ATI_draw_buffers
extern GLboolean GL_ATI_draw_buffers_Flag;
extern PFNGLDRAWBUFFERSATIPROC glDrawBuffersATI;
#endif

#ifdef GL_ATI_texture_mirror_once
extern GLboolean GL_ATI_texture_mirror_once_Flag;
#endif

#ifdef GL_ATI_texture_float
extern GLboolean GL_ATI_texture_float_Flag;
#endif

#ifdef GL_ATIX_texture_env_combine3
extern GLboolean GL_ATIX_texture_env_combine3_Flag;
#endif 

#ifdef GL_ATIX_texture_env_route
extern GLboolean GL_ATIX_texture_env_route_Flag;
#endif 

#ifdef GL_ATIX_vertex_shader_output_point_size
extern GLboolean GL_ATIX_vertex_shader_output_point_size_Flag;
#endif

#ifdef GL_NV_texgen_reflection
extern GLboolean GL_NV_texgen_reflection_Flag;
#endif

#ifdef GL_NV_blend_square
extern GLboolean GL_NV_blend_square_Flag;
#endif 

#ifdef GL_NV_point_sprite
extern GLboolean GL_NV_point_sprite_Flag;
extern PFNGLPOINTPARAMETERINVPROC glPointParameteriNV; 
extern PFNGLPOINTPARAMETERIVNVPROC glPointParameterivNV;
#endif 

#ifdef GL_NV_occlusion_query
extern GLboolean GL_NV_occlusion_query_Flag;
extern PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueriesNV;
extern PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueriesNV;
extern PFNGLISOCCLUSIONQUERYNVPROC glIsOcclusionQueryNV;
extern PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQueryNV;
extern PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQueryNV;
extern PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryivNV;
extern PFNGLGETOCCLUSIONQUERYUIVNVPROC glGetOcclusionQueryuivNV;
#endif

#ifdef GL_HP_occlusion_test
extern GLboolean GL_HP_occlusion_test_Flag;
#endif

#ifdef GL_S3_s3tc
extern GLboolean GL_S3_s3tc_Flag;
#endif 

#ifdef GL_SGI_texture_edge_clamp
extern GLboolean GL_SGI_texture_edge_clamp_Flag;
#endif 

#ifdef GL_SGIS_generate_mipmap
extern GLboolean GL_SGIS_generate_mipmap_Flag;
#endif

#ifdef GL_SGIS_texture_border_clamp
extern GLboolean GL_SGIS_texture_border_clamp_Flag;
#endif

#ifdef GL_SGIS_texture_lod
extern GLboolean GL_SGIS_texture_lod_Flag;
#endif 

#ifdef GL_SGIS_texture_lod_bias
extern GLboolean GL_SGIS_texture_lod_bias_Flag;
#endif 

#ifdef GL_SGIS_multitexture
extern GLboolean GL_SGIS_multitexture_Flag;
extern PFNGLSELECTTEXTURESGISPROC glSelectTextureSGIS;
extern PFNGLSELECTTEXTURETRANSFORMSGISPROC glSelectTextureTransformSGIS;
extern PFNGLSELECTTEXTURECOORDSETSGISPROC glSelectTextureCoordSetSGIS;
extern PFNGLMULTITEXCOORD1DSGISPROC glMultiTexCoord1dSGIS;
extern PFNGLMULTITEXCOORD1DVSGISPROC glMultiTexCoord1dvSGIS;
extern PFNGLMULTITEXCOORD1FSGISPROC glMultiTexCoord1fSGIS;
extern PFNGLMULTITEXCOORD1FVSGISPROC glMultiTexCoord1fvSGIS;
extern PFNGLMULTITEXCOORD1ISGISPROC glMultiTexCoord1iSGIS;
extern PFNGLMULTITEXCOORD1IVSGISPROC glMultiTexCoord1ivSGIS;
extern PFNGLMULTITEXCOORD1SSGISPROC glMultiTexCoord1sSGIS;
extern PFNGLMULTITEXCOORD1SVSGISPROC glMultiTexCoord1svSGIS;
extern PFNGLMULTITEXCOORD2DSGISPROC glMultiTexCoord2dSGIS;
extern PFNGLMULTITEXCOORD2DVSGISPROC glMultiTexCoord2dvSGIS;
extern PFNGLMULTITEXCOORD2FSGISPROC glMultiTexCoord2fSGIS;
extern PFNGLMULTITEXCOORD2FVSGISPROC glMultiTexCoord2fvSGIS;
extern PFNGLMULTITEXCOORD2ISGISPROC glMultiTexCoord2iSGIS;
extern PFNGLMULTITEXCOORD2IVSGISPROC glMultiTexCoord2ivSGIS;
extern PFNGLMULTITEXCOORD2SSGISPROC glMultiTexCoord2sSGIS;
extern PFNGLMULTITEXCOORD2SVSGISPROC glMultiTexCoord2svSGIS;
extern PFNGLMULTITEXCOORD3DSGISPROC glMultiTexCoord3dSGIS;
extern PFNGLMULTITEXCOORD3DVSGISPROC glMultiTexCoord3dvSGIS;
extern PFNGLMULTITEXCOORD3FSGISPROC glMultiTexCoord3fSGIS;
extern PFNGLMULTITEXCOORD3FVSGISPROC glMultiTexCoord3fvSGIS;
extern PFNGLMULTITEXCOORD3ISGISPROC glMultiTexCoord3iSGIS;
extern PFNGLMULTITEXCOORD3IVSGISPROC glMultiTexCoord3ivSGIS;
extern PFNGLMULTITEXCOORD3SSGISPROC glMultiTexCoord3sSGIS;
extern PFNGLMULTITEXCOORD3SVSGISPROC glMultiTexCoord3svSGIS;
extern PFNGLMULTITEXCOORD4DSGISPROC glMultiTexCoord4dSGIS;
extern PFNGLMULTITEXCOORD4DVSGISPROC glMultiTexCoord4dvSGIS;
extern PFNGLMULTITEXCOORD4FSGISPROC glMultiTexCoord4fSGIS;
extern PFNGLMULTITEXCOORD4FVSGISPROC glMultiTexCoord4fvSGIS;
extern PFNGLMULTITEXCOORD4ISGISPROC glMultiTexCoord4iSGIS;
extern PFNGLMULTITEXCOORD4IVSGISPROC glMultiTexCoord4ivSGIS;
extern PFNGLMULTITEXCOORD4SSGISPROC glMultiTexCoord4sSGIS;
extern PFNGLMULTITEXCOORD4SVSGISPROC glMultiTexCoord4svSGIS;
extern PFNGLMULTITEXCOORD1DSGISPROC glMTexCoord1dSGIS;
extern PFNGLMULTITEXCOORD1DVSGISPROC glMTexCoord1dvSGIS;
extern PFNGLMULTITEXCOORD1FSGISPROC glMTexCoord1fSGIS;
extern PFNGLMULTITEXCOORD1FVSGISPROC glMTexCoord1fvSGIS;
extern PFNGLMULTITEXCOORD1ISGISPROC glMTexCoord1iSGIS;
extern PFNGLMULTITEXCOORD1IVSGISPROC glMTexCoord1ivSGIS;
extern PFNGLMULTITEXCOORD1SSGISPROC glMTexCoord1sSGIS;
extern PFNGLMULTITEXCOORD1SVSGISPROC glMTexCoord1svSGIS;
extern PFNGLMULTITEXCOORD2DSGISPROC glMTexCoord2dSGIS;
extern PFNGLMULTITEXCOORD2DVSGISPROC glMTexCoord2dvSGIS;
extern PFNGLMULTITEXCOORD2FSGISPROC glMTexCoord2fSGIS;
extern PFNGLMULTITEXCOORD2FVSGISPROC glMTexCoord2fvSGIS;
extern PFNGLMULTITEXCOORD2ISGISPROC glMTexCoord2iSGIS;
extern PFNGLMULTITEXCOORD2IVSGISPROC glMTexCoord2ivSGIS;
extern PFNGLMULTITEXCOORD2SSGISPROC glMTexCoord2sSGIS;
extern PFNGLMULTITEXCOORD2SVSGISPROC glMTexCoord2svSGIS;
extern PFNGLMULTITEXCOORD3DSGISPROC glMTexCoord3dSGIS;
extern PFNGLMULTITEXCOORD3DVSGISPROC glMTexCoord3dvSGIS;
extern PFNGLMULTITEXCOORD3FSGISPROC glMTexCoord3fSGIS;
extern PFNGLMULTITEXCOORD3FVSGISPROC glMTexCoord3fvSGIS;
extern PFNGLMULTITEXCOORD3ISGISPROC glMTexCoord3iSGIS;
extern PFNGLMULTITEXCOORD3IVSGISPROC glMTexCoord3ivSGIS;
extern PFNGLMULTITEXCOORD3SSGISPROC glMTexCoord3sSGIS;
extern PFNGLMULTITEXCOORD3SVSGISPROC glMTexCoord3svSGIS;
extern PFNGLMULTITEXCOORD4DSGISPROC glMTexCoord4dSGIS;
extern PFNGLMULTITEXCOORD4DVSGISPROC glMTexCoord4dvSGIS;
extern PFNGLMULTITEXCOORD4FSGISPROC glMTexCoord4fSGIS;
extern PFNGLMULTITEXCOORD4FVSGISPROC glMTexCoord4fvSGIS;
extern PFNGLMULTITEXCOORD4ISGISPROC glMTexCoord4iSGIS;
extern PFNGLMULTITEXCOORD4IVSGISPROC glMTexCoord4ivSGIS;
extern PFNGLMULTITEXCOORD4SSGISPROC glMTexCoord4sSGIS;
extern PFNGLMULTITEXCOORD4SVSGISPROC glMTexCoord4svSGIS;
#endif

#ifdef GL_KTX_buffer_region
extern GLboolean GL_KTX_buffer_region_Flag;
extern PFNGLBUFFERREGIONENABLEDEXTPROC glBufferRegionEnabled;
extern PFNGLNEWBUFFERREGIONEXTPROC glNewBufferRegion;
extern PFNGLDELETEBUFFERREGIONEXTPROC glDeleteBufferRegion;
extern PFNGLREADBUFFERREGIONEXTPROC glReadBufferRegion;
extern PFNGLDRAWBUFFERREGIONEXTPROC glDrawBufferRegion;
#endif

#ifdef GL_WIN_swap_hint
extern GLboolean GL_WIN_swap_hint_Flag;
extern PFNGLADDSWAPHINTRECTWINPROC glAddSwapHintRectWIN;
#endif

#ifdef WGL_ARB_multisample
extern GLboolean WGL_ARB_multisample_Flag;
#endif

#ifdef WGL_ARB_pixel_format
extern GLboolean WGL_ARB_pixel_format_Flag;
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
#endif

#ifdef WGL_ARB_make_current_read
extern GLboolean WGL_ARB_make_current_read_Flag;
extern PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
extern PFNWGLGETCURRENTREADDCARBPROC wglGetCurrentReadDCARB;
#endif

#ifdef WGL_ARB_pbuffer
extern GLboolean WGL_ARB_pbuffer_Flag;
extern PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
extern PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
extern PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
#endif

#ifdef WGL_ARB_extensions_string
extern GLboolean WGL_ARB_extensions_string_Flag;
extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
#endif

#ifdef WGL_EXT_extensions_string
extern GLboolean WGL_EXT_extensions_string_Flag;
extern PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;
#endif

#ifdef WGL_EXT_swap_control
extern GLboolean WGL_EXT_swap_control_Flag;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
#endif

#ifdef WGL_ARB_render_texture
extern GLboolean WGL_ARB_render_texture_Flag;
extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;
#endif

#ifdef WGL_ATI_pixel_format_float
extern GLboolean WGL_ATI_pixel_format_float_Flag;
#endif


/* OpenGL 1.4 */
#ifdef GL_EXT_multi_draw_arrays
extern PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElements;
#endif

#ifdef GL_EXT_fog_coord
extern PFNGLFOGCOORDFEXTPROC glFogCoordf;
extern PFNGLFOGCOORDDEXTPROC glFogCoordd;
extern PFNGLFOGCOORDFVEXTPROC glFogCoordfv;
extern PFNGLFOGCOORDDVEXTPROC glFogCoorddv;
extern PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointer;
#endif

#if GL_EXT_point_parameters
extern PFNGLPOINTPARAMETERFEXTPROC glPointParameterf;
extern PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfv;
#endif

#ifdef GL_EXT_secondary_color
extern PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3f;
extern PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3d;
extern PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3b;
extern PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3s;
extern PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3i;
extern PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ub;
extern PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3us;
extern PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3ui;
extern PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fv;
extern PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dv;
extern PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bv;
extern PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3sv;
extern PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3iv;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubv;
extern PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usv;
extern PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uiv;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointer;
#endif

#ifdef GL_EXT_blend_func_separate
extern PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparate;
#endif

#ifdef GL_ARB_window_pos
extern PFNGLWINDOWPOS2DARBPROC glWindowPos2d;
extern PFNGLWINDOWPOS2FARBPROC glWindowPos2f;
extern PFNGLWINDOWPOS2IARBPROC glWindowPos2i;
extern PFNGLWINDOWPOS2SARBPROC glWindowPos2s;
extern PFNGLWINDOWPOS2IVARBPROC glWindowPos2iv;
extern PFNGLWINDOWPOS2SVARBPROC glWindowPos2sv;
extern PFNGLWINDOWPOS2FVARBPROC glWindowPos2fv;
extern PFNGLWINDOWPOS2DVARBPROC glWindowPos2dv;
extern PFNGLWINDOWPOS3IARBPROC glWindowPos3i;
extern PFNGLWINDOWPOS3SARBPROC glWindowPos3s;
extern PFNGLWINDOWPOS3FARBPROC glWindowPos3f;
extern PFNGLWINDOWPOS3DARBPROC glWindowPos3d;
extern PFNGLWINDOWPOS3IVARBPROC glWindowPos3iv;
extern PFNGLWINDOWPOS3SVARBPROC glWindowPos3sv;
extern PFNGLWINDOWPOS3FVARBPROC glWindowPos3fv;
extern PFNGLWINDOWPOS3DVARBPROC glWindowPos3dv;
#endif

/* OpenGL 1.3 */
#ifdef GL_ARB_multitexture
extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
extern PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1d;
extern PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dv;
extern PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1f;
extern PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fv;
extern PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1i;
extern PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1iv;
extern PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1s;
extern PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1sv;
extern PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2d;
extern PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dv;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2i;
extern PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2iv;
extern PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2s;
extern PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2sv;
extern PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3d;
extern PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dv;
extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fv;
extern PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3i;
extern PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3iv;
extern PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3s;
extern PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3sv;
extern PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4d;
extern PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dv;
extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4f;
extern PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fv;
extern PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4i;
extern PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4iv;
extern PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4s;
extern PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4sv;
#endif

#ifdef GL_ARB_texture_compression
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImage;
#endif

#ifdef GL_ARB_multisample
extern PFNGLSAMPLECOVERAGEARBPROC glSampleCoverage;
#endif

#ifdef GL_ARB_transpose_matrix
extern PFNGLLOADTRANSPOSEMATRIXFARBPROC glLoadTransposeMatrixf;
extern PFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixd;
extern PFNGLMULTTRANSPOSEMATRIXFARBPROC glMultTransposeMatrixf;
extern PFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixd;
#endif

/* OpenGL 1.2 */
#ifdef GL_EXT_texture3D
extern PFNGLTEXIMAGE3DEXTPROC glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
#endif

#ifdef GL_EXT_draw_range_elements
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElements;
#endif


/* Initialize GL 1.4 entrypoints */
int SetupGL1_4();

/* Initialize GL 1.3 entrypoints */
int SetupGL1_3();

/* Initialize GL 1.2 entrypoints */
int SetupGL1_2();

/* Initialize WGL extension entrypoints */
int SetupWGLExtensions();

/* Initialize ARB extension entrypoints */
int SetupARBExtensions();

/* Initialize EXT extension entrypoints */
int SetupEXTExtensions();

/* Initialize ATI extension entrypoints */
int SetupATIExtensions();

/* Initialize GL2 extension entrypoints */
int SetupGL2Extensions();

int SetupExtensionsFromString(const char *);

#ifdef __cplusplus
} //extern C
#endif


#endif //ATI_EXTENSIONS_H